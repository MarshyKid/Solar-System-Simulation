#ifndef PHYSICS_H
#define PHYSICS_H
#include <cglm/cglm.h>

#define DISTANCE_SCALE 0.1f
#define G 10000.0f

typedef struct Body {
    vec3 pos;
    vec3 velocity;
    vec3 acceleration;
    double radius;
    float renderRadius;
    double mass;
    vec3 color;
} Body;

Body createBody(vec3 pos, vec3 velocity, double radius, double mass, vec3 color);

void updateAcceleration(Body *body1, Body *body2, float dt);

void zeroAcceleration(Body *body);

void update(Body *body, float dt);

#endif
