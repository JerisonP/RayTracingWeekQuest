/*
 * PPM (Portable Pixmap) is a simple image format. We use it because it only requires writing text to a file.
 * This format dictates how we represent most of our image data, as everything needs to be converted to PPM output eventually.
 *
 * PPM Example:
 * P3                  // Header: P3 means colors are in ASCII (text)
 * 3 2                 // Width (3 columns) and height (2 rows)
 * 255                 // Maximum color value per channel (0-255 for RGB)
 * 255 0 0             // Red pixel
 * 0 255 0             // Green pixel
 * 0 0 255             // Blue pixel
 * 255 255 255         // White pixel
 * 255 255 255         // White pixel
 * 0 0 0               // Black pixel
 *
 * The image is rendered row by row, from top to bottom, and within each row, from left to right.
 * Each pixel is an RGB triplet (red, green, blue values).
 */

#include <iostream>
using std::cout;
using std::endl;

int main() {
    // Image dimensions and color range
    int image_width = 256;
    int image_height = 256;
    double channel_range = 255.0;  // Max value for each RGB channel (typically 255)

    // Render the PPM header
    cout << "P3\n"             // Format identifier (ASCII colors)
         << image_width << ' '  // Width
         << image_height << '\n'// Height
         << channel_range       // Max channel value
         << endl;

    // Generate pixels row by row (top to bottom)
    for (int row = 0; row < image_height; row++) {
        // Generate pixels column by column (left to right)
        for (int col = 0; col < image_width; col++) {
            // Normalize coordinates to [0, 1] range
            // Red varies based on column: 0 (left, black) to 1 (right, full red)
            double r = static_cast<double>(col) / (image_width - 1);
            // Green varies based on row: 0 (top, black) to 1 (bottom, full green)
            double g = static_cast<double>(row) / (image_height - 1);
            // Blue is fixed at 0 (no blue component)
            double b = 0.0;

            // Scale normalized values to the channel range (0-255)
            int ir = static_cast<int>(r * channel_range);
            int ig = static_cast<int>(g * channel_range);
            int ib = static_cast<int>(b * channel_range);

            // Output the pixel in PPM format (space-separated RGB)
            cout << ir << ' ' << ig << ' ' << ib << endl;
        }
    }

    return 0;
}
