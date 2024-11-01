//ray tracer for an illuminated sphere
#include <stdio.h>
#include <math.h>
#include "rt.h"
#include "vp.h"
#include "light.h"
#include "sphere.h"

int intersect_sphere(RAY_T ray, SPHERE_T sphere, double *t, VP_T *inter_pt, VP_T *normal){
    //checks to see if the input ray intersects the sphere and calculates the t value, the intersection point, and the normal
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
    else{
        *t=t_2;
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

COLOR_T illuminate(RAY_T ray, VP_T inter_pt, COLOR_T obj_color, VP_T normal, LIGHT_T light_ray){ 
    //illuminates the pixel appropriately based on the light location and the pixel location
    COLOR_T color; 
    color.R = 0;
    color.G = 0;
    color.B = 0;
    color.R = obj_color.R*0.1; //ambient lighting
    color.G = obj_color.G*0.1;
    color.B = obj_color.B*0.1;
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

COLOR_T trace(RAY_T ray, SPHERE_T *spheres, COLOR_T *colors, LIGHT_T light_ray){
    //illuminates the pixel if the ray intersects the sphere, else the pixel is black
    double closest_t = 1000;
    int closest_object_index = -1;
    VP_T closest_int_pt;
    VP_T closest_normal;
    VP_T inter_pt;
    VP_T normal;

    for(int i = 0; i < NUM_OBJS; i++){
        double t;
        if(intersect_sphere(ray, spheres[i], &t, &inter_pt, &normal)){ 
            if(t < closest_t){
                closest_t = t;
                closest_object_index = i;
                closest_int_pt = inter_pt;
                closest_normal = normal;
            }
        }
    }
    if(closest_object_index != -1){
        return illuminate(ray, inter_pt, colors[closest_object_index], normal, light_ray);
    }
    else{
        COLOR_T black;
        black.R = 0;
        black.G = 0;
        black.B = 0;
        return black;
    }
}


int main(){
    SPHERE_T sphere;
    sphere.origin.x = 0;
    sphere.origin.y = 0;
    sphere.origin.z = 10;
    sphere.radius = 1;

    COLOR_T sphere_color;
    sphere_color.R = 1;
    sphere_color.G = 0;
    sphere_color.B = 0;

    SPHERE_T sphere2;
    sphere2.origin.x = 2; 
    sphere2.origin.y = 2;
    sphere2.origin.z = 10;
    sphere2.radius = 1;

    COLOR_T sphere2_color;
    sphere2_color.R = 0;
    sphere2_color.G = 0;
    sphere2_color.B = 1;

    SPHERE_T spheres[2];
    spheres[0] = sphere;
    spheres[1] = sphere2;
    COLOR_T colors[2];
    colors[0] = sphere_color;
    colors[1] = sphere2_color;

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

            //save closest_t, closest_int_pt, closest_normal, closest_object_index
            COLOR_T illuminated_color = trace(ray, spheres, colors, light_ray);
            pixel[0] = (unsigned char)(illuminated_color.R*255);
            pixel[1] = (unsigned char)(illuminated_color.G*255);
            pixel[2] = (unsigned char)(illuminated_color.B*255);
            printf("%c%c%c", pixel[0], pixel[1], pixel[2]);
        }
    }

    return 0;
}
