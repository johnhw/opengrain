/**    
    @file envelope.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "envelope.h"



// create a default, blank envelope
Envelope *create_envelope()
{
    Envelope *env = malloc(sizeof(*env));
    
    // some simple defaults
    env->attack = 0.1;
    env->decay = 0.1;
    env->shape = 1.0;
    env->type = ENVELOPE_TYPE_LINEAR;
    env->duration = 1.0;
        
    return env;
}


/* MUST CALL recompute_envelope() AFTER CALLING ANY OF THE FOLLOWING FUNCTIONS */

// set the envelope duration
void set_duration_envelope(Envelope *env, float duration)
{
    env->duration = duration;
    
}

// set the decay parameter
void set_decay_envelope(Envelope *env, float decay)
{
    env->decay = decay;
    
}

// set the attack parameter
void set_attack_envelope(Envelope *env, float attack)
{
    env->attack = attack;
}

// set the shape parameter
void set_shape_envelope(Envelope *env, float shape)
{
    env->shape = shape;

}

// set the envelope type
void set_type_envelope(Envelope *env, int type)
{
    env->type = type;
}

/* END OF recompute_envelope() REQUIRING FUNCTIONS */

// set all envelope parameters
void set_envelope(Envelope *env, int type, float attack, float decay, float shape, float duration)
{
   
    env->type = type;
    env->attack = attack;
    env->decay = decay;
    env->shape = shape;
    
    
    env->duration = duration;
        
    recompute_envelope(env);

}

// recompute envelope coefficients after a change of parameters
void recompute_envelope(Envelope *env)
{
   env->duration_samples = env->duration * GLOBAL_STATE.sample_rate;
   env->mid_samples = env->duration_samples/4;
   env->sample_divide = 1.0/env->duration_samples;
    
    // assign attacks and decays given the absolute or relative values
    if(env->attack<0)
        env->attack_time  = fabs(env->attack);         
    else   
        env->attack_time = env->attack*env->duration;      
    
        
    // decay time is time from the end 
    if(env->decay<0)
        env->decay_time = fabs(env->decay);                    
    else  
        env->decay_time = env->duration * env->decay;        
    
    
    env->attack_stop = env->attack_time * GLOBAL_STATE.sample_rate;
    env->decay_stop = env->duration_samples - env->decay_time * GLOBAL_STATE.sample_rate;
    
    
    
    // linear envelope
    if(env->type == ENVELOPE_TYPE_LINEAR)
    {
        env->coeff_a = (1.0/(env->attack_time*GLOBAL_STATE.sample_rate));
        env->coeff_b = (1.0/(env->decay_time*GLOBAL_STATE.sample_rate));           
    }
    
    
    
    // hamming envelope (no attack/decay/shape)
    if(env->type == ENVELOPE_TYPE_HAMMING)
    {
        env->coeff_a = 2*M_PI/(env->duration_samples-1);
    
    }
    
    // sinc envelope (no attack/decay)
    if(env->type == ENVELOPE_TYPE_SINC)
    {        
        env->coeff_a = (2*env->shape)/GLOBAL_STATE.sample_rate;    
    } 
    
    // FOF envelope (attack/decay)
    if(env->type == ENVELOPE_TYPE_FOF)
    {
        env->coeff_a = 1.0/env->decay_time; // should be decay        
        env->coeff_b = 1.0/env->attack_time;
        env->coeff_c = M_PI / env->coeff_b;
    }
    
    // shape is sharpness, from 0.0 (very sloping) to 100.0 (very straight)
    if(env->type == ENVELOPE_TYPE_EXP)
    {
        env->coeff_a = 1.0 - exp(-1.0 / (env->attack_time*GLOBAL_STATE.sample_rate) / env->shape);
        env->coeff_b = 1.0 - exp(-1.0 / (env->decay_time*GLOBAL_STATE.sample_rate) / env->shape);
        env->coeff_c = 1.0-(1.0/exp(1.0/env->shape));    
    }
    
    
    // Gaussian (shape only)
    if(env->type == ENVELOPE_TYPE_GAUSSIAN)
    {
        env->coeff_a = (env->duration_samples-1)/2;
        env->coeff_b = (1.0-env->shape) * env->coeff_a * 0.5;           
        
    }
    
    // Beta, shape is given by attack/decay components (but all beta distributions are duration relative)
    if(env->type == ENVELOPE_TYPE_BETA)
    {
        env->coeff_a = env->attack-1.0;
        env->coeff_b = env->decay-1.0;    
    }
    
    
        
        
    retrigger_envelope(env);
 

}


void destroy_envelope(Envelope *env)
{
    free(env);
}


void retrigger_envelope(Envelope *env)
{    
    env->phase_samples = 0;
    env->state_a = 0.0;
    env->state_b = 0.0;
}

float compute_envelope(Envelope *env)
{
    double q;
    
    
    // compute the phase in seconds, and in relative duration
    env->phase = env->phase_samples / (double) GLOBAL_STATE.sample_rate;
    env->phase_relative =  env->phase_samples / (double) env->duration_samples;
    env->phase_samples++;    
    
    
    // envelopes are silent outside their duration
    if(env->phase_samples>env->duration_samples)
        return 0.0;
    
    switch(env->type)
    {
        case ENVELOPE_TYPE_NONE:
            return 1.0;
            break;
            
        // attack -> sustain (1.0) -> decay                 
        case ENVELOPE_TYPE_LINEAR:
            if(env->phase_samples<env->attack_stop)
            {
                env->state_a += env->coeff_a;
                return env->state_a;
            }
            else if(env->phase_samples>env->decay_stop)
            {            
                if(env->state_a>0)
                    env->state_a -= env->coeff_b;
                else
                    return 0.0;
                return env->state_a;
            }
            else return 1.0;
        break;            
        
        
        case ENVELOPE_TYPE_BETA:
            return pow(env->phase_relative, env->coeff_a) * pow(1.0-env->phase_relative, env->coeff_a);
            break;
        
        case ENVELOPE_TYPE_EXP:
            if(env->phase_samples<env->attack_stop)
            {
                env->state_a = (1.0-env->coeff_a) * env->state_a + env->coeff_a;
                return env->state_a / env->coeff_c;            
            }                        
            if(env->phase_samples>env->decay_stop)
            {
                             
                env->state_b = (1.0-env->coeff_b) * env->state_b + env->coeff_b;
                return 1.0 - env->state_b / env->coeff_c;            
            }
            else
                return 1.0;
            break;
            
            
        
        case ENVELOPE_TYPE_SINC:                 
            q = (env->phase_samples-env->mid_samples) * env->coeff_a;
            if(fabs(q)<1e-6)
                return 1.0;
            else
                return sin(M_PI*q)/(M_PI*q);            
            break;
        
        
        
        case ENVELOPE_TYPE_FOF:                 
            if(env->phase < env->coeff_c)
            {
                return 0.5 * (1-cos(env->coeff_b*env->phase)) * exp(-env->coeff_a*env->phase);
            }
            else
            return exp(-env->coeff_a*env->phase);
            break;
        
        case ENVELOPE_TYPE_HAMMING:            
            return 0.54 - 0.46 * cos(env->coeff_a * env->phase_samples);
            break;
            
        case ENVELOPE_TYPE_GAUSSIAN:            
            q = ((env->phase_samples-env->coeff_a) / env->coeff_b);
            return exp(-0.5 * (q*q));
            
            break;
        
                                                
    }
    return 0.0;
    
}

//Apply the envelope to the entire buffer passed to it
void envelope_buffer(Envelope *env, Buffer *buffer)
{
    int i;
    for(i=0;i<buffer->n_samples;i++)    
        buffer->x[i] = buffer->x[i] * compute_envelope(env);        
}


