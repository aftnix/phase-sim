#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "progressbar.h"
#include "mc.h"

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
	setSpinEx(1, L);
	return L;	
}

int main() {
	int k;
	double M, M_av;
	double T;
	struct Lattice *L = initConfig(10, 2000, 12000, 4.0, 0.0, 50, 3509 );

	T = cfg.init_T;

	FILE *fp;
	fp = fopen("phase-sim.dat", "w");

	double dT = (cfg.final_T - cfg.init_T)/ cfg.steps;

	progressbar *progress = progressbar_new("Simulating", cfg.steps);

	for (k = 0; k <= cfg.steps; k++) {
		M = 0.0;
		M = metropolis(L, cfg, T);
		M_av = M/(cfg.sweeps - cfg.ignore);
		fprintf(fp,"%f    %f\n", T, M_av );
		T += dT;
		progressbar_inc(progress);
	}
	progressbar_finish(progress);
}