/***************************          server.c         ***************************** 
* Programmer:  Erik Whiting	
* 
* Course:  CSCI 4354.01 
* 
* Date:  November 14, 2021
* 
* Assignment:  Program #4: Memory Manager and CPU Scheduler
* 
* Environment:  Ubuntu Linux using the gcc compiler
* 
* Files Included: server.c, client.c, compileAndRun.sh (maybe)
* 
* Purpose:  To simulate an operating system’s paged memory allocation with stubbed 
*       processes (clients) and operating system (server). 
* 
* Input:  Multiple memory request structs 
* 
* Preconditions:  Memory requests do not exceed total amount of available memory
* 
* Output:  Server outputs which clients are occupying which frames. Server also displays
*         the client number and clock time after each time quantum (provided by memory request)
*         to show that the processes are being run. 
* 
* Postconditions:  Server exits. 
* 
* Algorithm: 
*          Create common FIFO
*          Read FIFO
*              get process ID
*              get memory request
*          Check available memory
*              If there is adequate memory 
*                  Schedule process to be run
*              Else there is not adequate memory
*                  Prepare an error message
*          If 80% of total frames have been allocated
*             Begin scheduling process
*             When process is over, unallocate its memory
*          Else 
*              Continue waiting for more memory requests
*          Create new FIFO with requesting process’s ID
*          Send either error message or run message to Client via new FIFO
*
* Estimates  		         Actuals
*   Design: 2 hours		TBD
*   Implementation: 2 hour	TBD
*   Testing: 2 hours		TBD
**********************************************************************/ 

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <time.h>

// All structs defined in here:
#include "data_structures.h"

static const char COMMON_FIFO[] = "./commonFifo";
static const int MAX_REQUESTS = 3;

// Make random frames and sizes, build "system":
int randomFrameSize();
int randomFrameCount();
struct frameAvailability initializeFrames();
// Print the current frame/memory availability:
void broadcastFrameAvailability(struct frameAvailability* fa);
// Print the values of the client's request:
void broadcastRequest(struct request* req);
// Determine if server should start executing requests:
int shouldStartExecuting(int initialAvailability, struct frameAvailability* fa);
// Functions for responding to and executing a single request:
int framesNeeded(int unitsRequested);
float calculateFragmentation(int requested, struct frameAvailability* fa);
int allocateMemory(int requested, struct frameAvailability* fa);
int processRequest(struct request* r, struct frameAvailability* fa);
int executeRequest(int currentTime, struct response* r, struct frameAvailability* fa);
// Functions for talking to client:
void createCommonFifo();
void respondToClient(struct response* r);
struct response getResponse(struct request* req,  struct frameAvailability* fa);

int main() {
    srand(time(NULL)); // For randomization
    struct frameAvailability fa = initializeFrames();
    broadcastFrameAvailability(&fa);
    
    // Get initial values of frames
    int initialFrames = fa.frames;
    createCommonFifo();

    int timeRightNow = 0;
    struct response responses[MAX_REQUESTS];
    int totalRequests = 0; // To determine if we should start executing
    for (int i = 0; i < MAX_REQUESTS; i++) {
        int commonFifo = open(COMMON_FIFO, O_RDONLY);
        if (commonFifo < 0) { printf("[Server] Couldn't open FIFO\n"); }
        struct request req;
        read(commonFifo, &req, sizeof(req));
        close(commonFifo);
        broadcastRequest(&req); // Prints received request.
        printf("Processing ...\n");
        struct response resp = getResponse(&req, &fa);
        resp.fragmentation = calculateFragmentation(req.memoryUnits, &fa);
        responses[i] = resp;
        totalRequests++;

        int shouldExecute = shouldStartExecuting(initialFrames, &fa); // CPU at 80%?
        if (shouldExecute > 0 || totalRequests >= MAX_REQUESTS) {
            for (int j = 0; j < totalRequests; j++) {
                struct response executingResponse = responses[j];
                printf("Executing request #%d\n", executingResponse.responseId);
                broadcastFrameAvailability(&fa);
                int finishTime = executeRequest(timeRightNow, &executingResponse, &fa);
                if (executingResponse.responseCode < 0) {
                    // Request was never run, so response time doesn't change
                    executingResponse.completionTime = timeRightNow;
                } else {
                    // execution time for request is added to current time
                    timeRightNow += executingResponse.burstTime;
                    executingResponse.completionTime = finishTime;
                }
                respondToClient(&executingResponse);
                printf("Done with request #%d\n", executingResponse.responseId);
            }
        }
    }

    return 0;
}

int processRequest(struct request* r, struct frameAvailability* fa) {
    printf("Request #%d asks for %d memory\n", r->id, r->memoryUnits);
    int allocation = allocateMemory(r->memoryUnits, fa);
    if (allocation < 0) {
        printf("Could not grant request #%d, not enough memory.\n\n", r->id);
    } else {
        printf("Allocated %d frames to request #%d.\n\n", allocation, r->id);
    }
    return allocation;
}

int executeRequest(int currentTime, struct response* r, struct frameAvailability* fa) {
    fa->frames += r->framesAllocated;
    return currentTime + r->burstTime;
}

int allocateMemory(int requested, struct frameAvailability* fa) {
    int framesNeeded = requested / fa->sizePerFrame;
    if (requested % fa->sizePerFrame > 0) framesNeeded++;
    if (fa->frames < framesNeeded) {
        return -1; // Failure code
    } else {
        fa->frames -= framesNeeded;
        return framesNeeded;
    }
}

float calculateFragmentation(int requested, struct frameAvailability* fa) {
    int framesNeeded = requested / fa->sizePerFrame;
    if (requested % fa->sizePerFrame > 0) framesNeeded++;
    if (fa->frames < framesNeeded) {
        // Fragmentation is irrelevant because this is never executed
        return 0.0;
    } else {
        int totalMemoryTaken = framesNeeded * fa->sizePerFrame;
        int memoryNotUsed = totalMemoryTaken - requested;
        float fragmentation = (float) memoryNotUsed / fa->sizePerFrame;
        return fragmentation;
    }
}

int randomFrameSize() {
    int r = 0;
    while (r == 0) {
        // Don't want a 0
        r = rand() % 80;
    }
    r += 16; // Want at least 16 per frame
    return r;
}

int randomFrameCount() {
    int r = 0;
    while (r == 0) {
        // Don't want a 0
        r = rand() % 30;
    }
    r += 10; // Want at least 10 frames
    return r;
}

struct frameAvailability initializeFrames() {
    struct frameAvailability fa = {
        .frames = randomFrameCount(),
        .sizePerFrame = randomFrameSize()
    };
    return fa;
}

struct response getResponse(struct request* req, struct frameAvailability* fa) {
    struct response resp;
    resp.responseId = req->id;
    resp.framesAllocated = processRequest(req, fa);
    if (resp.framesAllocated < 0) {
        resp.burstTime = 0; // Because we didn't execute the response
        resp.responseCode = -1; // Failure message
    } else {
        resp.burstTime = req->burstTime;
        resp.responseCode = 1;
    }
    resp.fragmentation = 0.0; // Calculated later
    resp.completionTime = 0; // Calculated later
    return resp;
}

void broadcastFrameAvailability(struct frameAvailability* fa) {
    printf("Current Availability ... \n");
    printf("Total frames: %d\n", fa->frames);
    printf("Size per frame: %d\n", fa->sizePerFrame);
    printf("Total memory available: %d\n", fa->frames * fa->sizePerFrame);
}

void broadcastRequest(struct request* req) {
    printf("Request ID: %d\n", req->id);
    printf("Memory Requested: %d\n", req->memoryUnits);
    printf("Burst Time: %d\n", req->burstTime);
}

void createCommonFifo() {
    mkfifo(COMMON_FIFO, 0666);
}

int shouldStartExecuting(int initialAvailability, struct frameAvailability* fa) {
    float capacityAvailable = (float) fa->frames / (float) initialAvailability;
    // Start when 80% is allocated, so when 20% is available:
    if (capacityAvailable <= 0.2f) {
        return 1;
    } else {
        return -1;
    }
};

void respondToClient(struct response* r) {
    char privateFifoName[30];
    sprintf(privateFifoName, "privateFifo_%d", r->responseId);
    int privateFifo = open(privateFifoName, O_WRONLY);
    if (privateFifo < 0) { printf("[Server] Couldn't open private fifo\n"); }
    write(privateFifo, r, sizeof(struct response));
    close(privateFifo);
}
