#include <gamesh/message.h>

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

GamMsgAgg agg;
int testVal = 0;
const int numSum = 100;
bool done = false;

int test_on_msg(void *args, size_t argsSize) {
	assert(argsSize == sizeof(int));
	testVal += *((int *) args);
	return 0;
}

void *read(void *ptr) {
	while(!done) {
		assert(gam_msg_agg_update(&agg) == 0);
	}
	assert(gam_msg_agg_update(&agg) == 0);
	pthread_exit(NULL);
}

void *write(void *ptr) {
	int i = 0;
	for(i = 0; i < numSum; i++) {
		assert(gam_msg_send(&agg, false, test_on_msg, &i, sizeof(int)) == 0);
	}
	done = true;
	pthread_exit(NULL);
}

int main() {
	assert(gam_msg_agg_init(&agg) == 0);
	
	int testArg = 1;
	gam_msg_send(&agg, false, test_on_msg, &testArg, sizeof(int));
	assert(testVal == 0);
	gam_msg_agg_update(&agg);
	assert(testVal == 1);
	
	gam_msg_send(&agg, true, test_on_msg, &testArg, sizeof(int));
	assert(testVal == 2);
	gam_msg_agg_update(&agg);
	assert(testVal == 2);
	
	testVal = 0;
	pthread_t w;
	pthread_t r;
	pthread_create(&w, NULL, write, NULL);
	pthread_create(&r, NULL, read, NULL);

	pthread_join(w, NULL);
	pthread_join(r, NULL);
	assert(testVal == numSum * (numSum - 1) / 2);
	
	gam_msg_agg_free(&agg);
	return 0;
}
