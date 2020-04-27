#ifndef RENDERERLIBRARY_MODEL_H
#define RENDERERLIBRARY_MODEL_H

#include "Mesh.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class Model{
public:
    Model()=default;
    
    Model(const std::string & modelPath ,bool gammaCorrected);
    Model(const std::string & modelPath);
    Model(const std::string & modelPath ,void (*additionalParameters)());
    Model(const std::string & modelPath ,void (*additionalParameters)() ,bool gammaCorrected);

    void initialize(const std::string & modelPath ,bool gammaCorrected);
    void initialize(const std::string & modelPath);
    void initialize(const std::string & modelPath ,void (*additionalParameters)());
    void initialize(const std::string & modelPath ,void (*additionalParameters)() ,bool gammaCorrected);

    void draw(Program shaderProgram);
    void drawInstanced(Program shaderProgram ,unsigned int numOfInstance);

    void setTextureUniforms(Program shaderProgram);

    GLuint getMeshVAO(unsigned int meshIndex);
    std::vector<GLuint> getAllMeshVAO();
private:
    void createModel(const std::string & modelPath ,void (*additionalParameters)());
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> loadedTexture;
    bool gammaCorrected=false;
    void processNode(aiNode *node ,const aiScene *scene ,void (*additionalParameters)());
    void processMesh(aiMesh *mesh ,const aiScene *scene ,void (*additionalParameters)());
    std::vector<Texture> loadTexture(aiMaterial *material ,aiTextureType type ,void (*additionalParameters)());
};

#endif