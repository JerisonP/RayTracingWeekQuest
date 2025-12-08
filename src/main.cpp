/*
 * PPM (Portable Pixmap) is a simple image format. We use it because it only requires writing text to a file.
 * This format dictates how we represent most of our image data, as everything needs to be converted to PPM output eventually.
 *
 * PPM Example:
 * P3 // Header: P3 means colors are in ASCII (text)
 * 3 2 // Width (3 columns) and height (2 rows)
 * 255 // Maximum color value per channel (0-255 for RGB)
 * 255 0 0 // Red pixel
 * 0 255 0 // Green pixel
 * 0 0 255 // Blue pixel
 * 255 255 255 // White pixel
 * 255 255 255 // White pixel
 * 0 0 0 // Black pixel
 *
 * The image is rendered row by row, from top to bottom, and within each row, from left to right.
 * Each pixel is an RGB triplet (red, green, blue values).
 */

#include "rtweekend.h"  // For shared utilities and constants; centralizes common definitions to avoid redundancy across files.
#include "hittable.h"   // For hit interface; supports polymorphic scene composition.
#include "hittable_list.h"  // For aggregate objects; enables building complex scenes from primitives.
#include "sphere.h"     // For sphere geometry; chosen as a simple primitive for initial testing and extension.

/* Ray Coloring */
// Computes color via intersection or fallback gradient; designed as a hook for future material and lighting models, using hittable reference for scene modularity.
color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, interval(0, infinity), rec)) {
        // Simple normal-based shading for visualization; remaps to [0,1] range for positive colors, aiding debug of surface orientations.
        return 0.5 * (rec.normal + color(1,1,1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

/* Main Entry Point */
// Orchestrates scene setup, camera, and rendering; uses local scopes for clarity and to minimize global state.
int main() {
    /* Image Setup */
    // Aspect ratio and width fixed for development; height derived to maintain proportions without distortion.
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    /* Scene Setup */
    // List-based world for easy addition of objects; starts with basic spheres for ground and foreground to test intersections.
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    /* Camera Setup */
    // Pinhole model with fixed params for simplicity; viewport scaled for 90-degree FOV, centered at origin for symmetric views.
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    auto camera_center = point3(0, 0, 0);
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);
    // Deltas precomputed to reduce per-pixel math, enhancing loop efficiency.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;
    // Half-pixel offset for center sampling; minimizes aliasing in basic setup.
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    /* Render Header */
    // Text-based PPM for portability; outputs to stdout for easy redirection.
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    /* Render Loop */
    // Row-major iteration matches PPM order; supports future parallelization via threading.
    for (int j = 0; j < image_height; j++) {
        // Progress logging to stderr; decouples from image output for clean PPM files.
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);
            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}
