#ifndef CAMERA_H
#define CAMERA_H
#include <cglm/cglm.h>

typedef struct Camera {
    vec3 pos;
    vec3 front;
    vec3 up;
    float pitch;
    float yaw;
    float roll;
    float fov;
    float speed;
} Camera;

Camera cameraCreate(void);

void cameraViewMat(Camera *cam, mat4 destination);

void cameraProjMat(Camera *cam, float aspectRatio, mat4 destination);

void cameraForward(Camera *cam, float dt);

void cameraRight(Camera *cam, float dt);

void cameraBack(Camera *cam, float dt);

void cameraLeft(Camera *cam, float dt);

void cameraUp(Camera *cam, float dt);

void cameraDown(Camera *cam, float dt);

void cameraProcessMouse(Camera *cam, float dx, float dy, float sensitivity);

#endif
