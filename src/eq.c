/**    
    @file eq.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "eq.h"

// Create a stereo EQ object, with no initial eq objects
StereoEQ *create_eq()
{
    StereoEQ *eq = malloc(sizeof(*eq));
    eq->left_low = NULL;
    eq->right_low = NULL;
    eq->left_high = NULL;
    eq->right_high = NULL;
    eq->left_peak_1= NULL;
    eq->right_peak_1 = NULL;
    eq->left_peak_2= NULL;
    eq->right_peak_2 = NULL;
    eq->left_peak_3= NULL;
    eq->right_peak_3 = NULL;    
    return eq;
}


// destroy the eq object, and any sub-biquads if they were created
void destroy_eq(StereoEQ *eq)
{
    if(eq->left_low) free(eq->left_low);
    if(eq->right_low) free(eq->right_low);
    if(eq->left_high) free(eq->left_high);
    if(eq->right_high) free(eq->right_high);
    
    if(eq->left_peak_1) free(eq->left_peak_1);
    if(eq->right_peak_1) free(eq->right_peak_1);
    if(eq->left_peak_2) free(eq->left_peak_2);
    if(eq->right_peak_2) free(eq->right_peak_2);
    if(eq->left_peak_3) free(eq->left_peak_3);
    if(eq->right_peak_3) free(eq->right_peak_3);
    
    
    free(eq);
}


// process a stereo sample
void compute_eq(StereoEQ *eq, float l, float r, float *out_l, float *out_r)
{
       if(eq->left_low) l=process_biquad(eq->left_low, l);
       if(eq->right_low) r=process_biquad(eq->right_low, r);
       
       if(eq->left_high) l=process_biquad(eq->left_high, l);
       if(eq->right_high) r=process_biquad(eq->right_high, r);
       
       if(eq->left_peak_1) l=process_biquad(eq->left_peak_1, l);
       if(eq->right_peak_1) r=process_biquad(eq->right_peak_1, r);
       if(eq->left_peak_2) l=process_biquad(eq->left_peak_2, l);
       if(eq->right_peak_2) r=process_biquad(eq->right_peak_2, r);
       if(eq->left_peak_3) l=process_biquad(eq->left_peak_3, l);
       if(eq->right_peak_3) r=process_biquad(eq->right_peak_3, r);
       
       
       *out_l = l;
       *out_r = r;

}


// set the low-shelf eq, and enable low-shelfing if it wasn't already enabled
void set_low_eq(StereoEQ *eq, float freq, float boostdB)
{

    if(freq==0.0 || boostdB==0.0)
    {
        if(eq->left_low) destroy_biquad(eq->left_low);
        if(eq->right_low) destroy_biquad(eq->right_low);
        eq->left_low = NULL;
        eq->right_low = NULL;        
        return;
    }
    if(!eq->left_low) eq->left_low = create_biquad();
    if(!eq->right_low) eq->right_low = create_biquad();
    biquad_lowshelf(eq->left_low , freq, boostdB, 0.5);
    biquad_lowshelf(eq->right_low , freq, boostdB, 0.5);
   
}


// set the high-shelf eq, and enable high-shelfing if it wasn't already enabled
void set_high_eq(StereoEQ *eq, float freq, float boostdB)
{
     if(freq==0.0 || boostdB==0.0)
    {
        if(eq->left_high) destroy_biquad(eq->left_high);
        if(eq->right_high) destroy_biquad(eq->right_high);
        eq->left_high = NULL;
        eq->right_high = NULL;
        
        return;
    }

    if(!eq->left_high) eq->left_high = create_biquad();
    if(!eq->right_high) eq->right_high = create_biquad();
    biquad_highshelf(eq->left_high , freq, boostdB, 0.5);
    biquad_highshelf(eq->right_high , freq, boostdB, 0.5);

}

// set the peaking eq, and enable peaking if it wasn't already enabled
void set_peak_eq_1(StereoEQ *eq, float freq, float boostdB)
{
    if(freq==0.0 || boostdB==0.0)
    {
        if(eq->left_peak_1) destroy_biquad(eq->left_peak_1);
        if(eq->right_peak_1) destroy_biquad(eq->right_peak_1);
        eq->left_peak_1 = NULL;
        eq->right_peak_1 = NULL;
        
        return;
    }
    if(!eq->left_peak_1) eq->left_peak_1 = create_biquad();
    if(!eq->right_peak_1) eq->right_peak_1 = create_biquad();
    biquad_peaking(eq->left_peak_1 , freq, boostdB, 0.5);
    biquad_peaking(eq->right_peak_1 , freq, boostdB, 0.5);
}


// set the peaking eq, and enable peaking if it wasn't already enabled
void set_peak_eq_2(StereoEQ *eq, float freq, float boostdB)
{
    if(freq==0.0 || boostdB==0.0)
    {
        if(eq->left_peak_2) destroy_biquad(eq->left_peak_2);
        if(eq->right_peak_2) destroy_biquad(eq->right_peak_2);
        eq->left_peak_2 = NULL;
        eq->right_peak_2 = NULL;
        
        return;
    }
    if(!eq->left_peak_2) eq->left_peak_2 = create_biquad();
    if(!eq->right_peak_2) eq->right_peak_2 = create_biquad();
    biquad_peaking(eq->left_peak_2 , freq, boostdB, 0.5);
    biquad_peaking(eq->right_peak_2 , freq, boostdB, 0.5);
}

// set the peaking eq, and enable peaking if it wasn't already enabled
void set_peak_eq_3(StereoEQ *eq, float freq, float boostdB)
{
    if(freq==0.0 || boostdB==0.0)
    {
        if(eq->left_peak_3) destroy_biquad(eq->left_peak_3);
        if(eq->right_peak_3) destroy_biquad(eq->right_peak_3);
        eq->left_peak_3 = NULL;
        eq->right_peak_3 = NULL;
        
        return;
    }
    if(!eq->left_peak_3) eq->left_peak_3 = create_biquad();
    if(!eq->right_peak_3) eq->right_peak_3 = create_biquad();
    biquad_peaking(eq->left_peak_3 , freq, boostdB, 0.5);
    biquad_peaking(eq->right_peak_3 , freq, boostdB, 0.5);
}



