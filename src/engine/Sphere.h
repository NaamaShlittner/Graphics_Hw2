#pragma once
#include "Object3D.h"

class Sphere : public Object3D {
public:
    glm::vec3 center{0.0f};
    float radius = 1.0f;

    Sphere() = default;

    Sphere(const glm::vec3& c, float r) 
        : center(c), radius(r) {}

    Hit intersect(const Ray& ray) override;
};
