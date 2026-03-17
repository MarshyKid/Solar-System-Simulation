#include "physics.h"
#include <cglm/cglm.h>

void updatePos(Body *body, float dt);
void updateVel(Body *body, float dt);

Body createBody(vec3 pos, vec3 velocity, double radius, double mass, vec3 color) {
    return (Body) {
        .pos = {pos[0], pos[1], pos[2]},
        .velocity = {velocity[0], velocity[1], velocity[2]},
        .acceleration = {0.0f, 0.0f, 0.0f},
        .radius = radius,
        .renderRadius = radius * DISTANCE_SCALE,
        .mass = mass,
        .color = {color[0], color[1], color[2]},
    };
}

void update(Body *body, float dt) {
    updateVel(body, dt);
    updatePos(body, dt);
}

void updateAcceleration(Body *body1, Body *body2, float dt) {
    //a = Gm/r^2
    float r = glm_vec3_distance(body1->pos, body2->pos);
    float m2 = body2->mass;

    float a = G * m2 / (r * r);

    vec3 dir;
    glm_vec3_sub(body2->pos, body1->pos, dir);
    glm_vec3_normalize(dir);
    glm_vec3_scale(dir, a, dir);

    glm_vec3_add(body1->acceleration, dir, body1->acceleration);
}

void zeroAcceleration(Body *body) {
    glm_vec3_zero(body->acceleration);
}

void updatePos(Body *body, float dt) {
    vec3 translation;
    glm_vec3_scale(body->velocity, dt, translation);
    glm_vec3_add(body->pos, translation, body->pos);
}

void updateVel(Body *body, float dt) {
    vec3 translation;
    glm_vec3_scale(body->acceleration, dt, translation);
    glm_vec3_add(body->velocity, translation, body->velocity);
}
