#ifndef SHAPE_H
#define SHAPE_H

#include "physics.h"

typedef struct Shape {
    float *vertices;
    unsigned int vertexCount;
    unsigned int stride;
    unsigned int *indices;
    unsigned int indexCount;
} Shape;

Shape cube(void);

Shape sphere(void);

Shape grid(Body *bodies, int objCount);

Shape testSphere(void);

#endif
