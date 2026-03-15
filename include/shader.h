#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

typedef struct Shader {
    unsigned int id;
} Shader;

char *loadFile(const char *filename);
Shader createShader(const char *vertexPath, const char *fragmentPath);
void use(Shader shaderProgram);
void setFloat(Shader shaderProgram, const char *uniformName, float value);

#endif
