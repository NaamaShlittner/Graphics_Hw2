#include "SceneBuilder.h"


SceneBuilder &SceneBuilder::setObjects(const std::vector<std::shared_ptr<Object3D>> &objects)
{
    this->objects = objects;
    return *this;
}

SceneBuilder &SceneBuilder::setLights(const std::vector<std::shared_ptr<LightSource>> &lights)
{
    this->lights = lights;
    return *this;
}

SceneBuilder &SceneBuilder::setAmbientLight(const glm::vec3 &ambientLight)
{
    this->ambientLight = ambientLight;
    return *this;
}

SceneBuilder &SceneBuilder::setCamera(const Camera &camera)
{
    this->camera = camera;
    return *this;
}

std::unique_ptr<Scene> SceneBuilder::build()
{
    return std::make_unique<Scene>(objects, lights, ambientLight, camera);
}
