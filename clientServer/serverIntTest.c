/*********************************   serverIntTest.c   *****************************
*
* makes 2 fifos named FIFO_to_server and FIFO_to_client
* opens FIFO_to_server for reading and FIFO_to_client for writing
* reads an integer from the client, doubles it and writes it back to the client
*
**********************************************************************************/

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
  int fda;		// internal handle for FIFO_to_server
  int fdb;		// internal handle for FIFO_to_client
  int finish;		// lets me know how many bytes were read ???
  int number[3];		// integer read from client
  int numberDoubled;	// integer doubled and written to client

  /* Create the fifos and open them  */
  if ((mkfifo("FIFO_to_server",0666)<0 && errno != EEXIST))
	{
	perror("cant create FIFO_to_server");
	exit(-1);
	}

   if ((mkfifo("FIFO_to_client",0666)<0 && errno != EEXIST))
	{
	perror("cant create FIFO_to_client");
	exit(-1);
	}

  if((fda=open("FIFO_to_server", O_RDONLY))<0)
     printf("cant open fifo to write");

  if((fdb=open("FIFO_to_client", O_WRONLY))<0)
     printf("cant open fifo to read");

  int sum = 0;
  finish=read(fda, &number, sizeof(number)); 	//read the number

  for (int i = 0; i < 3; i++) {
    sum += number[i];
  }

  printf("Server: Writing sum to client\n");
  write(fdb, &sum, sizeof(sum));

  if(finish == 1)
    printf("\nServer: This says I am ready to close ");

  close(fda);
  close(fdb);
  unlink("FIFO_to_server");
  unlink("FIFO_to_client");



}
