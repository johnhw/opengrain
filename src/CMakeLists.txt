## OpenGrain 
## Copyright 2011 John Williamson
## License: BSD3



# choose either BSD sockets or WinSock
# SimCList requires some of the definitions in these libraries
if(WIN32)
    set(SOCKET_LIB ws2_32)
else()
    set(SOCKET_LIB socket)
endif()

# set libraries used
set(OPENGRAIN_LIBRARIES libsndfile-1;${SOCKET_LIB})

# link with libresample if required
if(USE_LIBRESAMPLE)    
    list(APPEND OPENGRAIN_LIBRARIES libresample)
endif()


# default is the dummy audio driver
set(SYS_AUDIO sys_dummy_audio.c)
set(SOUNDFILE simplewav.c)

# if we're using portaudio, use sys_portaudio.c and link with portaudio_x86
if(USE_PORTAUDIO)
    list(APPEND OPENGRAIN_LIBRARIES portaudio_x86)
    set(SYS_AUDIO sys_portaudio.c)
endif()

if(USE_LIBSNDFILE)
    set(SOUNDFILE libsndfile.c)
endif()


set(OPENGRAIN_SOURCE_FILES)

## the list of source files making up opengrain
set(OPENGRAIN_ORIG_SOURCE_FILES 
${SYS_AUDIO} 
${SOUNDFILE}
api/errors
api/base_api
api/audio_api
api/error_codes
location
matrix
grain_model
complex_buffer 
utils 
random_reverb 
single_distribution 
impulsetrigger 
trigger 
multisinegrain
pluckgrain
convolver 
glissgrain 
buffer 
compressor 
padsyngrain 
impulsegrain
wavegrain
wavereader
noisegrain 
spatializer 
stream_fx 
rms 
sinegrain 
grain 
crossdelay 
grain_source 
wavewriter 
widener 
envelope 
resonator_bank 
grain_stream  
grainmixer 
distributions 
random 
eq 
allpass 
biquad 
moddelayline 
svf 
audio 
reverb 
delayline 
pitchtrigger
feedbackdelay 
output 
fmgrain
dsfgrain
rc_generator
trigger_tests
grain_tests
chorus
hrtf
analoggrain
voicegrain
)


## default is not to use optimized version
if(NOT OPENGRAIN_OPTIMIZED)
    set(OPENGRAIN_OPTIMIZED 0)
endif()

## choose whether to select the source file as listed
## or whether to append _opt and use the optimized version
## optimized versions are only usef if OPENGRAIN_BUILD_OPTIMIZED 
## is true
if(${OPENGRAIN_OPTIMIZED})
    message("   Building OPTIMIZED version")
else()
    message("   Building unoptimized version")
endif()

foreach(SRC_FILE IN LISTS OPENGRAIN_ORIG_SOURCE_FILES)
    if(EXISTS "${SRC_FILE}_opt.c" AND ${OPENGRAIN_OPTIMIZED})
        list(APPEND OPENGRAIN_SOURCE_FILES ${SRC_FILE}_opt)
        message("   Using optimized file ${SRC_FILE}_opt.c instead of ${SRC_FILE}")
        
    else()
        list(APPEND OPENGRAIN_SOURCE_FILES ${SRC_FILE})
    endif()
endforeach(SRC_FILE)


## opengrain + the external included source files

# add_executable(test_opengrain 
# ${OPENGRAIN_SOURCE_FILES}
# ${RAND_DIR}/rand.c 
# ${SIMCLIST_DIR}/simclist.c 
# ${RINGBUFFER_DIR}/pa_ringbuffer.c
# ${KISSFFT_DIR}/kiss_fft.c 
# ${KISSFFT_DIR}/tools/kiss_fftr.c 
# )


add_library(opengrain 
${OPENGRAIN_SOURCE_FILES}
${RAND_DIR}/rand.c 
${SIMCLIST_DIR}/simclist.c 
${RINGBUFFER_DIR}/pa_ringbuffer.c
${KISSFFT_DIR}/kiss_fft.c 
${KISSFFT_DIR}/tools/kiss_fftr.c 
)



# set the link libraries
# target_link_libraries(test_opengrain ${OPENGRAIN_LIBRARIES})
lapi.o lcode.o ldebug.o ldo.o ldump.o lfunc.o lgc.o llex.o lmem.o 
	lobject.o lopcodes.o lparser.o lstate.o lstring.o ltable.o ltm.o  
	lundump.o lvm.o lzio.o $(COCO_O)
lauxlib.o lbaselib.o ldblib.o liolib.o lmathlib.o loslib.o ltablib.o 
	lstrlib.o loadlib.o linit.o lcoco.o


