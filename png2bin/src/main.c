#include<string.h>
#include<errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "io_helpers/io_helpers.h"

int p2b(const char* in_path, const char* out_path, int scale) 
{
    rgb_image image;
    int res = 0;
    int fnres = image_read(in_path, &image);
    if ( fnres != 0) 
    {
        fprintf(stderr, "Cannot read image: %d\n", fnres);
        return 1;
    }
    FILE* outfile = fopen(out_path, "wb");
    if(outfile == NULL)
    {
        image_dispose(image); 
        fprintf(stderr, "Could not open output file '%s', errno %d\n", out_path, errno);
        return 2;
    }

    int new_height = image.height * scale;
    int img_field = image.width * image.height;
    fnres = fwrite(&image.width, sizeof(int), 1, outfile);
    if(fnres != 1) res = 3;
    fnres = fwrite(&new_height, sizeof(int), 1, outfile);
    if(fnres != 1) res = 3;
    for(int i = 0 ; i < scale ; i++) {
        fnres = fwrite(image.r_channel, sizeof(byte), img_field, outfile);
        if(fnres != img_field) res = 3;
    }

    for(int i = 0 ; i < scale ; i++) {
        fnres = fwrite(image.g_channel, sizeof(byte), img_field, outfile);
        if(fnres != img_field) res = 3;
    } 
    for(int i = 0 ; i < scale ; i++) {
        fnres = fwrite(image.b_channel, sizeof(byte), img_field, outfile);
        if(fnres != img_field) res = 3;
    }
    for(int i = 0 ; i < scale ; i++) {
        fnres = fwrite(image.alpha_channel, sizeof(byte), img_field, outfile);
        if(fnres != img_field) res = 3;
    }
    fclose(outfile);
    image_dispose(image);
    return res;
}

int read_binary(FILE* infile, rgb_image* image)
{
    int fnres;
    int width, height;
    fnres = fread(&width, sizeof(int), 1, infile);
    if(fnres != 1) return 4;
    fnres = fread(&height, sizeof(int), 1, infile);
    if(fnres != 1) return 5; 
    int img_field = width * height;
    image_alloc(image, width, height);
    fnres = fread(image->r_channel, sizeof(byte), img_field, infile);
    if(fnres != img_field) return 6;
    fnres = fread(image->g_channel, sizeof(byte), img_field, infile);
    if(fnres != img_field) return 7;
    fnres = fread(image->b_channel, sizeof(byte), img_field, infile);
    if(fnres != img_field) return 8;
    fnres = fread(image->alpha_channel, sizeof(byte), img_field, infile);
    if(fnres != img_field) return 9;
    return 0;
}

int b2p(const char* in_path, const char* out_path) 
{
    rgb_image image;
    int fnres;
    FILE* infile = fopen(in_path, "rb");
    if(infile == NULL)
    {
        fprintf(stderr, "Could not input file '%s', errno %d\n", in_path, errno);
        return 2;
    }

    fnres = read_binary(infile, &image);
    if(fnres == 0) {
        if(image_write(out_path, image) == 0) {
            fnres = 10;
        }
    }
    if(fnres != 0) {
        fprintf(stderr, "Cannot write image, result: %d\n", fnres);
    }
    image_dispose(image);
    fclose(infile);
    return fnres;
}

int main(int argc, const char** argv)
{
    if(argc != 4 && argc != 5) {
        fprintf(stderr, "3 or 4 arguments should be provided: MODE (p2b - png to bin or b2p - bin to png) IN_FILE OUT_FILE [OUT_SCALE - only for p2b, default 1]\n");
        return 3;
    }

    const char* mode = argv[1];
    if(strcmp(mode, "p2b") == 0) {
        int scale;
        if(argc == 5) {
            scale = atoi(argv[4]);
        }else {
            scale = 1;
        }
        return p2b(argv[2], argv[3], scale);
    } else if(strcmp(mode, "b2p") == 0) {
        return b2p(argv[2], argv[3]);
    } else {
        fprintf(stderr, "Wrong argument specified as mode (p2b | b2p)\n");
        return 4;
    }
}
