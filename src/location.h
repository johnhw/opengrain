/**    
    @file location.h
    @brief Provides management of 3D positions
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __LOCATION_H__
#define __LOCATION_H__
#include "utils.h"
#include "matrix.h"
#include <stdlib.h>
#include <math.h>

typedef struct Location3D
{
    float elevation, azimuth, distance;
    float x,y,z;    
} Location3D;

Location3D *create_location();
void destroy_location(Location3D *location);
void set_spherical_location(Location3D *location, float azimuth, float elevation, float distance);
void set_cartesian_location(Location3D *location, float x, float y, float z);
void transform_location(Location3D *dest, Location3D *src, Matrix3D *m);







#endif