/**    
    @file grain_model.c
    @brief Holds the distributions for the basic parameters for generating grains, and 
    provides the functionality for configuring a grain from those parameter distributions.
    
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "grain_model.h"






/** Create a new grain model object
    and return it.
    @return Newly-allocated GrainModel object.
    */
GrainModel *create_grain_model(void)
{
    GrainModel *model;
    
    model = malloc(sizeof(*model));
        
    model->rate_mode = RATE_MODE_REGULAR;
    
    // distributions
    model->time = create_distribution();
    model->source = create_distribution();
    model->rate = create_distribution();
    
    model->amplitude = create_distribution();
    model->azimuth = create_distribution();
    model->elevation = create_distribution();
    model->x = create_distribution();
    model->y = create_distribution();
    model->z = create_distribution();    
    model->distance = create_distribution();
    model->duration = create_distribution();
    model->attack = create_distribution();
    model->decay = create_distribution();
    model->shape = create_distribution();
    model->frequency = create_distribution();
    model->envelope_type = ENVELOPE_TYPE_NONE;
    model->spatial_mode = SPATIAL_MODE_POLAR;
    return model;
}


/** Set the rate mode for this model (stochastic, regular, or triggered)
    @param model The model to set the rate of
    @param rate_mode The new rate mode
*/
void set_rate_mode_model(GrainModel *model, int rate_mode)
{
    model->rate_mode = rate_mode;
}

/** Set the envelope type for this model. Grains in one stream
    must all share the same envelope type.
    @param model The model to set the rate of
    @param type The new envelope type
*/
void set_envelope_type_model(GrainModel *model, int type)
{
    model->envelope_type = type;
}


/** Delete a grain model and free all of its
    distributions 
    @param model The model to destroy
    */
void destroy_grain_model(GrainModel *model)
{
    
    // destroy the distributions
    destroy_distribution(model->time);
    destroy_distribution(model->rate);
    destroy_distribution(model->source);    
    destroy_distribution(model->amplitude);
    destroy_distribution(model->azimuth);
    destroy_distribution(model->elevation);
    destroy_distribution(model->distance);
    destroy_distribution(model->x);
    destroy_distribution(model->y);
    destroy_distribution(model->z);
    destroy_distribution(model->frequency);
    destroy_distribution(model->duration);
    destroy_distribution(model->attack);
    destroy_distribution(model->decay);
    destroy_distribution(model->shape);
    destroy_distribution(model->duration);        
    free(model);
}



/** Fill in a grain's general parameters. Samples the parameters from the model
    distributions, sets the source, creates the envelope and attaches the
    relevant grain specifics.
    
    @param model The grain model containing the distributions to sample from
    @param sources The list of GrainSources which match the indices in the model->source distribution
    @param when The start time of the grain, in samples
    @param grain The grain structure to fill out   
*/    
void fill_from_grain_model(GrainModel *model, list_t *sources, int when, Grain *grain)
{
    void *specifics;
    GrainSource *source;
    float attack, decay, shape;
    
    float azimuth, elevation, distance;
    float x,y,z;
    
    int type;
    
    source = list_get_at(sources, (int)sample_from_distribution(model->source));
    // if we got a valid source
    if(source!=NULL && source->valid)
    {
        
        // sample parameters
        grain->amplitude = dB_to_gain(sample_from_distribution(model->amplitude));                
        
        if(model->spatial_mode == SPATIAL_MODE_POLAR)
        {
            azimuth = sample_from_distribution(model->azimuth);
            elevation = sample_from_distribution(model->elevation);
            distance = sample_from_distribution(model->distance);
            set_spherical_location(grain->location, azimuth, elevation, distance);           
        }
        else
        {
            x = sample_from_distribution(model->x);
            y = sample_from_distribution(model->y);
            z = sample_from_distribution(model->z);
            set_cartesian_location(grain->location, x,y,z);           
        }

        // set the duration and source
        grain->duration = sample_from_distribution(model->duration);
        grain->source = source;       
        
        attack = sample_from_distribution(model->attack);
        decay = sample_from_distribution(model->decay);
        shape = sample_from_distribution(model->shape);
        type = model->envelope_type;        
        
        grain->frequency = sample_from_distribution(model->frequency);
       
        
        // set the envelope
        set_envelope(grain->envelope, type, attack, decay, shape, grain->duration);
        reset_grain(grain);
        
        // get a grain synthesizer object from a source, and attach it to this grain
        specifics = revive_specifics_source(source, grain);        
        grain->specifics = specifics;
        
        // set the start time
        grain->samples_passed = -when;               
    }
}




/** Return the time in seconds until the next grain from this model
    is due to be triggered.
    @param model The model to sample from
    @return The time in seconds until the next grain, or -1 if never
*/

float next_time_grain_model(GrainModel *model)
{
    float t;   
    t = -1;    
    
    // regular grains
    if(model->rate_mode==RATE_MODE_REGULAR)
    {
            // new grain at regular rate, plus some time blurring from time_distribution
            t =(1.0/sample_from_distribution(model->rate) + sample_from_distribution(model->time));
            
    }                    
    // stochastic grains
    if(model->rate_mode==RATE_MODE_STOCHASTIC)
    {
            t =(sample_from_distribution(model->rate)) ;            
            t =  ((exp_double() / t) + sample_from_distribution(model->time));                        
    }    
    return t;
}


void set_spatial_mode_grain_model(GrainModel *model, int mode)
{
    model->spatial_mode = mode;
}

Distribution *get_frequency_grain_model(GrainModel *model)
{
    return model->frequency;
}

Distribution *get_x_grain_model(GrainModel *model)
{
    return model->x;
}

Distribution *get_y_grain_model(GrainModel *model)
{
   return model->y;
}

Distribution *get_z_grain_model(GrainModel *model)
{
   return model->z;
}



Distribution *get_rate_grain_model(GrainModel *model)
{
    return model->rate;
}

Distribution *get_time_grain_model(GrainModel *model)
{
    return model->time;
}

Distribution *get_source_grain_model(GrainModel *model)
{
    return model->source;
}   

Distribution *get_azimuth_grain_model(GrainModel *model)
{
    return model->azimuth;
}

Distribution *get_elevation_grain_model(GrainModel *model)
{
    return model->elevation;
}

Distribution *get_distance_grain_model(GrainModel *model)
{
    return model->distance;
}

Distribution *get_duration_grain_model(GrainModel *model)
{
    return model->duration;
}

Distribution *get_amplitude_grain_model(GrainModel *model)
{
    return model->amplitude;
}

Distribution *get_attack_grain_model(GrainModel *model)
{
    return model->attack;
}

Distribution *get_decay_grain_model(GrainModel *model)
{
    return model->decay;
}

Distribution *get_shape_grain_model(GrainModel *model)
{
    return model->shape;
}

