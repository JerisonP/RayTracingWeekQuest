#ifndef COLOR_H
#define COLOR_H

#include "rtweekend.h"
#include "vec3.h"
#include "interval.h"

using color = vec3;

inline double linear_to_gamma(double linear_component) {
	if (linear_component > 0) {
		return std::sqrt(linear_component);
	}

	return 0;
}

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity(0.000, 0.999);
    int rbyte =	static_cast<int>(255 * intensity.clamp(r)); 
    int gbyte = static_cast<int>(255 * intensity.clamp(g));
    int bbyte = static_cast<int>(255 * intensity.clamp(b)); 

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
