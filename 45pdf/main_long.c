#include "wrappers.h"
#include "cleanup.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stddef.h>

// cut -d':' -f7 /etc/passwd | sort | uniq -c | sort -n -k 1,1
// 							 A		B		  C

int main(void)
{
	atexit(clean);
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
		call(execlp, "cut", "cut", "-d:", "-f7", "/etc/passwd", (char*)NULL);
	}
	closefd(a[1]);
	int b[2];
	call(pipe, b);
	registerfd(b[0]);
	registerfd(b[1]);
	callrn(p, fork);
	if(!p)
	{
		closefd(b[0]);
		call(dup2, a[0], 0);
		closefd(a[0]);
		call(dup2, b[1], 1);
		closefd(b[1]);
		call(execlp, "sort", "sort", (char*)NULL);
	}
	closefd(a[0]);
	closefd(b[1]);
	int c[2];
	call(pipe, c);
	registerfd(c[0]);
	registerfd(c[1]);
	callrn(p, fork);
	if(!p)
	{
		closefd(c[0]);
		call(dup2, b[0], 0);
		closefd(b[0]);
		call(dup2, c[1], 1);
		closefd(c[1]);
		call(execlp, "uniq", "uniq", "-c", (char*)NULL);
	}
	closefd(b[0]);
	closefd(c[1]);
	call(dup2, c[0], 0);
	closefd(c[0]);
	call(execlp, "sort", "sort", "-n", "-k", "1,1", (char*)NULL);
}
	
