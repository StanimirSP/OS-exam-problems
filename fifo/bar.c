#define _POSIX_C_SOURCE 199309L
#include "wrappers.h"
#include "cleanup.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

const int attempts = 10;

int main(int argc, char** argv)
{
	atexit(clean);
	if(argc != 2)
		errx(1, "invalid number of arguments");
	int fd, i;
	for(i = 0; i<attempts; i++)
	{
		fd = open("pipe", O_RDONLY);
		if(fd != -1) break;
		fprintf(stderr, "attempt %d to open pipe failed\n", i+1);
		struct timespec t = {1, 0};
		nanosleep(&t, NULL);
	}
	if(i == attempts)
		err(2, "could not open pipe for reading");
	call(dup2, fd, 0);
	call(execl, argv[1], argv[1], (char*)NULL);
}
