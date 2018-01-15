#include <stdlib.h>
#include <stdio.h>
#include "io_helpers.h"
#include <errno.h>
#define CHANNELS_COUNT 4

int image_alloc(rgb_image *image, int width, int height)
{
    image->width = width;
    image->height = height;
    int pixels_count = width * height;

    image->r_channel = (byte *)malloc(sizeof(byte) * pixels_count);
    image->g_channel = (byte *)malloc(sizeof(byte) * pixels_count);
    image->b_channel = (byte *)malloc(sizeof(byte) * pixels_count);
    image->alpha_channel = (byte *)malloc(sizeof(byte) * pixels_count);

    printf("%X %X %X %X %d", image->r_channel, image->g_channel, image->b_channel, image->alpha_channel, pixels_count);

    if(image->r_channel == NULL || image->g_channel == NULL || image->b_channel == NULL || image->alpha_channel == NULL )
    {
        return 100;
    } else {
        return 0;
    }
}

static int read_binary(FILE* infile, rgb_image* image)
{
    int fnres;
    int width, height;
    fnres = fread(&width, sizeof(int), 1, infile);
    if(fnres != 1) return 4;
    fnres = fread(&height, sizeof(int), 1, infile);
    if(fnres != 1) return 5; 
    int px_count = width * height;
    image_alloc(image, width, height);
    fnres = fread(image->r_channel, sizeof(byte), px_count, infile);
    if(fnres != px_count) return 6;
    fnres = fread(image->g_channel, sizeof(byte), px_count, infile);
    if(fnres != px_count) return 7;
    fnres = fread(image->b_channel, sizeof(byte), px_count, infile);
    if(fnres != px_count) return 8;
    fnres = fread(image->alpha_channel, sizeof(byte), px_count, infile);
    if(fnres != px_count) return 9;
    return 0;
}


int image_read(const char *filename, rgb_image *image)
{
    int result;
    FILE* infile = fopen(filename, "rb");
    if (infile == NULL)
    {
        fprintf(stdout, "Cannot read file, errno: %d", errno);
        return RESULT_CANNOT_READ;
    }
    result = read_binary(infile, image);
    return result;
}

int write_binary(FILE* outfile, rgb_image* image)
{
    int px_count = image->width * image->height;
    int fnres = fwrite(&image->width, sizeof(int), 1, outfile);
    if(fnres != 1) return 3;
    fnres = fwrite(&image->height, sizeof(int), 1, outfile);
    if(fnres != 1) return 4;
    fnres = fwrite(image->r_channel, sizeof(byte), px_count, outfile);
    if(fnres != px_count) return 5;
    fnres = fwrite(image->g_channel, sizeof(byte), px_count, outfile);
    if(fnres != px_count) return 6;
    fnres = fwrite(image->b_channel, sizeof(byte), px_count, outfile);
    if(fnres != px_count) return 7;
    fnres = fwrite(image->alpha_channel, sizeof(byte), px_count, outfile);
    if(fnres != px_count) return 8;
    return 0;
}

int image_write(const char *filename, rgb_image image)
{
    int res = 0;
    FILE* outfile = fopen(filename, "wb");
    if(outfile == NULL)
    {
        fprintf(stderr, "Could not open output file '%s', errno %d\n", filename, errno);
        return 2;
    }
    res = write_binary(outfile, &image);
    fclose(outfile);
    return res;
}
void image_dispose(rgb_image image)
{
    free(image.r_channel);
    free(image.g_channel);
    free(image.b_channel);
    free(image.alpha_channel);
}
