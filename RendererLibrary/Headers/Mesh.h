#ifndef RENDERERLIBRARY_MESH_H
#define RENDERERLIBRARY_MESH_H

#include "Program.h"
#include "Texture.h"
#include "Vertex.h"
#include <vector>

class Mesh{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh()=default;

    Mesh(const std::vector<Vertex> &vertices ,const std::vector<unsigned int> &indices ,const std::vector<Texture> &textures);
    void initialize(const std::vector<Vertex> &vertices ,const std::vector<unsigned int> &indices ,const std::vector<Texture> &textures);

    void draw(Program shaderProgram);
    void drawInstanced(Program shaderProgram ,unsigned int numOfInstance);

    void setTextureUniform(Program shaderProgram);
    GLuint getVAO();
private:
    void createMesh();
    GLuint vbo,vao,ebo;
};

#endif