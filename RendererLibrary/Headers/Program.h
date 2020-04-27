#ifndef RENDERERLIBRARY_PROGRAM_H
#define RENDERERLIBRARY_PROGRAM_H

#include "Shader.h"
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Program{
public:
    Program() = default;
    void initialize();
    void attachShader(Shader &shader);
    void attachShader(Shader shader);
    void link();
    void use();

    void setInt(const std::string &varName ,int val);
    void setFloat(const std::string &varName ,float val);
    void setVec2(const std::string &varName ,glm::vec2 val);
    void setVec3(const std::string &varName ,glm::vec3 val);
    void setVec4(const std::string &varName ,glm::vec4 val);
    void setMat3(const std::string &varName ,glm::mat3 val);
    void setMat4(const std::string &varName ,glm::mat4 val);

    GLuint operator()();
    void Delete();
private:
    GLuint programID;
    std::unordered_map<std::string,int> uniformMap;
    int findUniform(const std::string &varName);
};

#endif
