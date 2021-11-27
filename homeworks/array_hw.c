/******************  arrayTest.c  **********************
*
*  Purpose: This program allows a user to enter an array
*	    of integers from the keyboard. The array is 
*           then printed to the screen
*
*  ... has an error
*******************************************************/
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/errno.h>
#include<fcntl.h>

int main(void)

{
int ArrayOne[10];

int i;
printf("Enter integers to an array:");
for(i=0;i<5;i++)
    {
    scanf("%d",&ArrayOne[i]);
    }

printf("The entered array is:");
for(i=0;i<10;i++)
    {
    printf("%d",ArrayOne[i]);
    }
}
