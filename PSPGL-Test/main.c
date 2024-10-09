#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <GLES/gl.h>
#include <GLES/egl.h>
// #include <pspgu.h>
// #include <pspgum.h>

// TODO compare PSPGL and PSPGU
// draw a cube in this and PSPGU
PSP_MODULE_INFO("pspGL Test", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);

int exit_callback() {}

int callback_thread() {}

int setup_callbacks(void) {}
