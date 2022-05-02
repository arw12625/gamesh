#ifndef CON_QUEUE_H
#define CON_QUEUE_H

#include <pthread.h>
#include <stddef.h>
#include <stdbool.h>

#define LOCK_CON_QUEUE

#ifdef LOCK_CON_QUEUE


// To implement message and event queues
// Messages: describe task to accomplish, usually known destination
// Events: describe things that have happened, usually unknown destination, possibly multiple

typedef struct __node_t {
  void *data;
  struct __node_t *next;
} con_queue_node_t;

typedef struct __queue_t {
  size_t data_size;
  con_queue_node_t *_head;
  con_queue_node_t *_tail;
  pthread_mutex_t _head_lock;
  pthread_mutex_t _tail_lock;
} con_queue_t;


#endif /* LOCK_CON_QUEUE */

/* TODO - implement bounded queue with multiple readers/writers. See
https://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks-usage.pdf
https://pages.cs.wisc.edu/~remzi/OSTEP/threads-cv.pdf
https://github.com/remzi-arpacidusseau/ostep-code/blob/master/threads-cv/pc.c
*/

/**
 * @brief Initialize the concurrent queue.
 *
 * Initialize a concurrent queue at @p q with entries having constant size @p data_size.
 * @param q Pointer to the queue.
 * @param data_size Size of entries in the queue.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int con_queue_init(con_queue_t *q, size_t data_size);

/**
 * @brief Push an entry to the queue.
 *
 * Push an entry to the tail of the queue @p q with data from @p data.
 * This locks others from pushing.
 * @param q Pointer to the queue.
 * @param data Pointer to data to be added.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int con_queue_enqueue(con_queue_t *q, const void *data);

/**
 * @brief Pop an entry from the queue.
 *
 * Pop an entry from the head of the queue @p q and store result at @p data.
 * This locks others from popping.
 * @param q Pointer to the queue.
 * @param data Pointer to data to be popped.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int con_queue_dequeue(con_queue_t *q, void *data);

/**
 * @brief Push an array of entries to the queue.
 *
 * Push an array of entries @p data to the tail of the queue @p q
 * This locks others from pushing.
 * @param q Pointer to the queue.
 * @param data Pointer to data to be added.
 * @param num_data Number of entries to add.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int con_queue_enqueue_array(con_queue_t *q, const void *data, size_t num_data);

/**
 * @brief Pop multiple entries from the queue into an array.
 *
 * Pop entries from the head of the queue @p q and store result at @p data.
 * The number of entries popped is the minimum of the total number of entries in the queue and @p max_num_data.
 * This locks others from popping.
 * @param q Pointer to the queue.
 * @param data Pointer to store the popped data.
 * @param num_data Pointer to store the number of entries popped at.
 * @param max_num_data The maximum number of entries to be popped.
 * @return Returns 0 if successful, nonzero otherwise.
 */
 int con_queue_dequeue_array(con_queue_t *q, void *data, size_t *num_data, size_t max_num_data);

/**
 * @brief Free the queue from memory.
 *
 * Free the memory allocated to the queue @p q and destroy locks. 
 * @param q Pointer to the queue.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int con_queue_free(con_queue_t *q);

/**
 * @brief Get the number of entries in the queue
 *
 * Count the number of entries in the queue @p q and return the result.
 * @param q Pointer to the queue.
 * @return Returns the number of entries.
 */
size_t con_queue_get_size(con_queue_t *q);

/**
 * @brief Check if the queue is empty.
 *
 * Check if there are any entries in the queue @q
 * @param q Pointer to the queue.
 * @return Returns if the queue is empty.
 * @warning the queue being nonempty does not guarantee it will be for subsequent operations under multithreading
 */
bool con_queue_is_empty(con_queue_t *q);

#endif
/* CON_QUEUE_H */