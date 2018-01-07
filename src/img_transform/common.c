#include"common.h"

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

void fill_lt_corner_kernel(double kernel[4][4], byte* data, int w)
{
    for(int i = 0 ; i < 3 ; i ++)
    {
        for(int j = 0 ; j < 3 ; j ++)
        {
            kernel[i + 1][j + 1] = (double)data[i * w + j];
        }
    }
    for(int i = 1 ; i < 4 ; i ++) {
        kernel[0][i] = kernel[1][i];
        kernel[i][0] = kernel[i][1];
    }
    kernel[0][0] = kernel[1][1];
}

void fill_lb_corner_kernel(double kernel[4][4], byte* data, int w, int h)
{
    for(int i = 0 ; i < 3 ; i ++)
    {
        for(int j = 0 ; j < 3 ; j ++)
        {
            kernel[i][j + 1] = (double)data[(h - 3 + i) * w + j];
        }
    }
    for(int i = 0 ; i < 3 ; i ++) {
        kernel[3][i + 1] = kernel[2][i + 1];
        kernel[i][0] = kernel[i][1];
    }
    kernel[3][0] = kernel[2][1];
}

void fill_rt_corner_kernel(double kernel[4][4], byte* data, int w)
{
    for(int i = 0 ; i < 3 ; i ++)
    {
        for(int j = 0 ; j < 3 ; j ++)
        {
            kernel[i + 1][j] = (double)data[(i + 1) * w - 3 + j];
        }
    }
    for(int i = 0 ; i < 3 ; i ++) {
        kernel[0][i] = kernel[1][i];
        kernel[i + 1][3] = kernel[i + 1][2];
    }
    kernel[0][3] = kernel[1][2];
}

void fill_rb_corner_kernel(double kernel[4][4], byte* data, int w, int h)
{
    for(int i = 0 ; i < 3 ; i ++)
    {
        for(int j = 0 ; j < 3 ; j ++)
        {
            kernel[i][j] = (double)data[(h + i - 2) * w - 3 + j];
        }
    }
    for(int i = 0 ; i < 3 ; i ++) {
        kernel[3][i] = kernel[2][i];
        kernel[i][3] = kernel[i][2];
    }
    kernel[3][3] = kernel[2][2];
}

void fill_top_edge_kernel(double kernel[4][4], byte* data, int w, int col)
{
    for(int i = 0 ; i < 3 ; i ++)
    {
        for(int j = 0 ; j < 4 ; j ++)
        {
            kernel[i + 1][j] = (double)data[i * w + col + j - 1];
        }
    }
    for(int j = 0 ; j < 4 ; j ++) {
        kernel[0][j] = kernel[1][j];
    }
}

void fill_bottom_edge_kernel(double kernel[4][4], byte* data, int h, int w, int col)
{
    for(int i = 0 ; i < 3 ; i ++)
    {
        for(int j = 0 ; j < 4 ; j ++)
        {
            kernel[i][j] = (double)data[(h - 3 + i) * w + col + j - 1];
        }
    }
    for(int j = 0 ; j < 4 ; j ++) {
        kernel[3][j] = kernel[2][j];
    }
}

void fill_left_edge_kernel(double kernel[4][4], byte* data, int w, int row)
{
    for(int i = 0 ; i < 4 ; i ++)
    {
        for(int j = 0 ; j < 3 ; j ++)
        {
            kernel[i][j + 1] = (double)data[(row + i - 1) * w + j];
        }
        kernel[i][0] = kernel[i][1];
    }
}

void fill_right_edge_kernel(double kernel[4][4], byte* data, int w, int row)
{
    for(int i = 0 ; i < 4 ; i ++)
    {
        for(int j = 0 ; j < 3 ; j ++)
        {
            kernel[i][j] = (double)data[(row + i) * w - 3 + j];
        }
        kernel[i][3] = kernel[i][2];
    }
}

byte normalize(double value) {
    if(value < 0) {
        value = 0;
    }else if(value > 255) {
        value = 255;
    }
    return (byte)value;
}

