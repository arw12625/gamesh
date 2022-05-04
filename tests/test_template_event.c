#include <gamesh/template_event.h>

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

gam_template_event_agg agg;
int test_val = 0;
const int num_sum = 100;
bool done = false;

int test_on_template_event(const gam_template_event *e) {
	test_val += *e;
	return 0;
}

void *read(void *ptr) {
	while(!done) {
		assert(gam_template_event_agg_on_update(&agg) == 0);
	}
	assert(gam_template_event_agg_on_update(&agg) == 0);
	pthread_exit(NULL);
}

void *write(void *ptr) {
	int i = 0;
	gam_template_event e;
	for(i = 0; i < num_sum; i++) {
		e = i;
		assert(gam_template_event_dispatch(&agg, false, &e) == 0);
	}
	done = true;
	pthread_exit(NULL);
}

int main() {
	gam_template_event test_arg = 1;
	
	assert(gam_template_event_agg_init(&agg) == 0);
		
	assert(gam_template_event_dispatch(&agg, false, &test_arg) == 0);
	assert(gam_template_event_agg_on_update(&agg) == 0);
	assert(test_val == 0);
	
	assert(gam_template_event_agg_register(&agg, test_on_template_event) == 0);
	
	assert(gam_template_event_dispatch(&agg, false, &test_arg) == 0);
	assert(test_val == 0);
	assert(gam_template_event_agg_on_update(&agg) == 0);
	assert(test_val == 1);
	
	assert(gam_template_event_dispatch(&agg, true, &test_arg) == 0);
	assert(test_val == 2);
	assert(gam_template_event_agg_on_update(&agg) == 0);
	assert(test_val == 2);
	
	test_val = 0;
	pthread_t w;
	pthread_t r;
	pthread_create(&w, NULL, write, NULL);
	pthread_create(&r, NULL, read, NULL);

	pthread_join(w, NULL);
	pthread_join(r, NULL);
	assert(test_val == num_sum * (num_sum - 1) / 2);
	
	assert(gam_template_event_agg_free(&agg) == 0);
	return 0;
}
