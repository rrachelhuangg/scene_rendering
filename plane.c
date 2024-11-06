//implements plane functionality
#include "rt.h"
#include "vp.h"
int intersect_plane(RAY_T ray, OBJ_T *object, double *t, VP_T *inter_pt, VP_T *normal){
    //checks to see if the input ray intersects the plane and calculates the t value, the intersection point, and the normal
    PLANE_T plane = object->plane;
    double dp = dot(plane.normal, ray.dir);
    if(dp==0){
        return 0;
    }
    else{
        *t = -(dot(plane.normal, ray.origin)+plane.D)/dp;
        if(*t < 0){
            return 0;
        }
        else{
            VP_T intersection_point;
            intersection_point.x=ray.origin.x+(*t)*ray.dir.x;
            intersection_point.y=ray.origin.y+(*t)*ray.dir.y;
            intersection_point.z=ray.origin.z+(*t)*ray.dir.z;
            *inter_pt=intersection_point;
            *normal = plane.normal;
            return 1;
        }
    }
}