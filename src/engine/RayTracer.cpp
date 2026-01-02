#include "RayTracer.h"
#include "Object3D.h"
#include "Spotlight.h"
#include <iostream>
#include <limits>
#include <algorithm>

const float REFRACTION_INX_AIR_2_MAT = 1.0f / 2.6f;

RayTracer::RayTracer(const Scene &scene, int screenPixelWidth, int screenPixelHeight, int maxRecursionDepth) : scene(scene), screenPixelWidth(screenPixelWidth), screenPixelHeight(screenPixelHeight), maxRecursionDepth(maxRecursionDepth)
{
}

/// @brief Traces the closest hit of a ray with a list of objects.
/// @param ray The ray to trace.
/// @param objects The list of objects to test for intersection.
/// @return An optional TraceResult containing the closest hit information if any.
std::optional<TraceResult> RayTracer::traceClosestHit(const Ray &ray, const std::vector<std::shared_ptr<Object3D>> &objects) const
{
    std::optional<TraceResult> closestHit = std::nullopt;
    float closestDist = std::numeric_limits<float>::infinity();

    for (const auto &object : objects)
    {
        std::optional<Hit> hit = object->intersect(ray);
        if (hit && hit->dist < closestDist)
        {
            if (hit->dist < 0.0f)
            {
                continue; // Ignore negative distances - behind the ray origin
            }
            closestDist = hit->dist;
            closestHit.emplace(TraceResult{object, *hit});
        }
    }

    return closestHit;
}

/// @brief Determines if a point is in shadow relative to a light source.
/// @param point The point to test for shadow.
/// @param normal The surface normal at the point.
/// @param objects The list of objects that may cast shadows.
/// @param light The light source to test against.
/// @return True if the point is in shadow, false otherwise.
bool RayTracer::isInShadow(const glm::vec3 &point, const glm::vec3 &normal, const std::vector<std::shared_ptr<Object3D>> &objects, const LightSource &light) const
{
    const float EPS = 1e-4f; // epsilon to avoid self-intersection
    glm::vec3 fromPointToLight = glm::normalize(light.directionFrom(point));

    Ray shadowRay(point + EPS * normal, fromPointToLight); // offset along normal to avoid self-intersection

    float maxDist = light.maxShadowDistance(point);

    auto occ = traceClosestHit(shadowRay, objects);
    return occ && occ->hit.dist > EPS && occ->hit.dist < (maxDist - EPS);
}

/// @brief Shades a hit point by calculating lighting and reflections.
/// @param ray The ray that we send out from the camera.
/// @param objects The list of objects in the scene.
/// @param ambientLight The ambient light color.
/// @param lightSorces The list of light sources.
/// @param maxDepth The maximum recursion depth for recursion.
/// @return
glm::vec3 RayTracer::shadeHit(const Ray &ray, const std::vector<std::shared_ptr<Object3D>> &objects, const glm::vec3 &ambientLight, const std::vector<std::shared_ptr<LightSource>> &lightSources, int maxDepth) const
{
    auto traceResult = traceClosestHit(ray, objects);

    // no hit found
    if (!traceResult)
    {
        return glm::vec3(0.0f);
    }

    const auto &obj = traceResult->object;
    const glm::vec3 P = traceResult->hit.hitPoint;

    glm::vec3 N = traceResult->hit.normal;

    // Make sure normal faces against incoming ray
    if (glm::dot(N, ray.dir) > 0.0f)
    {
        N = -N;
    }
    glm::vec3 V = glm::normalize(-ray.dir); // view direction

    const glm::vec3 Ka = obj->ColorAtPoint(P);
    const glm::vec3 Kd = obj->ColorAtPoint(P);
    const glm::vec3 Ks = obj->specularConst;
    const float n = obj->shininess;
    const glm::vec3 Ie = obj->emission;

    // emission + ambient
    glm::vec3 color = Ie + Ka * ambientLight;

    // Lighting loop
    for (const auto &light : lightSources)
    {
        if (isInShadow(P, N, objects, *light))
        {
            continue;
        }

        glm::vec3 L = light->directionFrom(P);               // from P to light (already normalized at directionFrom)
        glm::vec3 Ii = light->intensityAt(traceResult->hit); // light intensity at P (RGB)

        // Diffuse
        float NdotL = std::max(glm::dot(N, L), 0.0f);
        glm::vec3 diffuse = Kd * NdotL;

        // Specular
        glm::vec3 R = glm::reflect(-L, N);
        float VdotR = std::max(glm::dot(V, R), 0.0f);
        float specFactor = std::pow(VdotR, n);
        glm::vec3 specular = Ks * specFactor;

        // Accumulate color
        color += (diffuse + specular) * Ii;
    }

    // Reflection
    if (obj->reflectiveConst > 0.0f)
    {
        if (ray.depth < maxDepth)
        {
            const float EPS = 1e-4f;
            glm::vec3 reflDir = glm::reflect(ray.dir, N);
            Ray reflRay(P + EPS * reflDir, reflDir, ray.depth + 1);
            glm::vec3 reflectedColor = shadeHit(reflRay, objects, ambientLight, lightSources, maxDepth);

            color = obj->reflectiveConst * reflectedColor + (1.0f - obj->reflectiveConst) * color;
        }
        else
        {
            color = obj->reflectiveConst * glm::vec3(0.0f, 0.0f, 0.0f) + (1.0f - obj->reflectiveConst) * color;
        }
    }

    // Refraction
    if (obj->refractiveConst > 0.0f)
    {
        if (ray.depth < maxDepth)
        {
            const float EPS = 1e-4f;
            glm::vec3 refrDir = glm::refract(ray.dir, N, REFRACTION_INX_AIR_2_MAT);
            Ray refrRay(P + EPS * refrDir, refrDir, ray.depth + 1);
            glm::vec3 refractedColor = shadeHit(refrRay, objects, ambientLight, lightSources, maxDepth);

            color = obj->refractiveConst * refractedColor + (1.0f - obj->refractiveConst) * color;
        }
        else
        {
            color = obj->refractiveConst * glm::vec3(0.0f, 0.0f, 0.0f) + (1.0f - obj->refractiveConst) * color;
        }
    }
    // Clamp final color to [0,1]
    color = glm::clamp(color, glm::vec3(0.0f), glm::vec3(1.0f));
    return color;
}

std::vector<unsigned char> RayTracer::renderImage()
{
    std::vector<unsigned char> image(screenPixelWidth * screenPixelHeight * 4, 0);

    for (int y = 0; y < screenPixelHeight; ++y)
    {
        for (int x = 0; x < screenPixelWidth; ++x)
        {
            Ray ray = scene.cam.generateRay(x, y, screenPixelWidth, screenPixelHeight);
            glm::vec3 color = shadeHit(ray, scene.objs, scene.ambientLight, scene.lights, maxRecursionDepth);

            int index = (y * screenPixelWidth + x) * 4;
            // convert channels and clamp each to [0,255]
            image[index] = static_cast<unsigned char>(glm::clamp((std::round(color.r * 255.0f)), 0.f, 255.f));
            image[index + 1] = static_cast<unsigned char>(glm::clamp((std::round(color.g * 255.0f)), 0.f, 255.f));
            image[index + 2] = static_cast<unsigned char>(glm::clamp((std::round(color.b * 255.0f)), 0.f, 255.f));
            image[index + 3] = 255; // Alpha channel
            // printf("Rendered pixel (%d, %d): Color(%d, %d, %d, %d)\n", x, y, image[index], image[index + 1], image[index + 2], image[index + 3]);
        }
    }

    return image;
}
