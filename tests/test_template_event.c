#include <gamesh/template_event.h>

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

GamTemplateEventAgg agg;
int testVal = 0;
const int numSum = 100;
bool done = false;

int test_on_template_event(const GamTemplateEvent *e) {
	testVal += *e;
	return 0;
}

void *read(void *ptr) {
	while(!done) {
		assert(gam_template_event_agg_update(&agg) == 0);
	}
	assert(gam_template_event_agg_update(&agg) == 0);
	pthread_exit(NULL);
}

void *write(void *ptr) {
	int i = 0;
	GamTemplateEvent e;
	for(i = 0; i < numSum; i++) {
		e = i;
		assert(gam_template_event_dispatch(&agg, false, &e) == 0);
	}
	done = true;
	pthread_exit(NULL);
}

int main() {
	GamTemplateEvent testArg = 1;
	
	assert(gam_template_event_agg_init(&agg) == 0);
		
	assert(gam_template_event_dispatch(&agg, false, &testArg) == 0);
	assert(gam_template_event_agg_update(&agg) == 0);
	assert(testVal == 0);
	
	assert(gam_template_event_agg_add(&agg, test_on_template_event) == 0);
	
	assert(gam_template_event_dispatch(&agg, false, &testArg) == 0);
	assert(testVal == 0);
	assert(gam_template_event_agg_update(&agg) == 0);
	assert(testVal == 1);
	
	assert(gam_template_event_dispatch(&agg, true, &testArg) == 0);
	assert(testVal == 2);
	assert(gam_template_event_agg_update(&agg) == 0);
	assert(testVal == 2);
	
	testVal = 0;
	pthread_t w;
	pthread_t r;
	pthread_create(&w, NULL, write, NULL);
	pthread_create(&r, NULL, read, NULL);

	pthread_join(w, NULL);
	pthread_join(r, NULL);
	assert(testVal == numSum * (numSum - 1) / 2);
	
	assert(gam_template_event_agg_free(&agg) == 0);
	return 0;
}
