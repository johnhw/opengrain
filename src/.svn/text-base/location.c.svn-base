/**    
    @file location.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "location.h"


/** Return a new, empty location */
Location3D *create_location()
{
    Location3D *location = malloc(sizeof(*location));
    set_cartesian_location(location, 0,0,1);
    return location;
}

/** Free a location object.
    @arg location The object to free. */
void destroy_location(Location3D *location)
{
    free(location);
}

/** Set this location object's position using spherical co-ordinates.
    Note this sets both the spherical and cartesian location of this point (i.e.
    it internally converts the position to an x,y,z one)
    Co-ordinates are in degrees, and range from -180 -> 180 for azimuth,
    and -90 -> 90 for elevation, where 0 azimuth, 0 elevation is level, straight ahead.
    @arg location The location to set.
    @arg azimuth Azimuth (longitude) angle, in degrees.
    @arg elevation Elevation (latitude) angle, in degrees.
    @arg distance Distance */
void set_spherical_location(Location3D *location, float azimuth, float elevation, float distance)
{
    float r, theta, phi;
    location->azimuth = azimuth;
    location->elevation = elevation;
    location->distance = distance;
    r = distance;
    theta = TO_RADIANS(elevation);
    phi = TO_RADIANS(azimuth);
    
    location->x = r * cos(theta) * cos(phi);
    location->y = r * cos(theta) * sin(phi);
    location->z = r * sin(theta);    
}

/** Set this object's position to a position in Cartesian coordinates.
    @arg location The location object being set.
    @arg x The x coordinate.
    @arg y The y coordinate.
    @arg z The z coordinate. */        
void set_cartesian_location(Location3D *location, float x, float y, float z)
{
    location->x = x;
    location->y = y;
    location->z = z;
    location->distance = sqrt(x*x+y*y+z*z);
    location->azimuth = TO_DEGREES(atan2(y,x));
    location->elevation = 90-TO_DEGREES(acos(z/location->distance));            
}


/** Apply a matrix to this location, and copy the transformed version into src.
    src can equal dest to do an in-place transform.
    @arg dest The location which will be set to the transformed position
    @arg src The location which will be transformed.
    @arg m The matrix to transform with. */
void transform_location(Location3D *dest, Location3D *src, Matrix3D *m)
{
    float x,y,z;
    transform_vector_matrix(m, src->x, src->y, src->z, &x, &y, &z);
    set_cartesian_location(dest, x, y, z);
}
