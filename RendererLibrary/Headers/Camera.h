#ifndef RENDERERLIBRARY_CAMERA_H
#define RENDERERLIBRARY_CAMERA_H

#include <glm/gtc/matrix_transform.hpp>

class Camera{
public:
    float yaw,pitch;
    glm::vec3 front,up,right,position;

    Camera()=default;

    Camera(float yaw ,float pitch ,glm::vec3 position);
    void initialize(float yaw ,float pitch ,glm::vec3 position);

    void updateDirections();
    void updateViewMatrix();
    void updateCamera();

    glm::mat4 getViewMatrix();
    void constrainPitch(float min ,float max);
private:
    glm::mat4 viewMatrix;
};

#endif