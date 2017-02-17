/**    
    @file matrix.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __MATRIX_H__
#define __MATRIX_H__
#include "utils.h"
#include <stdlib.h>
#include <math.h>



typedef struct Matrix3D
{
    float m[16];    
} Matrix3D;



Matrix3D *create_matrix();
void destroy_matrix(Matrix3D *m);
void translate_matrix(Matrix3D *m, float x, float y, float z);
void rotate_matrix(Matrix3D *m, float angle, float x, float y, float z);
void scale_matrix(Matrix3D *m,float x, float y, float z);
void identity_matrix(Matrix3D *m);
void transform_vector_matrix(Matrix3D *m, float x, float y, float z, float *out_x, float *out_y, float *out_z);
void multiply_matrix(Matrix3D *dest, Matrix3D *src);
void load_matrix(Matrix3D *dest, float *src);
void transpose_matrix(Matrix3D *dest);
void multiply_matrix_raw(Matrix3D *dest, float *m);
void print_matrix(Matrix3D *m);
void test_matrix(void);
void copy_matrix(Matrix3D *dest, Matrix3D *src);






#endif