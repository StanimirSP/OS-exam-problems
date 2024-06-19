#include "wrappers.h"
#include "cleanup.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
	atexit(clean);
	setbuf(stdout, NULL);
	int hasNum = argc > 1 && !strcmp(argv[1], "-n");
	argv += 1 + hasNum;
	char *noArgs[] = {"-", NULL};
	char **p = *argv? argv: noArgs;
	unsigned long line = 1;
	while(*p)
	{
		int fd = STDIN_FILENO;
		if(strcmp(*p, "-"))
		{
			callr(fd, open, *p, O_RDONLY);
			registerfd(fd);
		}
		char prev = '\n', cur;
		ssize_t rd;
		while((rd = read(fd, &cur, 1)) == 1)
		{
			if(prev == '\n' && hasNum)
				call(printf, "%6lu ", line++);
			// call(printf, "%c", cur);
			if(write(STDOUT_FILENO, &cur, 1) != 1)
				err(10, "write failed");
			prev = cur;	
		}
		if(rd == -1)
			err(10, "read failed");
		p++;
		closefd(fd);
	}
	return 0;
}
