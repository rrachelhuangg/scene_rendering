//implements vector functions
#include "vp.h"
#include <math.h>

double length(VP_T v){
    //calculates the length of the input vector
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

VP_T normalize(VP_T v){
    //normalizes the input vector
    VP_T norm_v;
    double ray_length = length(v);
    norm_v.x = v.x/ray_length;
    norm_v.y = v.y/ray_length;
    norm_v.z = v.z/ray_length;
    return norm_v;
}

double dot(VP_T v_1, VP_T v_2){
    //returns the dot product of the two input vectors
    return v_1.x*v_2.x+v_1.y*v_2.y+v_1.z*v_2.z;
}