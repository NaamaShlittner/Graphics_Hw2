#include <glm/glm.hpp>
#include <limits>
#include "DirectionalLight.h"
#include "LightSource.h"

DirectionalLight::DirectionalLight(const glm::vec3& dir, const glm::vec3& intens) : LightSource(dir, intens)
{}

glm::vec3 DirectionalLight::directionFrom(const glm::vec3& P) const
{
    return glm::normalize(-direction);
}

// I_l = I_0 * (D * L)
glm::vec3 DirectionalLight::intensityAt(const Hit& hit) const
{
    return intensity * glm::dot(hit.normal, direction);
}

float DirectionalLight::maxShadowDistance(const glm::vec3& P) const
{
    return std::numeric_limits<float>::infinity();
}

