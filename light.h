//atten factor = 1/(0.002 * dl^2 + 0.02 * dl + 0.2) (multiply by diffuse and spectral)

//illuminate and shadow should go here
#ifndef LIGHT_H
#define LIGHT_H

int shadow_test(VP_T inter_pt, VP_T normal, OBJ_T *objects, OBJ_T *closest_object, LIGHT_T light_ray);

COLOR_T illuminate(RAY_T ray, VP_T inter_pt, SCENE_T *scene, OBJ_T *closest_object, VP_T normal);

#endif 