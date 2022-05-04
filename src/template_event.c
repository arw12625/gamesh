#include <gamesh/template_event.h>
#include <log.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// TODO - unify DEBUG info, i.e. set max file size for all files
#define TEMPLATE_EVENT_MAX_FILE_SIZE 128

int gam_template_event_format(const gam_template_event *e, char *buf, size_t buf_size
	#if DEBUG
				   , time_t msg_time, size_t msg_line, char *msg_file
	#endif
				  ) {
	char *format = "\nTemplate Event\n"
				"Value: %d\n"
				#if DEBUG
				"Originating\n"
				"\ttime: %s\n"
				"\tline: %u\n"
				"\tfile: %s\n"
				#endif
				"\n";
	#if DEBUG
	char time_buf[64];
	strftime(time_buf, 64, "%m/%d/%Y, %H:%M:%S", localtime(&msg_time));
	#endif
	snprintf(buf, buf_size, format, *e
				#if DEBUG
				, time_buf, msg_line, msg_file
				#endif
			);
	
	return 0;
}

int gam_template_event_agg_init(gam_template_event_agg *agg) {
	agg->_event_queue = malloc(sizeof(con_queue_t));
	if(agg->_event_queue == NULL) {
		return 1;
	}
	memset(agg->_consumers, 0, TEMPLATE_EVENT_MAX_NUM_CONSUMERS);
	agg->_num_consumers = 0;
	return con_queue_init(agg->_event_queue, sizeof(gam_template_event));
}

int gam_template_event_agg_free(gam_template_event_agg *agg) {
	return con_queue_free(agg->_event_queue);
}

int actual_event_dispatch(const gam_template_event_agg *agg, const gam_template_event *e) {
	int ret_val = 0;
	size_t i;
	for(i = 0; i < agg->_num_consumers && ret_val == 0; i++) {
		ret_val = agg->_consumers[i](e);
	}
	return ret_val;
}

int _gam_template_event_dispatch(const gam_template_event_agg *agg, bool immediate, const gam_template_event *e
#if DEBUG
				  , time_t msg_time, size_t msg_line, char *msg_file
#endif
				 ) {
	
	#if DEBUG
		char buf[256];
		gam_template_event_format(e, buf, 256, msg_time, msg_line, msg_file);
		log_info(buf);
	#endif
	
	if(immediate) {
		return actual_event_dispatch(agg, e);
	} else {
		return con_queue_enqueue(agg->_event_queue, e);
	}
}

int gam_template_event_agg_register(gam_template_event_agg *agg, gam_on_template_event on_template_event) {
	agg->_consumers[agg->_num_consumers++] = on_template_event;
	return 0;
}

int gam_template_event_agg_on_update(const gam_template_event_agg *agg) {
	size_t num_avail = 0;
	gam_template_event event_buf[TEMPLATE_EVENT_BUFFER_SIZE];
	if(con_queue_dequeue_array(agg->_event_queue, event_buf, &num_avail, TEMPLATE_EVENT_BUFFER_SIZE) != 0) {
		return 1;
	}
	size_t i;
	int ret_val = 0;
	for(i = 0; i < num_avail && ret_val == 0; i++) {
		ret_val = actual_event_dispatch(agg, event_buf+i);
	}
	return ret_val;
}
