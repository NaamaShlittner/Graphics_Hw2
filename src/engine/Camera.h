#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
    Camera() = default;
    
    Camera(float screenWidth, float screenHeight, glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f),
           glm::vec3 forwardVector = glm::vec3(0.0f, 0.0f, -1.0f))
        : screenWidth(screenWidth), screenHeight(screenHeight),
          position(position), upVector(upVector), forwardVector(forwardVector) {};

    ~Camera() = default;

private:
    glm::vec3 position;
    glm::vec3 upVector;
    glm::vec3 forwardVector;
    float distanceToProjectionPlane;
    float screenWidth;
    float screenHeight;
};