#include <gamesh/template_event.h>
#include <log.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// TODO - unify DEBUG info, i.e. set max file size for all files
#define TEMPLATE_EVENT_MAX_FILE_SIZE 128

int gam_template_event_format(const GamTemplateEvent *e, char *buf, size_t buf_size
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

int gam_template_event_agg_init(GamTemplateEventAgg *agg) {
	agg->_eventQueue = malloc(sizeof(ConQueue));
	if(agg->_eventQueue == NULL) {
		return 1;
	}
	memset(agg->_listeners, 0, TEMPLATE_EVENT_MAX_NUM_LISTENERS);
	agg->_numListeners = 0;
	return con_queue_init(agg->_eventQueue, sizeof(GamTemplateEvent));
}

int gam_template_event_agg_free(GamTemplateEventAgg *agg) {
	return con_queue_free(agg->_eventQueue);
}

int actual_event_dispatch(const GamTemplateEventAgg *agg, const GamTemplateEvent *e) {
	int ret_val = 0;
	size_t i;
	for(i = 0; i < agg->_numListeners && ret_val == 0; i++) {
		ret_val = agg->_listeners[i](e);
	}
	return ret_val;
}

int _gam_template_event_dispatch(const GamTemplateEventAgg *agg, bool immediate, const GamTemplateEvent *e
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
		return con_queue_enqueue(agg->_eventQueue, e);
	}
}

int gam_template_event_agg_add(GamTemplateEventAgg *agg, GamOnTemplateEvent on_template_event) {
	agg->_listeners[agg->_numListeners++] = on_template_event;
	return 0;
}

int gam_template_event_agg_update(const GamTemplateEventAgg *agg) {
	size_t numAvail = 0;
	GamTemplateEvent eventBuf[TEMPLATE_EVENT_BUFFER_SIZE];
	if(con_queue_dequeue_array(agg->_eventQueue, eventBuf, &numAvail, TEMPLATE_EVENT_BUFFER_SIZE) != 0) {
		return 1;
	}
	size_t i;
	int retVal = 0;
	for(i = 0; i < numAvail && retVal == 0; i++) {
		retVal = actual_event_dispatch(agg, eventBuf+i);
	}
	return retVal;
}
