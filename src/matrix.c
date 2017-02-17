/**    
    @file matrix.c
    @brief Transformation matrix operations.
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "matrix.h"
#include <math.h>
#include <stdio.h>


static float id_matrix[] = {  1,0,0,0,
                    0,1,0,0,
                    0,0,1,0,
                    0,0,0,1};
                    

Matrix3D *create_matrix()
{
     Matrix3D *m = malloc(sizeof(*m));
     identity_matrix(m);
     return m;
}

void destroy_matrix(Matrix3D *m)
{
    free(m);
}

/** Translate a matrix by the given values.
    @arg m Matrix to be translated. Will be scaled in place.
    @arg x Translate for x-coordinate.
    @arg y Translate for y-coordinate.
    @arg z Translate for z-coordinate.*/      
void translate_matrix(Matrix3D *m, float x, float y, float z)
{
    int i;
    float temp[16];    
    for(i=0;i<16;i++)
        temp[i] = id_matrix[i];
    temp[3] = x;
    temp[7] = y;
    temp[11] = z;
    multiply_matrix_raw(m, temp);
}

/** Rotate a matrix around the given axis by a certain number of degrees.
    @arg m Matrix to rotate
    @arg angle Angle, in degrees, to rotate
    @arg x x component of axis vector
    @arg y y component of axis vector
    @arg z z component of axis vector */    
void rotate_matrix(Matrix3D *m, float angle, float x, float y, float z)
{
    float r, theta, c, s, c1;
    float temp[16];    
    theta = TO_RADIANS(angle);
    c = cos(theta);
    s = sin(theta);
    c1 = 1-c;
    
    // normalize
    r = sqrt(x*x+y*y+z*z);
    x /= r;
    y /= r;
    z /= r;
    
    temp[0] = c + x*x*c1;
    temp[1] = x*y*c1 - z*s;
    temp[2] = x*z*c1 + y*s;
    temp[3] = 0;
    
    temp[4] = y*x*c1 + z*s;
    temp[5] = c + y*y*c1;
    temp[6] = y*z*c1 - x*s;
    temp[7] = 0;
    
    temp[8] = z*x*c1-y*s;
    temp[9] = z*y*c1 + x*s;
    temp[10] = c+z*z*c1;
    temp[11] = 0;
    
    temp[12] = 0;
    temp[13] = 0;
    temp[14] = 0;
    temp[15] = 1;
    multiply_matrix_raw(m, temp);     
}


/** Scale a matrix by the given values.
    @arg m Matrix to be scaled. Will be scaled in place.
    @arg x Scale for x-coordinate.
    @arg y Scale for y-coordinate.
    @arg z Scale for z-coordinate.*/        
void scale_matrix(Matrix3D *m,  float x, float y, float z)
{
    int i;
    float temp[16];    
    for(i=0;i<16;i++)
        temp[i] = id_matrix[i];
    temp[0] = x;
    temp[5] = y;
    temp[10] = z;
    multiply_matrix_raw(m, temp); 
}

/** Set a matrix to the identity.
    @arg m Matrix to reset. */
void identity_matrix(Matrix3D *m)
{
    int i;
    for(i=0;i<16;i++)    
        m->m[i] = id_matrix[i];    
}

/** Multiply a vector by a matrix. 
    @arg m matrix to transform the vector by
    @arg x input x position
    @arg y input y position
    @arg z input z position
    @arg out_x pointer to output x position (can just point to x/y/z, if in-place transform is desired)
    @arg out_y pointer to output y position 
    @arg out_z pointer to output z position */
    
void transform_vector_matrix(Matrix3D *m, float x, float y, float z, float *out_x, float *out_y, float *out_z)
{
    int i,j;
    float sum, temp[4], in[4];
    
    in[0] = x;
    in[1] = y;
    in[2] = z;
    in[3] = 1;    
    
    for(i=0;i<4;i++)
    {
        sum = 0.0;
        for(j=0;j<4;j++)        
            sum += m->m[j+4*i] * in[j];        
        temp[i] = sum;
    }
    
    *out_x = temp[0];
    *out_y = temp[1];
    *out_z = temp[2];
    // don't need w    

    
    
}


/** Multiply a matrix by another. Computes dest = dest * src (post-multiplication)
    @arg dest First multiplicand, and destination
    @arg src Second multiplicand
*/

void multiply_matrix(Matrix3D *dest, Matrix3D *src)
{
    int i,j,k;
    float sum;
    float temp[16];
    
    // multiply, result in temp
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
        {           
            sum = 0.0;
            for(k=0;k<4;k++)            
                sum += dest->m[i+4*k] * src->m[k+4*j];
            temp[i+4*j] = sum;                    
        }
        
    // copy temp into matrix
    for(i=0;i<16;i++)
        dest->m[i] = temp[i];

}

/** Multiply a matrix by another in raw form. Computes dest = dest * src (post-multiplication)
    @arg dest First multiplicand, and destination
    @arg src Second multiplicand
*/

void multiply_matrix_raw(Matrix3D *dest, float *m)
{
    int i,j,k;
    float sum;
    float temp[16];
    
    // multiply, result in temp
    for(i=0;i<4;i++)
        for(j=0;j<4;j++)
        {           
            sum = 0.0;
            for(k=0;k<4;k++)            
                sum += dest->m[i+4*k] * m[k+4*j];
            temp[i+4*j] = sum;                    
        }
        
    // copy temp into matrix
    for(i=0;i<16;i++)
        dest->m[i] = temp[i];

}

/** Load a matrix from a 16 value float vector. Matrices are in row-major order [[1,2,3,4], [5,6,7,8],....]
    @arg dest Matrix to write into
    @arg src float array which must have 16 values to load into dest
*/
void load_matrix(Matrix3D *dest, float *src)
{
    int i;
    for(i=0;i<16;i++)    
        dest->m[i] = src[i];    
}


/** Copy a matrix into another.
    @arg dest Destination to be written to.
    @arg src Source to copy from.
    */
void copy_matrix(Matrix3D *dest, Matrix3D *src)
{
   int i;
    for(i=0;i<16;i++)    
        dest->m[i] = src->m[i];    
}

/** In-place transpose a 4x4 transformation matrix
    @arg dest Matrix to transpose
*/
void transpose_matrix(Matrix3D *dest)
{
    int i,j;
    float t;
    for(i=0;i<3;i++)
        for(j=i+1;j<4;j++)
        {
            t = dest->m[i+4*j];
            dest->m[i+4*j] = dest->m[j+4*i];
            dest->m[j+4*i] = t;        
        }
}

/** Print out a matrix.
    @arg m The matrix to print */   
void print_matrix(Matrix3D *m)
{
    int i,j;
    printf("    ");
    for(j=0;j<4;j++)
    {
        for(i=0;i<4;i++)        
            printf("%.1f ", m->m[i+4*j]);                
        printf("\n    ");
    }
    printf("\n");
}

/** Test the matrix operations */
void test_matrix(void)
{
    Matrix3D *a, *b;
    float x, y, z;
    
    a = create_matrix();
    b = create_matrix();      
    
    printf("Identity\n");
    print_matrix(a);
    
    printf("Translate\n");
    translate_matrix(a, 20, 10, 5);
    print_matrix(a);    
        
    printf("Scale\n");
    scale_matrix(a, 10, 10, 10);
    print_matrix(a);
       
    printf("Transform vector\n");
    transform_vector_matrix(a, 1,1,1, &x, &y, &z);
    printf("[%f %f %f]\n\n", x, y, z);
    
    
          
    printf("Transpose\n");
    transpose_matrix(a);
    print_matrix(a);
    
    printf("Identity\n");
    identity_matrix(a);
    print_matrix(a);
    
    printf("Transform vector\n");
    transform_vector_matrix(a, -100,0,0, &x, &y, &z);
    printf("[%f %f %f]\n\n", x, y, z);
    
    
    printf("Rotate 90 about y\n");
    rotate_matrix(a, 90, 0, 1, 0);
    print_matrix(a);
    
    printf("Transform vector\n");
    transform_vector_matrix(a, 1,0,0, &x, &y, &z);
    printf("[%f %f %f]\n\n", x, y, z);
    
    printf("Rotate 45 about z\n");
    rotate_matrix(a, 45, 0, 0, 1);
    print_matrix(a);
    
    printf("Transform vector\n");
    transform_vector_matrix(a, 0,0,1, &x, &y, &z);
    printf("[%f %f %f]\n\n", x, y, z);
    
    

}