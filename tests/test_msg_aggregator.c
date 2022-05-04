#include <gamesh/msg_aggregator.h>

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

gam_msg_agg agg;
int test_val = 0;
const int num_sum = 100;
bool done = false;

int test_on_msg(const void *args, size_t args_size) {
	assert(args_size == sizeof(int));
	test_val += *((int *) args);
	//test_val += (int)(*args);
	return 0;
}

void *read(void *ptr) {
	while(!done) {
		assert(gam_msg_agg_on_update(&agg) == 0);
	}
	assert(gam_msg_agg_on_update(&agg) == 0);
	pthread_exit(NULL);
}

void *write(void *ptr) {
	int i = 0;
	for(i = 0; i < num_sum; i++) {
		assert(gam_msg_send(&agg, false, test_on_msg, &i, sizeof(int)) == 0);
	}
	done = true;
	pthread_exit(NULL);
}

int main() {
	gam_msg_agg_init(&agg);
	int test_arg = 1;
	_gam_msg_send(&agg, false, test_on_msg, &test_arg, sizeof(int),
				 time(NULL), __LINE__, __FILE__);
	assert(test_val == 0);
	gam_msg_agg_on_update(&agg);
	assert(test_val == 1);
	
	gam_msg_send(&agg, true, test_on_msg, &test_arg, sizeof(int));
	assert(test_val == 2);
	gam_msg_agg_on_update(&agg);
	assert(test_val == 2);
	
	test_val = 0;
	pthread_t w;
	pthread_t r;
	pthread_create(&w, NULL, write, NULL);
	pthread_create(&r, NULL, read, NULL);

	pthread_join(w, NULL);
	pthread_join(r, NULL);
	assert(test_val == num_sum * (num_sum - 1) / 2);
	
	gam_msg_agg_free(&agg);
	return 0;
}
