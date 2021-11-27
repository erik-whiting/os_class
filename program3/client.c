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

struct request {
    int id;
    int printers;
    int disks;
    int scanners;
    int burstTime;
};

struct response {
    int   id;
    int   turnaround;
    float avgTurnAround;
};

int randomBurstTime();
int randomResourceRequest();
void broadcastMessage(struct request r);
void broadcastResponse(struct response r);
void broadcastRejected(int id);

int main() {
    srand(time(NULL));
    int thisPid = getpid();
    char privateFifoName[30];
    sprintf(privateFifoName, "privateFifo_%d", thisPid);
    mkfifo(privateFifoName, 0666);
    
    struct request r = {
        .id = thisPid,
        .printers = randomResourceRequest(),
        .disks = randomResourceRequest(),
        .scanners = randomResourceRequest(),
        .burstTime = randomBurstTime()
    };

    broadcastMessage(r);
    // Send request
    char *commonFifoName = "./commonFIFO";
    int commonFifo = open(commonFifoName, O_WRONLY);
    write(commonFifo, &r, sizeof(r));
    close(commonFifo);

    // Read response
    struct response resp;
    int privateFifo = open(privateFifoName, O_RDONLY);
    read(privateFifo, &resp, sizeof(resp));

    broadcastResponse(resp);
}

int randomBurstTime() {
    int r;
    r = rand() % 60;
    return r;
}

int randomResourceRequest() {
    int r;
    r = rand() % 5;
    return r;
}

void broadcastMessage(struct request r) {
    printf("Request from client%d: ", r.id);
    printf("P%d, D%d, S%d, burst %d", r.printers, r.disks, r.scanners, r.burstTime);
    printf("\n");
}

void broadcastResponse(struct response r) {
    printf("\n************************************\n");
    if (r.turnaround < 0) {
        broadcastRejected(r.id);
    } else {
        printf("Returned to client %d from server:\n", r.id);
        printf("%d (turnaround time), ", r.turnaround);
        printf("%f (average turnaround time)\n", r.avgTurnAround);
    }
    printf("\n************************************\n");
}

void broadcastRejected(int id) {
    printf("\n************************************\n");
    printf("Message from server:\n");
    printf("Request %d rejected, not enough resources\n", id);
    printf("\n************************************\n");
}
