#include <SDL.h>

// Trying to combine the TestPSPProgram main.c with the SDL graphics
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>

// ERROR these conflict with SDL for some reason. 
//PSP_MODULE_INFO("SDL Test", 0, 1, 0);
//PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

int exit_callback(int arg1, int arg2, void *common) {
    sceKernelExitGame();
    return 0;
}

int callback_thread(SceSize args, void *argp) {
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}


int setup_callbacks(void) {
    int thid = sceKernelCreateThread("update_thread", callback_thread, 0x11, 0xFA0, 0, 0);
    if(thid >= 0)
        sceKernelStartThread(thid, 0, 0);
    return thid;
}


int main(int argc, char *argv[]) {
    setup_callbacks();

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

    SDL_Window * window = SDL_CreateWindow(
        "window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        480,
        272,
        0
    );

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect square = {216, 96, 34, 64}; 

    int running = 1;
    SDL_Event event;
    while (running) { 
        // Process input
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    // End the loop if the programs is being closed
                    running = 0;
                    break;
                case SDL_CONTROLLERDEVICEADDED:
                    // Connect a controller when it is connected
                    SDL_GameControllerOpen(event.cdevice.which);
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
                    if(event.cbutton.button == SDL_CONTROLLER_BUTTON_START) {
                        // Close the program if start is pressed
                        running = 0;
                    }
                    break;
            }
        }

        // Clear the screen
        SDL_RenderClear(renderer);

        // Draw a red square
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &square);

        // Draw everything on a white background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
