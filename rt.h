
#ifndef RT_H
#define RT_H
#define NUM_OBJS 3
#include "vp.h"
#include <stdio.h>

//ray type
typedef struct{
    VP_T origin;
    VP_T dir;
} RAY_T;

//ray type
typedef struct{
    double R;
    double G;
    double B;
} COLOR_T;

typedef struct{
    VP_T origin;
    double radius;
} SPHERE_T;

typedef struct{
    VP_T normal; 
    double D;
} PLANE_T;

//object type
typedef struct OBJ{
    union{
        SPHERE_T sphere;
        PLANE_T plane;
    };
    // char type;
    COLOR_T color;
    int checker;
    COLOR_T color2;
    int (*intersect)(RAY_T ray, struct OBJ *obj, double *t, VP_T *int_pt, VP_T *normal);
    struct OBJ *next;
} OBJ_T;

typedef struct{
    VP_T light_loc;
    double D;
} LIGHT_T;

typedef struct{
    OBJ_T *objs;
    LIGHT_T light;
    double start_x;
    double start_y;
    double pixel_size;
} SCENE_T;

//light type should also be here
//intersect sphere should go in sphere.c

//need all the function prototypes to safeguard against mismatch

// int intersect_sphere(RAY_T ray, SPHERE_T sphere, double *t, VP_T *inter_pt, VP_T *normal);

// COLOR_T illuminate(RAY_T ray, VP_T inter_pt, SCENE_T *scene, OBJ_T *closest_object, VP_T normal);

// COLOR_T trace(RAY_T ray, SPHERE_T sphere, COLOR_T sphere_color, VP_T light_loc);

#endif
