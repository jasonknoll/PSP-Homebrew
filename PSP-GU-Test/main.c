#include <pspkernel.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspdisplay.h>
#include <pspctrl.h>

#include <math.h>

// TODO add pspGL 

// From the examples
PSP_MODULE_INFO("PSP Graphics-Utility Test", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);


#define BUFFER_WIDTH 512
#define BUFFER_HEIGHT 272
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT BUFFER_HEIGHT

char list[0x20000] __attribute__((aligned(64)));
// static unsigned int __attribute__((aligned(16))) list[26144]; // I have no clue what this is from the example
//extern unsigned char logo_start[]; // idk something for the example logo
int isRunning;


// I'm 99% sure these functions just allow the game to close correctly
int exit_callback(int arg1, int arg2, void *common) {
    isRunning = 0;
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


void initGu() {
    sceGuInit();

    // I gotta read the documentation man
    // These setup the buffers
    sceGuStart(GU_DIRECT, list);
    sceGuDrawBuffer(GU_PSM_8888, (void*)0, BUFFER_WIDTH);
    sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, (void*)0x88000, BUFFER_WIDTH);
    sceGuDepthBuffer((void*)0x110000, BUFFER_WIDTH);

    // Set up "viewport"
    sceGuOffset(2048 - (SCREEN_WIDTH / 2), 2048 - (SCREEN_HEIGHT / 2));
    sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    sceGuDepthRange(65535, 0); // Use the full buffer for depth testing - buffer is reversed order

    sceGuDepthFunc(GU_GEQUAL);
    sceGuEnable(GU_DEPTH_TEST); // NOTE what is depth testing???

    sceGuFinish();
    sceGuDisplay(GU_TRUE);
}


void endGu() {
    sceGuDisplay(GU_FALSE);
    sceGuTerm(); // terminate
}


void startFrame() {
    sceGuStart(GU_DIRECT, list);
    sceGuClearColor(0xFFFFFFFF); // ABGR for white background 
    sceGuClear(GU_COLOR_BUFFER_BIT);
}


void endFrame() {
    sceGuFinish();
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
}


typedef struct {
    unsigned short u, v;
    short x, y, z;
} Vertex;

// TODO implement the cube example
typedef struct {
    float u, v;
    unsigned int color;
    float x, y, z;
} CubeVertex;

// TODO add vertex array
//struct CubeVertex __attribute__((aligned(16))) vertices[12*3];


void drawRect(float x, float y, float w, float h) {

    Vertex* vertices = (Vertex*)sceGuGetMemory(2 * sizeof(Vertex));

    vertices[0].x = x;
    vertices[0].y = y;

    vertices[1].x = x + w;
    vertices[1].y = y + h;

    sceGuColor(0xFF00FF00); // Green
    sceGuDrawArray(GU_SPRITES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 2, 0, vertices);
}

// TODO
void drawTriangle() {}

void drawCirlce() {}


int main() {
    // Make exiting with the home button possible
    setup_callbacks();

    // Setup the library used for rendering
    initGu();

    // TODO add controller inputs
    SceCtrlData pad;

    int x = 216;
    int y = 96;

    sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    // NOTE all the examples use a very primitive main loop
    isRunning = 1;
    while(isRunning) {
        sceCtrlReadBufferPositive(&pad, 1);
        
        if (pad.Buttons != 0) {
            if (pad.Buttons & PSP_CTRL_RIGHT) {
                x += 5;
            } else if (pad.Buttons & PSP_CTRL_LEFT) {
                x -= 5;
            }

            if (pad.Buttons & PSP_CTRL_UP) {
                y -= 5;
            } else if (pad.Buttons & PSP_CTRL_DOWN) {
                y += 5;
            }
        }

        startFrame();         

        drawRect(x, y, 34, 64);

        endFrame();
    }

    return 0;
}