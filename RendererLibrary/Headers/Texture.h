#ifndef RENDERERLIBRARY_TEXTURE_H
#define RENDERERLIBRARY_TEXTURE_H

#include <GL/glew.h>
#include <iostream>
#include <stb_image.h>

enum TEXTURE_TYPES {TEXTURE_AMBIENT=0 ,TEXTURE_DIFFUSE ,TEXTURE_SPECULAR ,TEXTURE_REFLECT ,TEXTURE_NORMAL};

class Texture{
public:
    Texture() = default;

    Texture(const std::string & texturePath);
    void initialize(const std::string & texturePath);

    Texture(const std::string & texturePath ,void (*additionalParameters)());
    void initialize(const std::string & texturePath ,void (*additionalParameters)());

    Texture(const std::string & texturePath ,void (*additionalParameters)() ,bool gammaCorrected);
    void initialize(const std::string & texturePath ,void (*additionalParameters)() ,bool gammaCorrected);

    Texture(const std::string & texturePath ,void (*additionalParameters)() ,const std::string & textureName ,bool gammaCorrected);
    void initialize(const std::string & texturePath ,void (*additionalParameters)() ,const std::string & textureName ,bool gammaCorrected);

    Texture(const std::string & texturePath ,const std::string & textureName ,bool gammaCorrected);
    void initialize(const std::string & texturePath ,const std::string & textureName ,bool gammaCorrected);

    Texture(const std::string & texturePath ,void (*additionalParameters)(),const std::string & textureName ,int textureType ,bool gammaCorrected);
    void initialize(const std::string & texturePath ,void (*additionalParameters)(),const std::string & textureName ,int textureType ,bool gammaCorrected);

    std::string getName();
    bool isGammaCorrected();
    int getType();
    GLuint operator()();
private:
    void createTexture(const std::string & texturePath ,void (*additionalParameters)());

    bool gammaCorrected=false;
    std::string textureName;
    GLuint textureID;
    int textureType;
};

#endif
