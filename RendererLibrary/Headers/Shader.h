#ifndef RENDERERLIBRARY_SHADER_H
#define RENDERERLIBRARY_SHADER_H

#include <GL/glew.h>
#include <iostream>

class Shader{
public:
    Shader() = default;

    Shader(const std::string &shaderPath ,GLint shaderType);
    void initialize(const std::string &shaderPath ,GLint shaderType);

    GLuint operator()();
    GLint getType();

    void detach(GLuint programID);
    void Delete();
private:
    void createShader(const std::string &shaderPath);
    std::string readShaderPath(const std::string &shaderPath);
    GLuint shaderID;
    GLint shaderType;
};

#endif