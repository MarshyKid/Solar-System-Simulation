#include "camera.h"
#include <stdlib.h>
#include <cglm/cglm.h>
#include <math.h>

Camera cameraCreate(void) {
    Camera cam = {
        .pos = {0.0f, 0.0f, 3.0f},
        .front = {0.0f, 0.0f, -1.0f},
        .up = {0.0f, 1.0f, 0.0f},
        .pitch = 0.0f,
        .yaw = -90.0f,
        .roll = 0.0f,
        .fov = 45.0f,
        .speed = 10.0f,
    };

    return cam;
}

void cameraUpdateFront(Camera *cam) {
        cam->front[0] = cos(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
        cam->front[1] = sin(glm_rad(cam->pitch));
        cam->front[2] = sin(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch));
        glm_vec3_normalize(cam->front);
}

void cameraViewMat(Camera *cam, mat4 destination) {
    cameraUpdateFront(cam);

    vec3 lookAt;
    glm_vec3_add(cam->pos, cam->front, lookAt);
    glm_lookat(cam->pos, lookAt, cam->up, destination);
}

void cameraProjMat(Camera *cam, float aspectRatio, mat4 destination) {
    glm_perspective(glm_rad(cam->fov), aspectRatio, 0.1f, 100.0f, destination);
}

void cameraForward(Camera *cam, float dt) {
    vec3 movementVec;
    glm_vec3_scale((vec3) {cam->front[0], 0.0f, cam->front[2]}, cam->speed * dt, movementVec);
    glm_vec3_add(cam->pos, movementVec, cam->pos);
}

void cameraRight(Camera *cam, float dt) {
    vec3 movementVec;
    glm_vec3_cross(cam->front, cam->up, movementVec);
    glm_vec3_scale(movementVec, cam->speed * dt, movementVec);
    glm_vec3_add(cam->pos, movementVec, cam->pos);
}

void cameraLeft(Camera *cam, float dt) {
    vec3 movementVec;
    glm_vec3_cross(cam->front, cam->up, movementVec);
    glm_vec3_scale(movementVec, -1 * cam->speed * dt, movementVec);
    glm_vec3_add(cam->pos, movementVec, cam->pos);
}

void cameraBack(Camera *cam, float dt) {
    vec3 movementVec;
    glm_vec3_scale((vec3) {cam->front[0], 0.0f, cam->front[2]}, -1 * cam->speed * dt, movementVec);
    glm_vec3_add(cam->pos, movementVec, cam->pos);
}

void cameraUp(Camera *cam, float dt) {
    vec3 movementVec;
    glm_vec3_scale((vec3) {0.0f, 1.0f, 0.0f}, cam->speed * dt, movementVec);
    glm_vec3_add(cam->pos, movementVec, cam->pos);
}

void cameraDown(Camera *cam, float dt) {
    vec3 movementVec;
    glm_vec3_scale((vec3) {0.0f, 1.0f, 0.0f}, -1 * cam->speed * dt, movementVec);
    glm_vec3_add(cam->pos, movementVec, cam->pos);
}

void cameraProcessMouse(Camera *cam, float dx, float dy, float sensitivity) {
    cam->pitch += sensitivity * dy;
    if (cam->pitch > 89.0f) {
        cam->pitch = 89.0f;
    }
    if (cam->pitch < -89.0f) {
        cam->pitch = -89.0f;
    }
    cam->yaw += sensitivity * dx;
}
