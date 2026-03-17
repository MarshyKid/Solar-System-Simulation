#include "render.h"
#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

//void renderScene(GLFWwindow *window, Camera *cam, Shader *shaderProgram, RenderObject *renderObjects, int count) {
//    glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    use(*shaderProgram);
//
//    //matrix calc
//    mat4 view;
//    mat4 projection;
//    mat4 model;
//
//    glm_mat4_identity(view);
//    glm_mat4_identity(projection);
//    glm_mat4_identity(model);
//
//    //camera
//    cameraViewMat(cam, view);
//    int viewLoc = glGetUniformLocation(shaderProgram->id, "view");
//    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float *) view);
//
//    //fov
//    int vw, vh;
//    glfwGetWindowSize(window, &vw, &vh);
//    cameraProjMat(cam, (float)vw/(float)vh, projection);
//    int projectionLoc = glGetUniformLocation(shaderProgram->id, "projection");
//    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float *) projection);
//
//    RenderObject sun = renderObjects[0];
//    vec3 lightSource = (vec3) {sun.pos[0], sun.pos[1], sun.pos[2]};
//
//    //pos, size
//    for (int i = 0; i < count; i++) {
//        int modelLoc = glGetUniformLocation(shaderProgram->id, "model");
//        glm_mat4_identity(model);
//        
//        //move to pos
//        glm_translate(model, renderObjects[i].pos);
//
//        //scale size
//        vec3 scaleVec = {
//            renderObjects[i].scale,
//            renderObjects[i].scale,
//            renderObjects[i].scale,
//        };
//        glm_scale(model, scaleVec);
//
//        //glm_rotate(model, glfwGetTime() * glm_rad(60.0f), (vec3) {0.0f, 1.0f, 0.0f});
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *) model);
//
//        //set color
//        int colorLoc = glGetUniformLocation(shaderProgram->id, "color");
//        float *color = renderObjects[i].color;
//        glUniform3f(colorLoc, color[0], color[1], color[2]);
//
//        //light dir
//        int lightDirLoc = glGetUniformLocation(shaderProgram->id, "lightDir");
//        vec3 lightDir;
//        glm_vec3_sub(lightSource, renderObjects[i].pos, lightDir);
//        glm_vec3_normalize(lightDir);
//        int sunLoc = glGetUniformLocation(shaderProgram->id, "sun");
//        if (i == 0) {
//            glUniform1i(sunLoc, 1);
//        } else {
//            glUniform1i(sunLoc, 0);
//        }
//        glUniform3f(lightDirLoc, lightDir[0], lightDir[1], lightDir[2]);
//       
//        meshDraw(renderObjects[i].mesh);
//    }
//}

void renderTriangle(GLFWwindow *window, Camera *cam, Shader *shaderProgram, Mesh *mesh, RenderObject *renderList, int count) {
    glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //aspect
    int vw, vh;
    glfwGetWindowSize(window, &vw, &vh);
    float aspect = (float)vw / (float)vh;
    int aspectLoc = glGetUniformLocation(shaderProgram->id, "aspect");
    glUniform1f(aspectLoc, aspect);

    //camera
    int cameraPosLoc = glGetUniformLocation(shaderProgram->id, "cameraPos");
    glUniform3f(cameraPosLoc, cam->pos[0], cam->pos[1], cam->pos[2]);

    int cameraForwardLoc = glGetUniformLocation(shaderProgram->id, "cameraForward");
    glUniform3f(cameraForwardLoc, cam->front[0], cam->front[1], cam->front[2]);

    int cameraRightLoc = glGetUniformLocation(shaderProgram->id, "cameraRight");
    vec3 right;
    glm_vec3_cross(cam->front, cam->up, right);
    glm_vec3_normalize(right);
    glUniform3f(cameraRightLoc, right[0], right[1], right[2]);

    int cameraUpLoc = glGetUniformLocation(shaderProgram->id, "cameraUp");
    glUniform3f(cameraUpLoc, cam->up[0], cam->up[1], cam->up[2]);

    //add sphere info
    float spheres[count * 3];
    float radiuses[count];
    float colors[count * 3];

    for (int i = 0; i < count; i++) {
        spheres[3*i] = renderList[i].pos[0];
        spheres[3*i + 1] = renderList[i].pos[1];
        spheres[3*i + 2] = renderList[i].pos[2];

        radiuses[i] = renderList[i].scale;

        colors[3*i] = renderList[i].color[0];
        colors[3*i + 1] = renderList[i].color[1];
        colors[3*i + 2] = renderList[i].color[2];
    }

    int spheresLoc = glGetUniformLocation(shaderProgram->id, "spheres");
    glUniform3fv(spheresLoc, count, spheres);

    int radiusesLoc = glGetUniformLocation(shaderProgram->id, "radiuses");
    glUniform1fv(radiusesLoc, count, radiuses);

    int colorsLoc = glGetUniformLocation(shaderProgram->id, "colors");
    glUniform3fv(colorsLoc, count, colors);

    int countLoc = glGetUniformLocation(shaderProgram->id, "count");
    glUniform1i(countLoc, count);

    use(*shaderProgram);
    meshDraw(mesh);
}
