#include "RayTracer.h"
#include "Object3D.h"
#include "Spotlight.h"
#include <limits>

std::optional<TraceResult> RayTracer::traceClosestHit(const Ray& ray, const std::vector<std::shared_ptr<Object3D>>& objects) const {
    std::optional<TraceResult> closestHit = std::nullopt;
    float closestDist = std::numeric_limits<float>::infinity();

    for (const auto& object : objects) {
        std::optional<Hit> hit = object->intersect(ray);
        if (hit && hit->dist < closestDist) {
            if(hit->dist < 0.0f) {
                continue; // Ignore negative distances
            }
            closestDist = hit->dist;
            closestHit.emplace(TraceResult{object, *hit});
        }
    }

    return closestHit;
}

bool RayTracer::isInShadow(const glm::vec3& point, const std::vector<std::shared_ptr<Object3D>>& objects, const LightSource& light) const {
    const float EPS = 1e-4f;//epsilon to avoid self-intersection
    glm::vec3 L = glm::normalize(light.directionFrom(point));

    Ray shadowRay(point + EPS * L, L);//so we don't self-intersect

    float maxDist = light.maxShadowDistance(point);
        
    auto occ = traceClosestHit(shadowRay, objects);
    return occ && occ->hit.dist > EPS && occ->hit.dist < (maxDist - EPS);
}

glm::vec3 RayTracer::shadeHit(const Ray& ray, const std::vector<std::shared_ptr<Object3D>>& objects, const glm::vec3& ambientLight, const std::vector<std::shared_ptr<LightSource>>& lightSorces, int maxDepth) const {
    auto traceResult = traceClosestHit(ray, objects);

    //no hit found, return background color
    if(!traceResult) {
        return glm::vec3(0.0f, 0.0f, 0.0f);// Background color
    }
    const glm::vec3& normal = traceResult->hit.normal;
    const glm::vec3& materialDiffuse = traceResult->object->color;
    glm::vec3 baseColor = traceResult->object->color * ambientLight;
    
    glm::vec3 diffusedColor;
    for (const auto& light : lightSorces) {
        diffusedColor += materialDiffuse * light->intensityAt(traceResult->hit.hitPoint) *
            std::max(glm::dot(normal, glm::normalize(light->directionFrom(traceResult->hit.hitPoint))), 0.0f);
    }


    return baseColor+defusedColor;
}

