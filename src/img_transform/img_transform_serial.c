#include<stdlib.h>
#include"common.h"
#include <time.h>

static void resize_channel(byte* input, byte* output, int w, int h, int scale_factor) {
    double kernel[4][4], kernel2[4][4], kernel3[4][4], kernel4[4][4];
    int new_w = w * scale_factor;
    int new_h = h * scale_factor;

    fill_lt_corner_kernel(kernel, input, w);
    fill_lb_corner_kernel(kernel2, input, w, h);
    fill_rt_corner_kernel(kernel3, input, w);
    fill_rb_corner_kernel(kernel4, input, w, h);
    for(int k = 0 ; k < scale_factor; k++) {
        for(int l = 0 ; l < scale_factor ; l++) {
            output[k * new_w + l] = normalize(bicubic_interpolation(kernel, (double)l / scale_factor, (double)k / scale_factor));
            output[(new_h - 2 * scale_factor + k) * new_w + l] = normalize(bicubic_interpolation(kernel2, (double)l / scale_factor, (double)k / scale_factor));
            output[(new_h - scale_factor + k) * new_w + l] = normalize(bicubic_interpolation(kernel2, (double)l / scale_factor, 1 + (double)k / scale_factor));
            output[(k + 1) * new_w - 2 * scale_factor + l] = normalize(bicubic_interpolation(kernel3, (double)l / scale_factor, (double)k / scale_factor));
            output[(k + 1) * new_w - scale_factor + l] = normalize(bicubic_interpolation(kernel3, 1 + (double)l / scale_factor, (double)k / scale_factor));
            output[(new_h - 2 * scale_factor + k + 1) * new_w - 2 * scale_factor + l] = normalize(bicubic_interpolation(kernel4, (double)l / scale_factor, (double)k / scale_factor));
            output[(new_h - 2 * scale_factor + k + 1) * new_w - scale_factor + l] = normalize(bicubic_interpolation(kernel4, (double)l / scale_factor, 1 + (double)k / scale_factor));
            output[(new_h - scale_factor + k + 1) * new_w - 2 * scale_factor + l] = normalize(bicubic_interpolation(kernel4, 1 + (double)l / scale_factor, (double)k / scale_factor));
            output[(new_h - scale_factor + k + 1) * new_w - scale_factor + l] = normalize(bicubic_interpolation(kernel4, 1 + (double)l / scale_factor, 1 + (double)k / scale_factor));
        }
    }

    for(int i = 1 ; i < w - 2 ; i ++) {
        fill_top_edge_kernel(kernel, input, w, i);
        fill_bottom_edge_kernel(kernel2, input, h, w, i);
        for(int k = 0 ; k < scale_factor; k++) {
            for(int l = 0 ; l < scale_factor ; l++) {
                output[k * new_w + i * scale_factor + l] = normalize(bicubic_interpolation(kernel, (double)l / scale_factor, (double)k / scale_factor));
                output[(new_h - 2 * scale_factor + k) * new_w + i * scale_factor + l] = normalize(bicubic_interpolation(kernel2, (double)l / scale_factor, (double)k / scale_factor));
                output[(new_h - scale_factor + k) * new_w + i * scale_factor + l] = normalize(bicubic_interpolation(kernel2, (double)l / scale_factor, 1 + (double)k / scale_factor));
            }
        }
    }

    for(int j = 1 ; j < h - 2 ; j ++) {
        fill_left_edge_kernel(kernel, input, w, j);
        fill_right_edge_kernel(kernel2, input, w, j);
        for(int k = 0 ; k < scale_factor; k++) {
            for(int l = 0 ; l < scale_factor ; l++) {
                output[(j * scale_factor + k) * new_w + l] = normalize(bicubic_interpolation(kernel, (double)l / scale_factor, (double)k / scale_factor));
                output[(j * scale_factor + k + 1) * new_w - 2 * scale_factor + l] = normalize(bicubic_interpolation(kernel2, (double)l / scale_factor, (double)k / scale_factor));
                output[(j * scale_factor + k + 1) * new_w - scale_factor + l] = normalize(bicubic_interpolation(kernel2, 1 + (double)l / scale_factor, (double)k / scale_factor));
            }
        }
    }

    for(int i = 1 ; i < h - 2 ; i++) 
    {
        for(int j = 1 ; j < w - 2 ; j ++)
        {
            fill_kernel(kernel, input, w, i, j);
            for(int k = 0; k < scale_factor ; k++) {
                for(int l = 0 ; l < scale_factor ; l++) {
                    double calculated_value = bicubic_interpolation(kernel, (double)l / scale_factor, (double)k / scale_factor);

                    output[new_w * (i * scale_factor + k) + (j * scale_factor + l)] = normalize(calculated_value);
                }
            }
        }
    }
}

double run_scaling_serial(rgb_image* input, rgb_image* output, int factor)
{
    clock_t start, end;

    image_alloc(output, input->width * factor, input->height * factor);
    start = clock();

    resize_channel(input->r_channel, output->r_channel, input->width, input->height, factor);
    resize_channel(input->g_channel, output->g_channel, input->width, input->height, factor);
    resize_channel(input->b_channel, output->b_channel, input->width, input->height, factor);
    resize_channel(input->alpha_channel, output->alpha_channel, input->width, input->height, factor);
    
    end = clock();
    return ((double) (end - start)) / CLOCKS_PER_SEC;
}


