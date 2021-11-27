/**********************************   structTest.c   ********************
*   
*   Purpose: Uses a struct to hold name and count
*
*************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <fcntl.h>

int main (void)
{

  struct values {
     char string[13];
     int integer;
     float floating_number;
   } input;

  printf("Enter a string of 12 characters: ");
  scanf("%s", input.string);
  input.string[12] = '\0';

  printf("\nEnter an integer: ");
  scanf("%d", &input.integer);

  printf("\nEnter a floating point number: ");
  scanf("%e", &input.floating_number);

  float sum = input.floating_number + (float) input.integer;

  printf("\nYour string is %s", input.string);
  printf("\nThe sum of %d and %.2f is %.2f", input.integer, input.floating_number, sum);
  printf ("\nall done!\n");

}
