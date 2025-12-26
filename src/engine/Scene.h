#include <vector>
#include <memory>
#include "Camera.h"
#include "Object3D.h"
#include "LightSource.h"

class Camera; // wtf is camera? where is it defined?

class Scene
{
private:
    /* data */
public:
    std::vector<std::shared_ptr<Object3D>> objs;
    std::vector<std::shared_ptr<LightSource>> lights;
    glm::vec3 ambientLight={0.0f,0.0f,0.0f};   // Ambient intensity (r,g,b)
    Camera cam;
    Scene() = default;
};

