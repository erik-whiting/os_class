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
  int pid = getpid();
  char fifoName[14];
  sprintf(fifoName, "FIFO_%d", pid);
  struct privateFIFOStruct fifoStruct;
  fifoStruct.clientPID = pid;
  strcpy(fifoStruct.fifoName, fifoName);

  int publicFIFO = open("publicFIFO", O_WRONLY);
  write(publicFIFO, &fifoStruct, sizeof(fifoStruct));
  close(publicFIFO);
  
  int privateFIFO = mkfifo(fifoStruct.fifoName, 0666);
  printf("Client created FIFO with name: %s\n", fifoStruct.fifoName);
  printf("Client waiting on server\n");
  sleep(8);
  
  char message[21];
  int fda = open(fifoStruct.fifoName, O_RDONLY);
  if (fda < 0) { printf("Couldn't reopen private FIFO\n"); }
  read(fda, &message, sizeof(message));
  printf("Client received message:\n");
  printf("%s\n", message);
  
  close(fda);
  unlink(fifoStruct.fifoName);

  return 0;
}