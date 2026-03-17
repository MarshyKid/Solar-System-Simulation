#include "mesh.h"

Mesh meshCreate(float *vertices, unsigned int vertexCount, unsigned int *indices, unsigned int indexCount) {
    //VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    //VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    //elem buffer
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexCount, indices, GL_STATIC_DRAW);
    unsigned int stride = 3;
    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    //normal
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    Mesh resMesh = {
        .VAO = VAO,
        .VBO = VBO,
        .EBO = EBO,
        .indexCount = indexCount,
        .vertexCount = vertexCount
    };

    return resMesh;
}

void meshDraw(Mesh *mesh) {
    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
