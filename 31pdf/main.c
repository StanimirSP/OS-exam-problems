#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <stdlib.h>
#include "cleanup.h"

void countingSort(int fd, uint32_t* buf)
{
	uint8_t c;
	ssize_t rd;
	while((rd = read(fd, &c, 1)) > 0)
		buf[c]++;
	if(rd == -1)
		err(5, "read failed");
}

void printSorted(int fd, uint32_t* buf)
{
	for(int i=0; i<256; i++)
		while(buf[i]--)
		{
			uint8_t c = i;
			if(write(fd, &c, 1) != 1)
				err(6, "write failed");
		}
}

int main(int argc, char** argv)
{
	atexit(unregall);
	if(argc != 2)
		errx(1, "Usage: %s <filename>", argv[0]);
	int fd = open(argv[1], O_RDWR);
	if(fd == -1)
		err(2, "error opening %s", argv[1]);
	registerfd(fd);
	uint32_t buf[256] = {0};
	countingSort(fd, buf);
	if(lseek(fd, 0, SEEK_SET) == -1)
		err(3, "seek failed");
	printSorted(fd, buf);
}
