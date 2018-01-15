#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "io_helpers/io_helpers.h"
#include "img_transform/img_transform_serial.h"
#include "img_transform/img_transform_parallel.h"

int perform_scaling(const char* input_path, const char* output_path, int (*resize_function)(rgb_image*, rgb_image*, int, double*), int scaling_factor, double* time) {
    rgb_image image, output;
    int result = image_read(input_path, &image);
    if(result != 0) {
        exit(result);
    }

    result = resize_function(&image, &output, scaling_factor, time);

    if(output_path != NULL)
    {
        image_write(output_path, output);
    }
    image_dispose(image);
    if(result == 0) {
        image_dispose(output);
    }
    return result;
}

int main(int argc, const char** argv)
{
    double time;
    int result;
    int (*scaling_fun)(rgb_image*, rgb_image*, int, double*);
    if(argc != 4 && argc != 5) {
        fprintf(stderr, "3 or 4 arguments should be provided: alg<SERIAL> input_path, resize_factor, [output_path]");
        exit(1);
    }

    if(strcmp(argv[1], "SERIAL") == 0) {
        scaling_fun = &run_scaling_serial;
    } else if(strcmp(argv[1], "PARALLEL") == 0) {
        scaling_fun = &run_scaling_parallel;
    } else {
        fprintf(stderr, "Wrong function, currently supported: SERIAL, PARALLEL");
        exit(1);
    }
    if(argc == 4) {
        result = perform_scaling(argv[2], NULL, scaling_fun, atoi(argv[3]), &time);
    } else if(argc == 5) {
        result = perform_scaling(argv[2], argv[4], scaling_fun, atoi(argv[3]), &time);
    }
    printf("%lf", time);
    return result;
}
