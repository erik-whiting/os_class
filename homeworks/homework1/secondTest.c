/**********************  secondTest.c  ********************
*
*  Purpose: This program gets a single character from the 
*           keyboard and prints it as a two-character string
*
************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <fcntl.h>

int main(void)
{
  
  int firstNumber;
  int secondNumber; 
  printf("Enter first number: ");
  scanf("%d", &firstNumber);
  printf("Enter second integer: ");
  scanf("%d", &secondNumber);
  int sum = firstNumber + secondNumber;
  printf("\nThe sum of %d and %d is %d", firstNumber, secondNumber, sum);
  printf ("\nall done!\n");
   
}
