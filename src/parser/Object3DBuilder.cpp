#include "Object3DBuilder.h"

Object3DBuilder& Object3DBuilder::setSphere() {
    type = ObjectType::Sphere;
    return *this;
}

Object3DBuilder& Object3DBuilder::setPlane() {
    type = ObjectType::Plane;
    return *this;
}

Object3DBuilder& Object3DBuilder::setColor(const glm::vec3& color) {
    this->color = color;
    return *this;
}

Object3DBuilder& Object3DBuilder::setReflectiveConst(float reflectiveConst) {
    this->reflectiveConst = reflectiveConst;
    return *this;
}

Object3DBuilder& Object3DBuilder::setRefractiveConst(float refractiveConst) {
    this->refractiveConst = refractiveConst;
    return *this;
}

Object3DBuilder& Object3DBuilder::setCenter(const glm::vec3& center) {
    if (this->type != ObjectType::Sphere) {
        throw std::runtime_error("Center can only be set for sphere type");
    }
    this->center = center;
    return *this;
}

Object3DBuilder& Object3DBuilder::setRadius(float radius) {
    if (this->type != ObjectType::Sphere) {
        throw std::runtime_error("Radius can only be set for sphere type");
    }
    this->radius = radius;
    return *this;
}

Object3DBuilder& Object3DBuilder::setNormal(const glm::vec3& normal) {
    if (this->type != ObjectType::Plane) {
        throw std::runtime_error("Normal can only be set for plane type");
    }
    this->normal = normal;
    return *this;
}

Object3DBuilder& Object3DBuilder::setD(float d) {
    if (this->type != ObjectType::Plane) {
        throw std::runtime_error("D can only be set for plane type");
    }
    this->d = d;
    return *this;
}

std::unique_ptr<Object3D> Object3DBuilder::build() {
    if (type == ObjectType::Sphere) {
        auto sphere = std::make_unique<Sphere>(radius, center);
        sphere->color = color;
        sphere->reflectiveConst = reflectiveConst;
        sphere->refractiveConst = refractiveConst;
        return sphere;
    } else if (type == ObjectType::Plane) {
        auto plane = std::make_unique<Plane>(normal, d);
        plane->color = color;
        plane->reflectiveConst = reflectiveConst;
        plane->refractiveConst = refractiveConst;
        return plane;
    } else if (type == ObjectType::None) {
        throw std::runtime_error("Object type not set");
    }
    throw std::runtime_error("Unknown object type");
}

