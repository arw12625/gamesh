#include <gamesh/image_rec.h>

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>


int main() {
	gam_image_agg agg;
	assert(gam_image_agg_init(&agg) == 0);
	
	gam_image_rec image;
	assert(gam_image_load(&agg, &image, "coco.jpg", false) == 0);
	assert(gam_image_agg_on_update(&agg) == 0);
	assert(image.width == 1024);
	assert(image.height == 768);
	assert(image.channels == 3);
	
	assert(gam_image_agg_free(&agg) == 0);
	return 0;
}
