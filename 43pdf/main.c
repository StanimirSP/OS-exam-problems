#include "wrappers.h"
#include "cleanup.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stddef.h>

// cat argv[1] | sort

int main(int argc, char** argv)
{
	atexit(clean);
	if(argc != 2)
		errx(1, "Usage: %s <file>", argv[0]);
	int a[2];
	call(pipe, a);
	registerfd(a[0]);
	registerfd(a[1]);
	pid_t p;
	callrn(p, fork);
	if(!p)
	{
		closefd(a[0]);
		call(dup2, a[1], 1);
		closefd(a[1]);
		call(execlp, "cat", "cat", argv[1], (char*)NULL);
	}
	closefd(a[1]);
	call(dup2, a[0], 0);
	closefd(a[0]);
	call(execlp, "sort", "sort", (char*)NULL);
}
