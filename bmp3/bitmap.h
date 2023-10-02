#ifndef BMP3_BITMAP_H
#define BMP3_BITMAP_H

#pragma once  
/* Стоит включать только те заголовники, которые напрямую используются в этом файле.
 * При include в файл включается весь код из заголовка, что замедляет время компиляции.
 * Помещая включения в заголовок, при включении этого заголовка в другой файл (в большом проекте
 * один заголовок может быть включен во множество сурс-файлов) ты также вставляешь туда и лишний,
 * ненужный там код, который в итоге замедляет компиляцию еще сильнее. Так что некоторые из этих 
 * включений стоит перенести в сурс-файл.
 */
#include <fstream>
#include <vector>
#include "algorithm"   //А почему не <algorithm>?
#include <stdexcept>
#include <string>
#include <iostream>
#include "cmath"  //<cmath>?


#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{0x4D42};
    uint32_t file_size{0};
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t offset_data{0};
};
/* nit: Возможно, для такой учебной задачи такое обилие переменных излишне. Пользуешься же ты только двумя.
 * Можно сказать, что это заготовка под расширение программы, но можно на это и ответить, что как будут расширять
 * так и добавят все эти переменные.
 */
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
/* У тебя тут структура, в которой все поля по умолчанию public, однако в ней же ты пишешь
 * для функций явно, что они публичны. Вероятно, ты тогда хотел написать class?
 */
struct BMP {  
    /* Не знаю, какой политикой именования ты пользуешься, но может быть удобно давать полям класса
     * (если с учетом прошлого замечания ты действительно имел в виду класс) в конце имени еще и
     * нижнее подчеркивание. Так их в сурс-файлах можно будет отличить от локальных переменных.
     */
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
            //Вообще приведение типов в стиле си -- очень опасная штука. Задумайся над плюсовым вариантом
            inp.read((char *) &file_header, sizeof(file_header)); 
            if (file_header.file_type != 0x4D42) {
                throw std::runtime_error("Error! Unrecognized file format.");
            }
            inp.read((char *) &bmp_info_header, sizeof(bmp_info_header));
            
            inp.seekg(file_header.offset_data);
            bmp_info_header.size = sizeof(BMPInfoHeader);  //А зачем?
            file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader); //?

            file_header.file_size = file_header.offset_data;  //??

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
