#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <fstream>
#include <SDL2/SDL.h>
#include <vector>

struct PPMImage {
    std::string format; // default is P6 so if its not P6 = throw an error
    int width = 0;
    int height = 0;
    int max_Color_Value = 255; // default is 255 so if its not = throw an error
    std::vector<unsigned char> data;
};

void skipCommentsAndWhitespace(std::ifstream& file);

PPMImage loadPPM(const std::string& path);

SDL_Surface* ppmToSurface(const PPMImage& image);

#endif
