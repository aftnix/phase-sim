#ifndef __MC_H__
#define __MC_H__

#include "lattice.h"
#include "mc.h"
#include "cfg.h"

int metropolis(struct Lattice *L, struct config cfg, double T );

#endif