#include <gamesh/message.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MSG_MAX_FILE_SIZE 128

typedef struct GamMsg {
	GamOnMsg onMsg;
	void *args;
	size_t argsSize;
	#if DEBUG
	time_t msg_time;
	size_t msg_line;
	char msg_file[MSG_MAX_FILE_SIZE];
	#endif
	/* DEBUG */
} GamMsg;

int gam_msg_format(const GamMsg *m, char *buf, size_t bufSize);

int gam_msg_agg_init(GamMsgAgg *agg) {
	agg->_msgQueue = malloc(sizeof(ConQueue));
	if(agg->_msgQueue == NULL) {
		return 1;
	}
	return con_queue_init(agg->_msgQueue, sizeof(GamMsg));
}

int gam_msg_agg_free(GamMsgAgg *agg) {
	return con_queue_free(agg->_msgQueue);
}

int gam_msg_free(GamMsg *m) {
	//memset(m->args, 0, m->argsSize);
	free(m->args);
	//free(m);
	return 0;
}

int actual_msg_send(const GamMsgAgg *agg, GamMsg *m) {
	int ret_val = m->onMsg(m->args, m->argsSize);
	gam_msg_free(m);
	return ret_val;
}

int _gam_msg_send(const GamMsgAgg *agg, bool immediate, const GamOnMsg onMsg, const void *args, size_t argsSize
#if DEBUG
				  , time_t msg_time, size_t msg_line, char *msg_file
#endif
				 ) {
	//GamMsg *m = malloc(sizeof(GamMsg));
	GamMsg mm;
	GamMsg *m = &mm;
	if(m == NULL) {
		return 1;
	}
	m->onMsg = onMsg;
	m->argsSize = argsSize;
	if(argsSize == 0) {
		m->args = NULL;
	} else {
		m->args = malloc(argsSize);
		if(m->args == NULL) {
			return 1;
		}
		memcpy(m->args, args, argsSize);
	}
	#if DEBUG
	m->msg_time = msg_time;
	m->msg_line = msg_line;
	strncpy(m->msg_file, msg_file, MSG_MAX_FILE_SIZE);
	#endif
	
	if(immediate) {
		return actual_msg_send(agg, m);
	} else {
		return con_queue_enqueue(agg->_msgQueue, m);
	}
}

int gam_msg_agg_update(const GamMsgAgg *agg) {
	size_t num_avail = 0;
	GamMsg msg_buf[MSG_BUFFER_SIZE];
	if(con_queue_dequeue_array(agg->_msgQueue, msg_buf, &num_avail, MSG_BUFFER_SIZE) != 0) {
		return 1;
	}
	size_t i;
	int ret_val = 0;
	for(i = 0; i < num_avail && ret_val == 0; i++) {
		ret_val = actual_msg_send(agg, msg_buf+i);
	}
	return ret_val;
}

int gam_msg_format(const GamMsg *m, char *buf, size_t bufSize) {
	
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
	snprintf(buf, bufSize, format, m->onMsg
				#if DEBUG
				, time_buf, m->msg_line, m->msg_file
				#endif
			);
	
	return 0;
}