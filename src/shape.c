#include "shape.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cglm/cglm.h>

Shape cube(void) {
    unsigned int stride = 3;
    unsigned int vertexCount = 8;
    float *vertices = malloc(vertexCount * stride * sizeof(float));
    float cubeVertices[] = {
        //position         //color            /texture
        -0.5f, -0.5f, 0.5f, //bot left
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
    };
    memcpy(vertices, cubeVertices, sizeof(cubeVertices));

    unsigned int indexCount = 36;
    unsigned int *indices = malloc(sizeof(unsigned int) * indexCount);
    unsigned int cubeIndices[] = {
        0, 1, 3,
        2, 1, 0,

        4, 5, 7,
        6, 5, 4,

        0, 4, 7,
        0, 3, 7,

        1, 2, 6,
        1, 5, 6,

        1, 3, 7,
        1, 5, 7,

        0, 2, 4,
        2, 6, 4,

    };
    memcpy(indices, cubeIndices, sizeof(cubeIndices));

    Shape cubeShape = {
        .vertices = vertices,
        .vertexCount = vertexCount,
        .stride = stride,
        .indices = indices,
        .indexCount = indexCount
    };

    return cubeShape;
}

float *sphereVertices(int sectors, int stacks, int stride) {
    float dTheta = 2.0 * M_PI / (float) sectors;
    float dPhi = M_PI / (float) (stacks - 1);

    float *vertices = malloc(sizeof(float) * (sectors * stacks) * stride);

    for (int i = 0; i < stacks; i++) {
        float phi = (i * dPhi) - (M_PI / 2.0);
        float y = sin(phi);

        for (int j = 0; j < sectors; j++) {
            float theta = j * dTheta;
            float x = cos(phi) * cos(theta);
            float z = cos(phi) * sin(theta);

            int startIndex = (i * sectors + j) * stride;
            vertices[startIndex] = x;
            vertices[startIndex + 1] = y;
            vertices[startIndex + 2] = z;

            vec3 normal;
            glm_vec3_normalize_to((vec3) {x, y, z}, normal);
            vertices[startIndex + 3] = normal[0];
            vertices[startIndex + 4] = normal[1];
            vertices[startIndex + 5] = normal[2];
        }
    }

    return vertices;
}

unsigned int *sphereIndices(int sectors, int stacks) {
    int poleTriangles = sectors * 2;
    int triangleCount = 2 * sectors * (stacks - 2);
    unsigned int *indices = malloc(sizeof(unsigned int) * triangleCount * 3);
    int count = 0;

    for (int i = 0; i < stacks - 1; i++) {
        int v1 = i * sectors;
        int v2 = v1 + sectors;
        for (int j = 0; j < sectors; j++, v1++, v2++) {
            int v3 = v1 + 1;
            //lower band
            if (i != 0) {
                indices[count++] = v1;
                indices[count++] = v2;
                indices[count++] = v3;
            }

            //top band
            if (i != stacks - 2) {
                indices[count++] = v1 + 1;
                indices[count++] = v2;
                indices[count++] = v2 + 1;
            }
        }
    }

    return indices;
}

Shape sphere(void) {
    int stride = 6;

    int sectors = 24;
    int stacks = 24;
    int vertexCount = sectors * stacks;
    float *vertices = sphereVertices(sectors, stacks, stride);

    unsigned int *indices = sphereIndices(sectors, stacks);
    int indexCount = 2 * sectors * (stacks - 2) * 3;

    Shape sphereShape = {
        .vertices = vertices,
        .vertexCount = vertexCount,
        .stride = stride,
        .indices = indices,
        .indexCount = indexCount
    };

    return sphereShape;
}
