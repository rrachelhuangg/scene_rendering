//ray tracer for an illuminated sphere
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rt.h"
#include "vp.h"
#include "light.h"
#include "sphere.h"
#include "plane.h"

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
