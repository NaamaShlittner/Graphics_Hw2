#include "Spotlight.h"
#include <glm/glm.hpp>

glm::vec3 Spotlight::directionFrom(const glm::vec3 &P) const
{
    return glm::normalize(position - P);
}

Spotlight::Spotlight(const glm::vec3 &pos,
                     const glm::vec3 &dir,
                     const glm::vec3 &intens,
                     float cutoff)
    : LightSource(dir, intens),
      position(pos),
      cutoffAngle(cutoff)
{}

glm::vec3 Spotlight::intensityAt(const Hit& hit) const
{
    glm::vec3 spotDir = glm::normalize(direction);
    glm::vec3 toPoint = glm::normalize(hit.hitPoint - position);

    float cosTheta = glm::dot(spotDir, toPoint);
    float cosCutoff = glm::cos(cutoffAngle);

    if (cosTheta < cosCutoff)
    {
        return glm::vec3(0.0f);
    }

    return intensity * (cosTheta) * glm::dot(hit.normal, direction);
}

float Spotlight::maxShadowDistance(const glm::vec3 &P) const
{
    return glm::length(position - P);
}
