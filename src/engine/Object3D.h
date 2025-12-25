#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Ray.h"
#include "Hit.h"


// Enum for different material types
//we use thos for now for simplicity, they can be extened later
enum class MaterialType {
    Opaque,
    Reflective,
    Refractive
};

class Object3D{
public:
    glm::vec3 color={1.0f,1.0f,1.0f}; //default white color
    MaterialType material=MaterialType::Opaque; //default material is opaque

    virtual ~Object3D() = default;

    virtual Hit intersect(const Ray& ray)=0;//0 means it's a pure virtual function- makes the class abstract
};