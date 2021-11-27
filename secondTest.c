/**** secondTest.c ****/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <fcntl.h>

int main(void) {
	int number;
	printf("Enter a number: ");
	scanf("%d", &number);

	printf("\nOutput: the integer is %d", number);
	printf("\nThe number doubles is %d\n", number * 2);
	printf("\nall done!\n");
}
