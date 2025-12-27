#pragma once
#include <glm/glm.hpp>

struct Ray {
    glm::vec3 origin{0.0f};// starting point
    glm::vec3 dir{0.0f, 0.0f, -1.0f}; // direction, default pointing in -z - standard camera direction
    int depth = 0;// recursion depth for reflection/refraction rays
    //depth = 0 - ray from camera
    //depth >= 1 - ray after first reflection/refraction

    Ray() = default;
    Ray(const glm::vec3& o, const glm::vec3& d, int dep = 0)
        : origin(o), dir(glm::normalize(d)), depth(dep) {}
};
