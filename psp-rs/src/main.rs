#![no_std]
#![no_main]

use core::ffi::c_void;
use psp::sys::{self, DisplayPixelFormat, GuState, TexturePixelFormat};
use psp::vram_alloc::get_vram_allocator;
use psp::{BUF_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH};

psp::module!("sample_module", 1, 0);

// NOTE run with 'cargo psp'

// TODO remake the rectangle and cube examples here in Rust

// sceGuStart needs this list variable in all PSP programs idk.
// In the rs example, it uses 0x40000 allocated space, but the C examples
// used 0x20000
static mut LIST: psp::Align16<[u32; 0x40000]> = psp::Align16([0; 0x40000]);

fn psp_main() {
    psp::enable_home_button();

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

    // spooky
    unsafe {
        sys::sceGuInit();
        sys::sceGuStart(sys::GuContextType::Direct, LIST);
    }
}
