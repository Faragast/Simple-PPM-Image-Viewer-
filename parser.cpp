#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "parser.h"

void skipCommentsAndWhitespace(std::ifstream& file) {
    while (file.good()) {
        int c = file.peek();
        if (c == '#') {
            file.ignore(1024, '\n');
        } else if (isspace(c)) {
            file.ignore();
        } else {
            break;
        }
    }
}

PPMImage loadPPM(const std::string& path) {
    PPMImage image;
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    skipCommentsAndWhitespace(file);
    file >> image.format;
    if (image.format != "P6") {
        throw std::runtime_error("Unsupported format");
    }

    skipCommentsAndWhitespace(file);
    file >> image.width >> image.height >> image.max_Color_Value;

    if (image.max_Color_Value != 255) {
        throw std::runtime_error("Unsupported color depth");
    }

    file.ignore(1, '\n');

    image.data.resize(image.width * image.height * 3);
    file.read(reinterpret_cast<char*>(image.data.data()), image.data.size());

    if (!file.good()) {
        throw std::runtime_error("Failed to read image data");
    }

    return image;
}
SDL_Surface* ppmToSurface(const PPMImage& image) {
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
        0,
        image.width,
        image.height,
        24,
        SDL_PIXELFORMAT_RGB24
    );

    if (!surface) {
        throw std::runtime_error("Failed to create surface");
    }

    uint8_t* dst = static_cast<uint8_t*>(surface->pixels);
    size_t srcRowBytes = image.width * 3;

    for (int y = 0; y < image.height; y++) {
         memcpy(dst + y * surface->pitch, image.data.data() + y * srcRowBytes, srcRowBytes);
    }
    return surface;
}
