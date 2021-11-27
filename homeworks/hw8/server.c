#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/stat.h>

struct privateFIFOStruct {
  char fifoName[14];
  int clientPID;
};

int main() {
  struct privateFIFOStruct fifoStruct;

  mkfifo("publicFIFO", 0666);
  printf("Sleeping while client writes to public FIFO\n");
  sleep(5);

  int fda = open("publicFIFO", O_RDONLY);
  if (fda < 0) { printf("Couldn't open public FIFO\n"); }
  read(fda, &fifoStruct, sizeof(fifoStruct));
  close(fda);
  unlink("publicFIFO");

  char fifoName[14];
  sprintf(fifoName, "FIFO_%d", fifoStruct.clientPID);

  int privateFIFO = open(fifoName, O_WRONLY);
  if (privateFIFO < 1) { printf("Couldn't open private FIFO\n"); }
  char message[21] = "Message from server\n";
  write(privateFIFO, &message, sizeof(message));
  close(privateFIFO);

  printf("Server wrote message: %s", message);
  printf("to fifo %s\n", fifoName);

  return 0;
}