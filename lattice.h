
#ifndef __LATTICE_H__
#define __LATIICE_H__

struct Lattice{
	float E; //Total energy
	float M; // Magnetization
	int S// Size

	int *cursor; //Pointer to the actual data block.
};

void initLattice(int size); // Initialize a 2d lattice of specified size
float calculateE(struct Lattice *L); // Calculate the total energy of the lattice.
void setSpin(bool spin); // Sets all lattice sites to up/down.
int getMag(struct Lattice *L); // Gets the current magnetization of the lattice.
#endif

