#ifndef BMP3_BITMAP_H
#define BMP3_BITMAP_H

#pragma once

#include <fstream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <iostream>
#include <cmath>


#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{0x4D42};
    uint32_t offset_data{0};
};

struct BMPInfoHeader {
    int32_t width{0};
    int32_t height{0};
    uint16_t bit_count{0};
};

#pragma pack(pop)

class BMP {
    BMPFileHeader file_header_;
    BMPInfoHeader bmp_info_header_;
    char** dat_;

    void write(const char *fname);
    void rotate_right();
    void rotate_left();
    void apply_gaussian(int radius, float sigma);

    BMP(const char *fname) {
        std::ifstream inp{fname, std::ios_base::binary};
        if (inp) {
            inp.read(reinterpret_cast<char*>(&file_header_), sizeof(file_header_));
            if (file_header_.file_type != 0x4D42) {
                throw std::runtime_error("Error! Unrecognized file format.");
            }
            inp.read(reinterpret_cast<char*>(&bmp_info_header_), sizeof(bmp_info_header_));

            inp.seekg(file_header_.offset_data);

            if (bmp_info_header_.height < 0) {
                throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");
            }

            dat_ = new char*[bmp_info_header_.height];
            for (int i = 0; i < bmp_info_header_.height; ++i) {
                dat_[i] = new char[bmp_info_header_.width * 3];
            }

            for (int row = 0; row < bmp_info_header_.height; ++row) {
                for (int col = 0; col < bmp_info_header_.width * 3; ++col) {
                    dat_[row][col] = inp.get();
                }
                inp.seekg((4 - (bmp_info_header_.width * 3 % 4)) % 4, std::ios::cur);
            }
            inp.close();

        } else {
            throw std::runtime_error("Unable to open the input image file.");
        }
    }
};


#endif //BMP3_BITMAP_H
