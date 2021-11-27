/***************************
server.c
*****************************
* Programmer: Erik Whiting
*
* Course: CSCI 4354.01
*
* Date: September 30, 2021
*
* Assignment: Program #2: Client Server Program
*
* Environment: Ubuntu Linux using the gcc compiler
*
* Files Included: client.c, server.c
*
* Purpose: To read a struct via FIFO from a client. Struct will contain a pid, array, and
*
an integer to search for. Server will search for that integer within the array. If it is found,
*
the value to be returned will be the array index of that location, otherwise the value to be
*
returned will be -1. The server will create a FIFO named after the PID provided in the
*
struct and write to it the value to be returned.
*
* Input: Struct from client containing a pid, array size, integer array, and integer to be found
*
* Preconditions: Elements in the client-provided struct are between -100 and 100
*
* Output: If the integer to be searched for exists in the client-provided array, the output will
*
be the index location in the client-provided array of the searched-for integer.
*
If the integer does not exist in the array, the output will be “-1”
*
* Postconditions: The output is any number between -1 and the size of the client-provided array
*
* Algorithm:
*
Create common_FIFO
*
Open common_FIFO for reading
*
Read client-provided struct into local struct variable
*
Declare found to be false
*
Declare output to be -1
*
For each item in the struct array
*
if integer to search for matches the item
*
set found to true
*
set output to be current array location
*
Declare fifo_name to be “fifo_
” and the struct PID
*
Create FIFO named for the value stored in fifo_name
*
Open FIFO for writing
*
Write output to FIFO
*
Close and detach FIFO
*
* Estimates
Actuals
* Design: 1 hour
TBD
* Implementation: 2 hours TBD
* Testing: 1 hour
TBD
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

struct payload {
  char jobName[8];
  int size;
  int needle;
  int pid;
  int haystack[5];
};

int main() {
  char *fifoName = "./Common_FIFO";
  struct payload p;

  printf("[Server] Sleeping to wait for client\n");
  sleep(20);

  int commonFifo = open(fifoName, O_RDONLY);
  if (commonFifo < 0) { printf("[Server] Couldn't read common fifo\n"); }
  read(commonFifo, &p, sizeof(p));
  close(commonFifo);
  unlink(fifoName);

  printf("[Server] Received array of size %d\n", p.size);
  printf("[Server] Searching for %d\n", p.needle);
  printf("[Server] Job Name: %s\n", p.jobName);
  printf("[Server] PID: %d\n", p.pid);

  int index = -1;;
  for (int i = 0; i < p.size; i++) {
    if (p.haystack[i] == p.needle) {
      index = i;
    }
  }

  printf("[Server] Preparing to write response: %d\n", index);

  mkfifo("responseFifo", 0666);
  int responseFifo = open("responseFifo", O_WRONLY);
  if (responseFifo < 0) { printf("[Server] Coudln't open response fifo\n"); }  
  write(responseFifo, &index, sizeof(int));
  close(responseFifo);

  return 0;
}