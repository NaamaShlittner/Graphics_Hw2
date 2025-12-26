#pragma once
#include <memory>
#include <stdexcept>
#include <glm/glm.hpp>
#include "../engine/LightSource.h"
#include "../engine/DirectionalLight.h"
#include "../engine/Spotlight.h"

class LightBuilder {
    enum class LightType {
        Directional,
        Spotlight
    };

public:
    LightBuilder() = default;

    // Set light type
    LightBuilder& setDirectional() {
        type = LightType::Directional;
        return *this;
    }

    LightBuilder& setSpotlight() {
        type = LightType::Spotlight;
        return *this;
    }

    // Set common properties
    LightBuilder& setDirection(const glm::vec3& dir) {
        direction = dir;
        return *this;
    }

    LightBuilder& setIntensity(const glm::vec3& intens) {
        intensity = intens;
        return *this;
    }

    // Spotlight-specific properties
    LightBuilder& setPosition(const glm::vec3& pos) {
        if (type != LightType::Spotlight) {
            throw std::runtime_error("Position can only be set for Spotlight type");
        }
        position = pos;
        return *this;
    }

    LightBuilder& setCutoffAngle(float angle) {
        if (type != LightType::Spotlight) {
            throw std::runtime_error("Cutoff angle can only be set for Spotlight type");
        }
        cutoffAngle = angle;
        return *this;
    }

    // Build the light
    std::unique_ptr<LightSource> build() {
        if (type == LightType::Directional) {
            return std::make_unique<DirectionalLight>(direction, intensity);
        } else if (type == LightType::Spotlight) {
            return std::make_unique<Spotlight>(position, direction, intensity, cutoffAngle);
        }
        return nullptr;
    }

private:
    LightType type = LightType::Directional;
    glm::vec3 direction{0.0f, -1.0f, 0.0f}; // Default pointing down
    glm::vec3 intensity{1.0f, 1.0f, 1.0f};
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    float cutoffAngle = glm::radians(30.0f);
};