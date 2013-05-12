#include <stdio.h>
#include <stdlib.h>

struct config cfg;

struct Lattice * initConfig(int size, int ignore, int sweeps,
	float fT, float iT, int steps, int seed) {
	cfg.size = size;
	cfg.sweeps = sweeps;
	cfg.seed = seed;
	cfg.ignore = ignore;
	cfg.init_T = iT;
	cfg.final_T = fT;
	cfg.steps = steps;

	struct Lattice *L = initLattice(cfg.size);
	return L;	
}

int main() {
	struct Lattice *L = initConfig(40, 2000, 12000, 4.0, 0.0, 50, 3509 );

	float dT = (cfg.final_T - cfg.init_T)/ cfg.steps;

	for (k = 0; k <= cfg.steps; k++) {
		M = 0.0;
		M = matropolis(cfg, L);

		M_av = M/(cfg.sweeps - cfg.ignore);
		T += dT;
	}
}