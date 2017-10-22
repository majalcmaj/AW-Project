#include <stdio.h>
#include <stdlib.h>
#include "io_helpers/io_helpers.h"

int main()
{
    rgb_image image;
    int result;
    result = image_read("res/dupa.png", &image);
    // printf("Result read: %d\n", result);
    result = image_write("res/dupa-new.jpg", image);
    // printf("Result write: %d\n", result);
    image_dispose(image);
}
