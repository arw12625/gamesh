#include <gamesh/lib.h>
#include <stdio.h>


int main() {
	int a = 1;
	int b = 2;
	int absum = test_add(a, b);
    printf("%d + %d = %d\n", a, b, absum);

    return 0;
}
