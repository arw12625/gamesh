#include <gamesh/con_queue.h>
#include <time.h>
#include <stdbool.h>

#define MSG_BUFFER_SIZE 100

typedef int (*gam_on_msg)(const void *args, size_t args_size);

typedef struct msg_agg_t {
	con_queue_t *_msg_queue;
} gam_msg_agg;

/**
 * @brief Initialize the message aggregator.
 *
 * Initialize the message aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_msg_agg_init(gam_msg_agg *agg);

/**
 * @brief Free the message aggregator.
 *
 * Free the message aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_msg_agg_free(gam_msg_agg *agg);

/**
 * @brief Update message aggregator to process messages.
 *
 * Update the message aggregator at @p agg to process messages.
 * Pops an array of MSG_BUFFER_SIZE messages from the queue,
 * and calls their on_msg function with the provided argument.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_msg_agg_on_update(const gam_msg_agg *agg);

// TODO - document macro in doxygen
/**
 * @brief Send a message using the aggregator.
 *
 * Use the aggregator at @p agg to send a message.
 * On sending, the @p on_msg function is called with arguments
 * @p args and @p args_size. This is done immediately if 
 * @p immediate is true, otherwise the message is placed on a queue.
 * This allows queuing of messages on one queue and sending on another.
 * @param agg Pointer to the aggregator.
 * @param immediate Whether or not to immediately send the message.
 * @param on_msg Pointer to function to call on sending.
 * @param args Pointer to arguments for @p on_msg.
 * @param args_size Size of arguments in @p args.
 * @return Returns 0 if successful, nonzero otherwise.
 */

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