#include "Sphere.h"


// returns a hit object if the ray intersects the sphere, std::nullopt otherwise
std::optional<Hit> Sphere::intersect(const Ray& ray) {

    glm::vec3 L = ray.origin - center;
    if(glm::length(L) < radius){
        //ray origin is inside the sphere
        return std::nullopt;
    }
    float tm = glm::dot(ray.dir, L);
    float d2 = glm::dot(L, L) - tm * tm;
    if(d2 > radius * radius){
        //ray misses the sphere
        return std::nullopt;
    }
    float th = sqrt(radius * radius - d2);
    float t1 = tm - th;
    float t2 = tm + th;
    float t = (t1 > 0.0f) ? t1 : t2; // choose the closest positive intersection
    if(t < 0.0f){
        //both intersections are negative
        return std::nullopt;
    }
    Hit hit;
    hit.dist = t;
    hit.hitPoint = ray.origin + t * ray.dir;
    hit.normal = glm::normalize(hit.hitPoint - center);
    if(t2 > 0.0f){
        hit.exitPoint = ray.origin + t2 * ray.dir;
    }
    glm::vec3 I = ray.dir;
    glm::vec3 N = hit.normal;
    hit.refractionRayDir = glm::normalize(I - 2.0f * glm::dot(N, I) * N); //refraction direction using Snell's law for air to sphere material

    return hit;
}
