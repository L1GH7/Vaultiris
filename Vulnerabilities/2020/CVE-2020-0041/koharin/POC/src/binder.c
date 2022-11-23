#include "binder.h"

#define BIO_F_SHARED    0x01  /* needs to be buffer freed */
#define BIO_F_OVERFLOW  0x02  /* ran out of space */
#define BIO_F_IOERROR   0x04
#define BIO_F_MALLOCED  0x08  /* needs to be free()'d */
// binder_open: open binder device(/dev/hwbinder) file & map binder to user space

struct binder_state *binder_open(const char* binder_driver, size_t mapsize)
{
    struct binder_state *bs;
    struct binder_version vers;

    // 1. binder driver open
    if(!(bs = calloc(1, sizeof(*bs)))){
        fprintf(stderr, "[-] binder object malloc error\n");
        return NULL;
    }

    if((bs->fd = open(binder_driver, O_RDWR)) < 0 ){
        fprintf(stderr, "[-] binder open error: %s\n", strerror(errno));
        free(bs);
        return NULL;
    }

    // binder 입출력 설정 바꾸는 코드 BINDER_VERSION: 요청 코드, ver: binder version 포인터. 
    if ((ioctl(bs->fd, BINDER_VERSION, &vers) == -1) || (vers.protocol_version != BINDER_CURRENT_PROTOCOL_VERSION)) {
        fprintf(stderr, "[-] binder: kernel driver version (%d) differs from user space version (%d)\n", vers.protocol_version, BINDER_CURRENT_PROTOCOL_VERSION);
        free(bs);
        return NULL;
    }
    
    // 2. Kernel에서 Binder IPC Data 수신 위한 공유 메모리 영역 확보
    bs->mapsize = mapsize;
    if((bs->mapped = mmap(NULL, mapsize, PROT_READ, MAP_PRIVATE, bs->fd, 0)) == MAP_FAILED){ 
        fprintf(stderr, "[-] binder device mapping error %s\n", strerror(errno));
        close(bs->fd);
        return NULL;
    }
    printf("[+] binder map success\n");
    return bs;
}

// binder_state 값으로 Context Manager(Service Manager) 설정
int binder_become_context_manager(struct binder_state *bs){
    return ioctl(bs->fd, BINDER_SET_CONTEXT_MGR, 0);
}

uint32_t binder_read_next(struct binder_state *bs, void *data, uint32_t *remaining, uint32_t *consumed)
{
	int res;
	uint32_t cmd;
	void *ptr, *end;

//	log_info("remaining: %x\nconsumed: %x\n", *remaining, *consumed);

	if (!*remaining) {
		/* Read the first 8 bytes. */
//		log_info("before read\n");
		res = binder_read(bs->fd, data, 32 * sizeof(uint32_t));
//		log_info("after read: %x\n", res);
		if (res < 0) {
			log_err("binder_read_next: %s\n", strerror(errno));
			return (uint32_t)-1;
		}

		*remaining = res;
		*consumed = 0;
	}


	ptr = data;
	ptr += *consumed;
	end = ptr + *remaining;

	cmd = *(uint32_t *)ptr;

	*consumed += sizeof(uint32_t);
	*remaining -= sizeof(uint32_t);
	ptr += sizeof(uint32_t);

	//log_info("cmd: %s\n", cmd_name(cmd));
	switch (cmd) {
		case BR_NOOP:
			res = 0;
			break;

		case BR_RELEASE:
		case BR_DECREFS:
		case BR_ACQUIRE:
		case BR_INCREFS:
			res =2 * sizeof(uint64_t);
			*consumed += res;
			*remaining -= res;
			break;
		case BR_REPLY:
		case BR_TRANSACTION:
			res = sizeof(struct binder_transaction_data);
			*consumed += res;
			*remaining -= res;
			break;
		case BR_FAILED_REPLY:
		case BR_TRANSACTION_COMPLETE:
			res = 0;
			break;
		default:
			log_err("Unhandle command %s\n", cmd_name(cmd));
			exit(1);
			return (uint32_t)-1;

	}

	/* Update ptr and size */
	return cmd;
}

void binder_loop(struct binder_state *bs, binder_handler func){
    int res;
    struct binder_write_read bwr;
    uint32_t readbuf[32];

    bwr.write_size = 0;
    bwr.write_consumed = 0;
    bwr.write_buffer = 0;

    readbuf[0] = BC_ENTER_LOOPER;
    binder_write(bs, readbuf, sizeof(uint32_t));

    for(;;){
        bwr.read_size = sizeof(readbuf);
        bwr.read_consumed = 0;
        bwr.read_buffer = (uintptr_t)readbuf;

        if((res = ioctl(bs->fd, BINDER_WRITE_READ, &bwr)) < 0){
            fprintf(stderr, "[-] binder_loop: ioctl failed (%s)\n", strerror(errno));
            break;
        }

        if((res = binder_parse(bs, 0, (uintptr_t)readbuf, bwr.read_consumed, func)) == 0){
            fprintf(stderr, "[-] binder_loop: unexpected reply\n");
            break;
        }
        if(res < 0){
            fprintf(stderr, "[-] binder_loop: io error %d %s\n", res, strerror(errno));
            break;
        }
    }
}

void binder_acquire(struct binder_state *bs, uint32_t target)
{
    uint32_t cmd[2];
    cmd[0] = BC_ACQUIRE;
    cmd[1] = target;
    binder_write(bs, cmd, sizeof(cmd));
}

void binder_release(struct binder_state *bs, uint32_t target)
{
    uint32_t cmd[2];
    cmd[0] = BC_RELEASE;
    cmd[1] = target;
    binder_write(bs, cmd, sizeof(cmd));
}

int binder_write(struct binder_state *bs, void *data, size_t len){
    struct binder_write_read bwr;
    int res;

    bwr.write_size = len;
    bwr.write_consumed = 0;
    bwr.write_buffer = (uintptr_t) data;
    bwr.read_size = 0;
    bwr.read_consumed = 0;
    bwr.read_buffer = 0;
    if((res = ioctl(bs->fd, BINDER_WRITE_READ, &bwr)) < 0){
        fprintf(stderr, "[-] binder_write: ioctl failed (%s)\n", strerror(errno));
    }
    return res;

}

/*
 * This is just sending 0x100 commands to free the buffer in a row,
 * saving us a few syscalls.
 */
void binder_free_buffers(struct binder_state *bs, binder_uintptr_t buffer_to_free)
{
    struct free_buf_data {
        uint32_t cmd_free;
        binder_uintptr_t buffer;
    } __attribute__((packed)) ;

    struct free_buf_data data[0x100];
    int i;

    for(i=0; i < 0x100; i++){
        data[i].cmd_free = BC_FREE_BUFFER;
        data[i].buffer = buffer_to_free;
    }

    binder_write(bs, &data[0], sizeof(data));
//    binder_write(bs, &data[0], sizeof(struct free_buf_data) * 0x10);

}


void binder_free_buffer(struct binder_state *bs, binder_uintptr_t buffer_to_free)
{
    struct {
        uint32_t cmd_free;
        binder_uintptr_t buffer;
    } __attribute__((packed)) data;
    data.cmd_free = BC_FREE_BUFFER;
    data.buffer = buffer_to_free;
    binder_write(bs, &data, sizeof(data));
}

void binder_send_reply(struct binder_state *bs,
                       struct binder_io *reply,
                       binder_uintptr_t buffer_to_free,
                       int status)
{
    struct {
        uint32_t cmd_free;
        binder_uintptr_t buffer;
        uint32_t cmd_reply;
        struct binder_transaction_data txn;
    } __attribute__((packed)) data;

    data.cmd_free = BC_FREE_BUFFER;
    data.buffer = buffer_to_free;
    data.cmd_reply = BC_REPLY;
    data.txn.target.ptr = 0;
    data.txn.cookie = 0;
    data.txn.code = 0;
    if (status) {
        data.txn.flags = TF_STATUS_CODE;
        data.txn.data_size = sizeof(int);
        data.txn.offsets_size = 0;
        data.txn.data.ptr.buffer = (uintptr_t)&status;
        data.txn.data.ptr.offsets = 0;
    } else {
        data.txn.flags = 0;
        data.txn.data_size = reply->data - reply->data0;
        data.txn.offsets_size = ((char*) reply->offs) - ((char*) reply->offs0);
        data.txn.data.ptr.buffer = (uintptr_t)reply->data0;
        data.txn.data.ptr.offsets = (uintptr_t)reply->offs0;
    }
    binder_write(bs, &data, sizeof(data));
}

int binder_parse(struct binder_state *bs, struct binder_io *bio, uint32_t *ptr, uint32_t size, binder_handler func)
{
    int r = 1;
    uint32_t *end = ptr + (size / 4);

    while (ptr < end) {
        uint32_t cmd = *ptr++;
#if TRACE
        fprintf(stderr,"%s:\n", cmd_name(cmd));
#endif
        switch(cmd) {
        case BR_NOOP:
            break;
        case BR_TRANSACTION_COMPLETE:
            break;
        case BR_INCREFS:
        case BR_ACQUIRE:
        case BR_RELEASE:
        case BR_DECREFS:
#if TRACE
            fprintf(stderr,"  %08x %08x\n", ptr[0], ptr[1]);
#endif
            ptr += 2;
            break;
        case BR_TRANSACTION: {
            struct binder_transaction_data *txn = (struct binder_transaction_data *) ptr;
            if ((end - ptr) * sizeof(uint32_t) < sizeof(*txn)) {
                fprintf(stderr, "[-] parse: txn too small!\n");
                return -1;
            }
            binder_dump_txn(txn);
            if (func) {
                unsigned rdata[256/4];
                struct binder_io msg;
                struct binder_io reply;
                int res;

                bio_init(&reply, rdata, sizeof(rdata), 4);
                bio_init_from_txn(&msg, txn);
                res = func(bs, txn, &msg, &reply);
                binder_send_reply(bs, &reply, txn->data.ptr.buffer, res);
            }
            ptr += sizeof(*txn) / sizeof(uint32_t);
            break;
        }
        case BR_REPLY: {
            struct binder_transaction_data *txn = (struct binder_transaction_data *) ptr;
            if ((end - ptr) * sizeof(uint32_t) < sizeof(*txn)) {
                fprintf(stderr, "[-] parse: reply too small!\n");
                return -1;
            }
            binder_dump_txn(txn);
            if (bio) {
                bio_init_from_txn(bio, txn);
                bio = 0;
            } else {
                    /* todo FREE BUFFER */
            }
            ptr += (sizeof(*txn) / sizeof(uint32_t));
            r = 0;
            break;
        }
        case BR_DEAD_BINDER: {
            struct binder_death *death = (struct binder_death *)(uint32_t) *(binder_uintptr_t*)ptr++;
            death->func(bs, death->ptr);
            break;
        }
        case BR_FAILED_REPLY:
            r = -1;
            break;
        case BR_DEAD_REPLY:
            r = -1;
            break;
        default:
            fprintf(stderr, "[-] parse: OOPS %d\n", cmd);
            return -1;
        }
    }

    return r;
}

void hexdump(void *_data, size_t len)
{
    unsigned char *data = _data;
    size_t count;

    for (count = 0; count < len; count++) {
        if ((count & 15) == 0)
            fprintf(stderr,"%04zu:", count);
        fprintf(stderr,"\\x%02x", *data);
        // fprintf(stderr," %02x %c", *data,
                // (*data < 32) || (*data > 126) ? '.' : *data);
        data++;
        if ((count & 15) == 15)
            fprintf(stderr,"\n");
    }
    if ((count & 15) != 0)
        fprintf(stderr,"\n");
}

void binder_dump_txn(struct binder_transaction_data *txn)
{
    struct flat_binder_object *obj;
    binder_size_t *offs = (binder_size_t *)(uintptr_t)txn->data.ptr.offsets;
    size_t count = txn->offsets_size / sizeof(binder_size_t);

    fprintf(stderr,"  target %016"PRIx64"  cookie %016"PRIx64"  code %08x  flags %08x\n",
            (uint64_t)txn->target.ptr, (uint64_t)txn->cookie, txn->code, txn->flags);
    fprintf(stderr,"  pid %8d  uid %8d  data %"PRIu64"  offs %"PRIu64"\n",
            txn->sender_pid, txn->sender_euid, (uint64_t)txn->data_size, (uint64_t)txn->offsets_size);
    hexdump((void *)(uintptr_t)txn->data.ptr.buffer, txn->data_size);
    while (count--) {
        obj = (struct flat_binder_object *) (((char*)(uintptr_t)txn->data.ptr.buffer) + *offs++);
        fprintf(stderr,"  - type %08x  flags %08x  ptr %016"PRIx64"  cookie %016"PRIx64"\n",
                obj->hdr.type, obj->flags, (uint64_t)obj->binder, (uint64_t)obj->cookie);
    }
}

void bio_init(struct binder_io *bio, void *data, size_t maxdata, size_t maxoffs)
{
    size_t n = maxoffs * sizeof(size_t);

    if (n > maxdata) {
        bio->flags = BIO_F_OVERFLOW;
        bio->data_avail = 0;
        bio->offs_avail = 0;
        return;
    }

    bio->data = bio->data0 = (char *) data + n;
    bio->offs = bio->offs0 = data;
    bio->data_avail = maxdata - n;
    bio->offs_avail = maxoffs;
    bio->flags = 0;
}

void bio_init_from_txn(struct binder_io *bio, struct binder_transaction_data *txn)
{
    bio->data = bio->data0 = (char *)(intptr_t)txn->data.ptr.buffer;
    bio->offs = bio->offs0 = (binder_size_t *)(intptr_t)txn->data.ptr.offsets;
    bio->data_avail = txn->data_size;
    bio->offs_avail = txn->offsets_size / sizeof(size_t);
    bio->flags = BIO_F_SHARED;

}
