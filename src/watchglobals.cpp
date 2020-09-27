// Cross-platform initialization of global objects
//
// See comment in watchglobals.h
// This is a single location to set NULL values to help find errors in startup code.

#include "watchglobals.h"

#ifndef WATCH_SIM
PCF8563_Class * rtc = nullptr;
#endif
