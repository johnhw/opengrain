/**    
    @file resonator_bank.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "resonator_bank.h"


// create a resonator bank
ResonatorBank *create_resonator_bank()
{

    ResonatorBank *bank;
    bank = malloc(sizeof(*bank));    
    bank->resonators = NULL;        
    bank->n_resonators = 0;
    return bank;
}




// destroy a resonator bank
void destroy_resonator_bank(ResonatorBank *bank)
{
    int i;
    for(i=0;i<bank->n_resonators;i++)
        destroy_biquad(bank->resonators[i]);
    if(bank->resonators)
        free(bank->resonators);
    free(bank);
}

// set the number of resonators in this bank
void set_n_resonators_resonator_bank(ResonatorBank *bank, int n)
{
    int i;
    if(bank->resonators)
    {
        for(i=0;i<bank->n_resonators;i++)
            destroy_biquad(bank->resonators[i]);    
        free(bank->resonators);
    }
    bank->n_resonators = n;
    bank->resonators = malloc(sizeof(*bank->resonators)*n);    
    
    // default resonator
    for(i=0;i<bank->n_resonators;i++)
    {
        bank->resonators[i] = create_biquad();
        // 1khz q=1.0 resonator
        biquad_bandpass(bank->resonators[i], 1000.0, 1.0);
    }
}

// set an individual resonator
void set_resonator_bank(ResonatorBank *bank, int n, float freq, float q)
{
    if(n>=0 && n<bank->n_resonators)
        biquad_bandpass(bank->resonators[n], freq, q);
}


// process a sample through all the resonators, in parallel configuration
float compute_resonator_bank(ResonatorBank *bank, float x)
{
    int i;
    float y;
    for(i=0;i<bank->n_resonators;i++)    
        y+=process_biquad(bank->resonators[i], x);   
    return y/bank->n_resonators;  
}

// create a multichannel resonator bank
MultichannelResonatorBank *create_multichannel_resonator_bank(int channels)
{
    int i;
    MultichannelResonatorBank *bank;
    bank = malloc(sizeof(*bank));
    bank->channels = channels;
    bank->resonators = malloc(sizeof(*bank->resonators)*channels);
    for(i=0;i<bank->channels;i++)
        bank->resonators[i] = create_resonator_bank();
        
    return bank;
        
}

// free a multichannel resonator bank
void destroy_multichannel_resonator_bank(MultichannelResonatorBank *bank)
{
   int i;
    for(i=0;i<bank->channels;i++)
        destroy_resonator_bank(bank->resonators[i]);
    free(bank->resonators);
    free(bank);    
}

// set the number of resonators
void set_n_resonators_multichannel_resonator_bank(MultichannelResonatorBank *bank, int n)
{
    int i;
    for(i=0;i<bank->channels;i++)
        set_n_resonators_resonator_bank(bank->resonators[i], n);
}

// 
void set_multichannel_resonator_bank(MultichannelResonatorBank *bank, int n, float freq, float q)
{
   int i;
    for(i=0;i<bank->channels;i++)
        set_resonator_bank(bank->resonators[i], n, freq, q);
}

void process_multichannel_resonator_bank(MultichannelResonatorBank *bank, Buffer **ins, Buffer **outs)
{
    int i,j;
    for(j=0;j<bank->channels;j++)
        for(i=0;i<ins[j]->n_samples;i++)
            outs[j]->x[i] = compute_resonator_bank(bank->resonators[j], ins[j]->x[i]);    
}

