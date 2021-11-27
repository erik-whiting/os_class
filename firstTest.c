#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <fcntl.h>

int main(void) {
	char singlechar[1];
	char outchar[2];

	printf("Enter a character: ");
	scanf("%c", singlechar);
	outchar[0] = singlechar[0];
	outchar[1] = 0;

	printf("\nOutput: the character is %s", outchar);
	printf("\nMessage here\n");
	printf("\nall done!\n");
}
