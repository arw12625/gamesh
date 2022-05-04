#include <gamesh/lib.h>

#include <stdio.h>

#include <log.h>




#include <pthread.h>

void *print_message_function( void *ptr );

void gam_init_log();
void gam_close_log();
void log_lock_func(bool lock, void *udata);

int main(void) {
	gam_init_log();
	
	int a = 1;
	int b = 2;
	int absum = test_add(a, b);
    log_info("%d + %d = %d\n", a, b, absum);
	
	
	pthread_t thread1, thread2;
    char *message1 = "Thread 1";
    char *message2 = "Thread 2";
    int  iret1, iret2;

    /* Create independent threads each of which will execute function */

    iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);
    iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2);

    /* Wait till threads are complete before main continues. Unless we  */
    /* wait we run the risk of executing an exit which will terminate   */
    /* the process and all threads before the threads have completed.   */

    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL); 

    log_info("Thread 1 returns: %d\n",iret1);
    log_info("Thread 2 returns: %d\n",iret2);
    
	
	gam_close_log();
	
	return 0;
}

static int x = 0;
void *print_message_function( void *ptr )
{
     char *message;
     message = (char *) ptr;
     log_info("%s \n", message);
     log_info("%d \n", ++x);
}

// Logging

static pthread_mutex_t gam_log_mutex;
static FILE *log_fp;
void gam_init_log(void) {
	if (pthread_mutex_init(&gam_log_mutex, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        exit(1);
    }
	log_set_lock(log_lock_func, &gam_log_mutex);

	log_fp = fopen("log.txt", "w+");
	log_add_fp(log_fp, 0);
}

void gam_close_log(void) {
    pthread_mutex_destroy(&gam_log_mutex);
	fclose(log_fp);
}

void log_lock_func(bool lock, void *udata) {
	if(lock) {
		pthread_mutex_lock(udata);
	} else {
		pthread_mutex_unlock(udata);
	}
}
