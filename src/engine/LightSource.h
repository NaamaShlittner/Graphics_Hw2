#pragma once
#include <glm/glm.hpp>

class LightSource
{
public:
    glm::vec3 direction{0.0f,0.0f,0.0f}; 
    glm::vec3 intensity{1.0f,1.0f,1.0f}; //default white light
    ~LightSource() = default;
    


private:
};