#include "bitmap.h"

int main() {
    BMP bmp;
    bmp.read("image.bmp");
    bmp.write("image_copy.bmp");
    bmp.rotate_left();
    bmp.write("image1_rotated_left.bmp");
    bmp.rotate_right();
    bmp.write("image1_rotated_right.bmp");
    bmp.apply_gaussian(7, 10.0);
    bmp.write("blurred_image.bmp");
}
