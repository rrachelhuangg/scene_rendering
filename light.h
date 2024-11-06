#ifndef LIGHT_H
#define LIGHT_H

int shadow_test(VP_T inter_pt, VP_T normal, OBJ_T *objects, OBJ_T *closest_object, LIGHT_T light_ray);

COLOR_T illuminate(RAY_T ray, VP_T inter_pt, SCENE_T *scene, OBJ_T *closest_object, VP_T normal);

#endif 