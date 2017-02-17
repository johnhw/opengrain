/**    
    @file reverb.c
    @brief An implementation of Jon Dattoro's reverb algorithm.
    See J. Dattoro, "Effect Design: Part 1: Reverberator and Other Filters" J. Audio Eng. Soc. 45:9 September 1997
    
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "reverb.h"



// Set the default parameters for a Dattoro reverberator
void set_default_reverb(DattoroReverb *reverb)
{
    reverb->decay = 0.7;
    reverb->decay_diffusion_1 = 0.6;
    reverb->decay_diffusion_2 = 0.6;
    reverb->input_diffusion_1 = 0.55;
    reverb->input_diffusion_2 = 0.625;
    reverb->bandwidth = 0.5;
    reverb->damping = 0.05;
}



// set the predelay, in seconds
void set_predelay_reverb(DattoroReverb *reverb, double predelay)
{
    set_delay(reverb->pre_delay, predelay*GLOBAL_STATE.sample_rate);
}


// set the bandwidth, from 0.0 to 0.99999
void set_bandwidth_reverb(DattoroReverb *reverb, double bandwidth)
{
    reverb->bandwidth = bandwidth;
}

// set the damping, from 0.0 to 0.99999
void set_damping_reverb(DattoroReverb *reverb, double damping)
{
   reverb->damping = damping;
}

// set the decay, from 0.0 to 0.99999
void set_decay_reverb(DattoroReverb *reverb, double decay)
{
   reverb->decay = decay;
}

// set the first decay diffusion, from 0.0 to 0.99999
void set_decay_diffusion_1_reverb(DattoroReverb *reverb, double decay_diffusion_1)
{
   reverb->decay_diffusion_1 = decay_diffusion_1;
}

// set the second decay diffusion, from 0.0 to 0.99999
void set_decay_diffusion_2_reverb(DattoroReverb *reverb, double decay_diffusion_2)
{
   reverb->decay_diffusion_2 = decay_diffusion_2;
}

// set the first input diffusion, from 0.0 to 0.99999
void set_input_diffusion_1_reverb(DattoroReverb *reverb, double input_diffusion_1)
{
   reverb->input_diffusion_1 = input_diffusion_1;
}

// set the second input diffusion, from 0.0 to 0.99999
void set_input_diffusion_2_reverb(DattoroReverb *reverb, double input_diffusion_2)
{
  reverb->input_diffusion_2 = input_diffusion_2;
}

void set_size_reverb(DattoroReverb *reverb, double factor)
{
    double sr_ratio;
    
    // fix delay lengths for differing sample rates
    sr_ratio = factor * (double)GLOBAL_STATE.sample_rate / 29761.0;
    set_delay(reverb->delay_142, 142*sr_ratio);
    
    
    set_delay(reverb->delay_379, 379*sr_ratio);
    
    set_delay(reverb->delay_107, 107*sr_ratio);
    
    set_delay(reverb->delay_277, 277*sr_ratio);
    
    
    set_mdelay(reverb->delay_672, 672*sr_ratio);
    set_delay(reverb->delay_4453, 4453*sr_ratio);
    
    set_delay(reverb->delay_4217, 4217*sr_ratio);
    
    set_delay(reverb->delay_3720, 3720*sr_ratio);
    
    set_delay(reverb->delay_3163, 3163*sr_ratio);
    
    set_delay(reverb->delay_1800, 1800*sr_ratio);
    
    set_delay(reverb->delay_2656, 2656*sr_ratio);
     set_mdelay(reverb->delay_908, 908*sr_ratio);
}


// Create a new reverb
DattoroReverb *create_reverb()
{
    
    DattoroReverb *reverb = malloc(sizeof(*reverb));
    reverb->pre_delay = create_delay();   
    set_predelay_reverb(reverb, 0.001);
    reverb->pre_sample = 0;
    reverb->diffusion_sample_a = 0;
    reverb->diffusion_sample_b = 0;
    

    
    
    reverb->delay_142 = create_delay();
    reverb->delay_379 = create_delay();
    reverb->delay_107 = create_delay();
    reverb->delay_277 = create_delay();
    reverb->delay_672 = create_mdelay();
    reverb->delay_908 = create_mdelay();    
    reverb->delay_4453 = create_delay();
    reverb->delay_4217 = create_delay();
    reverb->delay_3720 = create_delay();
    reverb->delay_3163 = create_delay();
    reverb->delay_1800 = create_delay();
    reverb->delay_2656 = create_delay();
    
    set_size_reverb(reverb, 1.0);
    
    set_modulation_mdelay(reverb->delay_672, 60.0, 1.25);              
    set_modulation_mdelay(reverb->delay_908, 40.0, 4.87);
    
    
    set_default_reverb(reverb);
    return reverb;
}

// set the modulation depth (0.0 -> 1.0)
void set_modulation_reverb(DattoroReverb *reverb, double modulation)
{
   set_modulation_mdelay(reverb->delay_672, 60.0*modulation, 1.25);
   set_modulation_mdelay(reverb->delay_908, 40.0*modulation, 4.87);
 

}

// Destroy a reverb and free all the delay lines
void destroy_reverb(DattoroReverb *reverb)
{

    destroy_delay(reverb->pre_delay);
    
    destroy_delay(reverb->delay_142);
    destroy_delay(reverb->delay_379);
    destroy_delay(reverb->delay_277);
    destroy_mdelay(reverb->delay_672);
    destroy_mdelay(reverb->delay_908);
    destroy_delay(reverb->delay_4453);
    destroy_delay(reverb->delay_4217);
    destroy_delay(reverb->delay_3720);
    destroy_delay(reverb->delay_3163);
    destroy_delay(reverb->delay_1800);
    destroy_delay(reverb->delay_2656);
        
    free(reverb);
}



// Take a stereo signal and compute the Dattoro reverb of it
void compute_reverb(DattoroReverb *reverb, float l, float r, float *out_l, float *out_r)
{
    float x, y, z, p, q, yl, yr;
    
    // Initial computation
    x = (l + r) / 2.0;
    x = delay(reverb->pre_delay, x);
    x = reverb->bandwidth *  x + (1-reverb->bandwidth) * reverb->pre_sample;
    reverb->pre_sample = x;
    
    
    
    // Sequential part
    
    // delay line 142
    y = delay_out(reverb->delay_142) * reverb->input_diffusion_1;
    z = x - y;
    delay_in(reverb->delay_142, z);
    x = y + z * reverb->input_diffusion_1;
    
    // delay line 107
    y = delay_out(reverb->delay_107) *  reverb->input_diffusion_1;
    z = x - y;
    delay_in(reverb->delay_107, z);
    x = y + z *  reverb->input_diffusion_1;
    
    // delay line 379
    y = delay_out(reverb->delay_379);
    z = x - y *  reverb->input_diffusion_2;
    delay_in(reverb->delay_379, z);
    x = y + z * reverb->input_diffusion_2;
    
    
    
    // delay line 277
    y = delay_out(reverb->delay_277);
    z = x - y *  reverb->input_diffusion_2;
    delay_in(reverb->delay_277, z);
    x = y + z * reverb->input_diffusion_2;
    
    p = reverb->decay * delay_out(reverb->delay_3720) + x;
    q = reverb->decay * delay_out(reverb->delay_3163) + x;
    
    // P Loop    
    // delay line 672
    y = mdelay_out(reverb->delay_672);
    z = p - y *  reverb->decay_diffusion_1;
    mdelay_in(reverb->delay_672, z);
    p = y + z * reverb->decay_diffusion_1;
       
    // delay/filter 4453
    p = delay(reverb->delay_4453, p);
    p = (1-reverb->damping)*p + reverb->damping * reverb->diffusion_sample_a;
    reverb->diffusion_sample_a = p;
    
    p = p * reverb->decay;
    
    // delay line 1800
    y = delay_out(reverb->delay_1800);
    z = p - y *  reverb->decay_diffusion_2;
    delay_in(reverb->delay_1800, z);
    p = y + z * reverb->decay_diffusion_2;
    
    // delay line 3720
    delay_in(reverb->delay_3720, p);
    
    
    
    // Q loop    
    // delay line 908
    y = mdelay_out(reverb->delay_908);
    z = q - y *  reverb->decay_diffusion_1;
    mdelay_in(reverb->delay_908, z);
    q = y + z * reverb->decay_diffusion_1;
       
    // delay/filter 4217
    q = delay(reverb->delay_4217, q);
    q = (1-reverb->damping)*q + reverb->damping * reverb->diffusion_sample_b;
    reverb->diffusion_sample_b = q;
    
    q = q * reverb->decay;
    
    // delay line 2656
    y = delay_out(reverb->delay_2656);
    z = q - y *  reverb->decay_diffusion_2;
    delay_in(reverb->delay_2656, z);
    q = y + z * reverb->decay_diffusion_2;
    
    // delay line 3720
    delay_in(reverb->delay_3163, q);
    
    
    
    // left taps
    yl = 0.6 * get_delay(reverb->delay_4217,  266);
    yl += 0.6 * get_delay(reverb->delay_4217, 2974);
    yl -= 0.6 * get_delay(reverb->delay_2656, 1913);
    yl += 0.6 * get_delay(reverb->delay_3163, 1996);
    yl -= 0.6 * get_delay(reverb->delay_4453, 1990);
    yl -= 0.6 * get_delay(reverb->delay_1800, 187);
    yl -= 0.6 * get_delay(reverb->delay_3720, 1066);
    
    
    
    // right taps
    yr = 0.6 * get_delay(reverb->delay_4453, 353);
    yr += 0.6 * get_delay(reverb->delay_4453, 3627);
    yr -= 0.6 * get_delay(reverb->delay_1800, 1228);
    yr += 0.6 * get_delay(reverb->delay_3720, 2673);
    yr -= 0.6 * get_delay(reverb->delay_4217, 2111);
    yr -= 0.6 * get_delay(reverb->delay_2656, 335);
    yr -= 0.6 * get_delay(reverb->delay_3163, 121);
    
    
    
    *out_l = yl;
    *out_r = yr;
    
    
}



