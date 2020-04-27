#ifndef RENDERERLIBRARY_CUBEMAP_TEXTURE_H
#define RENDERERLIBRARY_CUBEMAP_TEXTURE_H

#include <GL/glew.h>
#include <iostream>
#include <stb_image.h>
#include <vector>

class CubeMapTexture{
public:
    CubeMapTexture() = default;

    CubeMapTexture(const std::vector<std::string> &texturePath);
    CubeMapTexture(const std::vector<std::string> &texturePath ,bool gammaCorrected);

    void initialize(const std::vector<std::string> &texturePath);
    void initialize(const std::vector<std::string> &texturePath ,bool gammaCorrected);

    GLuint operator()();
    bool isGammaCorrected();

    void destroy();
private:
    void createTexture(const std::vector<std::string> &texturePath);

    GLuint textureID;
    bool gammaCorrected=false;
};

#endif