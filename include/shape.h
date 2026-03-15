#ifndef SHAPE_H
#define SHAPE_H

typedef struct Shape {
    float *vertices;
    unsigned int vertexCount;
    unsigned int stride;
    unsigned int *indices;
    unsigned int indexCount;
} Shape;

Shape cube(void);

Shape sphere(void);

Shape testSphere(void);

#endif
