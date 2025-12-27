#pragma once
#include <memory>
#include <stdexcept>
#include <glm/glm.hpp>
#include "../engine/Object3D.h"
#include "../engine/Sphere.h"
#include "../engine/Plane.h"

class Object3DBuilder
{
public:
    Object3DBuilder() = default;

    // Set object type
    Object3DBuilder &setSphere();

    Object3DBuilder &setPlane();

    // Set common properties
    Object3DBuilder &setColor(const glm::vec3 &color);

    Object3DBuilder &setReflectiveConst(float reflectiveConst);

    Object3DBuilder &setRefractiveConst(float refractiveConst);

    Object3DBuilder &setShininess(float shininess);

    // Sphere-specific properties
    Object3DBuilder &setCenter(const glm::vec3 &center);

    Object3DBuilder &setRadius(float radius);

    // Plane-specific properties
    Object3DBuilder &setNormal(const glm::vec3 &normal);

    Object3DBuilder &setD(float d);

    // Build the object
    std::unique_ptr<Object3D> build();

private:
    enum class ObjectType
    {
        None,
        Sphere,
        Plane
    } type = ObjectType::None;
    glm::vec3 color{1.0f, 1.0f, 1.0f};
    float reflectiveConst{0.0f};
    float refractiveConst{0.0f};
    float shininess{32.0f};
    // Sphere-specific
    float radius{1.0f};
    glm::vec3 center{0.0f, 0.0f, 0.0f};
    // Plane-specific
    glm::vec3 normal{0.0f, 1.0f, 0.0f};
    float d{0.0f};
};