#include <stdlib.h>
#include <math.h>

#include "rnd.h"
#include "mc.h"

static inline int prev(int x, struct Lattice *L) {
	int y;
	if (x == 0) 
		y = L->S - 1;
	else
		y = x - 1;
	return y;
}

static inline int next(int x, struct Lattice *L) {
	int y;
	if (x == (L->S -1))
		y = 0;
	else
		y = x + 1;
	return y;
}

int metropolis(struct Lattice *L, struct config cfg, double T )
{
	int n, i, j, M = 0;


	double p,x;

	int bondE = 0; // Bond energy

	for (n = 1; n <= cfg.sweeps; n++) {
		for (i = 0; i <= L->S; i++) {
			for (j = 0; j <= L->S; j++) {
  				bondE = (*(L->cursor + L->S * i +j)) * (
  					*(L->cursor + (L->S) * i + prev(j, L))
  					+ *(L->cursor + (L->S) * i + next(j, L))
  					+ *(L->cursor + (L->S) * prev(i,L) + j)
  					+ *(L->cursor + (L->S) * next(i,L) + j));
  				p = exp(-2.0 * bondE/T); //calculating boltzman propablity.



  				x = (double)rand()/((double) RAND_MAX) ;
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