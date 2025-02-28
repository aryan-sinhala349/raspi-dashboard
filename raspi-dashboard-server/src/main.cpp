#include <SDL2/SDL.h>

#include <stdio.h>
#include <cassert>
#include <string.h>

//Note: resolution of the Raspberry Pi Touch Display 2, you can change this as necessary
#define FRAMEBUFFER_WIDTH 1280
#define FRAMEBUFFER_HEIGHT 720

//Note: You shouldn't change these unless you want all the UI to break
#define RENDERER_LOGICAL_WIDTH 1280
#define RENDERER_LOGICAL_HEIGHT 720

//TODO: Introduce conditional compilation for this based on if this is a debug build or not 
//i.e., automatically enable logging in debug builds
#define ENABLE_LOGGING 1
#if ENABLE_LOGGING
    #define LOG_MSG(...) printf(__VA_ARGS__)
    #define LOG_ERR(...) fprintf(stderr, __VA_ARGS__)
#else
    #define LOG_MSG(...)
    #define LOG_ERR(...)
#endif

//In logical units
#define QUAD_SIZE 16

int main(int argc, char** argv)
{
    assert(SDL_Init(SDL_INIT_VIDEO) >= 0);

    Uint32 windowFlags = SDL_WINDOW_FULLSCREEN;
    SDL_Window* window = SDL_CreateWindow("Raspi-Dashboard Server", 0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, windowFlags);
    assert(window != nullptr);
    
    //Try creating a hardware accelerated context, then if that fails, try creating a software renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        LOG_MSG("Could not find a hardware accelerated rendering context, falling back to a software renderer!");
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
    }

    assert(renderer != nullptr);
    
    SDL_RenderSetLogicalSize(renderer, RENDERER_LOGICAL_WIDTH, RENDERER_LOGICAL_HEIGHT);

    //The position of the center of the quad (start in the center of the screen)
    int quadPosX = RENDERER_LOGICAL_WIDTH / 2;
    int quadPosY = RENDERER_LOGICAL_HEIGHT / 2;

    //Main loop
    bool running = true;
    while (running)
    {
        //Iterate through all events in the event queue
        SDL_Event event = { };
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                //If the user presses the escape key, exit (for debugging purposes only)
                case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        running = false;
    
                    break;
                }
                //If the user closes the window, exit
                case SDL_QUIT:
                {
                    running = false;
                    break;
                }
                //If the user clicks anywhere on the screen, update the quad's position
                case SDL_MOUSEBUTTONDOWN:
                {
                    quadPosX = ((float)event.button.x / FRAMEBUFFER_WIDTH) * RENDERER_LOGICAL_WIDTH;
                    quadPosY = ((float)event.button.y / FRAMEBUFFER_HEIGHT) * RENDERER_LOGICAL_HEIGHT;
                    break;
                }
                //If the user touches the screen, update the quad's position
                case SDL_FINGERDOWN:
                {
                    quadPosX = event.tfinger.x * RENDERER_LOGICAL_WIDTH;
                    quadPosY = event.tfinger.y * RENDERER_LOGICAL_HEIGHT;
                    break;
                }
            }
        }

        //Clear the backbuffer
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        //Draw a quad to the backbuffer
        SDL_Rect fillRect = { };
        fillRect.x = quadPosX - (QUAD_SIZE / 2);
        fillRect.y = quadPosY - (QUAD_SIZE / 2);
        fillRect.w = fillRect.h = QUAD_SIZE; 
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &fillRect);

        SDL_RenderPresent(renderer);
    }

    //Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
