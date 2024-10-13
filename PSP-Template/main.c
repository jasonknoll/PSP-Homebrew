// PSP
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>

// If using PSP Graphics Utility
#include <pspgu.h>
#include <pspgum.h> // GU-math

// If using PSPGL (may need to install from psp-pacman)
#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library

// C libraries
#include <stdlib.h>
#include <math.h>

// Module info
#define MODULE_NAME "PSP Template Project"
#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PATCH 0

#define BUFFER_WIDTH 512
#define BUFFER_HEIGHT 272
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT BUFFER_HEIGHT

#ifdef __PSP__
    // PSP specific code if multi-platform
#endif

// Setup module
PSP_MODULE_INFO(MODULE_NAME, 0, VERSION_MAJOR, VERSION_MINOR);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);

// Setup kernel callbacks
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

// Put some graphics logic here yo
// Usually more boilerplate 

int main() {
    setup_callbacks();

    // Controller
    SceCtrlData pad;

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    // Main loop
    for (;;) {
        sceCtrlReadBufferPositive(&pad, 1);

        // Game logic
    }

    return 0;
}