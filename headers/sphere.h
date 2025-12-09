#ifndef SPHERE_H
#define SPHERE_H

#include "rtweekend.h"  // For utility functions and types; centralized to reduce include clutter in geometry classes.
#include "hittable.h"   // For base interface; enables polymorphic use in scene lists.

/*
 * Concrete sphere geometry.
 * Derives from hittable for uniform integration into aggregate structures; uses private members for encapsulation and immutability post-construction.
 */
class sphere : public hittable {
private:
    point3 center;
    double radius;
    shared_ptr<material> mat;

public:
    /*
     * Constructor.
     * Takes const ref for efficiency; clamps radius to non-negative for robustness against invalid inputs.
     */
    sphere(const point3& center, double radius, shared_ptr<material> mat) 
	    : center(center), radius(std::fmax(0, radius)), mat(mat) {}

    /*
     * Intersection override.
     * Uses optimized quadratic form (with h) for fewer operations and better numerical stability; computes only necessary roots to minimize sqrt calls.
     */
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;
        auto discriminant = h * h - a * c;
        if (discriminant < 0) {
            return false;
        }
        auto sqrtd = std::sqrt(discriminant);
        // Prefers nearest root first for early closest-hit determination; checks interval to prune invalid intersections efficiently.
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                return false;
            }
        }
        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
	rec.mat = mat;
        return true;
    }
};

#endif
