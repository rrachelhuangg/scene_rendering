//ray tracer for an illuminated scene
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rt.h"
#include "vp.h"
#include "light.h"
#include "sphere.h"
#include "plane.h"

COLOR_T trace(RAY_T ray, SCENE_T *scene){
    //illuminates pixels based on how the ray intersects the scene's objects
    double closest_t = 1000;
    OBJ_T *closest_object = NULL;
    VP_T closest_int_pt;
    VP_T closest_normal;
    VP_T inter_pt;
    VP_T normal;

    OBJ_T *curr = scene->objs;
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
        return illuminate(ray, inter_pt, scene, closest_object, normal);
    }
    else{
        COLOR_T background;
        background.R = 0.3;
        background.G = 0.3;
        background.B = 0.5;
        return background;
    }
}

void init(SCENE_T *scene){
    //initialize the scene with the desired objects
    FILE *fp = fopen("scene2.txt","r");
    if(!fp){
        perror("Name of input scene file incorrect.");
        exit(1);
    }
    scene->objs=NULL;
    char object_type;
    while(fscanf(fp, " %c", &object_type)==1){
        if(object_type == 's'){
            OBJ_T *node = (OBJ_T *)malloc(sizeof(OBJ_T));
            if(node == NULL){
                exit(1);
            }
            fscanf(fp, "%lf %lf %lf", &node->sphere.origin.x, &node->sphere.origin.y, &node->sphere.origin.z);
            fscanf(fp, "%lf", &node->sphere.radius);
            fscanf(fp, "%lf %lf %lf", &node->color.R, &node->color.G, &node->color.B);
            node->checker = 0;
            node->intersect = intersect_sphere;
            node->next = scene->objs;
            scene->objs = node;       
        }
        else if(object_type == 'p'){
            OBJ_T *node = (OBJ_T *)malloc(sizeof(OBJ_T));
            if(node == NULL){
                exit(1);
            }
            fscanf(fp, "%lf %lf %lf", &node->plane.normal.x, &node->plane.normal.y, &node->plane.normal.z);
            fscanf(fp, "%lf", &node->plane.D);
            fscanf(fp, "%lf %lf %lf", &node->color.R, &node->color.G, &node->color.B);
            fscanf(fp, "%lf %lf %lf", &node->color2.R, &node->color2.G, &node->color2.B);
            node->checker = 1;
            node->intersect = intersect_plane;
            node->next = scene->objs;
            scene->objs = node;
        }
        else if(object_type == 'l'){
            fscanf(fp, "%lf %lf %lf", &scene->light.light_loc.x, &scene->light.light_loc.y, &scene->light.light_loc.z);
        }
    }
    fclose(fp);
}

int main(){
    SCENE_T scene;
    init(&scene);

    int NUM_ROWS = 480;
    int NUM_COLS = 640;

    scene.pixel_size = 1.0/((double)NUM_ROWS);
    scene.start_y = 0.5;
    scene.start_x = -((double)((double)NUM_COLS/(double)NUM_ROWS))/2.0;

    FILE *output_image_file = fopen("scene2output.ppm", "w");
    fprintf(output_image_file, "P6\n");
    fprintf(output_image_file, "%d %d\n", NUM_COLS, NUM_ROWS);
    fprintf(output_image_file, "%d\n", 255);

    int y, x;
    for(y = 0; y < NUM_ROWS; y++){
        for(x = 0; x < NUM_COLS; x++){
            RAY_T ray;
            ray.origin.x = 0;
            ray.origin.y = 0;
            ray.origin.z = 0;
            ray.dir.x = scene.start_x + (x*scene.pixel_size);
            ray.dir.y = scene.start_y - (y*scene.pixel_size);
            ray.dir.z = 1.0;

            ray.dir = normalize(ray.dir);

            unsigned char pixel[3];

            COLOR_T illuminated_color = trace(ray, &scene);
            pixel[0] = (unsigned char)(illuminated_color.R*255);
            pixel[1] = (unsigned char)(illuminated_color.G*255);
            pixel[2] = (unsigned char)(illuminated_color.B*255);
            fprintf(output_image_file, "%c%c%c", pixel[0], pixel[1], pixel[2]);
        }
    }

    fclose(output_image_file);

    while(scene.objs){
        OBJ_T *temp = scene.objs;
        scene.objs = scene.objs->next;
        free(temp);
    }

    return 0;
}
