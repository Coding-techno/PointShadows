#ifndef RENDERERLIBRARY_VERTEX_H
#define RENDERERLIBRARY_VERTEX_H

#include "Texture.h"
#include <glm/gtc/matrix_transform.hpp>

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoords;
};

#endif
