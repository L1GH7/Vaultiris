#include <stdbool.h>
#include <stdint.h>
#include "binder.h"
#define BINDER_BUFFER_SZ 128*1024
struct new_node{
	struct binder_state *bs;
	uint64_t handle;
	const char *endpoint_name;
   uint8_t name[16];
	uint64_t vma_start;
	bool second;
	pthread_t *th;
	int idx;
   int max;
   struct pending_node *pending_nodes;
   int num_pending;
	uint64_t addr;
   uint64_t kaddr;
	int target_fd;
	uint64_t file_addr;
	int ep_fd;
	pid_t tid;
};

struct new_node *node(const char *name);
