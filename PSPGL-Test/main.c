#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <GLES/gl.h>
#include <GLES/egl.h>
// #include <pspgu.h>
// #include <pspgum.h>

#define GREEN 0xff00ff00
#define BLUE 0xffff0000


// TODO compare PSPGL and PSPGU
// draw a cube in this and PSPGU
PSP_MODULE_INFO("pspGL Test", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);

int exit_callback() {}

int callback_thread() {}

int setup_callbacks(void) {}

int main() {
    setup_callbacks();

    for (;;) {
        // main loop
    }
}