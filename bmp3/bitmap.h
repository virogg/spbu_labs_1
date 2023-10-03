#ifndef BMP3_BITMAP_H
#define BMP3_BITMAP_H

#pragma once

#include <fstream>
#include <stdexcept>
#include <iostream>

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{0x4D42};
    uint32_t file_size{0};
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t offset_data{0};
//    uint16_t unused[6]{ 0 };
};

struct BMPInfoHeader {
    uint32_t size{0};
    int32_t width{0};
    int32_t height{0};
    uint16_t planes{1};
    uint16_t bit_count{0};
    uint32_t compression{0};
    uint32_t size_image{0};
    int32_t x_pixels_per_meter{0};
    int32_t y_pixels_per_meter{0};
    uint32_t colors_used{0};
    uint32_t colors_important{0};
//    uint32_t unused[16]{ 0 };
};

#pragma pack(pop)
class BMP {
private:
    BMPFileHeader file_header_;
    BMPInfoHeader bmp_info_header_;
    char** dat_;

public:
    void read(const char *fname);
    void write(const char *fname);
    void rotate_right();
    void rotate_left();
    void apply_gaussian(int radius, float sigma);
};


#endif //BMP3_BITMAP_H
