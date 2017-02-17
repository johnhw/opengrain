/**    
    @file errors.h
    @brief Code for reporting errors and warnings.
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/             

#ifndef __ERRORS_H__
#define __ERRORS_H__

/** Report a fatal error, which will immediately halt the system and exit the application.
    Format strings like printf().
    @arg code A valid error code from the codes listed in gr_errors.h
    @arg str An additional error string (with any optional printf() style format)
    @arg ... any additional parameters for the format from str
*/
void grFatalError(int code, char *str, ...);

/** Report an error, which will not stop further execution
    Format strings like printf().
    @arg code A valid error code from the codes listed in gr_errors.h
    @arg str An additional error string (with any optional printf() style format)
    @arg ... any additional parameters for the format from str
*/
void grError(int code, char *str, ...);


/** Clear the current error state (so last_error == GR_ERROR_NONE and last_error_string=="No error")
*/
void grClearError(void);



#endif