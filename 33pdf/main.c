#define _XOPEN_SOURCE 500
#include "cleanup.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <err.h>

int readNumber(int fd, uint32_t* n)
{
	ssize_t rd = read(fd, n, sizeof *n);
	if(!rd || rd == sizeof *n)
		return rd;
	err(6, "error read");
}

void writeNumber(int fd, uint32_t n)
{
	if(write(fd, &n, sizeof n) != sizeof n)
		err(7, "error write");
}

void Merge(int fdIn1, int fdIn2, int fdOut)
{
	uint32_t a, b;
	ssize_t r1 = readNumber(fdIn1, &a),
			r2 = readNumber(fdIn2, &b);
	while(r1 && r2)
	{
		if(a < b)
		{
			writeNumber(fdOut, a);
			r1 = readNumber(fdIn1, &a);
		}
		else
		{
			writeNumber(fdOut, b);
			r2 = readNumber(fdIn2, &b);
		}
	}
	while(r1)
	{
		writeNumber(fdOut, a);
		r1 = readNumber(fdIn1, &a);
	}
	while(r2)
	{
		writeNumber(fdOut, b);
		r2 = readNumber(fdIn2, &b);
	}
}

int cmp(const void* a, const void* b)
{
	uint32_t p = *(uint32_t*)a, q = *(uint32_t*)b;
	return (p>q) - (p<q);
}

off_t getFileSize(const char* file)
{
	struct stat st;
	if(stat(file, &st) == -1)
		err(3, "error stat");
	return st.st_size;
}

const char *tempFiles[2] = {"", ""};
void unlinkTemp()
{
	for(int i=0; i<2; i++)
		unlink(tempFiles[i]);
}
char temp1[] = "/tmp/XXXXXX";
char temp2[] = "/tmp/XXXXXX";

int main(int argc, char** argv)
{
	atexit(unregall);
	atexit(unlinkTemp);
	if(argc != 2)
		errx(1, "usage: %s <filename>", argv[0]);
	off_t size = getFileSize(argv[1]);
	if(size%sizeof(uint32_t))
		errx(2, "file %s is corrupt", argv[1]);
	int fd = open(argv[1], O_RDWR);
	if(fd == -1)
		err(4, "error opening %s", argv[1]);
	registerfd(fd);
	uint32_t *buf = malloc(size/2+1);
	if(!buf)
		errx(5, "memory allocation failed");
	registerptr(buf);
	off_t s1 = size/sizeof(uint32_t)/2*sizeof(uint32_t), s2 = size-s1;
	if(read(fd, buf, s1) != s1)
		err(6, "error read");
	qsort(buf, s1/sizeof(uint32_t), sizeof(uint32_t), cmp);
	int fd1 = mkstemp(temp1);
	if(fd1 == -1)
		err(8, "could not create temporary file");
	tempFiles[0] = temp1;
	registerfd(fd1);
	if(write(fd1, buf, s1) != s1)
		err(7, "error write");
	
	if(read(fd, buf, s2) != s2)
		err(6, "error read");
	qsort(buf, s2/sizeof(uint32_t), sizeof(uint32_t), cmp);
	int fd2 = mkstemp(temp2);
	if(fd2 == -1)
		err(8, "could not create temporary file");
	tempFiles[1] = temp2;
	registerfd(fd2);
	if(write(fd2, buf, s2) != s2)
		err(7, "error write");
	if(lseek(fd, 0, SEEK_SET) == -1 || lseek(fd1, 0, SEEK_SET) == -1 || lseek(fd2, 0, SEEK_SET) == -1)
		err(9, "error lseek");
	unregptr(buf);
	Merge(fd1, fd2, fd);
	return 0;
}
