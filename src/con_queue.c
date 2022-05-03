#include <gamesh/con_queue.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// concurrent queue implementation with one lock for queueing and one for dequeueing
#ifdef LOCK_CON_QUEUE


int con_queue_init(con_queue_t *q, size_t data_size) {
  if(data_size <= 0) {
	return 1;  
  }
	
  con_queue_node_t *dummy = malloc(sizeof(con_queue_node_t));
  if(!dummy) {
	  return 1;
  }
  dummy->data = NULL;
  dummy->next = NULL;

  q->data_size = data_size;
  q->_head = dummy;
  q->_tail = dummy;
  pthread_mutex_init(&q->_head_lock, NULL);
  pthread_mutex_init(&q->_tail_lock, NULL);
  
  return 0;
}

int con_queue_enqueue_array(con_queue_t *q, const void *data, size_t num_data) {
  
  if(num_data <= 0) {
	  return 1;
  }
  
  con_queue_node_t *new_tail;
  con_queue_node_t *last_node = NULL;
  con_queue_node_t *new_node;
  size_t i;
  for(i = num_data-1; i < num_data; i--) {
	  con_queue_node_t *new_node = malloc(sizeof(con_queue_node_t));
	  if(!new_node) {
		  return 1;
	  }
	  if(i == num_data-1) {
		  new_tail = new_node;
	  }
	  new_node->data = malloc(q->data_size);
	  if(!new_node) {
		  memset(new_node, 0, sizeof(con_queue_node_t));
		  free(new_node);
		  return 1;
	  }
	  memcpy(new_node->data, data + (i * q->data_size), q->data_size);
	  
	  new_node->next = last_node;
	  last_node = new_node;
  }
  
  pthread_mutex_lock(&q->_tail_lock);
  q->_tail->next = last_node;
  q->_tail = new_tail;
  pthread_mutex_unlock(&q->_tail_lock);
  return 0;
}


int con_queue_dequeue_array(con_queue_t *q, void *data, size_t *num_data, size_t max_num_data) {
	if(max_num_data <= 0) {
		*num_data = 0;
		return 1;
	}
  size_t i;
  pthread_mutex_lock(&q->_head_lock);
  con_queue_node_t *last_node = q->_head;
  for(i = 0; i < max_num_data; i++) {
	  con_queue_node_t *next_node = last_node->next;
	  if (next_node == NULL) {
		//memset(data, 0, q->data_size);
		break;
	  }
	  memcpy(data + (i * q->data_size), next_node->data, q->data_size);
	  free(next_node->data);
	  free(last_node);
	  last_node = next_node;
  }
  q->_head = last_node;
  pthread_mutex_unlock(&q->_head_lock);
  
  *num_data = i;
  return 0;
}


int con_queue_enqueue(con_queue_t *q, const void *data) {
  con_queue_node_t *new_node = malloc(sizeof(con_queue_node_t));
  if(!new_node) {
	  return 1;
  }
  new_node->data = malloc(q->data_size);
  if(!new_node) {
	  memset(new_node, 0, sizeof(con_queue_node_t));
	  free(new_node);
	  return 1;
  }
  memcpy(new_node->data, data, q->data_size);
  new_node->next = NULL;

  pthread_mutex_lock(&q->_tail_lock);
  q->_tail->next = new_node;
  q->_tail = new_node;
  pthread_mutex_unlock(&q->_tail_lock);
  return 0;
}

int con_queue_dequeue(con_queue_t *q, void *data) {
  pthread_mutex_lock(&q->_head_lock);
  con_queue_node_t *new_head = q->_head->next;
  if (new_head == NULL) {
    pthread_mutex_unlock(&q->_head_lock);
	//memset(data, 0, q->data_size);
    return 1;
  }
  con_queue_node_t *old_head = q->_head;
  q->_head = new_head;
  memcpy(data, new_head->data, q->data_size);
  pthread_mutex_unlock(&q->_head_lock);
  
  free(new_head->data);
  free(old_head);
  return 0;
}

int con_queue_free(con_queue_t *q) {
	con_queue_node_t *tmp;
	
	while(q->_head != NULL) {
		tmp = q->_head;
		q->_head = tmp->next;
		free(tmp->data);
		free(tmp);
	}

	pthread_mutex_destroy(q->_head_lock);
	pthread_mutex_destroy(q->_tail_lock);
	
	return 0;
	
}

size_t con_queue_get_size(con_queue_t *q) {
	
	pthread_mutex_lock(&q->_head_lock);
	size_t count = 0;
	con_queue_node_t *tmp = q->_head;
    while(tmp->next != NULL) {
		tmp = tmp->next;
		count++;
	}
	pthread_mutex_unlock(&q->_head_lock);
	return count;
}

bool con_queue_is_empty(con_queue_t *q) {
	
	pthread_mutex_lock(&q->_head_lock);
	bool is_empty = (q->_head->next == NULL);
	pthread_mutex_unlock(&q->_head_lock);
	return is_empty;
}


#endif
/* LOCK_CON_QUEUE */