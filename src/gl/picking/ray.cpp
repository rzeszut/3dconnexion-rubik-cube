#include <cmath>
#include <algorithm>

#include "gl/picking/ray.hpp"

using namespace glm;
using namespace optional;

namespace gl {
namespace picking {

Ray Ray::fromScreenPosition(float x, float y,
                            int width, int height,
                            const glm::mat4 &view,
                            const glm::mat4 &projection
                           ) {
    // ray start and end positions in normalized device coordinates
    // x, y in [-1, 1]
    // near plane maps to z = -1
    vec4 rayStart_device = vec4(
        (x / (float) width  - 0.5f) * 2.f,
        (y / (float) height - 0.5f) * 2.f,
        -1.f, 1.f);
    vec4 rayEnd_device = vec4(
        (x / (float) width  - 0.5f) * 2.f,
        (y / (float) height - 0.5f) * 2.f,
        0.f, 1.f);

    // goes from device to model
    mat4 inverseViewProjection = inverse(projection * view);
    vec4 rayStart_world = inverseViewProjection * rayStart_device;
    vec4 rayEnd_world = inverseViewProjection * rayEnd_device;

    rayStart_world /= rayStart_world.w;
    rayEnd_world /= rayEnd_world.w;

    Ray ray;
    ray.origin = vec3(rayStart_world);
    ray.direction = normalize(vec3(rayEnd_world - rayStart_world));
    return ray;
}

Optional<float> testRayOBBIntersection(const Ray &ray,
                                       const AABB &aabb,
                                       const glm::mat4 &model
                                      ) {
    float distanceMin = 0.f;
    float distanceMax = 100000.f;

    vec3 boxPosition_worldSpace(model[3]);
    vec3 delta = boxPosition_worldSpace - ray.origin;

    // FIXME: refactoring; remove code duplication

    // x axis
    {
        vec3 xAxis(model[0]);
        float e = dot(xAxis, delta);
        float f = dot(ray.direction, xAxis);

        if (fabs(f) > 0.001f) {
            float dist1 = (e + aabb.min.x) / f;
            float dist2 = (e + aabb.max.x) / f;

            if (dist1 > dist2) {
                std::swap(dist1, dist2);
            }

            if (dist2 < distanceMax) {
                distanceMax = dist2;
            }
            if (dist1 > distanceMin) {
                distanceMin = dist1;
            }

            if (distanceMax < distanceMin) {
                return Optional<float>();
            }
        } else if (aabb.min.x - e > 0.0f || aabb.max.x < 0.f) {
            // ray is almost parallel to the planes
            return Optional<float>();
        }
    }

    // y axis
    {
        vec3 yAxis(model[1]);
        float e = dot(yAxis, delta);
        float f = dot(ray.direction, yAxis);

        if (fabs(f) > 0.001f) {
            float dist1 = (e + aabb.min.y) / f;
            float dist2 = (e + aabb.max.y) / f;

            if (dist1 > dist2) {
                std::swap(dist1, dist2);
            }

            if (dist2 < distanceMax) {
                distanceMax = dist2;
            }
            if (dist1 > distanceMin) {
                distanceMin = dist1;
            }

            if (distanceMax < distanceMin) {
                return Optional<float>();
            }
        } else if (aabb.min.y - e > 0.0f || aabb.max.y < 0.f) {
            // ray is almost parallel to the planes
            return Optional<float>();
        }
    }

    // z axis
    {
        vec3 zAxis(model[2]);
        float e = dot(zAxis, delta);
        float f = dot(ray.direction, zAxis);

        if (fabs(f) > 0.001f) {
            float dist1 = (e + aabb.min.z) / f;
            float dist2 = (e + aabb.max.z) / f;

            if (dist1 > dist2) {
                std::swap(dist1, dist2);
            }

            if (dist2 < distanceMax) {
                distanceMax = dist2;
            }
            if (dist1 > distanceMin) {
                distanceMin = dist1;
            }

            if (distanceMax < distanceMin) {
                return Optional<float>();
            }
        } else if (aabb.min.z - e > 0.0f || aabb.max.z < 0.f) {
            // ray is almost parallel to the planes
            return Optional<float>();
        }
    }

    return Optional<float>(distanceMin);
}

}
}

