#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

int
main()
{
static char writebuf[40] = "Twiddle dee dee, Twiddle dum dum.......\n";
//	static char readbuf[41];

	const char *file;
	int fd, rv;
file = "testfile";

	printf("123456\n");

fd = open(file, O_WRONLY|O_CREAT|O_TRUNC, 0664);
	if (fd<0) {
		err(1, "%s: open for write %d", file, fd);
	}
rv = write(fd, writebuf, 40);
	if (rv<0) {
		err(1, "%s: write", file);
	}
	return 0;
}
