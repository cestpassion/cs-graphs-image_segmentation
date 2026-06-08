#include "Image.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image()
    : width(0), height(0), channels(0) {}

bool Image::empty() const {
    return width <= 0 || height <= 0 || channels <= 0 || pixels.empty();
}

static std::string getFileExtension(const std::string& path) {
    std::size_t dotPosition = path.find_last_of('.');

    if (dotPosition == std::string::npos) {
        return "";
    }

    std::string extension = path.substr(dotPosition + 1);

    std::transform(
        extension.begin(),
        extension.end(),
        extension.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        }
    );

    return extension;
}

bool Image::load(const std::string& path) {
    int loadedWidth = 0;
    int loadedHeight = 0;
    int loadedChannels = 0;

    /*
        O último parâmetro igual a 3 força a imagem a ser carregada como RGB.

        Isso simplifica o projeto porque toda imagem carregada começa com:
        channels = 3

        Depois, se o usuário passar --gray, convertemos para channels = 1.
    */
    unsigned char* data = stbi_load(
        path.c_str(),
        &loadedWidth,
        &loadedHeight,
        &loadedChannels,
        3
    );

    if (data == nullptr) {
        std::cerr << "Erro ao carregar imagem: " << path << std::endl;
        return false;
    }

    width = loadedWidth;
    height = loadedHeight;
    channels = 3;

    std::size_t totalSize = static_cast<std::size_t>(width) *
                            static_cast<std::size_t>(height) *
                            static_cast<std::size_t>(channels);

    pixels.assign(data, data + totalSize);

    stbi_image_free(data);

    return true;
}

bool Image::save(const std::string& path) const {
    if (empty()) {
        std::cerr << "Erro: tentativa de salvar imagem vazia." << std::endl;
        return false;
    }

    if (channels != 1 && channels != 3) {
        std::cerr << "Erro: apenas imagens com 1 ou 3 canais podem ser salvas." << std::endl;
        return false;
    }

    std::size_t expectedSize = static_cast<std::size_t>(width) *
                               static_cast<std::size_t>(height) *
                               static_cast<std::size_t>(channels);

    if (pixels.size() != expectedSize) {
        std::cerr << "Erro: tamanho do vetor de pixels inconsistente." << std::endl;
        return false;
    }

    std::string extension = getFileExtension(path);

    int success = 0;

    if (extension == "png") {
        int stride = width * channels;
        success = stbi_write_png(
            path.c_str(),
            width,
            height,
            channels,
            pixels.data(),
            stride
        );
    } else if (extension == "jpg" || extension == "jpeg") {
        int quality = 95;
        success = stbi_write_jpg(
            path.c_str(),
            width,
            height,
            channels,
            pixels.data(),
            quality
        );
    } else if (extension == "bmp") {
        success = stbi_write_bmp(
            path.c_str(),
            width,
            height,
            channels,
            pixels.data()
        );
    } else if (extension == "tga") {
        success = stbi_write_tga(
            path.c_str(),
            width,
            height,
            channels,
            pixels.data()
        );
    } else {
        std::cerr << "Erro: formato de saída não suportado: ." << extension << std::endl;
        std::cerr << "Use .png, .jpg, .jpeg, .bmp ou .tga." << std::endl;
        return false;
    }

    if (success == 0) {
        std::cerr << "Erro ao salvar imagem: " << path << std::endl;
        return false;
    }

    return true;
}

Image Image::toGrayscale() const {
    if (empty()) {
        return Image();
    }

    if (channels == 1) {
        return *this;
    }

    if (channels != 3) {
        throw std::runtime_error("Conversao para cinza exige imagem RGB com 3 canais.");
    }

    Image grayImage;
    grayImage.width = width;
    grayImage.height = height;
    grayImage.channels = 1;
    grayImage.pixels.resize(static_cast<std::size_t>(width) * height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char r = getPixel(x, y, 0);
            unsigned char g = getPixel(x, y, 1);
            unsigned char b = getPixel(x, y, 2);

            unsigned char grayValue = static_cast<unsigned char>(
                0.299 * r + 0.587 * g + 0.114 * b
            );

            grayImage.setPixel(x, y, 0, grayValue);
        }
    }

    return grayImage;
}

unsigned char Image::getPixel(int x, int y, int c) const {
    int index = getIndex(x, y, c);
    return pixels[index];
}

void Image::setPixel(int x, int y, int c, unsigned char value) {
    int index = getIndex(x, y, c);
    pixels[index] = value;
}

int Image::getIndex(int x, int y, int c) const {
    if (x < 0 || x >= width || y < 0 || y >= height || c < 0 || c >= channels) {
        throw std::out_of_range("Coordenada de pixel fora dos limites da imagem.");
    }

    return (y * width + x) * channels + c;
}

Image Image::createColoredLabelImage(
    const std::vector<int>& labels,
    int imageWidth,
    int imageHeight
) {
    Image output;
    output.width = imageWidth;
    output.height = imageHeight;
    output.channels = 3;

    std::size_t expectedSize = static_cast<std::size_t>(imageWidth) *
                               static_cast<std::size_t>(imageHeight);

    if (labels.size() != expectedSize) {
        std::cerr << "Erro: quantidade de labels diferente da quantidade de pixels." << std::endl;
        return Image();
    }

    output.pixels.resize(expectedSize * 3);

    std::unordered_map<int, std::array<unsigned char, 3>> labelColors;

    for (std::size_t i = 0; i < labels.size(); i++) {
        int label = labels[i];

        if (labelColors.find(label) == labelColors.end()) {
            unsigned int hash = static_cast<unsigned int>(label * 2654435761u);

            unsigned char r = static_cast<unsigned char>((hash >> 16) & 255);
            unsigned char g = static_cast<unsigned char>((hash >> 8) & 255);
            unsigned char b = static_cast<unsigned char>(hash & 255);

            if (r < 40 && g < 40 && b < 40) {
                r = 180;
                g = 180;
                b = 180;
            }

            labelColors[label] = {r, g, b};
        }

        int pixelIndex = static_cast<int>(i) * 3;

        output.pixels[pixelIndex] = labelColors[label][0];
        output.pixels[pixelIndex + 1] = labelColors[label][1];
        output.pixels[pixelIndex + 2] = labelColors[label][2];
    }

    return output;
}