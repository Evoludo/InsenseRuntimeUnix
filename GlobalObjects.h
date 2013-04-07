/*
 * File containing declarations of globals that are assumed to exist
 * by the compiler generated code
 *
 */
#ifndef GLOBAL_OBJS_H_
#define GLOBAL_OBJS_H_

#include "Bool.h"
//#include "lib/sensors.h"

#include "BSTMap.h"
//#include "InsenseRadio.h"

#include "unix.h"
#include "KeyboardComponent.h"
#include "NetworkReceiveComponent.h"
#include "NetworkSendComponent.h"

extern bool success;

// Radio (components for in and out)
extern BSTMap_PNTR serialiserMap;

//extern sensors_PNTR sensors_glob;

// Unix-specific components
extern KeyboardPNTR Keyboard_glob;
extern NetworkReceivePNTR NetworkReceive_glob;
extern NetworkSendPNTR NetworkSend_glob;

// functions
extern void initDALGlobalObjects();
extern void initUnixGlobalObjects();

#endif
