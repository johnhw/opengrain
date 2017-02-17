/**    
    @file resonator_bank.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __RESONATOR_BANK_H__
#define __RESONATOR_BANK_H__
#include "audio.h"
#include "biquad.h"



typedef struct ResonatorBank
{    
    Biquad **resonators;
    int n_resonators;
} ResonatorBank;

typedef struct MultichannelResonatorBank
{
    ResonatorBank **resonators;
    int channels;
} MultichannelResonatorBank;

ResonatorBank *create_resonator_bank(void);
void destroy_resonator_bank(ResonatorBank *bank);
void set_n_resonators_resonator_bank(ResonatorBank *bank, int n);
void set_resonator_bank(ResonatorBank *bank, int n, float freq, float q);
float compute_resonator_bank(ResonatorBank *bank, float x);

MultichannelResonatorBank *create_multichannel_resonator_bank(int channels);
void destroy_multichannel_resonator_bank(MultichannelResonatorBank *bank);
void set_n_resonators_multichannel_resonator_bank(MultichannelResonatorBank *bank, int n);
void set_multichannel_resonator_bank(MultichannelResonatorBank *bank, int n, float freq, float q);
void process_multichannel_resonator_bank(MultichannelResonatorBank *bank, Buffer **ins, Buffer **outs);





#endif

