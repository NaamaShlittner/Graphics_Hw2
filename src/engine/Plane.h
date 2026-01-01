#pragma once
#include "Object3D.h"

//ax + by + cz + d = 0

class Plane : public Object3D {
public:
    glm::vec3 normal{0.0f, 1.0f, 0.0f};//normal vector
    float d = 0.0f;//constant term in plane equation

    Plane() = default;

    // normalize the normal and scale d by the same factor so the plane stays the same
    Plane(const glm::vec3& n, float dVal);

    std::optional<Hit> intersect(const Ray& ray) override;
};
