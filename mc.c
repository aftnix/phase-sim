#include <stdlib.h>
#include <math.h>

#include "rnd.h"
#include "lattice.h"
#include "cfg.h"

float metropolis(struct Lattice *L, struct config cfg, double T )
{
	int n, i, j, M = 0;


	double p,x;

	int bondE = 0; // Bond energy

	for (n = 1; n <= cfg.sweeps; n++) {
		for (i = 0; i <= L->S; i++) {
			for (j = 0; j <= L->S; j++) {
  				bondE = *(L->cursor + (L->S) * i + j) *
  					(*(L->cursor + (L->S)* i + j - 1) +
  						*(L->cursor + L->S * i + j + 1) +
  							*(L->cursor + L->S * (i - 1) +j) +
  								*(L->cursor) + L->S * (i + 1) + j);

  				p = exp(-2.0 * bondE/T); //calculating boltzman propablity.

  				x = rnd(cfg.seed) * 16807;
  				if (p > x) {
  					*(L->cursor + L->S * i + j) = - *(L->cursor + L->S * i + j); 
  				}

			}
		}
		if (n > cfg.ignore) {
			M += getMag(L);
		}
	}

	return M;

}