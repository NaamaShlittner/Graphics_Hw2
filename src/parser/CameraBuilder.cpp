#include "CameraBuilder.h"

CameraBuilder& CameraBuilder::setScreenWidth(float width) {
    this->screenWidth = width;
    return *this;
}

CameraBuilder& CameraBuilder::setScreenHeight(float height) {
    this->screenHeight = height;
    return *this;
}

CameraBuilder& CameraBuilder::setPosition(const glm::vec3& position) {
    this->position = position;
    return *this;
}

CameraBuilder& CameraBuilder::setUpVector(const glm::vec3& upVector) {
    this->upVector = upVector;
    return *this;
}

CameraBuilder& CameraBuilder::setForwardVector(const glm::vec3& forwardVector) {
    this->forwardVector = forwardVector;
    return *this;
}

CameraBuilder& CameraBuilder::setDistanceToProjectionPlane(float distance) {
    this->distanceToProjectionPlane = distance;
    return *this;
}

std::unique_ptr<Camera> CameraBuilder::build() {
    return std::make_unique<Camera>(screenWidth, screenHeight, position, upVector, forwardVector);
}