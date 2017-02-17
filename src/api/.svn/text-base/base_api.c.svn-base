/**    
    @file base_api.c
    @brief Implementation of the basic parts of the OpenGrain API (init, shutdown, error reporting)
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/             

#include "base_api.h"
#include "../sys_audio.h"
#include "errors.h"
#include "audio_api.h"
GRContext *gr_context;


/** Set the default settings for the context.
    Sets all flags to the default state, creates all of the object lists,
    initialises the audio device prototype.
    @arg context The context to initialise
*/
static void setDefaultGrContext(GRContext *context)
{
    int i;
    
    // first disable everything
    for(i=0;i<GR_MAX_FLAGS;i++)
        context->global_flags[i] = 0;
    
    
    
}



/** Initialises the OpenGrain system. This must be called _before_ any other OpenGrain calls. */
void grInit(void)
{
    gr_context = malloc(sizeof(*gr_context));
    if(!gr_context)
    {
        // can't use grFatalError because that relies on a working context!
        fprintf(stderr, "Fatal error: Out of memory when allocating context.\n");
        exit(-1);
    }    
    grClearError();
    setDefaultGrContext(gr_context);  
    set_default_audio_api();    
    pre_init_sys_audio();
}

/** Shuts down the OpenGrain system, stopping playback, releasing audio devices, shutting down all
    playback threads and freeing all allocated memory */
void grShutdown(void)
{
    post_shutdown_sys_audio();
    free(gr_context->prototype);
    free(gr_context);
}

/** 
    Return the last error that occured.
    @return The integer error code, or 0 if no error 
**/
int grGetLastError(void)
{
    return gr_context->last_error;

}

/** Return the last error that occured as a string description.
    @return The description of the errorm or "No error" if no error.
**/
const char *grGetLastErrorString(void)
{
    if(gr_context->last_error>=0 && gr_context->last_error<=GR_MAX_ERROR_CODE)
        return opengrain_error_codes[gr_context->last_error];
    else
        return "Bad error code!";
}

/** Get the last error message in full, with any additional information
    that the error might have
*/
char *grGetLastErrorMessage(void)
{
    return gr_context->last_error_string;
}



/** Enable one of the global flags.
    @arg flag Flag to enable 
*/
void grEnable(int flag)
{
    if(flag>=0 && flag<=GR_MAX_FLAGS)    
        gr_context->global_flags[flag] = 1;
   else
        grError(GR_ERROR_BAD_FLAG, "Invalid flag in grEnable()");
    
}

/** Disable one of the global flags.
    @arg flag Flag to disable 
*/
void grDisable(int flag)
{
   if(flag>=0 && flag<=GR_MAX_FLAGS)    
        gr_context->global_flags[flag] = 0;
    else
        grError(GR_ERROR_BAD_FLAG, "Invalid flag in grDisable()");
    

}

/** Query the state of a global flag.
    Returns 1 if the given flag is enabled, 0 otherwise.
    @arg flag Flag to query
    @return 0 if flag is disabled, 1 if it is enabled
*/
int grIsEnabled(int flag)
{
  if(flag>=0 && flag<=GR_MAX_FLAGS)    
       return gr_context->global_flags[flag];
  else
  {
    grError(GR_ERROR_BAD_FLAG, "Invalid flag in grIsEnabled()"); 
    return 0;
  }
}
