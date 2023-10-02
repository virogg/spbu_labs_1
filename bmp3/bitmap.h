#ifndef BMP3_BITMAP_H
#define BMP3_BITMAP_H

#pragma once

#include <fstream>
#include <vector>
#include "algorithm"
#include <stdexcept>
#include <string>
#include <iostream>
#include "cmath"


#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{0x4D42};
    uint32_t file_size{0};
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t offset_data{0};
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
};

#pragma pack(pop)

struct BMP {
    BMPFileHeader file_header;
    BMPInfoHeader bmp_info_header;
    char** dat;

public:
    void write(const char *fname);
    void rotate_right();
    void rotate_left();
    void apply_gaussian(int radius, float sigma);

    BMP(const char *fname) {
        std::ifstream inp{fname, std::ios_base::binary};
        if (inp) {
            inp.read((char *) &file_header, sizeof(file_header));
            if (file_header.file_type != 0x4D42) {
                throw std::runtime_error("Error! Unrecognized file format.");
            }
            inp.read((char *) &bmp_info_header, sizeof(bmp_info_header));

            inp.seekg(file_header.offset_data);
            bmp_info_header.size = sizeof(BMPInfoHeader);
            file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

            file_header.file_size = file_header.offset_data;

            if (bmp_info_header.height < 0) {
                throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");
            }

            dat = new char*[bmp_info_header.height];
            for (int i = 0; i < bmp_info_header.height; ++i) {
                dat[i] = new char[bmp_info_header.width * 3];
            }

            for (int row = 0; row < bmp_info_header.height; ++row) {
                for (int col = 0; col < bmp_info_header.width * 3; ++col) {
                    dat[row][col] = inp.get();
                }
                inp.seekg((4 - (bmp_info_header.width * 3 % 4)) % 4, std::ios::cur);
            }
            inp.close();

        } else {
            throw std::runtime_error("Unable to open the input image file.");
        }
    }
};


#endif //BMP3_BITMAP_H
