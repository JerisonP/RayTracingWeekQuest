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
#include "camera.h"

int main() {
	hittable_list world;

	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 800;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	
	cam.render(world);
}
