/***************************         client.c        ****************************** 
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
*       processes (clients) and operating system (server)
* 
* Input:  Location of a common FIFO (to be provided by programmer in source code file)
* 
* Preconditions:  The server program is already running and a common FIFO exists
* 
* Output: Memory request struct (to server)
* 
* Postconditions:  Print the message received from the server program
* 
* Algorithm: 
*           Create memory request
*	 Send memory request and process ID via common FIFO
*           Close connection to common FIFO
*           Open FIFO with process ID
*           Read message from server
*           Print message to console 
*
* Estimates  		         Actuals
*   Design: 1 hours		TBD 
*   Implementation: 1 hour	TBD
*   Testing: 1 hours		TBD
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

// Get random values for request:
int randomMemoryRequest();
int randomBurstTime();
// Print all the values of this client's request:
void broadcastRequest(struct request* r);

// Functions for reading FIFO from server:
void createPrivateFifo();
void sendRequestToServer(struct request* r);
struct response readPrivateFifo();

int main() {
    srand(time(NULL)); // For randomization

    // Build request and send to server
    struct request r = {
        .id = getpid(),
        .memoryUnits = randomMemoryRequest(),
        .burstTime = randomBurstTime()
    };
    broadcastRequest(&r);

    // Create private fifo and receive server response
    createPrivateFifo(r.id);
    sendRequestToServer(&r);
    struct response resp = readPrivateFifo();
    printf("Response ...\n");
    if (resp.responseCode < 0) {
        // Server couldn't allocate
        printf("Server could not allocate %d memory units\n", r.memoryUnits);
    } else {
        // Server successfully allocated
        printf("Request #%d ", resp.responseId);
        printf("was allocated %d frames.\n", resp.framesAllocated);
        printf("Memory fragmentation for this request: %f\n", resp.fragmentation);
        printf("Completion time for this request: %d\n", resp.completionTime);
    }

    return 0;
}

int randomMemoryRequest() {
    int r = 0;
    while (r == 0) {
        // Don't want a 0
        r = rand() % 256;
    }
    return r;
}

int randomBurstTime() {
    int r = (rand() % 10) + 1;
    return r;
}

void broadcastRequest(struct request* r) {
    printf("Request ID: %d\n", r->id);
    printf("Request Size: %d\n", r->memoryUnits);
    printf("Burst Time: %d\n", r->burstTime);
}

void sendRequestToServer(struct request* req) {
    int commonFifo = open(COMMON_FIFO, O_WRONLY);
    if (commonFifo < 0) { printf("[Client] Couldn't open FIFO\n"); }
    write(commonFifo, req, sizeof(struct request));
    close(commonFifo);
    sleep(1);
}

void createPrivateFifo() {
    int thisPid = getpid();
    char privateFifoName[30];
    sprintf(privateFifoName, "privateFifo_%d", thisPid);
    mkfifo(privateFifoName, 0666);
}

struct response readPrivateFifo() {
    int thisPid = getpid();
    char privateFifoName[30];
    sprintf(privateFifoName, "privateFifo_%d", thisPid);
    struct response resp;
    int privateFifo = open(privateFifoName, O_RDONLY);
    if (privateFifo < 0) { printf("[Client] Couldn't open private FIFO\n"); }
    read(privateFifo, &resp, sizeof(resp));
    close(privateFifo);
    return resp;
}
