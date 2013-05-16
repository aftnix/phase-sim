#include <stdio.h>

#include "../rnd.h"

int main() {
	int i;
	printf("%f\n", rnd(100));
	printf("%f\n", rnd(100));

	for (i = 0; i < 1000; i++)
		printf("%f\n", rnd(3509));
	return 0;

}