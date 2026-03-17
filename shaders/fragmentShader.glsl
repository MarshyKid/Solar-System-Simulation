#version 330 core
out vec4 FragColor;

//in vec3 normal;
in vec3 pos;

uniform float aspect;
uniform int count;
uniform vec3 spheres[20];
uniform float radiuses[20];
uniform vec3 colors[20];

uniform vec3 cameraPos;
uniform vec3 cameraForward;
uniform vec3 cameraRight;
uniform vec3 cameraUp;

const float maxT = 1e30;

float getT(vec3 rayOrigin, vec3 rayDir, vec3 spherePos, float radius) {
    vec3 oToC = rayOrigin - spherePos;
    float c = dot(oToC, oToC) - radius * radius;
    float b = 2.0 * dot(oToC, rayDir);
    float a = dot(rayDir, rayDir);
    float discriminant = b * b - 4.0 * a * c;
    //line of ray = origin + rayDirection * t
    //see if dist(sphere center -> ray)^2 = radius has a solution
    //|o +dir*t - center|^2 = radius ^2
    //(o - center)^2 - radius^2 + 2 (o - center) (dir*t) + (dir * t)^2 = 0
    //solve for t:
    // discriminant = b^2 - 4ac

    //get t
    float t = (-b - sqrt(discriminant)) / (2.0 * a);
    if (discriminant < 0 || t < 0.0) {
        return maxT;
    }
 
    return t;
}

void main() {

    float radius = 0.5f;
    //ray from camera
    //adjust x value based on aspect
    vec3 rayDir = normalize(
        cameraForward + pos.x * cameraRight * aspect + pos.y * cameraUp
    );
    vec3 rayOrigin = cameraPos;

    float t = maxT;
    float lightScale = 0.0;
    bool hit = false;
    vec3 hitSphere = vec3(0.0, 0.0, 0.0);
    bool isLightSource = false;

    vec3 color = vec3(1.0, 1.0, 1.0);
    int sphereIndex = 0;
    for (int i = 0; i < count; i++) {
        float currT = getT(rayOrigin, rayDir, spheres[i], radiuses[i]);
        if (currT < t && t > 0.0) {
            hit = true;
            hitSphere = spheres[i];
            color = colors[i];
            t = currT;
            sphereIndex = i;
            if (i == 0) {
                isLightSource = true;
            } else {
                isLightSource = false;
            }
        }
    }

    if (hit) {
        vec3 hitPos = rayOrigin + rayDir * t;
        vec3 normal = normalize(hitPos - hitSphere);

        vec3 lightSource = spheres[0];
        rayOrigin = hitPos;
        rayDir = normalize(lightSource - hitPos);
        bool shadow = false;
        for (int i = 1; i < count; i++) {
            if (i != sphereIndex) {
                float currShadowT = getT(rayOrigin, rayDir, spheres[i], radiuses[i]);
                if (currShadowT > 0.0 && currShadowT < length(lightSource - hitPos)) {
                    shadow = true;
                }
            }
        }
        vec3 lightDir = normalize(lightSource - hitPos);
        lightScale = max(dot(normal, lightDir), 0.0);
        if (shadow) {
            lightScale = 0.0;
        }
        lightScale += 0.0; //ambient
    }
    if (isLightSource) {
        lightScale = 1.0;
    }

    FragColor = vec4(lightScale * color, 1.0);
}
