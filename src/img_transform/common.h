#ifndef _IMG_TRANSFORM_COMMON_H
#define _IMG_TRANSFORM_COMMON_H

#include"../io_helpers/io_helpers.h"

double bicubic_interpolation(double points[4][4], double x, double y);
void fill_kernel(double kernel[4][4], byte* data, int w, int row, int col); 
void fill_lt_corner_kernel(double kernel[4][4], byte* data, int w);
void fill_lb_corner_kernel(double kernel[4][4], byte* data, int w, int h);
void fill_rt_corner_kernel(double kernel[4][4], byte* data, int w);
void fill_rb_corner_kernel(double kernel[4][4], byte* data, int w, int h);
void fill_top_edge_kernel(double kernel[4][4], byte* data, int w, int col);
void fill_bottom_edge_kernel(double kernel[4][4], byte* data, int h, int w, int col);
void fill_left_edge_kernel(double kernel[4][4], byte* data, int w, int row);
void fill_right_edge_kernel(double kernel[4][4], byte* data, int w, int row);
byte normalize(double value);
#endif
