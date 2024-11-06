#ifndef VP_H
#define VP_H 

typedef struct{
    double x;
    double y;
    double z;
} VP_T;

double length(VP_T v);

VP_T normalize(VP_T v);

double dot(VP_T v_1, VP_T v_2);

#endif 