#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include "wrappers.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct Interval
{
	unsigned long start, stop;
};

struct Interval getInterval(const char* str)
{
	char *end;
	struct Interval i;
	i.start = strtoul(str, &end, 10);
	if(end == str || *end && *end != '-' || i.start < 1 || i.start == ULONG_MAX)
		errx(2, "invalid argument");
	if(!*end)
		i.stop = i.start;
	else
	{
		i.stop = strtoul(end+1, &end, 10);
		if(*end || i.stop < 1 || i.stop == ULONG_MAX || i.stop < i.start)
			errx(2, "invalid argument");
	}
	return i;
}

void cut(struct Interval i, char delim)
{
	unsigned long pos = 1;
	char ch;
	while(Read(STDIN_FILENO, &ch, 1))
	{
		if(ch == '\n')
		{
			Write(STDOUT_FILENO, &ch, 1);
			pos = 1;
			continue;
		}
		if(pos >= i.start && pos <= i.stop && ch != delim)
			Write(STDOUT_FILENO, &ch, 1);
		if(!delim || delim == ch)
			if(++pos > i.start && pos <= i.stop && delim)
				Write(STDOUT_FILENO, &ch, 1);
	}
}

int main(int argc, char** argv)
{
	if(argc != 3 && argc != 5)
		errx(1, "invalid number of arguments");
	if(!strcmp(argv[1], "-c"))
	{
		if(argc != 3)
			errx(1, "invalid number of arguments");
		cut(getInterval(argv[2]), 0);
		return 0;
	}
	if(!strcmp(argv[1], "-d"))
	{
		if(argc != 5 || strcmp(argv[3], "-f"))
			errx(1, "invalid arguments");
		cut(getInterval(argv[4]), *argv[2]);
		return 0;
	}
	err(2, "invalid argument");
}
