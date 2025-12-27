#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Ray.h"
#include "Hit.h"


// Enum for different material types

class Object3D{
public:
    glm::vec3 color={1.0f,1.0f,1.0f}; //default white color
    float shininess=10.0f; //default shininess

    float reflectiveConst=0.0f; // by default no reflection
    float refractiveConst=0.0f; // by default no refraction
    float shininess=32.0f; // default shininess

    virtual ~Object3D() = default;

    virtual std::optional<Hit> intersect(const Ray& ray)=0;//0 return std::nullopt if no hit, Hit object otherwise
};