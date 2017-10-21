#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"
#define FILENAME "res/test.png"

int main()
{
    int width, height, channels_count;
    unsigned char *data = stbi_load(FILENAME, &width, &height, &channels_count, 4);
    for(int i = 0 ; i < width * channels_count ; i++) 
    {
        printf("%02X ", data[i]);
    }
    free(data);
}
