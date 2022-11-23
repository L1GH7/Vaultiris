#include <stdint.h>
struct audio_ipi_reg_dma_t {
	uint32_t magic_header; 
	uint8_t task;  
	uint8_t reg_flag;
	uint16_t __reserved;
	uint32_t a2d_size;
	uint32_t d2a_size;
	uint32_t magic_footer;
};

#define AUDIO_IPI_IOCTL_REG_DMA 1074030869
