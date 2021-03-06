#include <gamesh/con_queue.h>

#include <assert.h>
#include <pthread.h>
#include <stdio.h>


#define TEST_ARR_SIZE 1000

ConQueue q;
int queueVals[TEST_ARR_SIZE];

void *enqueue(void *ptr) {
	size_t i; 
	for(i = 0; i < TEST_ARR_SIZE; i++) {
		//printf("En: %d\n", i);
		con_queue_enqueue(&q, queueVals+i);
	}
	pthread_exit(NULL);
}

void *dequeue(void *ptr) {
	size_t i = 0;
	int x;
	while(i < TEST_ARR_SIZE) {
		//printf("De\n");
		int success = con_queue_dequeue(&q, &x);
		if(success == 0) {
			assert(x == queueVals[i]);
			//printf("De: %d\n", i);
			i++;
		}
	}
	pthread_exit(NULL);
}

int main(void) {
	int x = 0;
	int a = 1;
	int b = 2;
	int c = 3;

	con_queue_init(&q, sizeof(int));
	assert(con_queue_dequeue(&q, &x) != 0);
	assert(con_queue_enqueue(&q, &a) == 0);
	assert(con_queue_dequeue(&q, &x) == 0); assert(x == a);
	assert(con_queue_enqueue(&q, &a) == 0);
	assert(con_queue_enqueue(&q, &b) == 0);
	assert(con_queue_enqueue(&q, &c) == 0);
	assert(con_queue_get_size(&q) == 3);
	assert(!con_queue_is_empty(&q));
	assert(con_queue_dequeue(&q, &x) == 0); assert(x == a);
	assert(con_queue_dequeue(&q, &x) == 0); assert(x == b);
	assert(con_queue_dequeue(&q, &x) == 0); assert(x == c);
	assert(con_queue_dequeue(&q, &x) != 0);
	assert(con_queue_get_size(&q) == 0);
	assert(con_queue_is_empty(&q));

	size_t i;
	int d[5] = {1,2,3,4,5};
	size_t numD = 5;
	int buf[5];
	size_t numBuf;
	assert(con_queue_enqueue_array(&q, d, numD) == 0);
	assert(con_queue_get_size(&q) == numD);
	assert(con_queue_dequeue_array(&q, buf, &numBuf, numD) == 0);
	assert(con_queue_is_empty(&q));
	for(i = 0; i < numD; i++) {
		assert(d[i] == buf[i]);
	}

	for(i = 0; i < TEST_ARR_SIZE; i++) {
		queueVals[i] = i * i - 500;
	}

	pthread_t enq;
	pthread_t deq;
	pthread_create(&enq, NULL, enqueue, NULL);
	pthread_create(&deq, NULL, dequeue, NULL);

	pthread_join(enq, NULL);
	pthread_join(deq, NULL);

	assert(con_queue_free(&q) == 0);
	return 0;
}
