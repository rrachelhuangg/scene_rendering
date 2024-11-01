#include <math.h>
#include "rt.h"
#include "vp.h"
int shadow_test(VP_T inter_pt, VP_T normal, OBJ_T *objects, int closest_object_index, LIGHT_T light_ray){ //static?
    RAY_T shadow_ray;
    shadow_ray.origin = inter_pt;
    shadow_ray.dir.x = light_ray.light_loc.x-inter_pt.x;
    shadow_ray.dir.y = light_ray.light_loc.y - inter_pt.y;
    shadow_ray.dir.z = light_ray.light_loc.z - inter_pt.z;
    shadow_ray.dir = normalize(shadow_ray.dir);
    double t; 
    for(int i = 0; i < NUM_OBJS; i++){
        if(i!= closest_object_index && objects[i].intersect(shadow_ray, &objects[i], &t, &inter_pt, &normal)){
            if(t>0){
                return 1;
            }
        }
    }
    return 0;
}

COLOR_T illuminate(RAY_T ray, VP_T inter_pt, OBJ_T *objects, int closest_object_index, VP_T normal, LIGHT_T light_ray){ //gets first pointer from the object (closest), scene should be the only thing passed to trace and illuminate,
    //illuminates the pixel appropriately based on the light location and the pixel location
    OBJ_T object = objects[closest_object_index];
    COLOR_T obj_color = object.color;
    if(object.checker==1){
        if((int)floor(inter_pt.x)+(int)floor(inter_pt.y)+(int)(inter_pt.z)&1){
            obj_color = object.color2;
        }
    }
    COLOR_T color; 
    color.R = 0;
    color.G = 0;
    color.B = 0;
    color.R = obj_color.R*0.1; //ambient lighting
    color.G = obj_color.G*0.1;
    color.B = obj_color.B*0.1;
    if(shadow_test(inter_pt, normal, objects, closest_object_index, light_ray)){
        return color;
    }
    VP_T l_vector;
    l_vector.x = light_ray.light_loc.x-inter_pt.x;
    l_vector.y = light_ray.light_loc.y-inter_pt.y;
    l_vector.z = light_ray.light_loc.z-inter_pt.z;
    l_vector = normalize(l_vector);
    double dL = length(l_vector);
    double attenuation = 1/(0.002*(dL*dL)+0.02*dL+0.2);
    double dp = dot(l_vector, normal); 
    if(dp>0){
        //diffuse lighting
        color.R += dp*obj_color.R*attenuation;
        color.G += dp*obj_color.G*attenuation;
        color.B += dp*obj_color.B*attenuation;
        //specular lighting
        VP_T r_vector;
        r_vector.x=l_vector.x-(normal.x*2*dp);
        r_vector.y=l_vector.y-(normal.y*2*dp);
        r_vector.z=l_vector.z-(normal.z*2*dp);
        r_vector=normalize(r_vector);
        double dp2 = dot(r_vector,ray.dir);
        if(dp2>0){
            color.R+=pow(dp2, 200)*attenuation;
            color.G+=pow(dp2, 200)*attenuation;
            color.B+=pow(dp2, 200)*attenuation;
        }
        
    }
    if(color.R>1){
        color.R = 1;
    }
    if(color.G>1){
        color.G = 1;
    }
    if(color.B>1){
        color.B = 1;
    }
    return color;
}