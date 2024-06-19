#define _XOPEN_SOURCE 500
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include "wrappers.h"
#include "cleanup.h"
#include <stdlib.h>

int cmpLess(const void* a, const void* b)
{
	uint32_t p = *(uint32_t*)a, q = *(uint32_t*)b;
	return (p > q) - (p < q);
}

off_t getFileSize(int fd)
{
	struct stat st;
	call(fstat, fd, &st);
	return st.st_size;
}

int sortHalf(uint32_t* buf, int inpFd, off_t cnt)
{
	Read(inpFd, buf, cnt*sizeof(uint32_t));
	qsort(buf, cnt, sizeof(uint32_t), cmpLess);
	char tmp[] = "/tmp/XXXXXX";
	int tmpFd;
	callr(tmpFd, mkstemp, tmp);
	registerfd(tmpFd);
	call(unlink, tmp);
	Write(tmpFd, buf, cnt*sizeof(uint32_t));
	call(lseek, tmpFd, 0, SEEK_SET);
	return tmpFd;
}

void merge(int fd[2], int fdOut)
{
	uint32_t a[2];
	ssize_t rd[2] = { Read(fd[0], a, sizeof(uint32_t)), Read(fd[1], a+1, sizeof(uint32_t)) };
	while(rd[0] || rd[1])
	{
		int takeFrom = !rd[0] || (rd[1] && a[1] <= a[0]);
		Write(fdOut, a + takeFrom, sizeof(uint32_t));
		rd[takeFrom] = Read(fd[takeFrom], a + takeFrom, sizeof(uint32_t));
	}
}

int main(int argc, char** argv)
{
	atexit(clean);
	if(argc != 3)
		errx(1, "Usage: %s <inputfile> <outputfile>", argv[0]);
	int fd;
	callr(fd, open, argv[1], O_RDONLY);
	registerfd(fd);
	off_t inputSz = getFileSize(fd);
	if(inputSz % sizeof(uint32_t))
		errx(2, "bad input file");
	inputSz /= sizeof(uint32_t);
	off_t cnt1 = inputSz/2, cnt2 = inputSz-cnt1;
	uint32_t* buf = malloc(cnt2*sizeof(uint32_t));
	if(!buf)
		err(3, "memory allocation failed");
	registerptr(buf);
	int tmp[2] = { sortHalf(buf, fd, cnt1), sortHalf(buf, fd, cnt2) };
	freeptr(buf);
	closefd(fd);
	callr(fd, open, argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	registerfd(fd);
	merge(tmp, fd);
	return 0;
}
