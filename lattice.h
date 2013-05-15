#ifndef __LATTICE_H__
#define __LATTICE_H__

#include <stdbool.h>

struct Lattice{
	float E; //Total energy
	float M; // Magnetization
	int S;// Size

	int *cursor; //Pointer to the actual data block.
};



struct Lattice * initLattice(int size); // Initialize a 2d lattice of specified size
float calculateE(struct Lattice *L); // Calculate the total energy of the lattice.
void setSpinEx(bool spin, struct Lattice *L); // Sets all lattice sites to up/down.
int getMag(struct Lattice *L); // Gets the current magnetization of the lattice.
void setSpin(struct Lattice *L, int i, int j, bool spin); // Sets the spin at (i,j)
bool getSPin(struct Lattice *L, int i, int j); // Gets the spin at (i,j)

#endif