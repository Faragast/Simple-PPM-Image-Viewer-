#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <exception>
#include <iostream>
#include <SDL2/SDL.h>
#include "parser.h"

int main(int argc, char *argv[]) {

    if(argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image_file>" << std::endl;
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    try {
        PPMImage image = loadPPM(argv[1]);

        SDL_Window *pwindow = SDL_CreateWindow(
            "Image Viewer",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            image.width,
            image.height,
            SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_SHOWN
        );

        SDL_Surface* surface = ppmToSurface(image);

        if (!surface){
            throw std::runtime_error("Cannot create surface");
        }
        SDL_Surface* winSurface = SDL_GetWindowSurface(pwindow);
        SDL_BlitSurface(surface, NULL, winSurface, NULL);
        SDL_UpdateWindowSurface(pwindow);

        bool quit = false;
        SDL_Event event;

        while (!quit) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT){
                    quit = true;
                }
            }
            SDL_Delay(100);
        }

        SDL_FreeSurface(surface);
        SDL_DestroyWindow(pwindow);

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        SDL_Quit();
    }

    SDL_Quit();
    return 0;
}
