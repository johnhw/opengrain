/**    
    @file test_initshutdown.c
    @brief Simplest OpenGrain test. Tests that the library can be initialised and then shutdown.
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/       

#include <stdio.h>
#include <stdlib.h>
#include <gr.h>


int main(int argc, char **argv)
{
    grInit();
    
    printf("grInit()\n");
    
    //This a test, so let's be paranoid
    if(grGetLastError() != GR_ERROR_NONE)    
        fprintf(stderr, "%s", grGetLastErrorMessage());
        
    grShutdown();
    printf("grShutdown()\n");
        
    return 0;
}