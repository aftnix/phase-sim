#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../lattice.h"

int main() {
	struct Lattice *L;
	int i,j;
	
	L = initLattice(10);

	setSpinEx(1, L);

	printf("Magnetization M : %d\n", getMag(L));

	for (i = 0; i < 5; i++)
		for (j = 0; j < 5; j++)
			if (i == j) {
				setSpin(L, i, j, 1);
			}
			else
				setSpin(L, i, j, 0);

	printf("Magnetization M: %d\n", getMag(L));
	return 0;		

}