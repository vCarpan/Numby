#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

const int WIDTH = 800, HEIGHT = 600;
const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

void drawPixel(SDL_Renderer *renderer, int x, int y, int r, int g, int b, int opacity);

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("DOOM!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Event windowEvent;
    bool keepRunning = true;
    bool draw = false;

    while (keepRunning) {
        Uint32 frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&windowEvent)) {
            switch (windowEvent.type) {
                case SDL_QUIT:
                    keepRunning = false;
                    break;
                case SDL_KEYDOWN:
                    if (windowEvent.key.keysym.sym == SDLK_s) {
                        draw = true;
                    }
                    break;
                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
 
        if (draw) {
            drawPixel(renderer, 10, 10, 255, 0, 0, 255);
        }


        SDL_RenderPresent(renderer);


        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void drawPixel(SDL_Renderer *renderer, int x, int y, int r, int g, int b, int opacity) {
    SDL_SetRenderDrawColor(renderer, r, g, b, opacity);
    SDL_RenderDrawPoint(renderer, x, y);
}
