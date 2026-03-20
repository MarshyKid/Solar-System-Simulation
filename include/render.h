#ifndef RENDER_H
#define RENDER_H
#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

typedef struct RenderObject {
    vec3 pos;
    vec3 color;
    float scale;
} RenderObject;

//void renderScene(GLFWwindow *window, Camera *cam, Shader *shaderProgram, RenderObject *renderObjects, int count);
void renderGrid(GLFWwindow *window, Camera *cam, Shader *shaderProgram, Mesh *mesh);
void renderTriangle(GLFWwindow *window, Camera *cam, Shader *shaderProgram, Mesh *mesh, RenderObject *renderList, int objCount);

#endif
