#pragma once
#include <glm/glm.hpp>
#include <limits>
#include "engine/Hit.h"

class LightSource
{
public:
    LightSource(glm::vec3 dir, glm::vec3 intens) : direction(dir), intensity(intens) {}

    glm::vec3 direction{0.0f, 0.0f, 0.0f};
    glm::vec3 intensity{1.0f, 1.0f, 1.0f}; // default white light
    ~LightSource() = default;

    // the direction is from the light to point P
    virtual glm::vec3 directionFrom(const glm::vec3 &P) const = 0;

    // intensity at point P
    virtual glm::vec3 intensityAt(const Hit& hit) const = 0;

    virtual float maxShadowDistance(const glm::vec3 &P) const
    {
        return std::numeric_limits<float>::infinity(); // directional default
    }

private:
};