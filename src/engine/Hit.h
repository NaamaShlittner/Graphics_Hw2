#pragma once
#include <glm/glm.hpp>
#include <limits>


class Hit
{
private:
    /* data */
public:
    float dist=std::numeric_limits<float>::infinity(); //float because distance can be fractional, infinity means no hit
    bool didHit = false; //indicates whether there was a hit
    //we set defult values to avoid uninitialized variable issues
    glm::vec3 hitPoint={0.0f,0.0f,0.0f};
    glm::vec3 normal={0.0f,0.0f,0.0f};
    glm::vec3 exitPoint={0.0f,0.0f,0.0f};
    glm::vec3 refractionRayDir={0.0f,0.0f,0.0f};


    Hit() = default;
};

