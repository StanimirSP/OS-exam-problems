#include "cleanup.h"
#include "wrappers.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

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

int main(int argc, char** argv)
{
	atexit(clean);
	if(argc != 4)
		errx(1, "Usage: %s <file1> <file2> <patch>", argv[0]);
	int fd1, fd2, fdp;
	callr(fd1, open, argv[1], O_RDONLY);
	registerfd(fd1);
	callr(fd2, open, argv[2], O_RDONLY);
	registerfd(fd2);
	if(getFileSize(fd1) != getFileSize(fd2))
		errx(2, "Files have different size");
	callr(fdp, open, argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	registerfd(fdp);
	ssize_t rd1, rd2;
	Char c = {.pos=0};
	while((rd1 = read(fd1, &c.old, 1)) == 1 && (rd2 = read(fd2, &c.new, 1)) == 1)
	{
		if(c.old != c.new && write(fdp, &c, sizeof c) != sizeof c)
			err(10, "write failed");
		c.pos++;
	}
	if(rd1 == -1 || rd2 == -1)
		err(10, "read failed");
	return 0;
}
