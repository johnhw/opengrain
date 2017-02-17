/**    
    @file errors.c
    @brief Code for reporting errors and warnings.
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/             
#include "gr.h"
#include "base_api.h"
#include "errors.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>



/** Report a fatal error, which will immediately halt the system and exit the application.
    Format strings like printf().
    @arg code A valid error code from the codes listed in gr_errors.h
    @arg str An additional error string (with any optional printf() style format)
    @arg ... any additional parameters for the format from str
*/
void grFatalError(int code, char *str, ...)
{
    va_list args;
    va_start (args, str);
    
    fprintf(stderr, "Error code: %d\n (%s)", code, opengrain_error_codes[code]);
    vfprintf(stderr, str,args);
    va_end(args);
    
    // shut down and exit
    grShutdown();
    exit(-1);
}

/** Report an error, which will not stop further execution
    Format strings like printf().
    @arg code A valid error code from the codes listed in gr_errors.h
    @arg str An additional error string (with any optional printf() style format)
    @arg ... any additional parameters for the format from str
*/
void grError(int code, char *str, ...)
{
    char error_temp[GR_MAX_ERROR_STRING_LENGTH+1];
    va_list args;
    va_start (args, str);
    gr_context->last_error = code;    
    
    vsnprintf(error_temp, GR_MAX_ERROR_STRING_LENGTH, str, args);
    snprintf(gr_context->last_error_string, GR_MAX_ERROR_STRING_LENGTH, "Error %d (%s): %s", gr_context->last_error, opengrain_error_codes[gr_context->last_error], error_temp);
    va_end(args);
}


/** Clear the current error state (so last_error == GR_ERROR_NONE and last_error_string=="No error")
*/
void grClearError(void)
{
    gr_context->last_error = GR_ERROR_NONE;
    strcpy(gr_context->last_error_string, "No error");
}
