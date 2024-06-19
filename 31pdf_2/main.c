#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	if(argc != 2)
		errx(1, "invalid number of arguments"); 
	int fd = open(argv[1], O_RDWR);
	if(fd == -1)
		err(2, "could not open file");
	uint64_t cnt[256]={0};
	do
	{
		unsigned char c;
		ssize_t rd = read(fd, &c, 1);
		if(rd == -1)
		{
			close(fd);
			err(3, "read failed");
		}
		if(rd == 0) break;
		cnt[c]++;
	} while(1);
	if(lseek(fd, 0, SEEK_SET) == -1)
	{
		close(fd);
		err(4, "lseek failed");
	}
	for(int i=0; i<256; i++)
		while(cnt[i]--)
		{
			unsigned char c = i;
			ssize_t wr;
		retry:
			wr = write(fd, &c, 1);
			if(wr == -1)
			{
				close(fd);
				err(5, "write failed");
			}
			if(wr == 0) goto retry;
		}
	close(fd);
	return 0;
}
