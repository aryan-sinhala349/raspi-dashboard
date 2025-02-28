#define NO_SDL_GLEXT
#include <SDL2/SDL.h>

#include <stdio.h>
#include <cassert>
#include <string.h>

#define FRAMEBUFFER_WIDTH 1280
#define FRAMEBUFFER_HEIGHT 720

#define ENABLE_LOGGING 1
#if ENABLE_LOGGING
    #define LOG_MSG(...) printf(__VA_ARGS__)
    #define LOG_ERR(...) fprintf(stderr, __VA_ARGS__)
#else
    #define LOG_MSG(...)
    #define LOG_ERR(...)
#endif

#define QUAD_SIZE 16

int main(int argc, char** argv)
{
    assert(SDL_Init(SDL_INIT_VIDEO) >= 0);

    Uint32 windowFlags = SDL_WINDOW_FULLSCREEN;
    SDL_Window* window = SDL_CreateWindow("Raspi-Dashboard Server", 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, windowFlags);
    assert(window != nullptr);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        LOG_MSG("Could not find a hardware accelerated rendering context, falling back to a software renderer!");
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
    }

    assert(renderer != nullptr);
    
    SDL_RenderSetLogicalSize(renderer, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

    int quadPosX = FRAMEBUFFER_WIDTH / 2;
    int quadPosY = FRAMEBUFFER_HEIGHT / 2;

    bool running = true;
    while (running)
    {
        SDL_Event event = { };
        while (SDL_PollEvent(&event))
        {
            LOG_MSG("0x%X\n", event.type);

            switch (event.type)
            {
                case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        running = false;
    
                    break;
                }
                case SDL_QUIT:
                {
                    running = false;
                    break;
                }
                case SDL_MOUSEBUTTONDOWN:
                {
                    quadPosX = event.button.x;
                    quadPosY = event.button.y;
                    break;
                }
                case SDL_FINGERDOWN:
                {
                    quadPosX = event.tfinger.x * FRAMEBUFFER_WIDTH;
                    quadPosY = event.tfinger.y * FRAMEBUFFER_HEIGHT;
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_Rect fillRect = { };
        fillRect.x = quadPosX - (QUAD_SIZE / 2);
        fillRect.y = quadPosY - (QUAD_SIZE / 2);
        fillRect.w = fillRect.h = QUAD_SIZE; 
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &fillRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
