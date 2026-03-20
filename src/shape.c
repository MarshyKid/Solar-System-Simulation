#include "shape.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cglm/cglm.h>
#include "physics.h"

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

float height(float x, float z, Body body) {
    float x2 = body.pos[0] * DISTANCE_SCALE;
    float z2 = body.pos[2] * DISTANCE_SCALE;
    float dx = x2 - x;
    float dz = z2 - z;
    float r = dx * dx + dz * dz;
    float mass = body.mass;
    return -sqrt(mass) * 2 / sqrt(r + 0.5);
}

void generateGridPoints(int gridStart, float inc, int gridSize, float yVal, vec3 *gridPoints, Body *bodies, int objCount) {
    for (int r = 0; r < gridSize; r++) {
        for (int c = 0; c < gridSize; c++) {
            float z = r * inc + gridStart;
            float x = c * inc + gridStart;
            float y = yVal;
            for (int b = 0; b < objCount; b++) {
                y += height(x, z, bodies[b]);
            }
            gridPoints[r * gridSize + c][0] = x;
            gridPoints[r * gridSize + c][1] = y;
            gridPoints[r * gridSize + c][2] = z;
        }
    }
}

void generateGridVertices(int gridSize, float lineWidth, vec3 *gridPoints, float *gridVertices) {
    //rows
    int seg = 0;
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize - 1; j++) {
            float x1 = gridPoints[i * (gridSize) + j][0];
            float y1 = gridPoints[i * (gridSize) + j][1];
            float z1 = gridPoints[i * (gridSize) + j][2];

            float x2 = gridPoints[i * (gridSize) + j + 1][0];
            float y2 = gridPoints[i * (gridSize) + j + 1][1];
            float z2 = gridPoints[i * (gridSize) + j + 1][2];

            //top left
            int start = seg++ * 4 * 3;
            gridVertices[start] = x1 - lineWidth / 2.0;
            gridVertices[start + 1] = y1;
            gridVertices[start + 2] = z1 - lineWidth / 2.0;

            //top right
            gridVertices[start + 3] = x2 + lineWidth / 2.0;
            gridVertices[start + 4] = y2;
            gridVertices[start + 5] = z2 - lineWidth / 2.0;
            
            //bot left
            gridVertices[start + 6] = x1 - lineWidth / 2.0;
            gridVertices[start + 7] = y1;
            gridVertices[start + 8] = z1 + lineWidth / 2.0;
            
            //bot right
            gridVertices[start + 9] = x2 + lineWidth / 2.0;
            gridVertices[start + 10] = y2;
            gridVertices[start + 11] = z2 + lineWidth / 2.0;
        }
    }

    //cols
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize - 1; j++) {
            float x = gridPoints[j * (gridSize) + i][0];
            float y = gridPoints[j * (gridSize) + i][1];
            float z = gridPoints[j * (gridSize) + i][2];

            float x2 = gridPoints[(j + 1) * (gridSize) + i][0];
            float y2 = gridPoints[(j + 1) * (gridSize) + i][1];
            float z2 = gridPoints[(j + 1) * (gridSize) + i][2];

            //top left
            int start = seg++ * 4 * 3;
            gridVertices[start] = x - lineWidth / 2.0;
            gridVertices[start + 1] = y;
            gridVertices[start + 2] = z - lineWidth / 2.0;

            //top right
            gridVertices[start + 3] = x + lineWidth / 2.0;
            gridVertices[start + 4] = y;
            gridVertices[start + 5] = z - lineWidth / 2.0;
            
            //bot left
            gridVertices[start + 6] = x2 - lineWidth / 2.0;
            gridVertices[start + 7] = y2;
            gridVertices[start + 8] = z2 + lineWidth / 2.0;
            
            //bot right
            gridVertices[start + 9] = x2 + lineWidth / 2.0;
            gridVertices[start + 10] = y2;
            gridVertices[start + 11] = z2 + lineWidth / 2.0;
        }
    }
}

void generateGridIndices(int segments, unsigned int *gridIndices) {
    for (int i = 0; i < segments; i++) {
        gridIndices[i * 2 * 3] = i * 4;
        gridIndices[i * 2 * 3 + 1] = i * 4 + 1;
        gridIndices[i * 2 * 3 + 2] = i * 4 + 2;

        gridIndices[i * 2 * 3 + 3] = i * 4 + 1;
        gridIndices[i * 2 * 3 + 4] = i * 4 + 2;
        gridIndices[i * 2 * 3 + 5] = i * 4 + 3;
    }
}

Shape grid(Body *bodies, int objCount) {
    int gridStart = -20;
    float inc = 0.2f;
    int gridEnd = 20;
    int gridSize = (gridEnd - gridStart) / inc + 1;
    float lineWidth = 0.02f;
    float yVal = 0.0f;
    vec3 *gridPoints = malloc(sizeof(vec3) * gridSize * gridSize);
    generateGridPoints(gridStart, inc, gridSize, yVal, gridPoints, bodies, objCount);

    int segments = 2 * (gridSize * (gridSize - 1)); //2x because same for col & row
    unsigned int gridVertexCount = segments * 4;
    float *gridVertices = malloc(sizeof(float) * gridVertexCount * 3);
    generateGridVertices(gridSize, lineWidth, gridPoints, gridVertices);

    unsigned int *gridIndices = malloc(sizeof(unsigned int) * segments * 2 * 3);
    generateGridIndices(segments, gridIndices);

    Shape grid = {
        .vertices = gridVertices,
        .vertexCount = gridVertexCount,
        .stride = 3,
        .indices = gridIndices,
        .indexCount = segments * 2 * 3,

    };

    free(gridPoints);

    return grid;
}
