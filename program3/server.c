#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/stat.h>


struct request {
    int id;
    int printers;
    int disks;
    int scanners;
    int burstTime;
};

struct resources {
    int printers;
    int disks;
    int scanners;
};

struct response {
    int   id;
    int   turnaround;
    float avgTurnAround;
};

void  broadcastResources (struct resources r);
void  broadcastRequest   (struct request req);
int   checkResources     (struct resources r, struct request req);
void  updateResources    (struct resources* r, struct request req);
float avgTurnAround      (int count, int times[3]);
void  respondToClient    (struct response r);

int main() {
    char *commonFifoName = "./commonFIFO";
    struct resources r = { .printers = 5, .disks = 3, .scanners = 6 };
    struct response responses[3];
    int burstTimes[] = {0, 0, 0};
    int turnaround = 0;
    broadcastResources(r);

    // Create then read common fifo
    mkfifo(commonFifoName, 0666);

    int requests_we_can_handle = 0; // To help us average the turnaround later
    for (int i = 0; i < 3; i++) {
        int commonFifo = open(commonFifoName, O_RDONLY);
        if (commonFifo < 0) { printf("[Server] Couldn't open FIFO"); }
        struct request req;
        read(commonFifo, &req, sizeof(req));
        struct response resp;
        resp.id = req.id;
        if (checkResources(r, req) > 0) {
            requests_we_can_handle++;
            updateResources(&r, req);
            broadcastRequest(req); printf("allocated\n");
            burstTimes[i] = req.burstTime;
            turnaround += req.burstTime;
            resp.turnaround = turnaround;
            broadcastResources(r);
        } else {
            broadcastRequest(req); printf("rejected\n");
            resp.turnaround = -1;
            resp.avgTurnAround = -1;
        }
        responses[i] = resp;
        close(commonFifo);
    }

    float calculatedAvgTurnaround = avgTurnAround(requests_we_can_handle, burstTimes);
    for (int j = 0; j < 3; j++) {
        responses[j].avgTurnAround = calculatedAvgTurnaround;
        respondToClient(responses[j]);
    }
    
    printf("[Server] Average turnaround: %f\n", calculatedAvgTurnaround);
}

void broadcastResources(struct resources r) {
    printf("[Server] P%d D%d S%d available\n", r.printers, r.disks, r.scanners);
}

void broadcastRequest(struct request req) {
    printf("[Server] P%d D%d S%d ", req.printers, req.disks, req.scanners);
}

int checkResources(struct resources r, struct request req) {
    // Return 0 if there are insufficient resources
    // Otherwise, return 1.
    if (req.printers > r.printers) { return 0; }
    if (req.disks > r.disks) { return 0; }
    if (req.scanners > r.scanners) { return 0; }

    return 1;
}

void updateResources(struct resources* r, struct request req) {
    if (checkResources(*r, req) > 0)  {
        r->printers -= req.printers;
        r->disks -= req.disks;
        r->scanners -= req.scanners;
    }
}

float avgTurnAround(int count, int times[3]) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += times[i];
    }
    float average = (float) total / (float) count;
    return average;
}

void respondToClient(struct response r) {
    char privateFifoName[30];
    sprintf(privateFifoName, "privateFifo_%d", r.id);
    int privateFifo = open(privateFifoName, O_WRONLY);
    write(privateFifo, &r, sizeof(r));
    if (privateFifo < 0) { printf("[Server] Couldn't write to client"); }
    close(privateFifo);
}
