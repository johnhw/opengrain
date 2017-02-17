/**    
    @file random_reverb.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "random_reverb.h"



// Set the default parameters for a Random reverberator
void set_default_random_reverb(RandomReverb *reverb)
{
    reverb->decay = 0.9;
    reverb->decay_diffusion_1 = 0.6;
    reverb->decay_diffusion_2 = 0.6;
    reverb->input_diffusion_1 = 0.55;
    reverb->input_diffusion_2 = 0.625;
    reverb->bandwidth = 0.5;
    reverb->damping = 0.05;
    reverb->grit = 0.0;
}



// set the predelay, in seconds
void set_predelay_random_reverb(RandomReverb *reverb, double predelay)
{
    set_mdelay(reverb->pre_delay, predelay*GLOBAL_STATE.sample_rate);
}


// set the bandwidth, from 0.0 to 0.99999
void set_bandwidth_random_reverb(RandomReverb *reverb, double bandwidth)
{
    reverb->bandwidth = bandwidth;
}

// set the damping, from 0.0 to 0.99999
void set_damping_random_reverb(RandomReverb *reverb, double damping)
{
   reverb->damping = damping;
}

// set the decay, from 0.0 to 0.99999
void set_decay_random_reverb(RandomReverb *reverb, double decay)
{
   reverb->decay = decay;
}

// set the first decay diffusion, from 0.0 to 0.99999
void set_decay_diffusion_1_random_reverb(RandomReverb *reverb, double decay_diffusion_1)
{
   reverb->decay_diffusion_1 = decay_diffusion_1;
}

// set the second decay diffusion, from 0.0 to 0.99999
void set_decay_diffusion_2_random_reverb(RandomReverb *reverb, double decay_diffusion_2)
{
   reverb->decay_diffusion_2 = decay_diffusion_2;
}

// set the first input diffusion, from 0.0 to 0.99999
void set_input_diffusion_1_random_reverb(RandomReverb *reverb, double input_diffusion_1)
{
   reverb->input_diffusion_1 = input_diffusion_1;
}

// set the second input diffusion, from 0.0 to 0.99999
void set_input_diffusion_2_random_reverb(RandomReverb *reverb, double input_diffusion_2)
{
  reverb->input_diffusion_2 = input_diffusion_2;
}

static int dattoro_delays [] = {142,379,107,277, 672,908,4453,4217,3720,3163,1800,2656};    
static int dattoro_left_taps [] = {0,0,0,0,0,0,0, 266, 2974, 1913, 1996, 1990, 187, 1066};
static int dattoro_right_taps [] = {0,0,0,0,0,0,0, 353,  3627, 1228, 2673, 2111, 335, 121};



void set_size_random_reverb(RandomReverb *reverb, double factor)
{
   double sr_ratio;
   int i,j, delay;
   reverb->size = factor;
   
      // fix delay lengths for differing sample rates
    sr_ratio = factor * (double)GLOBAL_STATE.sample_rate / 29761.0;
    
    
    if(reverb->random_mode)
    {
        for(i=0;i<12;i++)
        {    
            if(i<4)
                delay = sr_ratio * (uniform_double()*200 + 10);
            else if(i>=4 && i<6)
                delay = sr_ratio * (uniform_double()*500 + 100);        
            else
                delay = sr_ratio * (uniform_double()*3000 + 1000);
            set_mdelay(reverb->delays[i], delay);
            
            // modulating taps
            if(i>=4 && i<6)
            {
                reverb->modulations[i] = uniform_double() * delay * 0.5;
                reverb->modulation_freqs[i] = uniform_double() * 3 + 0.5;                        
            }
            else
            {
                reverb->modulations[i] = 0.0;
                reverb->modulation_freqs[i] = 0.0;               
            }
             
            // set the output taps
            for(j=0;j<reverb->n_channels;j++)            
                reverb->channel_taps[i+12*j] = delay * uniform_double();
            
        }   
    }
    else
    {
        // set it to the exact Dattoro parameters
        for(i=0;i<12;i++)
        {    
            delay = dattoro_delays[i] * sr_ratio;
            set_mdelay(reverb->delays[i], delay);
                                    
            // modulating taps
            if(i>=4 && i<6)
            {
                reverb->modulations[i] = uniform_double() * delay * 0.1;
                reverb->modulation_freqs[i] = uniform_double() * 3 + 0.5;                        
            }
            else
            {
                reverb->modulations[i] = 0.0;
                reverb->modulation_freqs[i] = 0.0;
                
            }            
            
            // set the output taps
            for(j=0;j<reverb->n_channels;j++)            
            {
                if(j==0)
                    reverb->channel_taps[i+12*j] = dattoro_left_taps[i] * sr_ratio;
                else if(j==1)                
                    reverb->channel_taps[i+12*j] = dattoro_right_taps[i] * sr_ratio;
                else
                    reverb->channel_taps[i+12*j] = delay * uniform_double();
                
            }
        }        
    }                
    // update the modulators
    set_modulation_random_reverb(reverb, reverb->modulation_extent);
    
}


// set whether or not this is a Dattoro or random reverb
void set_random_mode_random_reverb(RandomReverb *reverb, int random_mode)
{
    reverb->random_mode = random_mode;
    set_size_random_reverb(reverb, reverb->size);

}

// set the modulation depth of this reverb unit
void set_modulation_random_reverb(RandomReverb *reverb, double modulation)
{
    int i;
    reverb->modulation_extent = modulation;
    for(i=0;i<12;i++)    
        set_modulation_mdelay(reverb->delays[i], reverb->modulations[i]*modulation, reverb->modulation_freqs[i]);                
    

}

// Create a new reverb
RandomReverb *create_random_reverb(int n_channels)
{
    int i;
    RandomReverb *reverb = malloc(sizeof(*reverb));
    reverb->pre_delay = create_mdelay();   
    set_predelay_random_reverb(reverb, 0.001);
    reverb->pre_sample = 0;
    reverb->diffusion_sample_a = 0;
    reverb->diffusion_sample_b = 0;
    reverb->n_channels = n_channels;
    reverb->channel_taps = malloc(sizeof(*reverb->channel_taps)*12*n_channels);
    reverb->random_mode = 0;
        
    for(i=0;i<12;i++)    
        reverb->delays[i] = create_mdelay();
           
    reverb->modulation_extent = 1.0;
    
    set_size_random_reverb(reverb, 1.0);        
    set_default_random_reverb(reverb);
    return reverb;
}

// Destroy a reverb and free all the delay lines
void destroy_random_reverb(RandomReverb *reverb)
{
    int i;
    for(i=0;i<12;i++)    
        destroy_mdelay(reverb->delays[i]);        
        
    free(reverb->channel_taps);
    destroy_mdelay(reverb->pre_delay);
    free(reverb);
}



void compute_one_sample_random_reverb(RandomReverb *reverb, float in)
{
    float x, y, z, p, q;
    

    x = in;
    // Initial computation    
    x = mdelay(reverb->pre_delay, x);
    x = reverb->bandwidth *  x + (1-reverb->bandwidth) * reverb->pre_sample;
    reverb->pre_sample = x;
    
       
    // Sequential part
    
    // delay line 142
    y = mdelay_out(reverb->delays[0]) * reverb->input_diffusion_1;
    z = x - y;
    
    mdelay_in(reverb->delays[0], z);
    x = y + z * reverb->input_diffusion_1;
    
    // delay line 107
    y = mdelay_out(reverb->delays[1]) *  reverb->input_diffusion_1;
    z = x - y;
    
    mdelay_in(reverb->delays[1], z);
    x = y + z *  reverb->input_diffusion_1;
    
    // delay line 379
    y = mdelay_out(reverb->delays[2]);
    z = x - y *  reverb->input_diffusion_2;
    
    mdelay_in(reverb->delays[2], z);
    x = y + z * reverb->input_diffusion_2;
    
    
    
    // delay line 277
    y = mdelay_out(reverb->delays[3]);
    z = x - y *  reverb->input_diffusion_2;    
    mdelay_in(reverb->delays[3], z);
    x = y + z * reverb->input_diffusion_2;
    
    p = reverb->decay * mdelay_out(reverb->delays[8]) + x;
    q = reverb->decay * mdelay_out(reverb->delays[9]) + x;
    
    // P Loop    
    // delay line 672
    
    y = mdelay_out(reverb->delays[4]);
    z = p - y *  reverb->decay_diffusion_1;
    mdelay_in(reverb->delays[4], z);    
    p = y + z * reverb->decay_diffusion_1;



    
    // delay/filter 4453
    p = mdelay(reverb->delays[6], p);
    p = (1-reverb->damping)*p + reverb->damping * reverb->diffusion_sample_a;
    reverb->diffusion_sample_a = p;
    
    p = p * reverb->decay;

    // delay line 1800
    
    y = mdelay_out(reverb->delays[10]);
    z = p - y *  reverb->decay_diffusion_2;
    mdelay_in(reverb->delays[10], z);
    p = y + z * reverb->decay_diffusion_2;

    // delay line 3720
    mdelay_in(reverb->delays[8], p);
    
    
    
    // Q loop    
    // delay line 908
    
    y = mdelay_out(reverb->delays[5]);
    z = q - y *  reverb->decay_diffusion_1;
    mdelay_in(reverb->delays[5], z);
    q = y + z * reverb->decay_diffusion_1;

        
    // delay/filter 4217
    q = mdelay(reverb->delays[7], q);
    q = (1-reverb->damping)*q + reverb->damping * reverb->diffusion_sample_b;
    reverb->diffusion_sample_b = q;
    
    q = q * reverb->decay;
    
    // delay line 2656
    
    y = mdelay_out(reverb->delays[11]);
    z = q - y *  reverb->decay_diffusion_2;
    
    mdelay_in(reverb->delays[11], z);
    q = y + z * reverb->decay_diffusion_2;
    
    // delay line 3720
    mdelay_in(reverb->delays[9], q);
    
}


// Take a set of input signal and compute the Random reverb of it
void compute_random_reverb(RandomReverb *reverb, Buffer **in_buffers, int n_in_channels, Buffer **out_buffers)
{
    float x, t;
    int i, sign;
    int j, k;
    
    for(j=0;j<in_buffers[0]->n_samples;j++)
    {
    
        // get average of input channels
        x = 0.0;
        for(k=0;k<n_in_channels;k++)        
            x += in_buffers[k]->x[j];                
        x /= reverb->n_channels;
        
        compute_one_sample_random_reverb(reverb, x);
        
        // compute outputs
        for(k=0;k<reverb->n_channels;k++)
        {
            int *taps;
            taps = &reverb->channel_taps[12*k];
            t = 0;        
            sign = 1;
            // sum channel taps
            for(i=6;i<12;i++)
            {    
                if(taps[i])
                    t += sign * 0.6 * get_mdelay(reverb->delays[i], taps[i]);
                sign *= -1;    
            }
            
            out_buffers[k]->x[j] = t;
            
        }        
        
    }
    
    
}



