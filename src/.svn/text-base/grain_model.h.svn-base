/**    
    @file grain_model.h
    @brief Holds the distributions for the basic parameters for generating grains, and 
    provides the functionality for configuring a grain from those parameter distributions.
    
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __GRAIN_MODEL_H__
#define __GRAIN_MODEL_H__

#include "audio.h"
#include "distributions.h"
#include "grain_source.h"
#include "spatializer.h"
#include "envelope.h"
#include "location.h"

#define ENVELOPE_TYPE_NONE 0

#define DURATION_MODE_DETERMINISTIC
#define DURATION_MODE_POISSON

#define RATE_MODE_REGULAR 0
#define RATE_MODE_STOCHASTIC 1
#define RATE_MODE_TRIGGERED 2

#define TRIGGER_MODE_REGULAR 0
#define TRIGGER_MODE_STOCHASTIC 0


#define DURATION_INFINITE 1e20

// when to cut off grains if they get very quiet (especially if using DURATION_INFINITE)
#define AMPLITUDE_TERMINATION_THRESHOLD 1e-6

#define SPATIAL_MODE_POLAR 0
#define SPATIAL_MODE_CARTESIAN 1

/** @struct GrainModel
    Holds the distributions for the general grain parameters (that all grain types
    share).
    */
typedef struct GrainModel
{                
    int rate_mode;
    
    Distribution *rate;
    Distribution *time;
    Distribution *source;
    
       
    // common parameters
    Distribution *azimuth;
    Distribution *elevation;
    Distribution *distance;
    
    Distribution *x;
    Distribution *y;
    Distribution *z;
    
    
    int spatial_mode;
    
    Distribution *duration;
    Distribution *amplitude;
    Distribution *frequency;
        
    // Envelope
    int envelope_type;
    Distribution *attack;
    Distribution *decay;
    Distribution *shape;
                        
                        
                        
} GrainModel;




GrainModel *create_grain_model(void);
void destroy_grain_model(GrainModel *model);
float next_time_grain_model(GrainModel *model);
void fill_from_grain_model(GrainModel *stream, list_t *sources, int when, Grain *grain);



void set_rate_mode_model(GrainModel *model, int rate_mode);
void set_envelope_type_model(GrainModel *model, int type);

Distribution *get_rate_grain_model(GrainModel *model);
Distribution *get_time_grain_model(GrainModel *model);
Distribution *get_source_grain_model(GrainModel *model);
Distribution *get_azimuth_grain_model(GrainModel *model);
Distribution *get_elevation_grain_model(GrainModel *model);
Distribution *get_distance_grain_model(GrainModel *model);
Distribution *get_x_grain_model(GrainModel *model);
Distribution *get_y_grain_model(GrainModel *model);
Distribution *get_z_grain_model(GrainModel *model);
Distribution *get_frequency_grain_model(GrainModel *model);
void set_spatial_mode_grain_model(GrainModel *model, int mode);


Distribution *get_duration_grain_model(GrainModel *model);
Distribution *get_amplitude_grain_model(GrainModel *model);
Distribution *get_attack_grain_model(GrainModel *model);
Distribution *get_decay_grain_model(GrainModel *model);
Distribution *get_shape_grain_model(GrainModel *model);




#endif  