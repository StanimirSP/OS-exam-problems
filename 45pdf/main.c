#include "wrappers.h"
#include "cleanup.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stddef.h>

// cut -d':' -f7 /etc/passwd | sort | uniq -c | sort -n -k 1,1
// 							 A		B		  C

void execCmd(const char* cmd, char* const* argv, int readFd, int p[2])
{
	registerfd(p[0]);
	registerfd(p[1]);
	pid_t pid;
	callrn(pid, fork);
	if(!pid)
	{
		closefd(p[0]);
		call(dup2, readFd, 0); // will do nothing if readFd == 0
		closefd(readFd); // will do nothing if readFd == 0 - not registered
		call(dup2, p[1], 1);
		closefd(p[1]);
		call(execvp, cmd, argv);
	}
	closefd(p[1]);
}

int main(void)
{
	atexit(clean);
	int a[2];
	char *const args1[] = {"cut", "-d:", "-f7", "/etc/passwd", (char*)NULL};
	call(pipe, a);
	execCmd("cut", args1, 0, a);
	int b[2];
	char *const args2[] = {"sort", (char*)NULL};
	call(pipe, b);
	execCmd("sort", args2, a[0], b);
	closefd(a[0]);
	int c[2];
	char *const args3[] = {"uniq", "-c", (char*)NULL};
	call(pipe, c);
	execCmd("uniq", args3, b[0], c);
	closefd(b[0]);
	call(dup2, c[0], 0);
	closefd(c[0]);
	call(execlp, "sort", "sort", "-n", "-k", "1,1", (char*)NULL);
}

