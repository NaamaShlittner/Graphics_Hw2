#include "LightBuilder.h"

LightBuilder& LightBuilder::setDirectional() {
    type = LightType::Directional;
    return *this;
}
LightBuilder& LightBuilder::setSpotlight() {
    type = LightType::Spotlight;
    return *this;
}

LightBuilder& LightBuilder::setDirection(const glm::vec3& dir) {
    direction = dir;
    return *this;
}

LightBuilder& LightBuilder::setIntensity(const glm::vec3& intens) {
    intensity = intens;
    return *this;
}

LightBuilder& LightBuilder::setPosition(const glm::vec3& pos) {
    if (this->type != LightType::Spotlight) {
        throw std::runtime_error("Position can only be set for spotlight type");
    }
    position = pos;
    return *this;
}

LightBuilder& LightBuilder::setCutoffAngle(float angle) {
    if (this->type != LightType::Spotlight) {
        throw std::runtime_error("Cutoff angle can only be set for spotlight type");
    }
    cutoffAngle = angle;
    return *this;
}

std::unique_ptr<LightSource> LightBuilder::build() {
    if (type == LightType::Directional) {
        return std::make_unique<DirectionalLight>(direction, intensity);
    } else if (type == LightType::Spotlight) {
        return std::make_unique<Spotlight>(position, direction, intensity, cutoffAngle);
    } else if (type == LightType::None) {
        throw std::runtime_error("Light type not set");
    }
    throw std::runtime_error("Unknown light type");
}
