#ifndef RAY_H
#define RAY_H

#include "vec3.h" // For vec3 and point3 definitions

/*
 * A 3D ray class for use in ray tracing and graphics.
 * Members are private to encapsulate data and ensure immutability post-construction.
 * Tip: Rays are immutable by design to support safe sharing in multi-threaded environments.
 */
class ray {
private:
    point3 orig;
    vec3 dir;
public:
    /* Constructors */
    ray() {} // Default constructor provided for container compatibility and default initialization.
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {} // Parameterized constructor uses const refs to avoid copies and enhance efficiency.
    /* Accessors */
    // Inlined for performance in frequent access scenarios.
    const point3& origin() const { return orig; }
    const vec3 direction() const { return dir; }
    /* Utility methods */
    // Const method to enable functional-style computations without state changes.
    point3 at(double t) const {
        return orig + t * dir;
    }
};

#endif
