#ifndef BMP3_BITMAP_H
#define BMP3_BITMAP_H

#pragma once

#include <fstream>
#include <stdexcept>
#include <iostream>

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{0x4D42};
    uint16_t unused[6]{0};
};

struct BMPInfoHeader {
    uint32_t size{0};
    int32_t width{0};
    int32_t height{0};
    uint16_t planes{1};
    uint16_t bit_count{0};
    uint32_t unused[6]{0};
};

#pragma pack(pop)

class BMP {
private:
    BMPFileHeader file_header_;
    BMPInfoHeader bmp_info_header_;
    char **dat_;

public:
    void read(const char *fname);

    void write(const char *fname);

    void rotate_right();

    void rotate_left();

    void apply_gaussian(int radius, float sigma);
};


#endif //BMP3_BITMAP_H
