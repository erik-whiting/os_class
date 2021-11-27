/***************************
client.c
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

int getUserInput();

struct payload {
  char jobName[8];
  int size;
  int needle;
  int pid;
  int haystack[5];
};

int main() {
  int haystack[5];

  printf("[Client] Build haystack:\n");
  for (int i = 0; i < 5; i ++) {
    haystack[i] = getUserInput();
  }

  printf("[Client] Define needle:");
  int needle = getUserInput();
  
  int size = sizeof(haystack);

  struct payload p;
  strcpy(p.jobName, "Payroll");
  p.size = size;
  p.needle = needle;
  p.pid = getpid();
  memcpy(p.haystack, haystack, size);

  printf("[Client] Sending array of size %d\n", p.size);
  printf("[Client] Searching for %d\n", p.needle);
  printf("[Client] Job name %s\n", p.jobName);
  printf("[Client] PID is %d\n", p.pid);

  char *fifoName = "./Common_FIFO";
  mkfifo(fifoName, 0666);
  int commonFifo = open(fifoName, O_WRONLY);
  if (commonFifo < 0) { printf("[Client] Couldn't open common fifo\n"); }
  write(commonFifo, &p, sizeof(p));
  close(commonFifo);

  printf("[Client] Sleeping to wait for server\n");
  sleep(5);
  int responseFromServer;
  int responseFifo = open("responseFifo", O_RDONLY);
  if (responseFifo < 0) { printf("[Client] Couldn't read response fifo\n"); }
  read(responseFifo, &responseFromServer, sizeof(int));

  printf("[Client] Response from server: %d\n", responseFromServer);
  
  
  close(responseFifo);
  unlink("responseFifo");

  return 0;
}

int getUserInput() {
  int userInput = -101;
  while (userInput < -100 || userInput > 100) {
    printf("Enter a number between -100 and 100: ");
    scanf("%d", &userInput);
  }
  return userInput;
}
