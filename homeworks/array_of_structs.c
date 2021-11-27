#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct item {
	char name[50];
	int integer1;
	int integer2;
};

int main() {
	struct item items[3];

	for (int i = 0; i <= 2; i++) {
		char name[50];
		int integer1;
		int integer2;
		printf("Enter a name: ");
		scanf("%s", name);
		printf("Enter an integer: ");
		scanf("%d", &integer1);
		printf("Enter another integer: ");
		scanf("%d", &integer2);
		struct item new_item;
		new_item.integer1 = integer1;
		new_item.integer2 = integer2;
		strncpy(new_item.name, name, sizeof(name));
		items[i] = new_item;
	}

	for (int i = 0; i <= 2; i++) {
		int sum = items[i].integer1 + items[i].integer2;
		printf("Name: %s, Sum of integer 1 and 2: %d\n", items[i].name, sum);
	}

	return 0;
}
