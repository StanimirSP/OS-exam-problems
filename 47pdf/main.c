// find ~ -type f -printf "%T@ %p\n" | sort -n -t ' ' -k 1,1 -r | head -n 1 | cut -d ' ' -f2-
// 									 A							B			C

#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "cleanup.h"

int main(int argc, char** argv)
{
	atexit(unregall);
	if(argc != 2)
		errx(1, "Usage: %s <dir>", argv[0]);
	int A[2];
	if(pipe(A) == -1)
		err(2, "pipe failed");
	registerfd(A[0]);
	registerfd(A[1]);
	pid_t pid = fork();
	if(pid == 0)
	{
		unregfd(A[0]);
		if(dup2(A[1], 1) == -1)
			err(4, "dup2 failed");
		int fd = open("/dev/null", O_WRONLY);
		if(fd == -1)
			err(10, "open failed");
		registerfd(fd);
		if(dup2(fd, 2) == -1)
			err(10, "dup failed");
		unregfd(fd);
		if(execlp("find", "find", argv[1], "-type", "f", "-printf", "%T@ %p\n", (char*)NULL) == -1)
			err(5, "exec failed");
	}
	if(pid == -1)
		err(3, "fork failed");
	unregfd(A[1]);
	int B[2];
	if(pipe(B) == -1)
		err(2, "pipe failed");
	registerfd(B[0]);
	registerfd(B[1]);
	pid = fork();
	if(pid == 0)
	{
		if(dup2(A[0], 0) == -1)
			err(4, "dup2 failed");
		unregfd(B[0]);
		if(dup2(B[1], 1) == -1)
			err(4, "dup2 failed");
		if(execlp("sort", "sort", "-n", "-t", " ", "-k", "1,1", "-r", (char*)NULL) == -1)
			err(5, "exec failed");
	}
	if(pid == -1)
		err(3, "fork failed");
	unregfd(A[0]);
	unregfd(B[1]);
	int C[2];
	if(pipe(C) == -1)
		err(2, "pipe failed");
	registerfd(C[0]);
	registerfd(C[1]);
	pid = fork();
	if(pid == 0)
	{
		if(dup2(B[0], 0) == -1)
			err(4, "dup2 failed");
		unregfd(C[0]);
		if(dup2(C[1], 1) == -1)
			err(4, "dup2 failed");
		if(execlp("head", "head", "-n", "1", (char*)NULL) == -1)
			err(5, "exec failed");
	}
	if(pid == -1)
		err(3, "fork failed");
	unregfd(B[0]);
	unregfd(C[1]);
	if(dup2(C[0], 0) == -1)
		err(4, "dup2 failed");
	if(execlp("cut", "cut", "-d ", "-f2-", (char*)NULL)== -1)
		err(5, "exec failed");
}
