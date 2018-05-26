#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bloom.h"

int main() {
	int size = 0;
	int num_hash = 0;
	printf("Type the size and number of hash functions\n");
	scanf("%d %d", &size, &num_hash);
	struct bloom * table = create_bloom(size, num_hash);

	printf("Type <add X> or <check X>\n");
	while (1) {
		char command[10];
		scanf("%10s", command);
		int key = 0;
		if (strcmp(command, "exit") == 0) {
			break;
		}
		scanf("%d", &key);
		if (strcmp(command, "add") == 0) {
			if (add_bloom(table, key) == -1) {
				printf("Size overflow\n");
				free_bloom(table);
				return 0;
			}
		}
		if (strcmp(command, "check") == 0) {
			if (check_belong(table, key) == 0) {
				printf("Possibly\n");
			}
			else {
				printf("Not belong\n");
			}
		}
	}

	free_bloom(table);
	return 0;
}