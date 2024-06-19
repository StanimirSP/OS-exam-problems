#include "wrappers.h"
#include "cleanup.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	atexit(clean);
	if(argc != 2)
		errx(1, "invalid number of arguments");
	call(mkfifo, "pipe", 0644);
	int fd = open("pipe", O_WRONLY);
	call(unlink, "pipe");
	if(fd == -1)
		err(10, "open failed");
	registerfd(fd);
	call(dup2, fd, 1);
	call(execlp, "cat", "cat", argv[1], (char*)NULL);
}
