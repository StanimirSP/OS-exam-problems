#include "cleanup.h"
#include "wrappers.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct
{
	uint16_t pos;
	uint8_t old, new;
} Char;

off_t getFileSize(int fd)
{
	struct stat st;
	call(fstat, fd, &st);
	return st.st_size;
}

void copyContent(int from, int to) // args are file descriptors
{
	char buf[512];
	ssize_t rd;
	while((rd = read(from, buf, sizeof buf)) > 0)
		if(write(to, buf, rd) != rd)
			err(10, "write failed");
	if(rd == -1)
		err(10, "read failed");
}

int main(int argc, char** argv)
{
	atexit(clean);
	if(argc != 4)
		errx(1, "Usage: %s <file1> <file2> <patch>", argv[0]);
	int fd1, fd2, fdp;
	callr(fdp, open, argv[1], O_RDONLY);
	registerfd(fdp);
	if(getFileSize(fdp)%sizeof(Char))
		errx(2, "bad patch file");
	callr(fd1, open, argv[2], O_RDONLY);
	registerfd(fd1);
	callr(fd2, open, argv[3], O_RDWR | O_CREAT | O_TRUNC, 0644);
	registerfd(fd2);
	copyContent(fd1, fd2);
	Char c;
	off_t fileSz = getFileSize(fd2);
	ssize_t rd;
	while((rd = read(fdp, &c, sizeof c)) == sizeof c)
	{
		if(c.pos >= fileSz)
			errx(3, "bad offset");
		call(lseek, fd1, c.pos, SEEK_SET);
		uint8_t ch;
		if(read(fd1, &ch, 1) != 1)
			err(10, "read failed");
		if(ch != c.old)
			errx(4, "no byte with value %#" PRIx8 " in %s at position %#" PRIx16, ch, argv[2], c.pos);
		call(lseek, fd2, c.pos, SEEK_SET);
		if(write(fd2, &c.new, 1) != 1)
			err(10, "write failed");
	}
	if(rd)
		err(10, "read failed");
	return 0;
}
