#include <vector>
#include <memory>
#include "Camera.h"


class Object3D;
class Light;
class Camera;

class Scene
{
private:
    /* data */
public:
    std::vector<std::shared_ptr<Object3D>> objs;
    std::vector<std::shared_ptr<Light>> lights;
    glm::vec3 ambientLight={0.0f,0.0f,0.0f};   // Ambient intensity (r,g,b)
    Camera cam;
    Scene() = default;
};

