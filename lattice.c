#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lattice.h"

int masterLattice[100][100];

struct Lattice * initLattice(int size )
{
	struct Lattice *L;
	L = malloc(sizeof(struct Lattice));

	L->S = size;
	L->cursor = &masterLattice[0][0];// initializing the cursor

	memset(L->cursor,0,sizeof(masterLattice));// clearing up master lattice.

	return L;

}

void setSpinEx(bool spin, struct Lattice *L)
{
	int i = 0;
	if(spin) {
		for(i = 0; i < 2*(L->S) -1; i++)
			*(L->cursor)++ = 1;
	}
}

int getMag(struct Lattice *L)
{
	int i;
	int M;
	for (i = 0; i <= (L->S)*(L->S) - 1; i++) {
		M += *(L->cursor + i);
	}

	M = abs(M);
	return M;

}

void setSpin(struct Lattice *L, int i, int j, bool spin) 
{
	*(L->cursor + L->S * i + j) = spin;
}

int getSpin(struct Lattice *L, int i, int j)
{
	return *(L->cursor + L->S * i + j);
}