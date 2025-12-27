#pragma once
#include <glm/glm.hpp>
#include "LightSource.h"

class DirectionalLight : public LightSource {
public:
    glm::vec3 directionFrom(const glm::vec3& P) const override {
        return glm::normalize(-direction);
    }

    glm::vec3 intensityAt(const glm::vec3& P) const override {
        return intensity;
    }
};
