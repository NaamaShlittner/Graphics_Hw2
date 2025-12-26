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
};