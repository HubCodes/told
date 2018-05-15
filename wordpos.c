#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
	int fd = open(argv[1], O_RDONLY);
	char buf[1024];
	read(fd, buf, 1024);
	int pos = atoi(argv[2]);
	printf("%c\n", buf[pos]);
	return 0;
}
