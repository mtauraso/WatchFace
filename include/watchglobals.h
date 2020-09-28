
// This defines all the global pointers which access watch hardware
// This is so they can be accessed by GUI code
//
// For the most part the pattern we are going for is a global object pointer
// which has esp32 and windows implementations. The commitment of the init code (main/watchface.cpp) 
// is to initialize these pointers to non-null values, and instantiate the objects they point to
// such that they are callable by cross platform GUI code.

// watchglobals.cpp simply sets all these pointers to null values to help find errors in startup code

#ifndef WATCHGLOBALS_H
#define WATCHGLOBALS_H

// For RTC
#ifdef WATCH_SIM
#include "win/rtc.h"
#else
#include "esp32/config.h"
#endif
extern PCF8563_Class * rtc;


// For ps_malloc on the watch sim
#ifdef WATCH_SIM
#include <cstdlib>
#define ps_malloc malloc
#endif


#endif