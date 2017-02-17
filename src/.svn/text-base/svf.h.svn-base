/**    
    @file svf.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __SVF_H__
#define __SVF_H__

#include "audio.h"


#define SVF_LOWPASS 0
#define SVF_HIGHPASS 1
#define SVF_BANDPASS 2
#define SVF_NOTCH 3



typedef struct SVF
{
    double f;
    double res,damp,drive;
    double low, high, band, notch;
    int type;
} SVF;


typedef struct SVFDouble
{
   SVF *svf1, *svf2;
    int doubled;
} SVFDouble;



typedef struct MultichannelSVF
{
   SVF **filters;
   int channels;
} MultichannelSVF;

MultichannelSVF *create_multichannel_SVF(int channels);
void destroy_multichannel_SVF(MultichannelSVF *filter);
void set_freq_multichannel_SVF(MultichannelSVF *filter, float freq);
void set_q_multichannel_SVF(MultichannelSVF *filter, float q);
void set_type_multichannel_SVF(MultichannelSVF *filter, int type);
void set_drive_multichannel_SVF(MultichannelSVF *filter, float drive);
void process_multichannel_SVF(MultichannelSVF *filter, Buffer **ins, Buffer **outs);




SVF *create_SVF();
void destroy_SVF(SVF *svf);
void set_SVF(SVF *svf, double freq, double q, double drive, int type);
void process_SVF(SVF *svf, Buffer *in, Buffer *out);
float compute_SVF(SVF *svf, float sample);
void set_freq_SVF(SVF *svf, double freq);
void set_q_SVF(SVF *svf, double q);
void set_drive_SVF(SVF *svf, double drive);
void set_type_SVF(SVF *svf, int type);


SVFDouble *create_SVFDouble();
void destroy_SVFDouble(SVFDouble *svf);
void set_SVFDouble(SVFDouble *svf, double freq, double q, double drive, int type);
void set_freq_SVFDouble(SVFDouble *svf, double freq);
void set_q_SVFDouble(SVFDouble *svf, double q);
void set_drive_SVFDouble(SVFDouble *svf, double drive);
void set_type_SVFDouble(SVFDouble *svf, int type);
void process_SVFDouble(SVFDouble *svf, Buffer *in, Buffer *out);
float compute_SVFDouble(SVFDouble *svf, float sample);




#endif