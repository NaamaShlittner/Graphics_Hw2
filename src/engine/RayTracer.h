#pragma once
#include <glm/glm.hpp>
#include <optional>
#include <vector>
#include <memory>
#include "Hit.h"
#include "Scene.h"

class Ray;
class Object3D;
class LightSource;

struct TraceResult {
    std::shared_ptr<Object3D> const object;
    Hit hit;
};

class RayTracer
{
public:
    RayTracer(const Scene& scene, int screenPixelWidth, int screenPixelHeight, int maxRecursionDepth);
    ~RayTracer() = default;
    //find closest hit among all objects in the scene
    //returns std::nullopt if no hit is found
    //check every object in the scene for intersection with the ray
    std::optional<TraceResult> traceClosestHit(const Ray& ray, const std::vector<std::shared_ptr<Object3D>>& objects) const;
    
    //return the color at the intersection point
    glm::vec3 shadeHit(const Ray& ray, const std::vector<std::shared_ptr<Object3D>>& objects, const glm::vec3& ambientLight, const std::vector<std::shared_ptr<LightSource>>& lightSorces, int maxDepth) const;

    //check if point is in shadow relative to a light source
    bool isInShadow(const glm::vec3& point, const std::vector<std::shared_ptr<Object3D>>& objects, const LightSource& light) const;

    std::vector<unsigned char> renderImage(); // returns a flat array of pixel colors (RGB)

private:
    const Scene& scene;
    const int screenPixelWidth;
    const int screenPixelHeight;
    const int maxRecursionDepth;
};
