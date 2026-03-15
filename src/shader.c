#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

char *loadFile(const char *filename) {
    FILE *fp;
    fp = fopen(filename, "r");

    //get file size
    fseek(fp, 0, SEEK_END);
    long length = ftell(fp);
    rewind(fp);
    char *s = malloc(length + 1);

    fread(s, 1, length, fp);
    s[length] = '\0';

    fclose(fp);

    return s;
}

Shader createShader(const char *vertexPath, const char *fragmentPath) {
    //load
    const char *vertexShaderSource = loadFile(vertexPath);
    const char *fragmentShaderSource = loadFile(fragmentPath);

    //create shaders
    unsigned int vertexShader;
    unsigned int fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    //check for compilation error
    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR SHADER VERTEX COMP FAILED: %s\n", infoLog);
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR SHADER FRAGMENT COMP FAILED: %s\n", infoLog);
    }

    //create program object
    Shader shaderProgram = {.id = glCreateProgram() };

    //link
    glAttachShader(shaderProgram.id, vertexShader);
    glAttachShader(shaderProgram.id, fragmentShader);
    glLinkProgram(shaderProgram.id);

    //check for compile error
    glGetProgramiv(shaderProgram.id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram.id, 512, NULL, infoLog);
        printf("ERROR SHADER PROGRAM FAILED: %s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void use(Shader shaderProgram) {
    glUseProgram(shaderProgram.id);
}

void setFloat(Shader shaderProgram, const char *uniformName, float value) {
    glUniform1f(glGetUniformLocation(shaderProgram.id, uniformName), value);
}
