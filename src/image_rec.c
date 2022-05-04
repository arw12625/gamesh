#include <gamesh/image_rec.h>

#include <log.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <stdlib.h>
#include <string.h>

int gam_image_agg_init(gam_image_agg *agg) {
	agg->_msg_agg = malloc(sizeof(agg->_msg_agg));
	gam_msg_agg_init(agg->_msg_agg);
	memset(agg->_images, 0, IMAGE_REC_MAX_NUM_IMAGES);
	agg->_num_images = 0;
	if(agg->_msg_agg != NULL) {
		return 0;
	} else {
		return 1;
	}
}


int gam_image_agg_free(gam_image_agg *agg) {
	gam_msg_agg_free(agg->_msg_agg);
	free(agg->_msg_agg);
	return 0;
}

int gam_image_agg_on_update(const gam_image_agg *agg) {
	return gam_msg_agg_on_update(agg->_msg_agg);
}

struct image_context_t {
	gam_image_agg *agg;
	gam_image_rec *image;
};

int actual_image_load(gam_image_agg *agg, gam_image_rec *image) {
    image->img = stbi_load(image->file, &image->width, &image->height, &image->channels, 0);
    if(image->img == NULL) {
        log_error("Error in loading the image: %s\n", image->file);
        return 1;
    }
	agg->_images[agg->_num_images++] = image;
	return 0;
}

int actual_image_load_wrapper(void *args, size_t args_size) {
	if(args_size != sizeof(struct image_context_t)) {
		return 1;
	}
	struct image_context_t context = *(struct image_context_t*)args;
	return actual_image_load(context.agg, context.image);
}

int gam_image_load(gam_image_agg *agg, gam_image_rec *image, char *image_file, bool immediate) {
	image->file = malloc(strlen(image_file) + 1);
	if(image->file == NULL) {
		return 1;
	}
	strcpy(image->file, image_file);
	
	struct image_context_t context;
	context.agg = agg;
	context.image = image;
	return gam_msg_send(agg->_msg_agg, immediate, actual_image_load_wrapper, &context, sizeof(context));
}


int gam_image_free(const gam_image_agg *agg, gam_image_rec *image) {
	// TODO - should remove from aggregator?
	stbi_image_free(image->img);
	free(image->file);
	return 0;
}
