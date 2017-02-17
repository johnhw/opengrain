## OpenGrain 
## Copyright 2011 John Williamson
## License: BSD3


## OpenGrain build file

cmake_minimum_required(VERSION 2.6)
project(OPENGRAIN)



# version numbering
set (OPENGRAIN_VERSION_MAJOR 0)
set (OPENGRAIN_VERSION_MINOR 1)

set(CMAKE_BUILD_TYPE Debug )
message("Creating build files for OpenGrain v${OPENGRAIN_VERSION_MAJOR}.${OPENGRAIN_VERSION_MINOR}")
message("   Build type: ${CMAKE_BUILD_TYPE}")



  

 



# directory of Bob Burtle's rand.c (included in the source)
set(RAND_DIR "${OPENGRAIN_SOURCE_DIR}/extern/rand")

# directory of PortAudio's Pa_RingBuffer.c (included in the source)
set(RINGBUFFER_DIR "${OPENGRAIN_SOURCE_DIR}/extern/ringbuffer")

# directory of KISSFFT (included in source)
set(KISSFFT_DIR "${OPENGRAIN_SOURCE_DIR}/extern/kiss_fft129")

# the directory where simclist is installed (included in source)
set(SIMCLIST_DIR "${OPENGRAIN_SOURCE_DIR}/extern/simclist-1.5")


# the directory where portaudio is installed
set(PORTAUDIO_DIR "${OPENGRAIN_SOURCE_DIR}/portaudio")

# the directory where libsndfile is installed
set(LIBSNDFILE_DIR "c:/program files/libsndfile")

# the directory where libresample is installed
set(LIBRESAMPLE_DIR "${OPENGRAIN_SOURCE_DIR}/libresample-0.1.3")



 
# gcc flags
if(CMAKE_COMPILER_IS_GNUCXX)
    
    set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Werror -std=gnu89")
    set(CMAKE_C_FLAGS_RELEASE "-O3 -Wall -Werror -std=gnu89")
endif()


# the includes
include_directories(${OPENGRAIN_SOURCE_DIR} ${OPENGRAIN_SOURCE_DIR}/api ${PORTAUDIO_DIR}/include ${LIBSNDFILE_DIR}/include
                    ${OPENGRAIN_SOURCE_DIR}/extern ${LIBRESAMPLE_DIR}/include ${SIMCLIST_DIR} ${KISSFFT_DIR} ${KISSFFT_DIR}/tools ${RINGBUFFER_DIR} ${RAND_DIR})
              
# link directories          
link_directories(${PORTAUDIO_DIR}/lib ${LIBSNDFILE_DIR} ${LIBRESAMPLE_DIR}/lib ${LIBRESAMPLE_DIR})



# default use port audio
if(DEFINED USE_PORTAUDIO)
else()
    set(USE_PORTAUDIO 1)
endif()

# default use libresample
if(DEFINED USE_LIBRESAMPLE)
else()
    set(USE_LIBRESAMPLE 1)
endif()

if(DEFINED USE_LIBSNDFILE)
else()
    set(USE_LIBSNDFILE 1)
endif()

# use the portaudio library

if(USE_PORTAUDIO)
    message("   Using PortAudio for audio output")
    set (USE_AUDIO_LIBRARY "#define USE_PORTAUDIO 1")
endif()


if(USE_LIBSNDFILE)
    message("   Using libsndfile for file loading")
    set(USE_LIBSNDFILE_LIBRARY "#define USE_LIBSNDFILE 1")
endif()

# use the libresample library
FIND_LIBRARY(LIBRESAMPLE "libresample" ${LIBRESAMPLE_DIR})

if(USE_LIBRESAMPLE AND NOT LIBRESAMPLE STREQUAL "LIBRESAMPLE-NOTFOUND")        
    message("   Using Libresample for resampling")
    set (USE_LIBRESAMPLE_LIBRARY "#define USE_LIBRESAMPLE 1")
else()
    message("   WARNING: No libresample, using low-quality linear interpolation for wave file loading")

endif()


# configure the version header file
configure_file (
  ${OPENGRAIN_SOURCE_DIR}/src/opengrain.h.in
  ${OPENGRAIN_SOURCE_DIR}/src/opengrain.h
  )



# compile the source
add_subdirectory(src)

message("---------------------------------------------------------------")


project(OPENGRAIN_TESTS)
message("Building tests...")
add_subdirectory(tests)
add_subdirectory(python)
