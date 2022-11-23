#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <libvirt/libvirt.h>

int main(int argc, char **argv)
{
	virConnectPtr thisCP;
	int i, j;
	double total_time;
	struct timeval start, end;

	if (argc > 2)
	{
		fprintf(stderr, "Usage: ./perftest <times to run>");
		exit(1);
	}
	j = 10000;
	if (argc > 1)
		j = atoi(argv[1]);

	gettimeofday(&start, 0);
	for (i=0;i<j;i++)
	{
		thisCP = virConnectOpen("qemu:///session");
		virConnectClose(thisCP);
	}
	gettimeofday(&end, 0);

	total_time = 1e6 * (end.tv_sec-start.tv_sec) + (end.tv_usec-start.tv_usec);
	printf("%f microseconds to open and close a QEMU connection averaged over %d connections.\n", total_time/j, j);
	return 0;
}
