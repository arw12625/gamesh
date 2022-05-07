#ifndef TEMPLATE_EVENT_H
#define TEMPLATE_EVENT_H

#include <gamesh/con_queue.h>
#include <time.h>
#include <stdbool.h>

#define TEMPLATE_EVENT_BUFFER_SIZE 100
#define TEMPLATE_EVENT_MAX_NUM_LISTENERS 100

typedef int GamTemplateEvent;

typedef int (*GamOnTemplateEvent)(const GamTemplateEvent *e);

typedef struct GamTemplateEventAgg {
	ConQueue *_eventQueue;
	GamOnTemplateEvent _listeners[TEMPLATE_EVENT_MAX_NUM_LISTENERS];
	size_t _numListeners;
} GamTemplateEventAgg;

/**
 * @brief Initialize the event aggregator.
 *
 * Initialize the event aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_template_event_agg_init(GamTemplateEventAgg *agg);

/**
 * @brief Free the event aggregator.
 *
 * Free the event aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_template_event_agg_free(GamTemplateEventAgg *agg);

/**
 * @brief Update event aggregator to process events.
 *
 * Update the event aggregator at @p agg to process events.
 * Pops an array of TEMPLATE_EVENT_BUFFER_SIZE events from the queue,
 * and calls their on_template_event function with the provided argument.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_template_event_agg_update(const GamTemplateEventAgg *agg);

/**
 * @brief Add a consumer to the event aggregator.
 *
 * Add a consumer @p on_template_event to the event aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @param on_template_event Pointer to the consumer as a function.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_template_event_agg_add(GamTemplateEventAgg *agg, GamOnTemplateEvent on_template_event);

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
#define gam_template_event_dispatch(agg, immediate, event) \
		(_gam_template_event_dispatch(agg, immediate, event, \
					   time(NULL), __LINE__, __FILE__))
#else
#define gam_template_event_dispatch(agg, immediate, event) \
		(_gam_template_event_dispatch(agg, immediate, event))
#endif

int _gam_template_event_dispatch(const GamTemplateEventAgg *agg, bool immediate, const GamTemplateEvent *event
#if DEBUG
				  , time_t msg_time, size_t msg_line, char *msg_file
#endif
				 );
				 
#endif
/* TEMPLATE_EVENT_H */