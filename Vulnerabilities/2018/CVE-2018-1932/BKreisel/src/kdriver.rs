use std::ops::Drop;
use std::ptr;
use windows::core::*;
use windows::Win32::{
    Foundation::*,
    Storage::FileSystem::*,
    System::IO::DeviceIoControl,
};

pub struct KernelDriver {
    handle: HANDLE,
    device_name: String,
}

impl KernelDriver {
    pub fn new(name: &str) -> KernelDriver {
        let device_name = format!("\\\\.\\{}", name);
        println!("Opening Handle to Kernel Driver: {}", device_name);
        let name_u16: Vec<u16> = device_name.encode_utf16().collect();
        let handle: HANDLE = unsafe { CreateFileW(
            PCWSTR(name_u16.as_ptr()),
            FILE_GENERIC_READ | FILE_GENERIC_WRITE,
            FILE_SHARE_READ| FILE_SHARE_WRITE,
            ptr::null(),
            OPEN_EXISTING,
            FILE_FLAGS_AND_ATTRIBUTES(0),
            HANDLE(0),
        ).unwrap_or(INVALID_HANDLE_VALUE)};
        if handle.is_invalid() {
            panic!("Failed to grab handle for driver: {}", device_name);
        }
        println!("Acquired Handle: 0x{:x}", handle.0);
        KernelDriver {handle, device_name}
    }

    pub fn send_ioctl(&self, ioctl: u32, input: &mut Vec<u8>, output: Option<&mut Vec<u8>>) -> bool {
        let mut out_ptr = ptr::null_mut();
        let mut out_size = 0;
        if let Some(x) = output {
            out_ptr = x.as_mut_ptr();
            out_size = x.len();
        }

        println!("Sending IOCTL: 0x{:x} with 0x{:x} bytes of data", ioctl, input.len());
        let ret = unsafe {DeviceIoControl(
            self.handle,
            ioctl,
            input.as_mut_ptr() as _,
            input.len() as _,
            out_ptr as _,
            out_size as _,
            ptr::null_mut(),
            ptr::null_mut()
        )};
        return ret.as_bool();
    }
}

impl Drop for KernelDriver {
    fn drop(&mut self) {
        if !self.handle.is_invalid() {
            println!("Closing Handle to Kernel Driver: {}", self.device_name);
            unsafe {CloseHandle(self.handle)};
        }
    }
}
