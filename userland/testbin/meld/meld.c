#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

int
main()
{
static char writebuf[40] = "Twiddle dee dee, Twiddle dum dum.......\n";
	static char readbuf[41];

	const char *file;
	int fd, rv, x = 0;
for(int i = 0; i < 100000000000; i++){
x +=100000;
}
	if (argc == 0) {
		warnx("No arguments - running on \"testfile\"");
		file = "testfile";
	}
	else if (argc == 2) {
		file = argv[1];
	}
	else {
		errx(1, "Usage: filetest <filename>");
	}

	fd = open(file, O_WRONLY|O_CREAT|O_TRUNC, 0664);
	if (fd<0) {
		err(1, "%s: open for write", file);
	}
	printf("test");
}
