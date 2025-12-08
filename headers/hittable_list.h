#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "rtweekend.h"
#include "hittable.h"

#include <memory>  // For std::shared_ptr; enables shared ownership of hittables for flexible scene management.
#include <vector>  // For dynamic array; chosen for efficient addition and iteration in scene building.

/*
 * Aggregate hittable container.
 * Inherits from hittable to allow nested lists or uniform treatment in scenes; uses shared_ptr for polymorphism and lifetime management.
 */
class hittable_list : public hittable {
public:
    std::vector<std::shared_ptr<hittable>> objects;

    // Default constructor for empty lists; supports incremental building via add().
    hittable_list() {}

    // Convenience constructor; initializes with one object for common single-item scenarios.
    hittable_list(std::shared_ptr<hittable> object) { add(object); }

    // Clear method; provided for reusability and scene resetting without recreating the list.
    void clear() { objects.clear(); }

    // Add method; appends to vector for O(1) amortized time, suitable for large scenes.
    void add(std::shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    /*
     * Override for collective hit testing.
     * Iterates sequentially for simplicity; designed for extension to acceleration structures like BVH in performance-critical evolutions.
     */
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& object : objects) {
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};

#endif
