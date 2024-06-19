#include <err.h>
#include <unistd.h>
#include <stddef.h>

#define call(func, ...) \
    if((func)(__VA_ARGS__) == -1) \
        err(10, #func " failed")

#define callr(ret, func, ...) \
    if(((ret)=(func)(__VA_ARGS__)) == -1) \
        err(10, #func " failed")

#define callrn(ret, func) \
	if(((ret)=(func)()) == -1) \
		err(10, #func " failed")

int Read(int fd, void* buf, size_t cnt)
{
    ssize_t rd = read(fd, buf, cnt);
    if(!rd || rd == (ssize_t)cnt)
        return rd;
    err(10, "read failed");
}

void Write(int fd, const void* buf, size_t cnt)
{
    if(write(fd, buf, cnt) != (ssize_t)cnt)
        err(10, "write failed");
}
