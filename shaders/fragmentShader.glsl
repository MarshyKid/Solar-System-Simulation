#version 330 core
out vec4 FragColor;

//in vec3 normal;
in vec3 pos;

uniform float aspect;
uniform int objCount;
uniform vec3 spheres[20];
uniform float radiuses[20];
uniform vec3 colors[20];

uniform vec3 cameraPos;
uniform vec3 cameraForward;
uniform vec3 cameraRight;
uniform vec3 cameraUp;
uniform float fov;

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

float getPlaneT(vec3 rayOrigin, vec3 rayDir, float planeY) {
    //line of ray = origin + rayDir * t
    //if y value of line of ray = planeY, intersects
    //origin.y + rayDir.y * t = planeY 
    //-> t = (planeY - origin.y) / rayDir.y
    float t = (planeY - rayOrigin.y) / rayDir.y;
    if (t < 0.0) {
        return maxT;
    }
    return t;
}

float height(float x, float z) {
    float r = length(vec2(x, z));
    return - 1.6 / sqrt(r * r + 0.2);
}

float getCurvedPlaneT(vec3 rayOrigin, vec3 rayDir, float planeY) {
    //walk along ray and check if y at ray(t) >,= or < height(x, z)
    float t = 0.0;
    float inc = 0.05;
    float maxDist = 100.0;

    float prevT = 0.0;
    float prevDiff = 1e9;

    for (int i = 0; i < 4000; i++) {
        vec3 p = rayOrigin + rayDir * t;
        float surfaceY = planeY + height(p.x, p.z);
        float diff = p.y - surfaceY;

        if (diff <= 0.0 && prevDiff > 0.0) {
            //binary search
            float a = prevT;
            float b = t;

            for (int j = 0; j < 10; j++) {
                float m = 0.5 * (a + b);
                vec3 midPoint = rayOrigin + rayDir * m;
                float midDiff = midPoint.y - surfaceY;

                if (midDiff > 0.0) {
                    a = m;
                } else {
                    b = m;
                }
            }
             return 0.5 * (a + b);
        }

        prevDiff = diff;
        t += inc;

        if (t > maxDist) {
            break;
        }
    }

    return maxT;
}

void main() {

    float radius = 0.5f;
    //ray from camera
    //adjust x value based on aspect
    vec3 rayDir = normalize(
        cameraForward + pos.x * cameraRight * aspect * tan(radians(fov) / 2) + pos.y * cameraUp * tan(radians(fov) / 2)
    );
    vec3 rayOrigin = cameraPos;

    float t = maxT;
    float lightScale = 0.0;
    bool hit = false;
    vec3 hitSphere = vec3(0.0, 0.0, 0.0);
    bool isLightSource = false;

    vec3 color = vec3(1.0, 1.0, 1.0);
    int sphereIndex = 0;
    int hitType = -1; //0: sphere, 1: plane
    
    //grid
   //float planeT = getCurvedPlaneT(rayOrigin, rayDir, -2.0f);
   //if (planeT < t && t > 0.0) {
   //    hit = true;
   //    hitType = 1;
   //    color = vec3(1.0f, 1.0f, 1.0f);
   //    isLightSource = false;
   //    t = planeT;
   //    vec3 hitPos = rayOrigin + rayDir * t;
   //    float gridLineWidth = 0.05;
   //    float distToLineX = abs(fract(hitPos.x));
   //    float distToLineZ = abs(fract(hitPos.z));
   //    if (distToLineX < gridLineWidth / 2.0 || distToLineZ < gridLineWidth / 2.0) {
   //        lightScale = 1.0;
   //    } else {
   //        t = maxT;
   //        lightScale = 0.0;
   //    }
   //}

    for (int i = 0; i < objCount; i++) {
        float currT = getT(rayOrigin, rayDir, spheres[i], radiuses[i]);
        if (currT < t && t > 0.0) {
            hit = true;
            hitSphere = spheres[i];
            hitType = 0;
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

    if (!hit) {
        discard;
    }
    if (hit && hitType == 0) {
        vec3 hitPos = rayOrigin + rayDir * t;
        vec3 normal = normalize(hitPos - hitSphere);

        vec3 lightSource = spheres[0];
        rayOrigin = hitPos;
        rayDir = normalize(lightSource - hitPos);
        bool shadow = false;
        for (int i = 1; i < objCount; i++) {
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

    //generate grid
    if (hit && hitType == 1) {
    }
    if (isLightSource) {
        lightScale = 1.0;
    }

    FragColor = vec4(lightScale * color, 1.0);
}
