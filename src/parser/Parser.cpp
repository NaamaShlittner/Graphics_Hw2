#include "SceneBuilder.h"
#include "LightBuilder.h"
#include "CameraBuilder.h"
#include "Object3DBuilder.h"
#include <fstream>
#include <sstream>
#include <string>

std::unique_ptr<Scene> buildFromFile(const std::string &filename)
{
    SceneBuilder sceneBuilder = SceneBuilder();
    
    // open the file:
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open scene file: " + filename);
    }


    return sceneBuilder.build();
}

CameraBuilder buildCameraFromFile(std::ifstream &file)
{    
    CameraBuilder cameraBuilder = CameraBuilder();

    std::string line;
    char lineType;
    float x, y, z, w;
    
    // Parse camera lines (e, u, f)
    for (int i = 0; i < 3; i++) {
        if (!std::getline(file, line)) {
            throw std::runtime_error("Invalid scene file: missing camera data");
        }
        
        // Parse each line in this format: <lineType> <x> <y> <z> <w>
        std::istringstream iss(line);
        if (!(iss >> lineType >> x >> y >> z >> w)) {
            throw std::runtime_error("Invalid scene file: malformed camera line");
        }
        
        switch(lineType) {
            case 'e': // posiotion and distance to screen
                cameraBuilder.setPosition(glm::vec3(x, y, z))
                            .setDistanceToProjectionPlane(w);
                break;
            case 'u': // up vector and screen height
                cameraBuilder.setUpVector(glm::vec3(x, y, z))
                            .setScreenHeight(w);
                break;
            case 'f': // forward vector and screen width
                cameraBuilder.setForwardVector(glm::vec3(x, y, z))
                            .setScreenWidth(w);
                break;
            default:
                throw std::runtime_error("Invalid scene file: unexpected line type in camera section");
        }
    }
    
    return cameraBuilder;
}