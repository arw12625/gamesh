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

typedef struct ConQueueNode {
  void *data;
  struct ConQueueNode *next;
} ConQueueNode;

typedef struct ConQueue {
  size_t _dataSize;
  ConQueueNode *_head;
  ConQueueNode *_tail;
  pthread_mutex_t _headLock;
  pthread_mutex_t _tailLock;
} ConQueue;


#endif
/* LOCK_CON_QUEUE */

/* TODO - implement bounded queue with multiple readers/writers. See
https://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks-usage.pdf
https://pages.cs.wisc.edu/~remzi/OSTEP/threads-cv.pdf
https://github.com/remzi-arpacidusseau/ostep-code/blob/master/threads-cv/pc.c
*/

/**
 * @brief Initialize the concurrent queue.
 *
 * Initialize a concurrent queue at @p q with entries having constant size @p _dataSize.
 * @param q Pointer to the queue.
 * @param _dataSize Size of entries in the queue.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int con_queue_init(ConQueue *q, size_t _dataSize);

/**
 * @brief Push an entry to the queue.
 *
 * Push an entry to the tail of the queue @p q with data from @p data.
 * This locks others from pushing.
 * @param q Pointer to the queue.
 * @param data Pointer to data to be added.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int con_queue_enqueue(ConQueue *q, const void *data);

/**
 * @brief Pop an entry from the queue.
 *
 * Pop an entry from the head of the queue @p q and store result at @p data.
 * This locks others from popping.
 * @param q Pointer to the queue.
 * @param data Pointer to data to be popped.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int con_queue_dequeue(ConQueue *q, void *data);

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
int con_queue_enqueue_array(ConQueue *q, const void *data, size_t num_data);

/**
 * @brief Pop multiple entries from the queue into an array.
 *
 * Pop entries from the head of the queue @p q and store result at @p data.
 * The number of entries popped is the minimum of the total number of entries in the queue and @p maxNumData.
 * This locks others from popping.
 * @param q Pointer to the queue.
 * @param data Pointer to store the popped data.
 * @param num_data Pointer to store the number of entries popped at.
 * @param maxNumData The maximum number of entries to be popped.
 * @return Returns 0 if successful, nonzero otherwise.
 */
 int con_queue_dequeue_array(ConQueue *q, void *data, size_t *num_data, size_t maxNumData);

/**
 * @brief Free the queue from memory.
 *
 * Free the memory allocated to the queue @p q and destroy locks. 
 * @param q Pointer to the queue.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int con_queue_free(ConQueue *q);

/**
 * @brief Get the number of entries in the queue
 *
 * Count the number of entries in the queue @p q and return the result.
 * @param q Pointer to the queue.
 * @return Returns the number of entries.
 */
size_t con_queue_get_size(ConQueue *q);

/**
 * @brief Check if the queue is empty.
 *
 * Check if there are any entries in the queue @q
 * @param q Pointer to the queue.
 * @return Returns if the queue is empty.
 * @warning the queue being nonempty does not guarantee it will be for subsequent operations under multithreading
 */
bool con_queue_is_empty(ConQueue *q);

#endif
/* CON_QUEUE_H */