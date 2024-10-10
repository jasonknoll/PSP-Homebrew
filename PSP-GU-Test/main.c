#include <pspkernel.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspdisplay.h>
#include <pspctrl.h>

#include <math.h>

// TODO Draw shapes using both GL and GU and compare 
// Then render some 3D shapes >:)

// sce == "Sony Computer Entertainment" I think

// From the examples
PSP_MODULE_INFO("PSP Graphics-Utility Test", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);


#define BUFFER_WIDTH 512
#define BUFFER_HEIGHT 272
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT BUFFER_HEIGHT

// NOTE do I need to un-attribute this later?? Some C stuff I usually forget to worry about?
char __attribute__((aligned(64))) list[0x20000];
static unsigned int __attribute__((aligned(16))) cubeList[26144]; // cube example used this one
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
    void* fbp0 = guGetStaticVramBuffer(BUFFER_WIDTH,SCREEN_HEIGHT,GU_PSM_8888);
	void* fbp1 = guGetStaticVramBuffer(BUFFER_WIDTH,SCREEN_HEIGHT,GU_PSM_8888);
	void* zbp = guGetStaticVramBuffer(BUFFER_WIDTH,SCREEN_HEIGHT,GU_PSM_4444);

    sceGuInit();

    // I gotta read the documentation man
    // These setup the buffers
    //sceGuStart(GU_DIRECT, cubeList);
    sceGuStart(GU_DIRECT, list);
    sceGuDrawBuffer(GU_PSM_8888, fbp0, BUFFER_WIDTH);
    sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, fbp1, BUFFER_WIDTH);
    sceGuDepthBuffer(zbp, BUFFER_WIDTH);

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


CubeVertex __attribute__((aligned(16))) vertices[12*3] = 
{
    {0, 0, 0xff7f0000, -1, -1, 1}, // u, v, color, x, y, z
    {1, 0, 0xff7f0000,-1, 1, 1}, // 4
    {1, 1, 0xff7f0000, 1, 1, 1}, // 5

    {0, 0, 0xff7f0000,-1,-1, 1}, // 0
    {1, 1, 0xff7f0000, 1, 1, 1}, // 5
    {0, 1, 0xff7f0000, 1,-1, 1}, // 1

    {0, 0, 0xff7f0000,-1,-1,-1}, // 3
    {1, 0, 0xff7f0000, 1,-1,-1}, // 2
    {1, 1, 0xff7f0000, 1, 1,-1}, // 6

    {0, 0, 0xff7f0000,-1,-1,-1}, // 3
    {1, 1, 0xff7f0000, 1, 1,-1}, // 6
    {0, 1, 0xff7f0000,-1, 1,-1}, // 7

    {0, 0, 0xff007f00, 1,-1,-1}, // 0
    {1, 0, 0xff007f00, 1,-1, 1}, // 3
    {1, 1, 0xff007f00, 1, 1, 1}, // 7

    {0, 0, 0xff007f00, 1,-1,-1}, // 0
    {1, 1, 0xff007f00, 1, 1, 1}, // 7
    {0, 1, 0xff007f00, 1, 1,-1}, // 4

    {0, 0, 0xff007f00,-1,-1,-1}, // 0
    {1, 0, 0xff007f00,-1, 1,-1}, // 3
    {1, 1, 0xff007f00,-1, 1, 1}, // 7

    {0, 0, 0xff007f00,-1,-1,-1}, // 0
    {1, 1, 0xff007f00,-1, 1, 1}, // 7
    {0, 1, 0xff007f00,-1,-1, 1}, // 4

    {0, 0, 0xff00007f,-1, 1,-1}, // 0
    {1, 0, 0xff00007f, 1, 1,-1}, // 1
    {1, 1, 0xff00007f, 1, 1, 1}, // 2

    {0, 0, 0xff00007f,-1, 1,-1}, // 0
    {1, 1, 0xff00007f, 1, 1, 1}, // 2
    {0, 1, 0xff00007f,-1, 1, 1}, // 3

    {0, 0, 0xff00007f,-1,-1,-1}, // 4
    {1, 0, 0xff00007f,-1,-1, 1}, // 7
    {1, 1, 0xff00007f, 1,-1, 1}, // 6

    {0, 0, 0xff00007f,-1,-1,-1}, // 4
    {1, 1, 0xff00007f, 1,-1, 1}, // 6
    {0, 1, 0xff00007f, 1,-1,-1}, // 5
};


void drawRect(float x, float y, float w, float h) {

    Vertex* vertices = (Vertex*)sceGuGetMemory(2 * sizeof(Vertex));

    vertices[0].x = x;
    vertices[0].y = y;

    vertices[1].x = x + w;
    vertices[1].y = y + h;

    sceGuColor(0xFF00FF00); // Green
    sceGuDrawArray(GU_SPRITES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 2, 0, vertices);
}

// TODO actually get this working
void drawCube(CubeVertex* vertices) {
    sceGumDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,12*3,0,vertices);
}

// TODO
void drawTriangle() {}

void drawCirlce() {}


int main() {
    // Make exiting without crashing possible lol
    setup_callbacks();

    // Setup the library used for rendering
    initGu();

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