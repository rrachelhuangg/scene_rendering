//ray tracer for an illuminated sphere
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rt.h"
#include "vp.h"
#include "light.h"
#include "sphere.h"
#include "plane.h"

//to-do:creative image, scene_t refactoring, aspect ratio, fix file I/O

COLOR_T trace(RAY_T ray, OBJ_T *objects, LIGHT_T light_ray){
    //illuminates the pixel if the ray intersects the sphere, else the pixel is black
    double closest_t = 1000;
    OBJ_T *closest_object = NULL;
    VP_T closest_int_pt;
    VP_T closest_normal;
    VP_T inter_pt;
    VP_T normal;

    OBJ_T *curr = objects;
    while(curr != NULL){
        double t;
        if(curr->intersect(ray, curr, &t, &inter_pt, &normal)){
            if(t < closest_t){
                closest_t = t;
                closest_object = curr;
                closest_int_pt = inter_pt;
                closest_normal = normal;
            }
        }
        curr=curr->next;
    }

    if(closest_object!=NULL){
        return illuminate(ray, closest_int_pt, objects, closest_object, closest_normal, light_ray);
    }
    else{
        COLOR_T background;
        background.R = 0.3;
        background.G = 0.3;
        background.B = 0.5;
        return background;
    }
}

void init(OBJ_T **objects){
    *objects = NULL;
    for(int i = 0; i < NUM_OBJS; i++){
        OBJ_T *node = (OBJ_T *)malloc(sizeof(OBJ_T));
        if(node == NULL){
            exit(1);
        }
        if(i==0){
            node->checker = 0;
            node->sphere.origin.x = 1;
            node->sphere.origin.y = 1;
            node->sphere.origin.z = 10;
            node->sphere.radius = 1;
            node->color.R = 1;
            node->color.G = 0;
            node->color.B = 0;
            node->intersect = intersect_sphere;
        }
        else if(i==1){
            node->checker = 0;
            node->sphere.origin.x = 3;
            node->sphere.origin.y = 3;
            node->sphere.origin.z = 9;
            node->sphere.radius = 1.5;
            node->color.R = 0;
            node->color.G = 0;
            node->color.B = 1;
            node->intersect = intersect_sphere;
        }
        else if(i==2){
            node->checker = 1;
            node->plane.normal.x = 0;
            node->plane.normal.y = 1;
            node->plane.normal.z = 0;
            node->plane.D = 0.9;
            node->color.R = 1.0;
            node->color.G = 1.0;
            node->color.B = 1.0;
            node->intersect = intersect_plane;
        }
        node->next = *objects;
        *objects = node;
    }
}

int main(){
    OBJ_T *objects;
    init(&objects);

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

    while(objects){
        OBJ_T *temp = objects;
        objects = objects->next;
        free(temp);
    }

    return 0;
}
