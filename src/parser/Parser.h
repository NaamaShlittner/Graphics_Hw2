#pragma once
#include <fstream>
#include <memory>
#include <string>
#include "SceneBuilder.h"
#include "CameraBuilder.h"
#include "LightBuilder.h"
#include "Object3DBuilder.h"

CameraBuilder parseCamera(std::ifstream &file);

glm::vec3 parseAmbiantLight(std::ifstream &file);

void parseObjectsAndLights(std::ifstream &file,
                           std::vector<std::shared_ptr<Object3DBuilder>> &objectBuilders,
                           std::vector<std::shared_ptr<LightBuilder>> &lightBuilders);

std::unique_ptr<Scene> buildFromFile(const std::string &filename);