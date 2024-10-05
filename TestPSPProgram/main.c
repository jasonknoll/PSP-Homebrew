#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <stdbool.h>

PSP_MODULE_INFO("Hello World", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

int exit_callback(int arg1, int arg2, void *common)
{
    sceKernelExitGame();
    return 0;
}

// Allows the program to exit I think
int callback_thread(SceSize args, void *argp)
{
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

int main(void)  {
    setup_callbacks();
    
    // This is how we're doing the debug printf
    // as graphics would be used instead usually
    pspDebugScreenInit();
    while(true) {
        pspDebugScreenSetXY(0, 0);
        pspDebugScreenPrintf("Hello World!");
        sceDisplayWaitVblankStart();
    }

    return 0;
}