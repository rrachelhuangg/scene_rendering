//ray tracer for an illuminated sphere
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rt.h"
#include "vp.h"
#include "light.h"
#include "sphere.h"
#include "plane.h"

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
    double dp = dot(l_vector, normal); 
    if(dp>0){
        //diffuse lighting
        color.R += dp*obj_color.R;
        color.G += dp*obj_color.G;
        color.B += dp*obj_color.B;
        //specular lighting
        VP_T r_vector;
        r_vector.x=l_vector.x-(normal.x*2*dp);
        r_vector.y=l_vector.y-(normal.y*2*dp);
        r_vector.z=l_vector.z-(normal.z*2*dp);
        r_vector=normalize(r_vector);
        double dp2 = dot(r_vector,ray.dir);
        if(dp2>0){
            color.R+=pow(dp2, 200);
            color.G+=pow(dp2, 200);
            color.B+=pow(dp2, 200);
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

COLOR_T trace(RAY_T ray, OBJ_T *objects, LIGHT_T light_ray){
    //illuminates the pixel if the ray intersects the sphere, else the pixel is black
    double closest_t = 1000;
    int closest_object_index = -1;
    VP_T closest_int_pt;
    VP_T closest_normal;
    VP_T inter_pt;
    VP_T normal;

    for(int i = 0; i < NUM_OBJS; i++){
        double t;
        if(objects[i].intersect(ray, &objects[i], &t, &inter_pt, &normal)){ 
            if(t < closest_t){
                closest_t = t;
                closest_object_index = i;
                closest_int_pt = inter_pt;
                closest_normal = normal;
            }
        }
    }
    if(closest_object_index != -1){
        return illuminate(ray, closest_int_pt, objects, closest_object_index, closest_normal, light_ray);
    }
    else{
        COLOR_T background;
        background.R = 0.3;
        background.G = 0.3;
        background.B = 0.5;
        return background;
    }
}

void init(OBJ_T *objects){
    objects[0].checker = 0;
    objects[0].sphere.origin.x = 1;
    objects[0].sphere.origin.y = 1;
    objects[0].sphere.origin.z = 10;
    objects[0].sphere.radius = 1;
    objects[0].color.R = 1;
    objects[0].color.G = 0;
    objects[0].color.B = 0;
    objects[0].intersect = intersect_sphere;

    objects[1].checker = 0;
    objects[1].sphere.origin.x = 3;
    objects[1].sphere.origin.y = 3;
    objects[1].sphere.origin.z = 9;
    objects[1].sphere.radius = 1.5;
    objects[1].color.R = 0;
    objects[1].color.G = 0;
    objects[1].color.B = 1;
    objects[1].intersect = intersect_sphere;

    objects[2].checker = 1;
    objects[2].plane.normal.x = 0;
    objects[2].plane.normal.y = 1;
    objects[2].plane.normal.z = 0;
    objects[2].plane.D = 0.9;
    objects[2].color.R = 1.0;
    objects[2].color.G = 1.0;
    objects[2].color.B = 1.0;
    objects[2].intersect = intersect_plane;
}

int main(){
    OBJ_T *objects = (OBJ_T *)malloc(NUM_OBJS*sizeof(OBJ_T));

    init(objects);

    LIGHT_T light_ray;
    light_ray.light_loc.x = 5;
    light_ray.light_loc.y = 10;
    light_ray.light_loc.z = 0;

    printf("P6\n");
    printf("%d %d\n", 1000, 1000);
    printf("%d\n", 255);

    int y, x;
    for(y = 0; y < 1000; y++){
        for(x = 0; x < 1000; x++){
            RAY_T ray;
            ray.origin.x = 0;
            ray.origin.y = 0;
            ray.origin.z = 0;
            ray.dir.x = -0.5+(x/1000.0);
            ray.dir.y = 0.5-(y/1000.0);
            ray.dir.z = 1.0;

            ray.dir = normalize(ray.dir);

            unsigned char pixel[3];

            COLOR_T illuminated_color = trace(ray, objects, light_ray);
            pixel[0] = (unsigned char)(illuminated_color.R*255);
            pixel[1] = (unsigned char)(illuminated_color.G*255);
            pixel[2] = (unsigned char)(illuminated_color.B*255);
            printf("%c%c%c", pixel[0], pixel[1], pixel[2]);
        }
    }

    free(objects);

    return 0;
}
