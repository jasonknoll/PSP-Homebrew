#![no_std]
#![no_main]

use core::ffi::c_void;
use psp::sys::{self, sceGuDisplay, sceGuEnable, SceCtrlData, ClearBuffer, DisplayPixelFormat, GuContextType, GuState, GuSyncBehavior, GuSyncMode, TexturePixelFormat, CtrlMode};
use psp::vram_alloc::get_vram_allocator;
use psp::{BUF_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH};
use psp::sys::DepthFunc::Equal;

psp::module!("sample_module", 1, 0);

// NOTE run with 'cargo psp'

// TODO remake the rectangle and cube examples here in Rust

// sceGuStart needs this list variable in all PSP programs idk.
// In the rs example, it uses 0x40000 allocated space, but the C examples
// used 0x20000
static mut LIST: psp::Align16<[u32; 0x40000]> = psp::Align16([0; 0x40000]);

fn psp_main() {
    psp::enable_home_button();



    // spooky
    unsafe {
        init_gu();

        let pad = SceCtrlData::default(); // controller

        sys::sceCtrlSetSamplingCycle(0);
        sys::sceCtrlSetSamplingMode(CtrlMode::Analog);

        loop {
            start_frame();

            todo!("Need to draw rectangle vertices and control it with d-pad");

            end_frame();
        }
    }
}

unsafe fn init_gu() {
    let mut allocator = get_vram_allocator().unwrap();
    let fbp0 = allocator
        .alloc_texture_pixels(BUF_WIDTH, SCREEN_HEIGHT, TexturePixelFormat::Psm8888)
        .as_mut_ptr_from_zero();
    let fbp1 = allocator
        .alloc_texture_pixels(BUF_WIDTH, SCREEN_HEIGHT, TexturePixelFormat::Psm8888)
        .as_mut_ptr_from_zero();
    let zbp = allocator
        .alloc_texture_pixels(BUF_WIDTH, SCREEN_HEIGHT, TexturePixelFormat::Psm4444)
        .as_mut_ptr_from_zero();

    sys::sceGuInit();
    sys::sceGuStart(
        sys::GuContextType::Direct,
        &mut LIST as *mut _ as *mut c_void,
    );
    sys::sceGuDrawBuffer(DisplayPixelFormat::Psm8888, fbp0 as _, BUF_WIDTH as i32);
    sys::sceGuDispBuffer(SCREEN_WIDTH as i32, SCREEN_HEIGHT as i32, fbp1 as _, BUF_WIDTH as i32);
    sys::sceGuDepthBuffer(zbp as _, BUF_WIDTH as i32);

    sys::sceGuOffset(2048 - (SCREEN_WIDTH/2), 2048 - (SCREEN_HEIGHT/2));
    sys::sceGuViewport(2048, 2048, SCREEN_WIDTH as i32, SCREEN_HEIGHT as i32);
    sceGuEnable(GuState::ScissorTest);
    sys::sceGuScissor(0, 0, SCREEN_WIDTH as i32, SCREEN_HEIGHT as i32);

    sys::sceGuDepthRange(65535, 0);

    sys::sceGuDepthFunc(Equal);
    sys::sceGuEnable(GuState::DepthTest);

    sys::sceGuFinish();
    sceGuDisplay(true);
}

unsafe fn start_frame() {
    sys::sceGuStart(GuContextType::Direct, &mut LIST as *mut _ as *mut c_void);
    sys::sceGuClearColor(0xffffffff);
    sys::sceGuClear(ClearBuffer::COLOR_BUFFER_BIT);
}

unsafe fn end_frame() {
    sys::sceGuFinish();
    sys::sceGuSync(GuSyncMode::Finish, GuSyncBehavior::Wait);
    sys::sceDisplayWaitVblankStart();
    sys::sceGuSwapBuffers();
}

unsafe fn draw_rect() {
    todo!()
}

unsafe fn draw_cube() {
    todo!()
}
