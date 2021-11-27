/***************************   clientIntTest.c   ************************
*
* opens FIFO_to_server for writing and FIFO_to_client for reading
* writes an integer to the server and reads the integer value doubled
*
*:*********************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/stat.h>

int main (void)
{
  int fda;		// to write to server
  int fdb;		// to read response from server
  int number[3];		// number from client
  int sum;	// sum of numbers to be written to client

  if((fda=open("FIFO_to_server", O_WRONLY))<0)
     printf("cant open fifo to write");

  if((fdb=open("FIFO_to_client", O_RDONLY))<0)
     printf("cant open fifo to read");

  for (int i = 0; i < 3; i++) {
   printf("Client: Please enter an integer: ");
   scanf("%d", &number[i]);
  }


  write(fda, &number, sizeof(number));
  printf("\nClient: Got the integer sent, now waiting for response ");
  read(fdb, &sum, sizeof(sum));
  printf("\nClient: received from server %d", sum);

  close(fda);
  close(fdb);

  printf ("\nall done!\n");

}
