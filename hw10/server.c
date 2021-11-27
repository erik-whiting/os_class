#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/stat.h>

struct payload {
    int cpuBurst;
    char privateFifo[30];
};

int main() {
    char *fifoName = "./Common_FIFO";
    mkfifo(fifoName, 0666);

}
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/stat.h>

struct payload {
    int cpuBurst;
    char privateFifo[30];
};

int main() {
    char *fifoName = "./Common_FIFO";
    mkfifo(fifoName, 0666);
    
}
