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

#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

auto blue  = color(0, 0, 1);
auto green = color(0, 1, 0);
auto red   = color(1, 0, 0);

auto yello = color(1, 1, 0);
auto magenta = color(1, 0, 1);
auto cyan = color(0, 1, 1);

auto white = color(1, 1, 1);
auto black = color(0, 0, 0);

// Placeholder for scene-dependent coloring; designed as a pure function to facilitate future extensions like shading models without side effects.

/*color ray_color(const ray& r) {
    const vec3& dir = r.direction();
    double fake_length = std::sqrt(dir.y() * dir.y() + dir.z() * dir.z());
    double unit_y = (fake_length > 0.0) ? dir.y() / fake_length : 0.0;  // Avoid div-by-zero, though unlikely here.
    auto a = 0.5 * (unit_y + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}
*/

bool hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = center - r.origin();
	auto a = dot(r.direction(), r.direction());
	auto b = -2.0 * dot(r.direction(), oc);
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = b*b - 4*a*c;
	return (discriminant >= 0);
}

color ray_color(const ray& r) {
	vec3 unit_direction = unit_vector(r.direction());
	if (hit_sphere(point3(0, 0, -1), 0.5, r))
	{
		auto a = 0.5 * (unit_direction.y() - unit_direction.x() + 1.0);
		return (1.0 - a)*green+ a*blue;
	}

	auto a = 0.5 * (unit_direction.y() + 2.0);
	return (1.0-a)*white + a*(color(0.5, 0.7, 1.0));
}


// Main function orchestrates setup and rendering; uses automatic variables for stack allocation to minimize dynamic memory overhead.
int main() {
    double channel_range = 255.0; // Fixed at 255 for standard 8-bit color depth compatibility.

    /* Image Setup */
    // Aspect ratio and width chosen for development; height derived to preserve proportions and avoid distortion.
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 800;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    /* Camera Setup */
    // Simple pinhole camera model with fixed parameters for baseline implementation; viewport sized for 90-degree FOV.
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
    auto camera_center = point3(0, 0, 0);
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);
    // Deltas precomputed to optimize loop performance by reducing per-pixel calculations.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;
    // Half-pixel offset applied for center-sampled rays, reducing aliasing in this orthographic-like setup.
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    /* Render Header */
    // Header uses text format for simplicity and portability across systems.
    std::cout << "P3\n"
              << image_width << ' '
              << image_height << '\n'
              << channel_range
              << std::endl;

    /* Render Loop */
    // Row-major order matches PPM specification; nested loops for straightforward parallelization potential.
    for (int row = 0; row < image_height; row++) {
        for (int col = 0; col < image_width; col++) {
            auto pixel_center = pixel00_loc + (col * pixel_delta_u) + (row * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    return 0;
}
