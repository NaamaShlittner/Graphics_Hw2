#pragma once
#include <glm/glm.hpp>
#include <optional>
#include <vector>
#include <memory>
#include "Hit.h"

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
    RayTracer() = default;
    ~RayTracer() = default;
    //find closest hit among all objects in the scene
    //returns std::nullopt if no hit is found
    //check every object in the scene for intersection with the ray
    std::optional<TraceResult> traceClosestHit(const Ray& ray, const std::vector<std::shared_ptr<Object3D>>& objects) const;
    
    //return the color at the intersection point
    glm::vec3 shadeHit(const Ray& ray, const std::vector<std::shared_ptr<Object3D>>& objects, const glm::vec3& ambientLight, const std::vector<std::shared_ptr<LightSource>>& lightSorces, int maxDepth) const;

    bool RayTracer::isInShadow(const glm::vec3& point, const std::vector<std::shared_ptr<Object3D>>& objects, const LightSource& light) const;

};
