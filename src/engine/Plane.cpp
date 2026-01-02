#include "Plane.h"

const float checkerboardScale = 0.55f; // size of each checker square

std::optional<Hit> Plane::intersect(const Ray &ray)
{
    glm::vec3 d0 = d - ray.origin;
    float denom = glm::dot(normal, ray.dir);
    if (glm::abs(denom) < 1e-6f)
    {
        // Ray is parallel to the plane
        return std::nullopt;
    }
    float t = glm::dot(normal, d0) / denom;
    if (t >= 0.0f)
    {
        Hit hit;
        hit.dist = t;
        hit.hitPoint = ray.origin + t * ray.dir;
        hit.normal = normal;
        hit.exitPoint = hit.hitPoint; // for a plane, the exit point is the same as the hit point
        glm::vec3 I = ray.dir;
        glm::vec3 N = hit.normal;
        hit.refractionRayDir = glm::normalize(I - 2.0f * glm::dot(N, I) * N); // refraction direction using Snell's law for air to plane material

        return hit;
    }
    return std::nullopt;
}

// we want to create a checkerboard pattern on the plane
glm::vec3 Plane::ColorAtPoint(const glm::vec3& point)
{
    glm::vec3 localPoint = point - (normal * (-d));

    int checkU = static_cast<int>(floor(glm::dot(localPoint, u) / checkerboardScale));
    int checkV = static_cast<int>(floor(glm::dot(localPoint, v) / checkerboardScale));

    if ((checkU + checkV) % 2 == 0)
        return color * glm::vec3(0.9f, 0.9f, 0.9f); // white
    else
        return color * glm::vec3(0.5f, 0.5f, 0.5f); // black
}

Plane::Plane(const glm::vec3 &n, float dVal)
{
    // here we also need to normalize the normal and the d term accordingly
    float len = glm::length(n);
    if (len > 0.0f)
    {
        normal = n / len;
        d = dVal / len;
    }
    else
    {
        normal = glm::vec3(0.0f, 1.0f, 0.0f);
        d = dVal;
    }

    // create arbitrary u and v vectors on the plane
    glm::vec3 helper =
        (fabs(normal.x) > 0.9f) ? glm::vec3(0,1,0)
                                : glm::vec3(1,0,0);

    u = glm::normalize(glm::cross(helper, normal));
    v = glm::normalize(glm::cross(normal, u));
}
