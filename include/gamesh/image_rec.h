#ifndef IMAGE_REC_H
#define IMAGE_REC_H

#include <gamesh/message.h>
#include <stdbool.h>

#define IMAGE_REC_MAX_NUM_IMAGES 100

typedef struct image_rec_t {
	char *file;
	unsigned char *img;
	int width;
	int height;
	int channels;
} gam_image_rec;

typedef struct image_agg_t {
	gam_msg_agg *_msg_agg;
	gam_image_rec *_images[IMAGE_REC_MAX_NUM_IMAGES];
	size_t _num_images;
} gam_image_agg;

/**
 * @brief Initialize the image aggregator.
 *
 * Initialize the image aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_image_agg_init(gam_image_agg *agg);

/**
 * @brief Free the image aggregator.
 *
 * Free the image aggregator at @p agg.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_image_agg_free(gam_image_agg *agg);

/**
 * @brief Update image aggregator to process image commands.
 *
 * Update the image aggregator at @p agg to process image commands.
 * @param agg Pointer to the aggregator.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_image_agg_on_update(const gam_image_agg *agg);

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
int gam_image_load(gam_image_agg *agg, gam_image_rec *image, char *image_file, bool immediate);

/**
 * @brief Free an image.
 *
 * Free the memory allocated for the image @p image
 * @param agg Pointer to the aggregator.
 * @param image Pointer to the image.
 * @return Returns 0 if successful, nonzero otherwise.
 */
int gam_image_free(const gam_image_agg *agg, gam_image_rec *image);		 
#endif
/* IMAGE_REC_H */