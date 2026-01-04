#version 300 es
precision highp float;
precision highp int;

struct Camera {
    vec3 pos;
    vec3 forward;
    vec3 right;
    vec3 up;
};

struct Plane {
    vec3 point;
    vec3 normal;
    vec3 color;
};

struct Sphere {
    vec3 center;
    float radius;
    vec3 color;
    int type; // 0: opaque, 1: reflective, 2: refractive
};

struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    float shininess;
    float cutoff; // if > 0.0 then spotlight else directional light
};

struct HitInfo {
    vec3 rayOrigin;
    vec3 rayDir;
    float t;
    vec3 baseColor;
    int inside; // 1 if inside the sphere, 0 otherwise
    vec3 hitPoint;
    vec3 normal;
    int type; // 0: diffuse, 1: reflective, 2: refractive, -1 if no hit
};

const vec3 AMBIENT_LIGHT = vec3(0.1, 0.2, 0.3);

const float NO_HIT = 1e20; // A large value representing no intersection
const float MAX_DIST_FOR_SHADOW = 1e4;

const int TYPE_DIFFUSE = 0;
const int TYPE_REFLECTIVE = 1;
const int TYPE_REFRACTIVE = 2;

const int MAX_SPHERES = 16;
const int MAX_LIGHTS = 4;
const int MAX_DEPTH = 5;

in vec2 vUV;
out vec4 FragColor;

uniform float uTime;
uniform ivec2 uResolution; // width and height of canvas


uniform Camera cam;
uniform Sphere uSpheres[MAX_SPHERES];
uniform int uNumSpheres;

uniform Light uLights[MAX_LIGHTS];
uniform int uNumLights;

uniform Plane uPlane;

vec3 checkerboardColor(vec3 rgbColor, vec3 hitPoint) {
    // Checkerboard pattern
    float scaleParameter = 2.0;
    float checkerboard = 0.0;
    if (hitPoint.x < 0.0) {
    checkerboard += floor((0.5 - hitPoint.x) / scaleParameter);
    }
    else {
    checkerboard += floor(hitPoint.x / scaleParameter);
    }
    if (hitPoint.z < 0.0) {
    checkerboard += floor((0.5 - hitPoint.z) / scaleParameter);
    }
    else {
    checkerboard += floor(hitPoint.z / scaleParameter);
    }
    checkerboard = (checkerboard * 0.5) - float(int(checkerboard * 0.5));
    checkerboard *= 2.0;
    if (checkerboard > 0.5) {
    return 0.5 * rgbColor;
    }
    return rgbColor;
}

bool intersectSphere(vec3 rayOrigin, vec3 rayDir, Sphere sphere, out float t, out vec3 normal, out int inside) {
    vec3 oc = rayOrigin - sphere.center;

    float b = dot(oc, rayDir);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;
    float h = b * b - c;

    if (h < 0.0) return false;

    h = sqrt(h);

    float t0 = -b - h;
    float t1 = -b + h;

    t = t0;
    inside = 0;

    if (t < 0.001) {
        t = t1;
        inside = 1;
        if (t < 0.001) return false;
    }

    vec3 hitPoint = rayOrigin + t * rayDir;
    normal = normalize(hitPoint - sphere.center);

    if (inside == 1)
        normal = -normal;

    return true;
}

bool intersectPlane(vec3 rayOrigin, vec3 rayDir, Plane plane, out float t, out vec3 normal) {
    float denom = dot(rayDir, plane.normal);

    if (abs(denom) < 0.0001)
        return false;

    t = dot(plane.point - rayOrigin, plane.normal) / denom;

    if (t < 0.001)
        return false;

    normal = plane.normal;
    return true;
}

bool isInShadow(vec3 point, vec3 lightDir) {
    for (int i = 0; i < uNumSpheres; i++) {
        Sphere sphere = uSpheres[i];
        vec3 oc = point - sphere.center;
        float b = dot(oc, lightDir);
        float c = dot(oc, oc) - sphere.radius * sphere.radius;
        float discriminant = b*b - c; // quadratic discriminant
        if (discriminant > 0.0) { // if discriminant > 0, there is an intersection, so the point is in shadow
            float t = -b - sqrt(discriminant);
            if (t > 0.001 && t < MAX_DIST_FOR_SHADOW) return true;
        }
    }

    float denom = dot(uPlane.normal, lightDir);
    if (abs(denom) > 0.0001) {
        float t = dot(uPlane.point - point, uPlane.normal) / denom;
        if (t > 0.001 && t < MAX_DIST_FOR_SHADOW) return true;
    }

    return false;
}

/* intersects scene. gets ray origin and direction, returns hit data*/
HitInfo intersectScene(vec3 rayOrigin, vec3 rayDir) {
    HitInfo closestHit;
    closestHit.t = NO_HIT;
    closestHit.type = -1;

    // sphere intersections
    for (int i = 0; i < uNumSpheres; i++) {
        float t;
        vec3 normal;
        int inside;
        if (intersectSphere(rayOrigin, rayDir, uSpheres[i], t, normal, inside)) {
            if (t < closestHit.t) {
                closestHit.t = t;
                closestHit.rayOrigin = rayOrigin;
                closestHit.rayDir = rayDir;
                closestHit.baseColor = uSpheres[i].color;
                closestHit.inside = inside;
                closestHit.hitPoint = rayOrigin + t * rayDir;
                closestHit.normal = normal;
                closestHit.type = uSpheres[i].type;
            }
        }
    }

    // plane intersection
    float tPlane;
    vec3 normalPlane;
    if (intersectPlane(rayOrigin, rayDir, uPlane, tPlane, normalPlane)) {
        if (tPlane < closestHit.t) {
            closestHit.t = tPlane;
            closestHit.hitPoint = rayOrigin + tPlane * rayDir;
            closestHit.rayOrigin = rayOrigin;
            closestHit.rayDir = rayDir;
            closestHit.baseColor = checkerboardColor(uPlane.color, closestHit.hitPoint);
            closestHit.inside = 0;
            closestHit.normal = normalPlane;
            closestHit.type = TYPE_DIFFUSE;
        }
    }
            
    return closestHit;

}

/* calculates color based on hit data and uv coordinates */
vec3 calcColor(HitInfo hit) {
    vec3 color = vec3(0.0);
    vec3 rayOrigin = hit.rayOrigin;
    vec3 rayDir = hit.rayDir;
    float weight = 1.0;

    const float airRefractiveIndex = 1.0;
    const float sphereRefractiveIndex = 1.5;

    for (int depth = 0; depth < MAX_DEPTH; depth++) {
        HitInfo h = hit;
        if (depth > 0) {h = intersectScene(rayOrigin, rayDir);} // if not depth 0, re-intersect the scene

        if (h.t == NO_HIT) {
            color += weight * vec3(0.0); // background
            break;
        }
        if (h.type == TYPE_DIFFUSE) 
        {
            // local lighting (specular + shadows)
            vec3 localColor = h.baseColor * AMBIENT_LIGHT;
            vec3 viewDir = normalize(-rayDir);

            for (int i = 0; i < uNumLights; i++) {
                Light light = uLights[i];
                vec3 lightDir;
                if (light.cutoff > 0.0) {
                    // spotlight
                    lightDir = normalize(light.position - h.hitPoint);
                    if (dot(lightDir, normalize(-light.direction)) < acos(light.cutoff)) continue;
                } else {
                    // directional
                    lightDir = normalize(-light.direction);
                }

                if (isInShadow(h.hitPoint + h.normal * 0.001, lightDir)) continue;

                // diffuse
                float diff = max(dot(h.normal, lightDir), 0.0);
                vec3 diffuse = diff * h.baseColor * light.color;

                // specular
                vec3 reflectDir = reflect(-lightDir, h.normal);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess);
                vec3 specular = spec * light.color;

                localColor += diffuse + specular;
            }

            color += weight * localColor;
            break; // no further tracing for diffuse surfaces
        }

        // handle reflection/refraction
        if (h.type == TYPE_REFLECTIVE) {
            rayOrigin = h.hitPoint + h.normal * 0.001;
            rayDir = reflect(rayDir, h.normal);
            weight *= 0.8; // attenuate
        } 
        else if (h.type == TYPE_REFRACTIVE) {
            rayOrigin = h.hitPoint - h.normal * 0.001; // enter inside
            float eta = h.inside == 1 ? sphereRefractiveIndex / airRefractiveIndex : airRefractiveIndex / sphereRefractiveIndex;
            rayDir = refract(rayDir, h.normal, eta);
            weight *= 0.8; // attenuate
        }
    }

    return color;
}

/* scales UV coordinates based on resolution
 * uv given uv are [0, 1] range
 * returns new coordinates where y range [-1, 1] and x scales according to window resolution
 */
vec2 scaleUV(vec2 uv) {
    float aspect = float(uResolution.x) / float(uResolution.y);
    uv = uv * 2.0 - 1.0;
    uv.x *= aspect;
    return uv;
}

void main() {
    vec2 uv = scaleUV(vUV);
    vec3 rayDir = normalize(cam.forward + uv.x * cam.right + uv.y * cam.up);

    HitInfo hitInfo = intersectScene(cam.pos, rayDir);

    vec3 color = calcColor(hitInfo);

    // test
    // remove when implementing raytracer
    // color = vec3(vUV.x, vUV.y, abs(sin(uTime)));
    FragColor = vec4(color, 1.0);
}

