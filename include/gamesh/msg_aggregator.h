#include <gamesh/con_queue.h>
#include <time.h>
#include <stdbool.h>

typedef int (*gam_on_msg)(const void *args, size_t args_size);

typedef struct msg_agg_t {
	con_queue_t *_msg_queue;
} gam_msg_agg;


int gam_msg_agg_init(gam_msg_agg *agg);

int gam_msg_agg_free(gam_msg_agg *agg);

int gam_msg_agg_on_update(const gam_msg_agg *agg);

#if DEBUG
#define gam_msg_send(agg, immediate, on_msg, args, args_size) \
		(_gam_msg_send(agg, immediate, on_msg, args, args_size, \
					   time(NULL), __LINE__, __FILE__))
#else
#define gam_msg_send(agg, immediate, on_msg, args, args_size) \
		(_gam_msg_send(agg, immediate, on_msg, args, args_size))
#endif

int _gam_msg_send(const gam_msg_agg *agg, bool immediate, const gam_on_msg on_msg, const void *args, size_t args_size
#if DEBUG
				  , time_t msg_time, size_t msg_line, char *msg_file
#endif
				 );