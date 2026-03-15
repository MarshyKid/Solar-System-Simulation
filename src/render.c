#include "render.h"
#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

vec3 lightSource = {0.0f, 0.0f, 0.0f};

void renderScene(GLFWwindow *window, Camera *cam, Shader *shaderProgram, RenderObject *renderObjects, int count) {
    glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    use(*shaderProgram);

    //matrix calc
    mat4 view;
    mat4 projection;
    mat4 model;

    glm_mat4_identity(view);
    glm_mat4_identity(projection);
    glm_mat4_identity(model);

    //camera
    cameraViewMat(cam, view);
    int viewLoc = glGetUniformLocation(shaderProgram->id, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float *) view);

    //fov
    int vw, vh;
    glfwGetWindowSize(window, &vw, &vh);
    cameraProjMat(cam, (float)vw/(float)vh, projection);
    int projectionLoc = glGetUniformLocation(shaderProgram->id, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float *) projection);

    //pos, size
    for (int i = 0; i < count; i++) {
        int modelLoc = glGetUniformLocation(shaderProgram->id, "model");
        glm_mat4_identity(model);
        
        //move to pos
        glm_translate(model, renderObjects[i].pos);

        //scale size
        vec3 scaleVec = {
            renderObjects[i].scale,
            renderObjects[i].scale,
            renderObjects[i].scale,
        };
        glm_scale(model, scaleVec);

        //glm_rotate(model, glfwGetTime() * glm_rad(60.0f), (vec3) {0.0f, 1.0f, 0.0f});
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *) model);

        //set color
        int colorLoc = glGetUniformLocation(shaderProgram->id, "color");
        float *color = renderObjects[i].color;
        glUniform3f(colorLoc, color[0], color[1], color[2]);

        //light dir
        int lightDirLoc = glGetUniformLocation(shaderProgram->id, "lightDir");
        vec3 lightDir;
        glm_vec3_sub(lightSource, renderObjects[i].pos, lightDir);
        glm_vec3_normalize(lightDir);
        glUniform3f(lightDirLoc, lightDir[0], lightDir[1], lightDir[2]);
       
        meshDraw(renderObjects[i].mesh);
    }
}
