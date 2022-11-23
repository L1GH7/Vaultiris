#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <linux/bpf.h>
#include <sys/socket.h>
#include <sys/syscall.h>

int obj_get_info_by_fd(int fd, uint32_t* out)
{
    union bpf_attr attr;
    struct bpf_map_info info;
    int ret;
    memset(&attr, 0, sizeof(attr));
    memset(&info, 0, sizeof(info));
    attr.info.bpf_fd = fd;
    attr.info.info_len = sizeof(info);
    attr.info.info = (uint64_t)(&info);
    ret = syscall(__NR_bpf, BPF_OBJ_GET_INFO_BY_FD, &attr, sizeof(attr));
    //printf("ret: %d\n", ret);
    *out = info.btf_id;
    return ret;
}

int bpf(int cmd, union bpf_attr *attrs)
{
    return syscall(__NR_bpf, cmd, attrs, sizeof(*attrs));
}

int create_map(union bpf_attr* attrs)
{
    return bpf(BPF_MAP_CREATE, attrs);
}

int update_map_element(int map_fd, uint64_t key, void* value, uint64_t flags)
{
    union bpf_attr attr =
    {
        .map_fd = map_fd,
        .key    = (uint64_t)&key,
        .value  = (uint64_t)value,
        .flags  = flags,
    };

    return bpf(BPF_MAP_UPDATE_ELEM, &attr);
}

int bpf_update_elem(int fd, const void *key, const void *value,
                    uint64_t flags)
{
    union bpf_attr attr = {
            .map_fd = fd,
            .key = (uint64_t)(key),
            .value = (uint64_t)(value),
            .flags = flags,
    };

    return syscall(__NR_bpf, BPF_MAP_UPDATE_ELEM, &attr, sizeof(attr));
}

int run_bpf_prog(struct bpf_insn* insn, uint32_t cnt, int* prog_fd_out, char* pVal, uint32_t size) {
    int ret = -1;
    int prog_fd = -1;
    char verifier_log_buff[0x200000] = {0};
    int socks[2] = {0};
    union bpf_attr prog_attrs =
    {
            .prog_type = BPF_PROG_TYPE_SOCKET_FILTER,
            .insn_cnt = cnt,
            .insns = (uint64_t) insn,
            .license = (uint64_t) "",
            .log_level = 2,
            .log_size = sizeof(verifier_log_buff),
            .log_buf = (uint64_t) verifier_log_buff
    };

    if (NULL != prog_fd_out) {
        prog_fd = *prog_fd_out;
    }

    if (0 >= prog_fd) {
        prog_fd = bpf(BPF_PROG_LOAD, &prog_attrs);
    }

    if (0 > prog_fd) {
        puts(verifier_log_buff);
        goto done;
    }

    if (0 != socketpair(AF_UNIX, SOCK_DGRAM, 0, socks)) {
        goto done;
    }

    if (0 != setsockopt(socks[0], SOL_SOCKET, SO_ATTACH_BPF, &prog_fd, sizeof(int))) {
        goto done;
    }

    if (pVal != NULL) {
        if (size != write(socks[1], pVal, size)) {
            goto done;
        }
    }
    else if (0x9 != write(socks[1], "abcdefghi", 0x9))
    {
        goto done;
    }


    if(NULL != prog_fd_out)
    {
        *prog_fd_out = prog_fd;
    }

    else
    {
        close(prog_fd);
    }

    ret = 0;

done:
    close(socks[0]);
    close(socks[1]);
    return ret;
}