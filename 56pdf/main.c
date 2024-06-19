#include <sys/types.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
	if(argc != 3)
		errx(1, "invalid number of args");
	int A[2];
	pipe(A);
	pid_t pid = fork();
	if(pid == 0)
	{
		close(A[0]);
		dup2(A[1], 1);
		close(A[1]);
		execlp("cat", "cat", argv[1], (char*)NULL);
	}
	close(A[1]);
	int fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	// check fd ...
	uint8_t c, prev = 0;
	while(read(A[0], &c, 1) == 1)
	{
		if(prev == 0x7D)
		{
			c ^= 0x20;
			write(fd, &c, 1);
			prev = 0;
			continue;
		}
		if(c != 0x55 && c != 0x7D)
			write(fd, &c, 1);
		prev = c;
	}
	close(fd);
	close(A[0]);
	return 0;
}
