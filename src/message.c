#include <gamesh/message.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MSG_MAX_FILE_SIZE 128

typedef struct msg_t_ {
	gam_on_msg on_msg;
	void *args;
	size_t args_size;
	#if DEBUG
	time_t msg_time;
	size_t msg_line;
	char msg_file[MSG_MAX_FILE_SIZE];
	#endif
	/* DEBUG */
} gam_msg;

int gam_msg_format(const gam_msg *m, char *buf, size_t buf_size);

int gam_msg_agg_init(gam_msg_agg *agg) {
	agg->_msg_queue = malloc(sizeof(con_queue_t));
	if(agg->_msg_queue == NULL) {
		return 1;
	}
	return con_queue_init(agg->_msg_queue, sizeof(gam_msg));
}

int gam_msg_agg_free(gam_msg_agg *agg) {
	return con_queue_free(agg->_msg_queue);
}

int gam_msg_free(gam_msg *m) {
	//memset(m->args, 0, m->args_size);
	free(m->args);
	//free(m);
	return 0;
}

int actual_msg_send(const gam_msg_agg *agg, gam_msg *m) {
	int ret_val = m->on_msg(m->args, m->args_size);
	gam_msg_free(m);
	return ret_val;
}

int _gam_msg_send(const gam_msg_agg *agg, bool immediate, const gam_on_msg on_msg, const void *args, size_t args_size
#if DEBUG
				  , time_t msg_time, size_t msg_line, char *msg_file
#endif
				 ) {
	//gam_msg *m = malloc(sizeof(gam_msg));
	gam_msg mm;
	gam_msg *m = &mm;
	if(m == NULL) {
		return 1;
	}
	m->on_msg = on_msg;
	m->args_size = args_size;
	if(args_size == 0) {
		m->args = NULL;
	} else {
		m->args = malloc(args_size);
		if(m->args == NULL) {
			return 1;
		}
		memcpy(m->args, args, args_size);
	}
	#if DEBUG
	m->msg_time = msg_time;
	m->msg_line = msg_line;
	strncpy(m->msg_file, msg_file, MSG_MAX_FILE_SIZE);
	#endif
	
	if(immediate) {
		return actual_msg_send(agg, m);
	} else {
		return con_queue_enqueue(agg->_msg_queue, m);
	}
}

int gam_msg_agg_on_update(const gam_msg_agg *agg) {
	size_t num_avail = 0;
	gam_msg msg_buf[MSG_BUFFER_SIZE];
	if(con_queue_dequeue_array(agg->_msg_queue, msg_buf, &num_avail, MSG_BUFFER_SIZE) != 0) {
		return 1;
	}
	size_t i;
	int ret_val = 0;
	for(i = 0; i < num_avail && ret_val == 0; i++) {
		ret_val = actual_msg_send(agg, msg_buf+i);
	}
	return ret_val;
}

int gam_msg_format(const gam_msg *m, char *buf, size_t buf_size) {
	
	char *format = "\nGame message\n"
				"On message: 0x%p\n"
				#if DEBUG
				"Originating\n"
				"\ttime: %s\n"
				"\tline: %u\n"
				"\tfile: %s\n"
				#endif
				"\n";
	#if DEBUG
	char time_buf[64];
	strftime(time_buf, 64, "%m/%d/%Y, %H:%M:%S", localtime(&m->msg_time));
	#endif
	snprintf(buf, buf_size, format, m->on_msg
				#if DEBUG
				, time_buf, m->msg_line, m->msg_file
				#endif
			);
	
	return 0;
}