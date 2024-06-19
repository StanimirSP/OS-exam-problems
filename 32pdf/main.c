#include <sys/types.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "cleanup.h"

typedef struct
{
	uint32_t pos, cnt;
} Pair;

off_t getFileSize(const char* file)
{
	struct stat st;
	if(stat(file, &st) == -1)
		err(2, "stat failed");
	return st.st_size;
}

int main(int argc, char** argv)
{
	atexit(unregall);
	if(argc != 4)
		errx(1, "Usage: %s <file1> <file2> <file3>", argv[0]);
	off_t f1size = getFileSize(argv[1]), f2size = getFileSize(argv[2]);
	if(f1size%sizeof(Pair) || f2size%sizeof(uint32_t))
		errx(3, "incorrect format of input files");
	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1)
		err(4, "error opening %s", argv[1]);
	registerfd(fd1);
	int fd2 = open(argv[2], O_RDONLY);
	if(fd2 == -1)
		err(4, "error opening %s", argv[2]);
	registerfd(fd2);
	f1size /= sizeof(Pair);
	f2size /= sizeof(uint32_t);
	int fd3 = open(argv[3], O_WRONLY | O_CREAT, 0644);
	if(fd3 == -1)
		err(8, "could not create file %s", argv[3]);
	while(f1size--)
	{
		Pair p;
		if(read(fd1, &p, sizeof p) != sizeof p)
			err(5, "read failed");
		if((uint64_t)p.pos + p.cnt > (uint64_t)f2size)
			errx(3, "incorrect data in %s: bad offset", argv[1]);
		if(lseek(fd2, p.pos*sizeof(uint32_t), SEEK_SET) == -1)
			err(6, "lseek failed");
		while(p.cnt--)
		{
			uint32_t number;
			if(read(fd2, &number, sizeof number) != sizeof number)
				err(5, "read failed");
			if(write(fd3, &number, sizeof number) != sizeof number)
				err(7, "write failed");
		}
	}
}
