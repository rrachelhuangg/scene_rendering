/*need a function prototype for intersect_plane*/
#ifndef PLANE_H
#define PLANE_H 

int intersect_plane(RAY_T ray, PLANE_T plane, double *t, VP_T *inter_pt, VP_T *normal);

#endif 