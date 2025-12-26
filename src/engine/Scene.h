#include <vector>
#include <memory>
#include "Camera.h"
#include "Object3D.h"
#include "LightSource.h"

class Scene
{
private:
    /* data */
public:
    const std::vector<std::shared_ptr<Object3D>> objs;
    const std::vector<std::shared_ptr<LightSource>> lights;
    const glm::vec3 ambientLight={0.0f,0.0f,0.0f};   // Ambient intensity (r,g,b)
    const Camera cam;

    Scene(std::vector<std::shared_ptr<Object3D>> objs,
          std::vector<std::shared_ptr<LightSource>> lights,
          glm::vec3 ambientLight,
          Camera cam)
        : objs(std::move(objs)), lights(std::move(lights)), ambientLight(ambientLight), cam(cam) {}
};

