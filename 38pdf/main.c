#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <string.h>

void fillMap(char* buf, const char* set)
{
	for(int i = 0; set[i]; i++)
		buf[set[i]] = 1;
}

void fillMap2(char* buf, const char* set1, const char* set2)
{
	for(int i = 0; i < 256; i++)
		buf[i] = i;
	for(int i = 0; set1[i]; i++)
		buf[set1[i]] = set2[i];
}

int main(int argc, char** argv)
{
	if(argc < 3 || argc > 4)
		errx(1, "Usage: %s [OPTION] SET1 [SET2]", argv[0]);
	char map[256] = {0}, ch;
	ssize_t rd;
	if(!strcmp(argv[1], "-d"))
	{
		if(argc > 3)
			warnx("extra arguments ignored");
		fillMap(map, argv[2]);
		while((rd = read(STDIN_FILENO, &ch, 1)) == 1)
			if(!map[ch] && write(STDOUT_FILENO, &ch, 1) != 1)
				err(10, "write failed");
		if(rd == -1)
			err(10, "read failed");
		return 0;
	}
	if(!strcmp(argv[1], "-s"))
	{
		if(argc > 3)
			warnx("extra arguments ignored");
		fillMap(map, argv[2]);
		char prev = 0;
		while((rd = read(STDIN_FILENO, &ch, 1)) == 1)
		{
			if((ch != prev || !map[ch]) && write(STDOUT_FILENO, &ch, 1) != 1)
				err(10, "write failed");
			prev = ch;
		}
		if(rd == -1)
			err(10, "read failed");
		return 0;
	}
	if(argc > 3)
		warnx("extra argument ignored");
	if(strlen(argv[1]) != strlen(argv[2]))
		errx(2, "length(SET1) != length(SET2)");
	fillMap2(map, argv[1], argv[2]);
	while((rd = read(STDIN_FILENO, &ch, 1)) == 1)
		if(write(STDOUT_FILENO, &map[ch], 1) != 1)
			err(10, "write failed");
	if(rd == -1)
		err(10, "read failed");
	return 0;
}
