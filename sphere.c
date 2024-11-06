//implements sphere functions
#include <math.h>
#include "rt.h"
#include "vp.h"

int intersect_sphere(RAY_T ray, OBJ_T *object, double *t, VP_T *inter_pt, VP_T *normal){
    //checks to see if the input ray intersects the sphere and calculates the t value, the intersection point, and the normal
    SPHERE_T sphere = object->sphere;
    double A = 1.0;
    double B = 2*(ray.dir.x*(ray.origin.x-sphere.origin.x)+
                  ray.dir.y*(ray.origin.y-sphere.origin.y)+
                  ray.dir.z*(ray.origin.z-sphere.origin.z));
    double C = (ray.origin.x-sphere.origin.x)*(ray.origin.x-sphere.origin.x)+
               (ray.origin.y-sphere.origin.y)*(ray.origin.y-sphere.origin.y)+
               (ray.origin.z-sphere.origin.z)*(ray.origin.z-sphere.origin.z)-
               (sphere.radius*sphere.radius);
    double discriminant = (B*B)-(4*A*C);

    if(discriminant<=0){
        return 0;
    }
    double t_1, t_2;
    t_1 = (-B + sqrt(discriminant))/(2*A);
    t_2 = (-B - sqrt(discriminant))/(2*A);
    if(t_1==t_2 || t_1<t_2){
        *t=t_1;
    }
    else if(t_2<t_1){
        *t = t_2;
    }

    VP_T intersection_point;
    intersection_point.x=ray.origin.x+(*t)*ray.dir.x;
    intersection_point.y=ray.origin.y+(*t)*ray.dir.y;
    intersection_point.z=ray.origin.z+(*t)*ray.dir.z;
    *inter_pt=intersection_point;

    VP_T normal_vector; 
    normal_vector.x=intersection_point.x-sphere.origin.x;
    normal_vector.y=intersection_point.y-sphere.origin.y;
    normal_vector.z=intersection_point.z-sphere.origin.z;
    normal_vector = normalize(normal_vector);
    *normal=normal_vector;

    return 1;
}