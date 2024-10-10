#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <GLES/gl.h>
#include <GLES/egl.h>
#include <GLES/glut.h>
// #include <pspgu.h>
// #include <pspgum.h>

#define GREEN 0xff00ff00 // ABGR format
#define BLUE 0xffff0000

#define BUFFER_WIDTH 512
#define BUFFER_HEIGHT 272
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT BUFFER_HEIGHT

// TODO compare PSPGL and PSPGU
// draw a cube in this and PSPGU
PSP_MODULE_INFO("pspGL Test", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);

int window; // because they told me to

int exit_callback() {}

int callback_thread() {}

int setup_callbacks(void) {}

void initGL(int width, int height) {

}

int main(int argc, char **argv) {
    setup_callbacks(&argc, argv);

    glutInit();

    for (;;) {
        // main loop
    }
}