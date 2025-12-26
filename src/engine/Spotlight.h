#pragma once
#include <glm/glm.hpp>
#include "LightSource.h"

class Spotlight : public LightSource
{
public:
    glm::vec3 position{0.0f,0.0f,0.0f}; //position of the spotlight
    float cutoffAngle=glm::radians(30.0f); //cutoff angle in radians, default 30 degrees

    Spotlight() = default;

    Spotlight(const glm::vec3& pos, const glm::vec3& dir, const glm::vec3& intens, float cutoff)
    {
        position = pos;
        direction = dir;
        intensity = intens;
        cutoffAngle = cutoff;
    }

    ~Spotlight() = default;
};