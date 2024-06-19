#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX 64

struct
{
	int fd[MAX]; // file descriptors to be closed
	int cnt;
} fdarr;

struct
{
	void *ptr[MAX]; // pointers to be freed
	int cnt;
} ptrarr={.cnt=0};

int registerfd(int fd)
{
	if(fdarr.cnt >= MAX)
		return 1;
	fdarr.fd[fdarr.cnt++] = fd;
	return 0;
}

int registerptr(void* ptr)
{
	if(ptrarr.cnt >= MAX)
		return 1;
	ptrarr.ptr[ptrarr.cnt++] = ptr;
	return 0;
}

int releasefd(int fd)
{
	for(int i=0; i<fdarr.cnt; i++)
		if(fdarr.fd[i] == fd)
		{
			fdarr.fd[i] = fdarr.fd[--fdarr.cnt];
			return fd;
		}
	return -1;
}

void* releaseptr(void* ptr)
{
	for(int i=0; i<ptrarr.cnt; i++)
		if(ptrarr.ptr[i] == ptr)
		{
			ptrarr.ptr[i] = ptrarr.ptr[--ptrarr.cnt];
			return ptr;
		}
	return NULL;
}

int unregfd(int fd)
{
	int f = releasefd(fd);
	if(f == -1)
		return 1;
	close(f);
	return 0;
}

int unregptr(void* ptr)
{
	void *p = releaseptr(ptr);
	if(!p)
		return 1;
	free(p);
	return 0;
}

void unregallfd(void)
{
	for(int i=0; i<fdarr.cnt; i++)
		close(fdarr.fd[i]);
	fdarr.cnt = 0;
}

void unregallptr(void)
{
	for(int i=0; i<ptrarr.cnt; i++)
		free(ptrarr.ptr[i]);
	ptrarr.cnt = 0;
}

void unregall(void)
{
	unregallptr();
	unregallfd();
}

#undef MAX
