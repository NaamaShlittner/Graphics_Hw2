#include "Plane.h"

std::optional<Hit> Plane::intersect(const Ray& ray) {
    glm::vec3 d0 = d-ray.origin;
    float denom = glm::dot(normal, ray.dir);
    if (glm::abs(denom) < 1e-6f) {
        // Ray is parallel to the plane
        return std::nullopt;
    }
    float t = glm::dot(normal, d0) / denom;
    if (t >= 0.0f) {
        Hit hit;
        hit.dist = t;
        hit.hitPoint = ray.origin + t * ray.dir;
        hit.normal = normal;
        hit.exitPoint = hit.hitPoint; //for a plane, the exit point is the same as the hit point
        glm::vec3 I = ray.dir;
        glm::vec3 N = hit.normal;
        hit.refractionRayDir = glm::normalize(I - 2.0f * glm::dot(N, I) * N); //refraction direction using Snell's law for air to plane material

        return hit;
    }
    return std::nullopt;
}
