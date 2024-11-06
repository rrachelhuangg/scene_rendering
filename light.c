//implements light ray functions
#include <math.h>
#include "rt.h"
#include "vp.h"
static int shadow_test(VP_T inter_pt, VP_T normal, SCENE_T *scene, OBJ_T *closest_object){
    //checks if the pixel is in shadow or not
    RAY_T shadow_ray;
    shadow_ray.origin = inter_pt;
    shadow_ray.dir.x = scene->light.light_loc.x-inter_pt.x;
    shadow_ray.dir.y = scene->light.light_loc.y - inter_pt.y;
    shadow_ray.dir.z = scene->light.light_loc.z - inter_pt.z;
    shadow_ray.dir = normalize(shadow_ray.dir);
    double t; 
    OBJ_T *curr = scene->objs;
    while(curr!=NULL){
        if(curr!=closest_object && curr->intersect(shadow_ray, curr, &t, &inter_pt, &normal)){
            if(t>0){
                return 1;
            }
        }
        curr=curr->next;
    }
    return 0;
}

COLOR_T illuminate(RAY_T ray, VP_T inter_pt, SCENE_T *scene, OBJ_T *closest_object, VP_T normal){
    //illuminates the pixel appropriately based on the light location and the pixel location
    COLOR_T obj_color = closest_object->color;
    if(closest_object->checker==1){
        if((int)floor(inter_pt.x)+(int)floor(inter_pt.y)+(int)(inter_pt.z)&1){
            obj_color = closest_object->color2;
        }
    }
    COLOR_T color; 
    color.R = 0;
    color.G = 0;
    color.B = 0;
    color.R = obj_color.R*0.1; //ambient lighting
    color.G = obj_color.G*0.1;
    color.B = obj_color.B*0.1;
    if(shadow_test(inter_pt, normal, scene, closest_object)){
        return color;
    }
    VP_T l_vector;
    l_vector.x = scene->light.light_loc.x-inter_pt.x;
    l_vector.y = scene->light.light_loc.y-inter_pt.y;
    l_vector.z = scene->light.light_loc.z-inter_pt.z;
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