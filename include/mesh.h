#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

typedef struct Mesh {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int indexCount;
    unsigned int vertexCount;
} Mesh;

Mesh meshCreate(float *vertices, unsigned int vertexCount, unsigned int *indices, unsigned int indexCount);

void meshDraw(Mesh *mesh);

#endif
