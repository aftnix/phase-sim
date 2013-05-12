#include <stdlib.h>
#include <rnd.h>

float metropolis(struct Lattice *L, struct config cfg )
{
	int n, i, j;

	int bondE = 0; // Bond energy

	for (n = 1; n <= cfg.sweeps; n++) {
		for (i = 0; i <= L->size; i++) {
			for (j = 0; j <= L->size; j++) {
  				bondE = *(L->cursor + (L->size) * i + j) *
  					(*(L->cursor + (L->size)* i + j - 1) +
  						*(L->cursor + L->size * i + j + 1) +
  							*(L->cursor + L->size * (i - 1) +j) +
  								*(L->cursor) + L->size * (i + 1) j);

  				p = exp(-2.0 * bondE/T); //calculating boltzman propablity.

  				x = rnd(cfg.seed) * 16807;
  				if (p > x) {
  					*(L->cursor + L->size * i + j) = - *(L->cursor + L->size * i + j); 
  				}

			}
		}
		if (n > cfg.ignore) {
			M += getMag(L);
		}
	}

	return M;

}