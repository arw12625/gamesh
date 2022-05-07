#include <gamesh/image_rec.h>

#include <log.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <stdlib.h>
#include <string.h>

int gam_image_agg_init(GamImageAgg *agg) {
	agg->_msgAgg = malloc(sizeof(agg->_msgAgg));
	gam_msg_agg_init(agg->_msgAgg);
	memset(agg->_images, 0, IMAGE_REC_MAX_NUM_IMAGES);
	agg->_numImages = 0;
	if(agg->_msgAgg != NULL) {
		return 0;
	} else {
		return 1;
	}
}


int gam_image_agg_free(GamImageAgg *agg) {
	gam_msg_agg_free(agg->_msgAgg);
	free(agg->_msgAgg);
	return 0;
}

int gam_image_agg_update(const GamImageAgg *agg) {
	return gam_msg_agg_update(agg->_msgAgg);
}

struct GamImageContext {
	GamImageAgg *agg;
	GamImageRec *image;
};

int actual_image_load(GamImageAgg *agg, GamImageRec *image) {
    image->img = stbi_load(image->file, &image->width, &image->height, &image->channels, 0);
    if(image->img == NULL) {
        log_error("Error in loading the image: %s\n", image->file);
        return 1;
    }
	agg->_images[agg->_numImages++] = image;
	return 0;
}

int actual_image_load_wrapper(void *args, size_t args_size) {
	if(args_size != sizeof(struct GamImageContext)) {
		return 1;
	}
	struct GamImageContext context = *(struct GamImageContext*)args;
	return actual_image_load(context.agg, context.image);
}

int gam_image_load(GamImageAgg *agg, GamImageRec *image, char *imageFile, bool immediate) {
	image->file = malloc(strlen(imageFile) + 1);
	if(image->file == NULL) {
		return 1;
	}
	strcpy(image->file, imageFile);
	
	struct GamImageContext context;
	context.agg = agg;
	context.image = image;
	return gam_msg_send(agg->_msgAgg, immediate, actual_image_load_wrapper, &context, sizeof(context));
}


int gam_image_free(const GamImageAgg *agg, GamImageRec *image) {
	// TODO - should remove from aggregator?
	stbi_image_free(image->img);
	free(image->file);
	return 0;
}
