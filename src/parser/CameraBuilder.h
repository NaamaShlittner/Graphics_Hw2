#pragma once
#include <memory>
#include <stdexcept>
#include <glm/glm.hpp>
#include "../engine/Camera.h"

class CameraBuilder
{
public:
    CameraBuilder() = default;

    CameraBuilder &setScreenWidth(float width);

    CameraBuilder &setScreenHeight(float height);

    CameraBuilder &setPosition(const glm::vec3 &position);

    CameraBuilder &setUpVector(const glm::vec3 &upVector);

    CameraBuilder &setForwardVector(const glm::vec3 &forwardVector);

    CameraBuilder &setDistanceToProjectionPlane(float distance);

    std::unique_ptr<Camera> build();

private:
    float screenWidth{800.0f};
    float screenHeight{600.0f};
    float distanceToProjectionPlane{1.0f};
    glm::vec3 position{0.0f, 0.0f, 3.0f};
    glm::vec3 upVector{0.0f, 1.0f, 0.0f};
    glm::vec3 forwardVector{0.0f, 0.0f, -1.0f};
};