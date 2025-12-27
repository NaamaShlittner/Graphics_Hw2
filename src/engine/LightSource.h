#pragma once
#include <glm/glm.hpp>

class LightSource
{
public:
    glm::vec3 direction{0.0f,0.0f,0.0f}; 
    glm::vec3 intensity{1.0f,1.0f,1.0f}; //default white light
    ~LightSource() = default;

    //the direction is from the light to point P
    virtual glm::vec3 directionFrom(const glm::vec3& P) const = 0;

    //intensity at point P
    virtual glm::vec3 intensityAt(const glm::vec3& P) const = 0;

private:
};