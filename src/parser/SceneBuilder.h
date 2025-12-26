#pragma once
#include <memory>
#include <stdexcept>
#include <glm/glm.hpp>
#include "../engine/Camera.h"
#include "../engine/Object3D.h"
#include "../engine/LightSource.h"
#include "../engine/Scene.h"

class SceneBuilder
{
public:
    SceneBuilder() = default;

    SceneBuilder &setObjects(const std::vector<std::shared_ptr<Object3D>> &objects);

    SceneBuilder &setLights(const std::vector<std::shared_ptr<LightSource>> &lights);

    SceneBuilder &setAmbientLight(const glm::vec3 &ambientLight);

    SceneBuilder &setCamera(const Camera &camera);

    std::unique_ptr<Scene> build();

    private:
    std::vector<std::shared_ptr<Object3D>> objects;
    std::vector<std::shared_ptr<LightSource>> lights;
    glm::vec3 ambientLight{0.0f, 0.0f, 0.0f};
    Camera camera = {};
};