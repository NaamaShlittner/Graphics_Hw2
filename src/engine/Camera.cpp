#include <cmath>
#include <glm/glm.hpp>
#include "engine/Camera.h"

Ray Camera::generateRay(int pixelX, int pixelY, int imageWidth, int imageHeight) const {
    // Calculate the pixel size in world units
    float pixelWidth = screenWidth / static_cast<float>(imageWidth);
    float pixelHeight = screenHeight / static_cast<float>(imageHeight);

    // Calculate the center of the projection plane
    glm::vec3 screenCenter = position + forwardVector * distanceToProjectionPlane;

    // Calculate the bottom-left corner of the projection plane
    glm::vec3 rightVector = glm::normalize(glm::cross(forwardVector, upVector));
    glm::vec3 trueUpVector = glm::normalize(glm::cross(rightVector, forwardVector));
    glm::vec3 bottomLeftCorner = screenCenter - (rightVector * (screenWidth / 2.0f)) - (trueUpVector * (screenHeight / 2.0f));

    // Calculate the world coordinates of the pixel
    glm::vec3 pixelWorldPosition = bottomLeftCorner + (rightVector * (pixelX + 0.5f) * pixelWidth) + (trueUpVector * (pixelY + 0.5f) * pixelHeight);

    // Create the ray from the camera position to the pixel world position
    glm::vec3 rayDirection = glm::normalize(pixelWorldPosition - position);
    return Ray(position, rayDirection);
}