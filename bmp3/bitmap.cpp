#include "bitmap.h"
#include <algorithm>
#include <cmath>

void RaiseError(const char* message) {
    std::cerr << message;
    exit(EXIT_FAILURE);
}

void BMP::read(const char *fname) {
    std::ifstream inp{fname, std::ios_base::binary};
    if (inp) {
        inp.read(reinterpret_cast<char*>(&file_header_), sizeof(file_header_));
        if (file_header_.file_type != 0x4D42) {
            std::cerr << "Error! Unrecognized file format.";
            return;
        }
        inp.read(reinterpret_cast<char*>(&bmp_info_header_), sizeof(bmp_info_header_));

        if (bmp_info_header_.height < 0) {
            RaiseError("The program can treat only BMP images with the origin in the bottom left corner!");
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
        RaiseError("Unable to open the input image file.");
        return;
    }
}

void BMP::write(const char *fname) {
    std::ofstream of(fname, std::ios::out | std::ios::binary);
    if (of) {
        if (bmp_info_header_.bit_count == 24) {
            of.write(reinterpret_cast<const char*>(&file_header_), sizeof(file_header_));
            of.write(reinterpret_cast<const char*>(&bmp_info_header_), sizeof(bmp_info_header_));

            for (int i = 0; i < bmp_info_header_.height; ++i) {
                of.write(dat_[i], bmp_info_header_.width * 3);
                for (int j = 0; j < (4 - (bmp_info_header_.width * 3 % 4)) % 4; j++) {
                    char paddingByte = 0;
                    of.write(&paddingByte, 1);
                }
            }
            of.close();
        } else {
            RaiseError("The program can treat only 24 bits per pixel BMP files");
            return;
        }
    } else {
        RaiseError("Unable to open the output image file.");
        return;
    }
}

void BMP::rotate_right() {
    int new_w = bmp_info_header_.height;
    int new_h = bmp_info_header_.width;

    char** new_data;
    new_data = new char* [new_h];
    for (int i = 0; i < new_h; ++i) {
        new_data[i] = new char[new_w * 3];
    }
    for (int i = 0; i < new_h; ++i) {
        for (int j = 0; j < new_w; ++j) {
            char* pixel = new_data[i] + j * 3;
            char* originalPixel = dat_[j] + (bmp_info_header_.width - 1 - i) * 3;
            pixel[0] = originalPixel[0];
            pixel[1] = originalPixel[1];
            pixel[2] = originalPixel[2];
        }
    }

    for (int i = 0; i < bmp_info_header_.width; ++i) {
        delete[] dat_[i];
    }
    delete[] dat_;

    dat_ = new_data;

    std::swap(bmp_info_header_.width, bmp_info_header_.height);
}

void BMP::rotate_left() {
    int new_w = bmp_info_header_.height;
    int new_h = bmp_info_header_.width;

    char** new_data;
    new_data = new char *[new_h];
    for (int i = 0; i < new_h; ++i) {
        new_data[i] = new char[new_w * 3];
    }
    for (int i = 0; i < new_h; ++i) {
        for (int j = 0; j < new_w; ++j) {
            char* pixel = new_data[i] + j * 3;
            char* originalPixel = dat_[bmp_info_header_.height - 1 - j] + i * 3;
            pixel[0] = originalPixel[0];
            pixel[1] = originalPixel[1];
            pixel[2] = originalPixel[2];
        }
    }
    for (int i = 0; i < bmp_info_header_.width; ++i) {
        delete[] dat_[i];
    }
    delete[] dat_;

    dat_ = new_data;

    std::swap(bmp_info_header_.width, bmp_info_header_.height);
}

void BMP::apply_gaussian(int radius, float sigma) {
    int width = bmp_info_header_.width;
    int height = bmp_info_header_.height;

    // Вычисляем ядро
    int kernelSize = 2 * radius + 1;
    float* kernel = new float[kernelSize];
    float kernelSum = 0.0;

    for (int i = 0; i < kernelSize; ++i) {
        int x = i - radius;
        kernel[i] = std::exp(-x * x / (2 * sigma * sigma));
        kernelSum += kernel[i];
    }

    // Нормируем ядро
    for (int i = 0; i < kernelSize; ++i) {
        kernel[i] /= kernelSum;
    }

    // Применяем горизонтальный блюр
    char** tempImage = new char*[height];
    for (int i = 0; i < height; ++i) {
        tempImage[i] = new char[width * 3];
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float r = 0.0, g = 0.0, b = 0.0;

            for (int i = -radius; i <= radius; ++i) {
                int xOffset = std::min(std::max(x + i, 0), width - 1);
                r += static_cast<uint8_t>(dat_[y][xOffset * 3 + 0]) * kernel[i + radius];
                g += static_cast<uint8_t>(dat_[y][xOffset * 3 + 1]) * kernel[i + radius];
                b += static_cast<uint8_t>(dat_[y][xOffset * 3 + 2]) * kernel[i + radius];
            }

            tempImage[y][x * 3 + 0] = static_cast<uint8_t>(r);
            tempImage[y][x * 3 + 1] = static_cast<uint8_t>(g);
            tempImage[y][x * 3 + 2] = static_cast<uint8_t>(b);
        }
    }

    // Применяем вертикальный блюр и сохраняем все обратно в dat
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            float r = 0.0, g = 0.0, b = 0.0;

            for (int i = -radius; i <= radius; ++i) {
                int yOffset = std::min(std::max(y + i, 0), height - 1);
                r += static_cast<uint8_t>(tempImage[yOffset][x * 3 + 0]) * kernel[i + radius];
                g += static_cast<uint8_t>(tempImage[yOffset][x * 3 + 1]) * kernel[i + radius];
                b += static_cast<uint8_t>(tempImage[yOffset][x * 3 + 2]) * kernel[i + radius];
            }

            dat_[y][x * 3 + 0] = static_cast<uint8_t>(r);
            dat_[y][x * 3 + 1] = static_cast<uint8_t>(g);
            dat_[y][x * 3 + 2] = static_cast<uint8_t>(b);
        }
    }

    // Очищаем память
    for (int i = 0; i < height; ++i) {
        delete[] tempImage[i];
    }
    delete[] tempImage;

    delete[] kernel;
}





