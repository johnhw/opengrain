/**    
    @file curve.h
    @brief Handling of time varying functions
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __CURVE_H__
#define __CURVE_H__
#include "audio.h"
#include <stdlib.h>
#include <math.h>

#define CURVE_STEP 0
#define CURVE_LINEAR 1
#define CURVE_EXPLOG 2
#define CURVE_REVEXPLOG 3
#define CURVE_HERMITE 4

typedef struct CurveSegement
{
    float value;
    float time;
    int type;
    CurveSegment *next;
    CurveSegment *prevprev;
    CurveSegment *prev;    
} CurveSegment;


typedef struct Curve
{
    CurveSegment *head;
    int loop_start, loop_end;
    float value;
    float *value_ptr;
} Curve;

Curve *create_curve();
void destroy_curve(Curve *curve);
void set_target_curve(Curve *curve, float *target);
void add_segment_curve(Curve *curve, float value, float time, int type);
void set_segment_curve(Curve *curve, int segment, float value, float time, int type);
void remove_segment_curve(Curve *curve, int segment);
void set_loop_curve(Curve *curve, int start, int end);
float get_value_curve(Curve *curve);
float get_position_curve(Curve *curve); 
void get_interpolated_value_curve(Curve *curve, Buffer *output);
void set_position_curve(Curve *curve, float position); 
void update_curve(Curve *curve, int samples);


typedef struct CurveManager
{
    list_t *curves;
} CurveManager;

CurveManager *create_curve_manager();
void destroy_curve_manager(CurveManager *manager);
void pump_curve_manager(CurveManager *manager, int samples);
void register_with_curve_manager(CurveManager *manager, Curve *curve);
void remove_from_curve_manager(CurveManager *manager, Curve *curve);


#endif