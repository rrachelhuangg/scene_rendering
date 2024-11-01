#ifndef SPHERE_H
#define SPHERE_H

#include "rt.h"
#include "vp.h"

int intersect_sphere(RAY_T ray, OBJ_T *object, double *t, VP_T *inter_pt, VP_T *normal);

#endif 