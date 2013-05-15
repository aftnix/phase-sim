#include <stdio.h>
#include <stdlib.h>

#include "../mc.h"
#include "../cfg.h"


int main() {
	int M;
	int i;

	struct Lattice *L;
	struct config cfg;

	cfg.sweeps = 12000;
	cfg.seed = 3509;
	cfg.ignore = 2000;

	L = initLattice(50);

	for (i = 0; i < 5; i++) {
		M = metropolis(L, cfg, i/2.0);
		printf(" Magnetization for T[%f] is M[%d]\n", i/2.0, M);

	}
	return 0;
}