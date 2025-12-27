#pragma once
#include <glm/glm.hpp>
#include "LightSource.h"

class Spotlight : public LightSource
{
public:
    glm::vec3 position{0.0f,0.0f,0.0f};
    float cutoffAngle = glm::radians(30.0f);

    Spotlight() = default;
    Spotlight(const glm::vec3& pos, const glm::vec3& dir,
              const glm::vec3& intens, float cutoff);

    glm::vec3 directionFrom(const glm::vec3& P) const override;
    glm::vec3 intensityAt(const glm::vec3& P) const override;
    float maxShadowDistance(const glm::vec3& P) const override;
};
