#include <stdio.h>

extern int __cdecl foo(char c);

int main(void) {
	printf("%d\n", foo(2));
	return 0;
}
