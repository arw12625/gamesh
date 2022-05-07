#ifndef MESSAGE_H
#define MESSAGE_H

#include <gamesh/con_queue.h>
#include <time.h>
#include <stdbool.h>

#define MSG_BUFFER_SIZE 100

typedef int (*GamOnMsg)(void *args, size_t argsSize);

typedef struct msg_agg_t {
	ConQueue *_msgQueue;
} GamMsgAgg;

/**
 * @brief Initialize the message aggregator.
 *
 * Initialize the message aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_msg_agg_init(GamMsgAgg *agg);

/**
 * @brief Free the message aggregator.
 *
 * Free the message aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_msg_agg_free(GamMsgAgg *agg);

/**
 * @brief Update message aggregator to process messages.
 *
 * Update the message aggregator at @p agg to process messages.
 * Pops an array of MSG_BUFFER_SIZE messages from the queue,
 * and calls their onMsg function with the provided argument.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_msg_agg_update(const GamMsgAgg *agg);

// TODO - document macro in doxygen
/**
 * @brief Send a message using the aggregator.
 *
 * Use the aggregator at @p agg to send a message.
 * On sending, the @p onMsg function is called with arguments
 * @p args and @p argsSize. This is done immediately if 
 * @p immediate is true, otherwise the message is placed on a queue.
 * This allows queuing of messages on one queue and sending on another.
 * @param agg Pointer to the aggregator.
 * @param immediate Whether or not to immediately send the message.
 * @param onMsg Pointer to function to call on sending.
 * @param args Pointer to arguments for @p onMsg.
 * @param argsSize Size of arguments in @p args.
 * @return Returns 0 if successful, nonzero otherwise.
 */

#if DEBUG
#define gam_msg_send(agg, immediate, onMsg, args, argsSize) \
		(_gam_msg_send(agg, immediate, onMsg, args, argsSize, \
					   time(NULL), __LINE__, __FILE__))
#else
#define gam_msg_send(agg, immediate, onMsg, args, argsSize) \
		(_gam_msg_send(agg, immediate, onMsg, args, argsSize))
#endif

int _gam_msg_send(const GamMsgAgg *agg, bool immediate, const GamOnMsg onMsg, const void *args, size_t argsSize
#if DEBUG
				  , time_t msg_time, size_t msg_line, char *msg_file
#endif
				 );
				 
#endif
/* HEADER_H */