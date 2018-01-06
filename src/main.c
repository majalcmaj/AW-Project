#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "io_helpers/io_helpers.h"

double interpolate(double p[4], double x) {
    return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
}

double bicubic_interpolation(double points[4][4], double x, double y)
{
    double vertical[4];
    for(int i = 0 ; i < 4 ; i++)
    {
        vertical[i] = interpolate(points[i], x);
    }
    return interpolate(vertical, y);
}

void fill_kernel(double kernel[4][4], byte* data, int w, int row, int col) 
{
    for(int i = 0 ; i < 4 ; i ++)
    {
        for(int j = 0 ; j < 4 ; j ++)
        {
            kernel[i][j] = (double)data[(row + i - 1) * w + col + j - 1];
        }
    }
}

void resize_channel(byte* input, byte* output, int w, int h, int scale_factor) {
    double kernel[4][4];
    int new_w = w * scale_factor;
    for(int i = 1 ; i < h - 1 ; i++) 
    {
        for(int j = 1 ; j < w - 1 ; j ++)
        {
            fill_kernel(kernel, input, w, i, j);
            for(int k = 0; k < scale_factor ; k++) {
                for(int l = 0 ; l < scale_factor ; l++) {
                    double calculated_value = bicubic_interpolation(kernel, (double)l / scale_factor, (double)k / scale_factor);
                    if(calculated_value < 0) {
                        calculated_value = 0;
                    }else if(calculated_value > 255) {
                        calculated_value = 255;
                    }
                    output[new_w * (i * scale_factor + k) + (j * scale_factor + l)] = (byte)calculated_value;
                }
            }
        }
    }
}

void run_scaling(const char* input_path, const char* output_path, int factor)
{
    rgb_image image, output;
    int result = image_read(input_path, &image);
    if(result != 0) {
        exit(result);
    }
    image_alloc(&output, image.width * factor, image.height * factor);
    resize_channel(image.r_channel, output.r_channel, image.width, image.height, factor);
    resize_channel(image.g_channel, output.g_channel, image.width, image.height, factor);
    resize_channel(image.b_channel, output.b_channel, image.width, image.height, factor);
    resize_channel(image.alpha_channel, output.alpha_channel, image.width, image.height, factor);
    image_write(output_path, output);
    image_dispose(image);
    image_dispose(output);
}

int main(int argc, const char** argv)
{
    if(argc != 4) {
        fprintf(stderr, "Exactly 3 arguments should be provided: input_path, output_path, resize_factor");
        exit(1);
    }

    run_scaling(argv[1], argv[2], atoi(argv[3]));
}
