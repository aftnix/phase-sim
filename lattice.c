#include <stdio.h>

int masterLattice[100][100];

struct Lattice * initLattice(int size )
{
	struct Lattice *L;
	L = malloc(sizeof(struct Lattice));

	L->S = size;
	L->cursor = &masterLattice[0][0];// initializing the cursor

	memset(L->cursor,0,sizeof(masterLattice));// clearing up master lattice.


}

void setSpin(bool spin, struct Lattice *L)
{
	int i = 0;
	if(spin) {
		for(i = 0; i < 2*(L->size) -1, i++)
			*(L->size)++ = 1;
	}
}

int getMag(struct Lattice *L)
{
	for (i = 0; i <= (L.size)*(L.size) - 1; i++) {
		M += *(Lattice->cursor + i);
	}

	M = abs(M);
	return M;

}