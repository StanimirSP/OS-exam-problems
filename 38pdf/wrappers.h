#include <err.h>

#define call(func, ...) \
    if((func)(__VA_ARGS__) == -1) \
        err(10, #func " failed")

#define callr(ret, func, ...) \
    if(((ret)=(func)(__VA_ARGS__)) == -1) \
        err(10, #func " failed")
