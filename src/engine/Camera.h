#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
    Camera() = default;

    Camera(float screenWidth, float screenHeight, glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f),
           glm::vec3 forwardVector = glm::vec3(0.0f, 0.0f, -1.0f),
            float distanceToProjectionPlane = 1.0f)
        : screenWidth(screenWidth), screenHeight(screenHeight),
          position(position), upVector(upVector), forwardVector(forwardVector), distanceToProjectionPlane(distanceToProjectionPlane) {};

    ~Camera() = default;

    float getScreenWidth() const { return screenWidth; }
    float getScreenHeight() const { return screenHeight; }
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getUpVector() const { return upVector; }
    glm::vec3 getForwardVector() const { return forwardVector; }
    float getDistanceToProjectionPlane() const { return distanceToProjectionPlane; }

private:
    float screenWidth = 800.0f;
    float screenHeight = 600.0f;
    glm::vec3 position = {0.0f, 0.0f, 3.0f};
    glm::vec3 upVector = {0.0f, 1.0f, 0.0f};
    glm::vec3 forwardVector = {0.0f, 0.0f, -1.0f};
    float distanceToProjectionPlane = 1.0f;
    
    
};