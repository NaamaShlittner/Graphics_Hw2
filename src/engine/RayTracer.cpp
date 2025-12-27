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

    // no hit found
    if (!traceResult) {
        return glm::vec3(0.0f);
    }

    const auto& obj = traceResult->object;
    const glm::vec3 P = traceResult->hit.hitPoint;

    glm::vec3 N = glm::normalize(traceResult->hit.normal);

    // Make sure normal faces against incoming ray
    if (glm::dot(N, ray.dir) > 0.0f) {
        N = -N;
    }
    glm::vec3 V = glm::normalize(-ray.dir);//view direction

    const glm::vec3 Ka = obj->color;
    const glm::vec3 Kd = obj->color;
    const glm::vec3 Ks = obj->specularConst;
    const float n  = obj->shininess;
    const glm::vec3 Ie = obj->emission;

    //emission + ambient
    glm::vec3 color = Ie + Ka * ambientLight;

    // Lighting loop
    for (const auto& light : lightSorces) {
        if (isInShadow(P, objects, *light)) {
            continue;
        }

        glm::vec3 L = glm::normalize(light->directionFrom(P));// from P to light
        glm::vec3 Ii = light->intensityAt(P);// light intensity at P (RGB)

        //Diffuse
        float NdotL = std::max(glm::dot(N, L), 0.0f);
        glm::vec3 diffuse = Kd * NdotL;

        //Specular
        glm::vec3 R = glm::reflect(-L, N);
        float VdotR = std::max(glm::dot(V, R), 0.0f);
        float specFactor = std::pow(VdotR, n);
        glm::vec3 specular = Ks * specFactor;

        color += (diffuse + specular)* Ii;
    }

    //Reflection
    if (obj->reflectiveConst > 0.0f && ray.depth < maxDepth) {
        const float EPS = 1e-4f;
        glm::vec3 reflDir = glm::reflect(ray.dir, N);
        Ray reflRay(P + EPS * reflDir, reflDir, ray.depth + 1);
        glm::vec3 reflectedColor = shadeHit(reflRay, objects, ambientLight, lightSorces, maxDepth);

        color += obj->reflectiveConst * reflectedColor;
    }

    color = glm::clamp(color, glm::vec3(0.0f), glm::vec3(1.0f));
    return color;
}

