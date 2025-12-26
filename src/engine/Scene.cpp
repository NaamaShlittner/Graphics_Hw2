#include "Scene.h"

Scene::Scene(std::vector<std::shared_ptr<Object3D>> objs,
             std::vector<std::shared_ptr<LightSource>> lights,
             glm::vec3 ambientLight,
             Camera cam)
    : objs(std::move(objs)), lights(std::move(lights)), ambientLight(ambientLight), cam(cam) {}
