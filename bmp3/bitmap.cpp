#include "bitmap.h"

void BMP::write(const char *fname) {
    std::ofstream of(fname, std::ios::out | std::ios::binary);
    if (of) {
        if (bmp_info_header.bit_count == 24) {
            of.write((const char *) &file_header, sizeof(file_header));
            of.write((const char *) &bmp_info_header, sizeof(bmp_info_header));

            for (int i = 0; i < bmp_info_header.height; ++i) {
                of.write(dat[i], bmp_info_header.width * 3);
                for (int j = 0; j < (4 - (bmp_info_header.width * 3 % 4)) % 4; j++) {
                    char paddingByte = 0;
                    of.write(&paddingByte, 1);
                }
            }
            of.close();
        } else {
            // Представь себе, в код-стайле гугла запрещены исключения. Я тоже считаю, что в этой программе они
            // смотрятся хорошо, но все равно советую почитать все плюсы и минусы их использования в интернете
            throw std::runtime_error("The program can treat only 24 bits per pixel BMP files");
        }
    } else {
        throw std::runtime_error("Unable to open the output image file.");
    }
}

void BMP::rotate_right() {
    int new_w = bmp_info_header.height;
    int new_h = bmp_info_header.width;

    char** new_data;
    new_data = new char *[new_h];
    for (int i = 0; i < new_h; ++i) {
        new_data[i] = new char[new_w * 3];
    }
    for (int i = 0; i < new_h; ++i) {
        for (int j = 0; j < new_w; ++j) {
            char* pixel = new_data[i] + j * 3;
            char* originalPixel = dat[j] + (bmp_info_header.width - 1 - i) * 3;
            // Чтоб так не делать, я бы на твоем месте завел структуру для пикселя
            pixel[0] = originalPixel[0];
            pixel[1] = originalPixel[1];
            pixel[2] = originalPixel[2];
        }
    }

    dat = new_data;
    /* Утечка идет от того, что ты теряешь указатель на данные, которые считал в конструкторе, указателем на которые 
     * и является dat
     */
    // Не удаляю new_data => утечка памяти. Если пытаюсь удалить, то изображение, которое получается на выходе,
    // искажено. Скорее всего эта проблема возникает из-за того, что когда я пишу dat = new_data, dat обращается по
    // адресу new_data, которая была удалена. Очевидно, та же проблема при повороте в другую сторону.

//    for (int i = 0; i < bmp_info_header.height; ++i) {
//        delete[] new_data[i];
//    }
//    delete[] new_data;

    std::swap(bmp_info_header.width, bmp_info_header.height); // Респект за говорящий код!
}

void BMP::rotate_left() {
    int new_w = bmp_info_header.height;
    int new_h = bmp_info_header.width;

    char** new_data;
    new_data = new char *[new_h];
    for (int i = 0; i < new_h; ++i) {
        new_data[i] = new char[new_w * 3];
    }
    for (int i = 0; i < new_h; ++i) {
        for (int j = 0; j < new_w; ++j) {
            char* pixel = new_data[i] + j * 3;
            char* originalPixel = dat[bmp_info_header.height - 1 - j] + i * 3;
            pixel[0] = originalPixel[0];
            pixel[1] = originalPixel[1];
            pixel[2] = originalPixel[2];
        }
    }
    dat = new_data;

    std::swap(bmp_info_header.width, bmp_info_header.height);
}

void BMP::apply_gaussian(int radius, float sigma) {
    int width = bmp_info_header.width;
    int height = bmp_info_header.height;

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
                r += static_cast<uint8_t>(dat[y][xOffset * 3 + 0]) * kernel[i + radius];
                g += static_cast<uint8_t>(dat[y][xOffset * 3 + 1]) * kernel[i + radius];
                b += static_cast<uint8_t>(dat[y][xOffset * 3 + 2]) * kernel[i + radius];
            }

            tempImage[y][x * 3 + 0] = (char)(r); //Вперемешку и сишные приведения и плюсовые. Непорядок
            tempImage[y][x * 3 + 1] = (char)(g);
            tempImage[y][x * 3 + 2] = (char)(b);
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

            dat[y][x * 3 + 0] = static_cast<uint8_t>(r);
            dat[y][x * 3 + 1] = static_cast<uint8_t>(g);
            dat[y][x * 3 + 2] = static_cast<uint8_t>(b);
        }
    }

    // Очищаем память
    for (int i = 0; i < height; ++i) {
        delete[] tempImage[i];
    }
    delete[] tempImage;

    delete[] kernel;
}





