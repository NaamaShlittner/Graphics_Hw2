#include "Spotlight.h"
#include <glm/glm.hpp>

glm::vec3 Spotlight::directionFrom(const glm::vec3& P) const
{
    return glm::normalize(position - P);
}

glm::vec3 Spotlight::intensityAt(const glm::vec3& P) const
{
    glm::vec3 spotDir = glm::normalize(direction);
    glm::vec3 toPoint = glm::normalize(P - position);

    float cosTheta = glm::dot(spotDir, toPoint);
    float cosCutoff = glm::cos(cutoffAngle);

    if (cosTheta < cosCutoff) {
        return glm::vec3(0.0f);
    }

    return intensity*(cosTheta);
}

float Spotlight::maxShadowDistance(const glm::vec3& P) const {
    return glm::length(position - P);
}
