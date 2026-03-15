#version 330 core
out vec4 FragColor;

in vec3 normal;

uniform vec3 color;
uniform vec3 lightDir;

void main() {
    float n = max(dot(normal, lightDir), 0.05);
    FragColor = vec4(n * color, 1.0f);
}
