#include <gamesh/con_queue.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// concurrent queue implementation with one lock for queueing and one for dequeueing
#ifdef LOCK_CON_QUEUE


int con_queue_init(ConQueue *q, size_t _dataSize) {
  if(_dataSize <= 0) {
	return 1;  
  }
	
  ConQueueNode *dummy = malloc(sizeof(ConQueueNode));
  if(!dummy) {
	  return 1;
  }
  dummy->data = NULL;
  dummy->next = NULL;

  q->_dataSize = _dataSize;
  q->_head = dummy;
  q->_tail = dummy;
  pthread_mutex_init(&q->_headLock, NULL);
  pthread_mutex_init(&q->_tailLock, NULL);
  
  return 0;
}

int con_queue_enqueue_array(ConQueue *q, const void *data, size_t num_data) {
  
  if(num_data <= 0) {
	  return 1;
  }
  
  ConQueueNode *new_tail;
  ConQueueNode *last_node = NULL;
  ConQueueNode *new_node;
  size_t i;
  for(i = num_data-1; i < num_data; i--) {
	  ConQueueNode *new_node = malloc(sizeof(ConQueueNode));
	  if(!new_node) {
		  return 1;
	  }
	  if(i == num_data-1) {
		  new_tail = new_node;
	  }
	  new_node->data = malloc(q->_dataSize);
	  if(!new_node) {
		  memset(new_node, 0, sizeof(ConQueueNode));
		  free(new_node);
		  return 1;
	  }
	  memcpy(new_node->data, data + (i * q->_dataSize), q->_dataSize);
	  
	  new_node->next = last_node;
	  last_node = new_node;
  }
  
  pthread_mutex_lock(&q->_tailLock);
  q->_tail->next = last_node;
  q->_tail = new_tail;
  pthread_mutex_unlock(&q->_tailLock);
  return 0;
}


int con_queue_dequeue_array(ConQueue *q, void *data, size_t *num_data, size_t maxNumData) {
	if(maxNumData <= 0) {
		*num_data = 0;
		return 1;
	}
  size_t i;
  pthread_mutex_lock(&q->_headLock);
  ConQueueNode *last_node = q->_head;
  for(i = 0; i < maxNumData; i++) {
	  ConQueueNode *next_node = last_node->next;
	  if (next_node == NULL) {
		//memset(data, 0, q->_dataSize);
		break;
	  }
	  memcpy(data + (i * q->_dataSize), next_node->data, q->_dataSize);
	  free(next_node->data);
	  free(last_node);
	  last_node = next_node;
  }
  q->_head = last_node;
  pthread_mutex_unlock(&q->_headLock);
  
  *num_data = i;
  return 0;
}


int con_queue_enqueue(ConQueue *q, const void *data) {
  ConQueueNode *new_node = malloc(sizeof(ConQueueNode));
  if(!new_node) {
	  return 1;
  }
  new_node->data = malloc(q->_dataSize);
  if(!new_node) {
	  memset(new_node, 0, sizeof(ConQueueNode));
	  free(new_node);
	  return 1;
  }
  memcpy(new_node->data, data, q->_dataSize);
  new_node->next = NULL;

  pthread_mutex_lock(&q->_tailLock);
  q->_tail->next = new_node;
  q->_tail = new_node;
  pthread_mutex_unlock(&q->_tailLock);
  return 0;
}

int con_queue_dequeue(ConQueue *q, void *data) {
  pthread_mutex_lock(&q->_headLock);
  ConQueueNode *new_head = q->_head->next;
  if (new_head == NULL) {
    pthread_mutex_unlock(&q->_headLock);
	//memset(data, 0, q->_dataSize);
    return 1;
  }
  ConQueueNode *old_head = q->_head;
  q->_head = new_head;
  memcpy(data, new_head->data, q->_dataSize);
  pthread_mutex_unlock(&q->_headLock);
  
  free(new_head->data);
  free(old_head);
  return 0;
}

int con_queue_free(ConQueue *q) {
	ConQueueNode *tmp;
	tmp = q->_head;
	q->_head = tmp->next;
	free(tmp);
	while(q->_head != NULL) {
		tmp = q->_head;
		q->_head = tmp->next;
		free(tmp->data);
		free(tmp);
	}

	pthread_mutex_destroy(&q->_headLock);
	pthread_mutex_destroy(&q->_tailLock);
	
	return 0;
	
}

size_t con_queue_get_size(ConQueue *q) {
	
	pthread_mutex_lock(&q->_headLock);
	size_t count = 0;
	ConQueueNode *tmp = q->_head;
    while(tmp->next != NULL) {
		tmp = tmp->next;
		count++;
	}
	pthread_mutex_unlock(&q->_headLock);
	return count;
}

bool con_queue_is_empty(ConQueue *q) {
	
	pthread_mutex_lock(&q->_headLock);
	bool is_empty = (q->_head->next == NULL);
	pthread_mutex_unlock(&q->_headLock);
	return is_empty;
}


#endif
/* LOCK_CON_QUEUE */