#pragma once
#include "Object3D.h"

//ax + by + cz + d = 0

class Plane : public Object3D {
public:
    glm::vec3 normal{0.0f, 1.0f, 0.0f};//normal vector
    float d = 0.0f;//constant term in plane equation
    glm::vec3 u; // tangent vector
    glm::vec3 v; // bitangent vector

    Plane() = default;

    Plane(const glm::vec3& n, float dVal);

    glm::vec3 ColorAtPoint(const glm::vec3& point) override;

    std::optional<Hit> intersect(const Ray& ray) override;
};
