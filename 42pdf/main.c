#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include "wrappers.h"
#include "cleanup.h"

struct Record
{
	uint32_t uid;
	uint32_t duration;
	uint32_t start, end;
} arr[1<<16];

off_t getFileSize(int fd)
{
	struct stat st;
	call(fstat, fd, &st);
	return st.st_size;
}

long double sqrld(long double x)
{
	return x*x;
}

uint64_t sqr(uint64_t x)
{
	return x*x;
}

long double dispersion(const struct Record* arr, int cnt, long double avg)
{
	long double D = 0;
	for(int i=0; i<cnt; i++)
		D += sqrld(fabsl(arr[i].duration-avg));
	return D / cnt;
}

int cmp(const void* a, const void* b)
{
	const struct Record *p = a, *q = b;
	if(p->uid != q->uid)
		return (p->uid > q->uid) - (p->uid < q->uid);
	return (p->duration < q->duration) - (p->duration > q->duration);
}

int main(int argc, char** argv)
{
	atexit(clean);
	if(argc != 2)
		errx(1, "Usage: %s <inputfile>", argv[0]);
	int fd;
	callr(fd, open, argv[1], O_RDONLY);
	registerfd(fd);
	if(getFileSize(fd) % sizeof *arr)
		errx(2, "bad input file");
	long double avg = 0;
	int cnt;
	for(cnt=0; Read(fd, arr+cnt, sizeof *arr); cnt++)
	{
		if(arr[cnt].end < arr[cnt].start)
			errx(3, "negative duration");
		arr[cnt].duration = arr[cnt].end - arr[cnt].start;
		avg += arr[cnt].duration;
	}
	closefd(fd);
	if(!cnt) return 0;
	avg /= cnt;
	long double D = dispersion(arr, cnt, avg);
//printf("avg = %Lf\n", avg);
//printf("D = %Lf\n", D);
	qsort(arr, cnt, sizeof *arr, cmp);
	if(sqr(arr[0].duration) > D)
		printf("%" PRIu32 " %" PRIu32 "\n", arr[0].uid, arr[0].duration);
	for(int i=1; i<cnt; i++)
		if(arr[i].uid != arr[i-1].uid && sqr(arr[i].duration) > D)
			printf("%" PRIu32 " %" PRIu32 "\n", arr[i].uid, arr[i].duration);
	return 0;
}
