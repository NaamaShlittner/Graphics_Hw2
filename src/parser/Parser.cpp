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
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open scene file: " + filename);
    }
    std::vector<std::shared_ptr<Object3DBuilder>> objectBuilders;
    std::vector<std::shared_ptr<LightBuilder>> lightBuilders;
    CameraBuilder cameraBuilder = buildCameraFromFile(file);
    glm::vec3 ambiantLight = parseAmbiantLight(file);

    parseObjectsAndLights(file, objectBuilders, lightBuilders);

    // after we have parsed everything, we build the camera, lights, objects and than the scene
    sceneBuilder.setCamera(*cameraBuilder.build())
        .setAmbientLight(ambiantLight);
    std::vector<std::shared_ptr<Object3D>> objects; // final objects list
    for (const auto &objBuilder : objectBuilders)
    {
        objects.push_back(std::shared_ptr<Object3D>(objBuilder->build().release()));
    }
    sceneBuilder.setObjects(objects);
    // release object builders memory
    objectBuilders.clear();
    std::vector<std::shared_ptr<LightSource>> lights; // final lights list
    for (const auto &lightBuilder : lightBuilders)
    {
        lights.push_back(std::shared_ptr<LightSource>(lightBuilder->build().release()));
    }
    sceneBuilder.setLights(lights);
    // release light builders memory
    lightBuilders.clear();

    // build and return the scene
    return sceneBuilder.build();
}

CameraBuilder buildCameraFromFile(std::ifstream &file)
{
    CameraBuilder cameraBuilder = CameraBuilder();

    std::string line;
    char lineType;
    float x, y, z, w;

    // Parse camera lines (e, u, f)
    for (int i = 0; i < 3; i++)
    {
        if (!std::getline(file, line))
        {
            throw std::runtime_error("Invalid scene file: missing camera data");
        }

        // Parse each line in this format: <lineType> <x> <y> <z> <w>
        std::istringstream iss(line);
        if (!(iss >> lineType >> x >> y >> z >> w))
        {
            throw std::runtime_error("Invalid scene file: malformed camera line");
        }

        switch (lineType)
        {
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

glm::vec3 parseAmbiantLight(std::ifstream &file)
{
    std::string line;
    char lineType;
    float r, g, b, c;

    // Read ambiant light line
    if (!std::getline(file, line))
    {
        throw std::runtime_error("Invalid scene file: missing ambiant light data");
    }

    // Parse the line in this format: a <r> <g> <b>
    std::istringstream iss(line);
    if (!(iss >> lineType >> r >> g >> b >> c) || lineType != 'a' || c != 1.0f)
    {
        throw std::runtime_error("Invalid scene file: malformed ambiant light line");
    }
    return glm::vec3(r, g, b);
}

void parseObjectsAndLights(std::ifstream &file,
                           std::vector<std::shared_ptr<Object3DBuilder>> &objectBuilders,
                           std::vector<std::shared_ptr<LightBuilder>> &lightBuilders)
{
    int lightsCount = 0;
    int lightsColored = 0;
    int spotlightsCount = 0;
    int spotlightsSet = 0;
    int objectsCount = 0;
    int objectsColored = 0;
    for (std::string line; std::getline(file, line);)
    {
        char lineType;
        float x, y, z, w;
        std::istringstream iss(line);
        if (!(iss >> lineType >> x >> y >> z >> w))
        {
            throw std::runtime_error("Invalid scene file: malformed camera line");
        }
        if (lineType == '0' || lineType == 'r' || lineType == 't')
        {
            // Object line
            auto objBuilder = std::make_shared<Object3DBuilder>();
            if (w > 0.0f)
            {
                // Sphere
                objBuilder->setSphere()
                    .setRadius(w)
                    .setCenter(glm::vec3(x, y, z));
            }
            else
            {
                // Plane
                objBuilder->setPlane()
                    .setNormal(glm::vec3(x, y, z))
                    .setD(w);
            }
            // set material properties
            if (lineType == 'r')
            {
                objBuilder->setReflectiveConst(1.0f);
            }
            else if (lineType == 't')
            {
                objBuilder->setRefractiveConst(1.0f);
            }
            objectBuilders.push_back(objBuilder);
            objectsCount++;
        }
        else if (lineType == 'c')
        {
            // coloring objects
            if (objectBuilders.empty())
            {
                throw std::runtime_error("Invalid scene file: color line before any object");
            }
            objectBuilders[objectsColored]->setColor(glm::vec3(x, y, z));
            objectBuilders[objectsColored]->setShininess(w);
            objectsColored++;
        }
        else if (lineType == 'd' || lineType == 'p' || lineType == 'i')
        {
            // new light source
            if (lineType == 'd')
            {
                auto lightBuilder = std::make_shared<LightBuilder>();
                lightBuilder->setDirection(glm::vec3(x, y, z));
                if (w = 0.0f)
                    lightBuilder->setDirectional();
                else
                {
                    lightBuilder->setSpotlight();
                    spotlightsCount++;
                }
                lightBuilders.push_back(lightBuilder);
                lightsCount++;
            }
            // position line for existing spotlights
            else if (lineType == 'p')
            {
                if (lightBuilders.empty())
                {
                    throw std::runtime_error("Invalid scene file: positions line before any light");
                }
                if (!lightBuilders[spotlightsSet].get()->isSpotlight())
                {
                    throw std::runtime_error("Invalid scene file: position line for non-spotlight light");
                }
                lightBuilders[spotlightsSet]->setPosition(glm::vec3(x, y, z));
                lightBuilders[spotlightsSet]->setCutoffAngle(w);
                spotlightsSet++;
            }
            // intensity line for existing light sources
            else if (lineType == 'i')
            {
                // Intensity line
                if (lightBuilders.empty() || w != 1.0f)
                {
                    throw std::runtime_error("Invalid scene file: intensity line before any light");
                }
                lightBuilders[lightsColored]->setIntensity(glm::vec3(x, y, z));
                lightsColored++;
            }
        }
        else
        {
            throw std::runtime_error("Invalid scene file: unexpected line type in objects/lights section");
        }
    }
}