#include <stdio.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <time.h>
#include <sys/stat.h>

struct payload {
    int cpuBurst;
    char privateFifo[30];
};

int main() {
    char *fifoName = "./Common_FIFO";
    int thisPid = getpid();
    char privateFifoName[30];
    sprintf(privateFifoName, "privateFifo_%d", thisPid);

    struct payload p;
    srand(time(NULL));
    p.cpuBurst = rand() % 100;
    strcpy(p.privateFifo, privateFifoName);

    printf("%s\n", p.privateFifo);
    printf("Random Number: %d\n", p.cpuBurst);

    return 0;
}
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <time.h>
#include <sys/stat.h>

struct payload {
    int cpuBurst;
    char privateFifo[30];
};

int main() {
    char *fifoName = "./Common_FIFO";
    int thisPid = getpid();
    char privateFifoName[30];
    sprintf(privateFifoName, "privateFifo_%d", thisPid);

    struct payload p;
    srand(time(NULL));
    p.cpuBurst = rand() % 100;
    strcpy(p.privateFifo, privateFifoName);

    printf("%s\n", p.privateFifo);
    printf("Random Number: %d\n", p.cpuBurst);

    return 0;
}
