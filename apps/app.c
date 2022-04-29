#include <gamesh/lib.h>

#include <stdio.h>

#include <log.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

int main() {
	int a = 1;
	int b = 2;
	int absum = test_add(a, b);
    log_info("%d + %d = %d\n", a, b, absum);
	
	int width, height, channels;
    unsigned char *img = stbi_load("res/image/coco.jpg", &width, &height, &channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    log_info("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);
	
    return 0;
}
