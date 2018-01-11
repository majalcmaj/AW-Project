#include <stdlib.h>
#include <stdio.h>
#include "io_helpers.h"
#define STBI_FAILURE_USERMSG 
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb_image_write.h"
#define CHANNELS_COUNT 4

void image_alloc(rgb_image *image, int width, int height)
{
    image->width = width;
    image->height = height;
    int pixels_count = width * height;

    image->r_channel = (byte *)malloc(sizeof(byte) * pixels_count);
    image->g_channel = (byte *)malloc(sizeof(byte) * pixels_count);
    image->b_channel = (byte *)malloc(sizeof(byte) * pixels_count);
    image->alpha_channel = (byte *)malloc(sizeof(byte) * pixels_count);
}

int image_read(const char *filename, rgb_image *image)
{
    int width, height, channels_count, pixels_count;
    byte *data = stbi_load(filename, &width, &height, &channels_count, CHANNELS_COUNT);
    if (data == NULL)
    {
        fprintf(stdout, stbi_failure_reason());
        return RESULT_CANNOT_READ;
    }
    if (channels_count != CHANNELS_COUNT)
    {
        return RESULT_WRONG_CHANNELS_COUNT;
    }
    image_alloc(image, width, height);
    pixels_count = width * height;
    for (int i = 0; i < pixels_count; i++)
    {
        image->r_channel[i] = data[i * CHANNELS_COUNT];
        image->g_channel[i] = data[i * CHANNELS_COUNT + 1];
        image->b_channel[i] = data[i * CHANNELS_COUNT + 2];
        image->alpha_channel[i] = data[i * CHANNELS_COUNT + 3];
    }
    free(data);
    return 0;
}

int image_write(const char *filename, rgb_image image)
{
    int pixels_count = image.width * image.height;
    byte *data = (byte *)malloc(sizeof(byte) * pixels_count * CHANNELS_COUNT);
    for (int i = 0; i < pixels_count; i++)
    {
        data[i * CHANNELS_COUNT] = image.r_channel[i];
        data[i * CHANNELS_COUNT + 1] = image.g_channel[i];
        data[i * CHANNELS_COUNT + 2] = image.b_channel[i];
        data[i * CHANNELS_COUNT + 3] = image.alpha_channel[i];
    }
    int result = stbi_write_png(filename, image.width, image.height, CHANNELS_COUNT, data, 0);
    free(data);
    return result;
}
void image_dispose(rgb_image image)
{
    free(image.r_channel);
    free(image.g_channel);
    free(image.b_channel);
    free(image.alpha_channel);
}
