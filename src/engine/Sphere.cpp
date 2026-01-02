#include "Sphere.h"
#include <cstdio>


// returns a hit object if the ray intersects the sphere, std::nullopt otherwise
std::optional<Hit> Sphere::intersect(const Ray& ray) {
    glm::vec3 L = center - ray.origin;
    if(glm::length(L) < radius){
        //ray origin is inside the sphere
        // if (!inside) {printf("Ray origin is inside the sphere\n"); inside = true;}
        return std::nullopt;
    }
    // geometric solution
    float tm = glm::dot(ray.dir, L);
    float d2 = glm::dot(L, L) - tm * tm;
    if(d2 > radius * radius){
        //ray misses the sphere
        // if (!missed) printf("Ray misses the sphere\n");
        missed = true;
        return std::nullopt;
    }
    // half chord distance
    float th = sqrt(radius * radius - d2);
    // t1 and t2 are the distances to the intersection points
    float t1 = tm - th;
    float t2 = tm + th;
    float t = (t1 > 0.0f) ? t1 : t2; // choose the closest positive intersection
    if(t < 0.0f){
        //both intersections are negative
        // if (!bothNegative) {printf("Both intersections are negative\n"); printf("t1: %.2f, t2: %.2f\n", t1, t2);}
        bothNegative = true;
        return std::nullopt;
    }
    // create Hit object
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
    // if (this->hitCount < 10) { printf("Sphere Intersection at point: %.2f\n", hit.dist); this->hitCount++; }

    return hit;
}

glm::vec3 Sphere::ColorAtPoint(const glm::vec3& point) {
    return color;
}
