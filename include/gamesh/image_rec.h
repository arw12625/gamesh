#ifndef IMAGE_REC_H
#define IMAGE_REC_H

#include <gamesh/message.h>
#include <stdbool.h>

#define IMAGE_REC_MAX_NUM_IMAGES 100

typedef struct GamImageRec {
	char *file;
	unsigned char *img;
	int width;
	int height;
	int channels;
} GamImageRec;

typedef struct GamImageAgg {
	GamMsgAgg *_msgAgg;
	GamImageRec *_images[IMAGE_REC_MAX_NUM_IMAGES];
	size_t _numImages;
} GamImageAgg;

/**
 * @brief Initialize the image aggregator.
 *
 * Initialize the image aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_image_agg_init(GamImageAgg *agg);

/**
 * @brief Free the image aggregator.
 *
 * Free the image aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_image_agg_free(GamImageAgg *agg);

/**
 * @brief Update image aggregator to process image commands.
 *
 * Update the image aggregator at @p agg to process image commands.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_image_agg_update(const GamImageAgg *agg);

/**
 * @brief Load an image.
 *
 * Load an image to be stored in @p image_file on the aggregator @p agg.
 * @param agg Pointer to the aggregator.
 * @param image Pointer to the image.
 * @param iamge_file Image file name.
 * @param immediate Whether to load image immediately or add to queue.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_image_load(GamImageAgg *agg, GamImageRec *image, char *image_file, bool immediate);

/**
 * @brief Free an image.
 *
 * Free the memory allocated for the image @p image
 * @param agg Pointer to the aggregator.
 * @param image Pointer to the image.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_image_free(const GamImageAgg *agg, GamImageRec *image);		 
#endif
/* IMAGE_REC_H */