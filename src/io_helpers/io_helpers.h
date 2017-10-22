#ifndef IO_HELPERS_H
#define IO_HELPERS_H

#define RESULT_OK 0
#define RESULT_CANNOT_READ 1
#define RESULT_WRONG_CHANNELS_COUNT 2

typedef unsigned char byte;
typedef struct {
    byte* r_channel;
    byte* g_channel;
    byte* b_channel;
    byte* alpha_channel;
    int width;
    int height;
} rgb_image;

int image_read(const char *filename, rgb_image *image);
int image_write(const char* filename, rgb_image image);
void image_dispose(rgb_image image);


#endif //!IO_HELPERS_H