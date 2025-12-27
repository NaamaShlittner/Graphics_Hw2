#include <glm/glm.hpp>
#include <limits>
#include "DirectionalLight.h"
#include "LightSource.h"


glm::vec3 DirectionalLight::directionFrom(const glm::vec3& P) const
{
    return glm::normalize(-direction);
}

glm::vec3 DirectionalLight::intensityAt(const glm::vec3& P) const
{
    return intensity;
}

float DirectionalLight::maxShadowDistance(const glm::vec3& P) const
{
    return std::numeric_limits<float>::infinity();
}

