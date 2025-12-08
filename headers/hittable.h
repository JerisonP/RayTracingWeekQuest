#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

/*
 * Hit record structure.
 * Designed as a lightweight container for intersection data, facilitating decoupling of geometry and material computations.
 */
class hit_record {
public:
    point3 p;
    vec3 normal;
    double t;
    bool front_face;

    /*
     * Normal orientation method.
     * Centralized to ensure consistent handling of ray-side determination, supporting efficient double-sided rendering.
     */
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

/*
 * Abstract hittable interface.
 * Virtual base for polymorphic scene objects, enabling uniform ray intersection queries across different geometries.
 */
class hittable {
public:
    virtual ~hittable() = default;

    /*
     * Intersection test.
     * Parameterized with t-interval to prune invalid hits early, optimizing for complex scenes and acceleration structures.
     */
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif
