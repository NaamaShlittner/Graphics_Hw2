#pragma once
#include <glm/glm.hpp>
#include <limits>
#include <optional> // intersection would return std::nullopt if no hit


class Hit
{
private:
public:
    float dist=std::numeric_limits<float>::infinity(); 
    glm::vec3 hitPoint={0.0f,0.0f,0.0f};
    glm::vec3 normal={0.0f,0.0f,0.0f};
    glm::vec3 exitPoint={0.0f,0.0f,0.0f};
    glm::vec3 refractionRayDir={0.0f,0.0f,0.0f};


    Hit() = default;
};

