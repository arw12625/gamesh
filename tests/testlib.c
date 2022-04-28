#define CATCH_CONFIG_MAIN
#include <assert.h>
#include <gamesh/lib.h>

int main() {
	assert(1 == test_add(2, -1) && "Adding failed");
	return 0;
}
