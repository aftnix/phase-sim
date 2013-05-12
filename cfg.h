#ifndef __CFG_H__
#define __CFG_H__
struct config {
	int sweeps;
	int seed;
	int steps;
	int ignore;
	float init_T;
	float final_T;
	int size;
};

#define MAGIC
#endif