#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "io_helpers/io_helpers.h"
#include "img_transform/img_transform_serial.h"

double perform_scaling(const char* input_path, const char* output_path, double (*resize_function)(rgb_image*, rgb_image*, int), int scaling_factor) {
    double time;
    rgb_image image, output;
    int result = image_read(input_path, &image);
    if(result != 0) {
        exit(result);
    }

    time = resize_function(&image, &output, scaling_factor);

    if(output_path != NULL)
    {
        image_write(output_path, output);
    }
    image_dispose(image);
    image_dispose(output);
    return time;
}


int main(int argc, const char** argv)
{
    double time;
    double (*scaling_fun)(rgb_image*, rgb_image*, int);
    if(argc != 4 && argc != 5) {
        fprintf(stderr, "3 or 4 arguments should be provided: alg<SERIAL> input_path, resize_factor, [output_path]");
        exit(1);
    }

    if(strcmp(argv[1], "SERIAL") == 0) {
        scaling_fun = &run_scaling_serial;
    } else {
        fprintf(stderr, "Wrong function, currently supported: SERIAL");
    }
    if(argc == 4) {
        time = perform_scaling(argv[2], NULL, scaling_fun, atoi(argv[3]));
    } else if(argc == 5) {
        time = perform_scaling(argv[2], argv[4], scaling_fun, atoi(argv[3]));
    }
    printf("%lf\n", time);
}
