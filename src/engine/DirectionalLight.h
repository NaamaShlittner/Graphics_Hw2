#pragma once
#include <glm/glm.hpp>
#include "LightSource.h"

class DirectionalLight : public LightSource
{
public:
    DirectionalLight() = default;

    DirectionalLight(const glm::vec3& dir, const glm::vec3& intens)
    {
        direction = dir;
        intensity = intens;
    }

    ~DirectionalLight() = default;
    glm::vec3 directionFrom(const glm::vec3& P) const override;
    glm::vec3 intensityAt(const glm::vec3& P) const override;

    float maxShadowDistance(const glm::vec3& P) const override;
};