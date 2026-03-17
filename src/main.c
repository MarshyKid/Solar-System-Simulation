#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "shader.h"
#include "stb_image.h"
#include "mesh.h"
#include "shape.h"
#include "camera.h"
#include "render.h"
#include "physics.h"

double earthRadius = 6.37;
double moonRadius = 1.74;

float dt = 0.0f;
float prevFrame = 0.0f;

//initial mousepos
float mouseX = 400;
float mouseY = 300;
int firstMouse = 1;

//mouse callback to handle look around
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    //get camera
    Camera *cam = glfwGetWindowUserPointer(window);

    if (firstMouse) {
        mouseX = xpos;
        mouseY = ypos;
        firstMouse = 0;
        return;
    }

    float xOffset = xpos - mouseX;
    float yOffset = mouseY - ypos;
    
    const float sensitivity = 0.1f;
    cameraProcessMouse(cam, xOffset, yOffset, sensitivity);
    mouseX = xpos;
    mouseY = ypos;
}

void processInput(GLFWwindow *window, Camera *cam) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
    //handle movement
    const float cameraSpeed = 3.0f * dt;
    vec3 movementVec;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraForward(cam, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraRight(cam, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraBack(cam, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraLeft(cam, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraDown(cam, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraUp(cam, dt);
    }

    //mouse movement
    glfwSetCursorPosCallback(window, mouse_callback);
}

GLFWwindow *createGLFWWindow(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Solar System", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return window;
    }

    printf("Successfully create GLFW window\n");
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);
    return window;
}

void wireframeMode(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void updateRenderList(RenderObject *renderList, Body *bodies, int objCount) {
    for (int i = 0; i < objCount; i++) {
        renderList[i] = (RenderObject) {
            .color = {
                bodies[i].color[0] / 255.0,
                bodies[i].color[1] / 255.0,
                bodies[i].color[2] / 255.0
            },
            .pos = {
                bodies[i].pos[0] * DISTANCE_SCALE,
                bodies[i].pos[1] * DISTANCE_SCALE,
                bodies[i].pos[2] * DISTANCE_SCALE
            },
            .scale = bodies[i].renderRadius,
        };
    }
}

int main(void) {
    //init camera
    Camera cam = cameraCreate();

    //initialise glfw
    GLFWwindow *window = createGLFWWindow();
    if (!window) {
        return -1;
    }
    //attach camera to window for callback to access
    glfwSetWindowUserPointer(window, &cam);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialise GLAD\n");
        return -1;
    }
    printf("Successfully initialised GLAD\n");

    //setting viewport dimensions
    glViewport(0, 0, 1500, 600);

    //program object
    Shader shaderProgram = createShader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

    Shape cubeShape = cube();
    Mesh cubeMesh = meshCreate(cubeShape.vertices, cubeShape.vertexCount * cubeShape.stride, cubeShape.indices, cubeShape.indexCount);

    Shape sphereShape = sphere();
    Mesh sphereMesh = meshCreate(sphereShape.vertices, sphereShape.vertexCount * sphereShape.stride, sphereShape.indices, sphereShape.indexCount);

    //wireframeMode(); //change primitive drawing to wireframe
    glEnable(GL_DEPTH_TEST);

    //physical objects
    vec3 origin = {150.0f, 0.0f, -0.0f};
    vec3 pos2 = {180.0f, 0.0f, -0.0f};
    vec3 pos3 = {0.0f, 0.0f, 0.0f};
    vec3 vel1 = {0.0f, 0.0f, 47.0f};
    vec3 vel2 = {0.0f, 0.0f, 0.0f};
    vec3 vel3 = {0.0f, 0.0f, 58.0f};
    vec3 white = {205.0, 205.0, 205.0};
    vec3 green = {21.0, 119.0, 40.0};
    vec3 orange = {255.0, 127.0, 0.0};
    Body bodies[] = {
        //pos, vel, radius, mass, color
        createBody(pos3, vel2, earthRadius * 2, 33.0f, orange),
        createBody(pos2, vel3, moonRadius, 1.0f / 81.3, white),
        createBody(origin, vel1, earthRadius, 1.0f, green),
    };

    int nObj = 3;
    //pos, color, scale
    RenderObject renderList[nObj];

    float triangleVertices[] = {
        -2.5f, -1.0f, 0.0f,
        2.5f, -1.0f, 0.0f,
        0.0f, 2.5f, 0.0f,
    };

    unsigned int triangleIndices[] = {
        0, 1, 2,
    };
    
    Mesh triangleMesh = meshCreate(triangleVertices, 9, triangleIndices, 3);
    
    while (!glfwWindowShouldClose(window)) {
        //update dt
        float currFrame = glfwGetTime();
        dt = currFrame - prevFrame;
        prevFrame = currFrame;

        //input
        processInput(window, &cam);

        //physics calc
        //zero acceleration first
        for (int i = 0; i < nObj; i++) {
            zeroAcceleration(&bodies[i]);
        }

        for (int i = 0; i < nObj; i++) {
            //update acceleration
            for (int j = i + 1; j < nObj; j++) {
                updateAcceleration(&bodies[i], &bodies[j], dt);
                updateAcceleration(&bodies[j], &bodies[i], dt);
            }
            
            update(&bodies[i], dt);
        }
        //render commands
        updateRenderList(renderList, bodies, nObj);
        //renderScene(window, &cam, &shaderProgram, renderList, nObj);
        renderTriangle(window, &cam, &shaderProgram, &triangleMesh, renderList, nObj);

        //check & call events, swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
