#include "cleanup.h"
#include "wrappers.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

off_t getFileSize(int fd)
{
	struct stat st;
	call(fstat, fd, &st);
	return st.st_size;
}

typedef struct
{
	uint16_t pos;
	uint8_t len, reserved;
} Str;

int main(int argc, char** argv)
{
	atexit(clean);
	if(argc != 5)
		errx(1, "Usage: %s <dat1> <idx1> <dat2> <idx2>", argv[0]);
	int fddat1, fdidx1, fddat2, fdidx2;
	callr(fdidx1, open, argv[2], O_RDONLY);
	registerfd(fdidx1);
	if(getFileSize(fdidx1)%4)
		errx(2, "incorrect format of %s file", argv[2]);
	callr(fddat1, open, argv[1], O_RDONLY);
	registerfd(fddat1);
	callr(fddat2, open, argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	registerfd(fddat2);
	callr(fdidx2, open, argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	registerfd(fdidx2);
	Str s;
	ssize_t rd;
	uint16_t outputPos = 0;
 	off_t sz1 = getFileSize(fddat1);
	while((rd = read(fdidx1, &s, sizeof s)) == sizeof s)
	{
		char buf[255];
		if(s.pos >= sz1)
			errx(3, "bad offset");
		call(lseek, fddat1, s.pos, SEEK_SET);
		if(read(fddat1, buf, s.len) != s.len)
			err(10, "read failed");
		if(s.len && *buf >= 'A' && *buf <= 'Z')
		{
			s.pos = outputPos;
			if(write(fddat2, buf, s.len) != s.len || write(fdidx2, &s, sizeof s) != sizeof s)
				err(10, "write failed");
			outputPos += s.len;
		}
	}
	if(rd)
		err(10, "read failed");
	return 0;
}
