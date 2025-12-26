#pragma once
#include <memory>
#include <stdexcept>
#include <glm/glm.hpp>
#include "../engine/LightSource.h"
#include "../engine/DirectionalLight.h"
#include "../engine/Spotlight.h"

class LightBuilder
{
public:
    LightBuilder() = default;

    // Set light type
    LightBuilder &setDirectional();

    LightBuilder &setSpotlight();

    // Set common properties
    LightBuilder &setDirection(const glm::vec3 &dir);

    LightBuilder &setIntensity(const glm::vec3 &intens);

    // Spotlight-specific properties
    LightBuilder &setPosition(const glm::vec3 &pos);

    LightBuilder &setCutoffAngle(float angle);

    // Build the light
    std::unique_ptr<LightSource> build();

private:
    enum class LightType
    {
        None,
        Directional,
        Spotlight
    } type = LightType::None;
    glm::vec3 direction{0.0f, -1.0f, 0.0f};
    glm::vec3 intensity{1.0f, 1.0f, 1.0f};
    // Spotlight-specific
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    float cutoffAngle = glm::radians(30.0f);
};