#ifndef VP_H
#define VP_H 

//vector type
typedef struct{
    double x;
    double y;
    double z;
} VP_T;

/*can add other vector operations?*/

double length(VP_T v);

VP_T normalize(VP_T v);

double dot(VP_T v_1, VP_T v_2);

#endif 