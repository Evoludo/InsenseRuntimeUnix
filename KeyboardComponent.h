#include "unix.h"
#include "String.h"

#ifndef KEYBOARD_COMP_H
#define KEYBOARD_COMP_H

typedef struct Keyboard *KeyboardPNTR , KeyboardStruct;
struct Keyboard
{
	void (*decRef)(KeyboardPNTR pntr);
	bool stopped;
	Channel_PNTR output_comp;
};

extern void Construct_Keyboard0( KeyboardPNTR this, int _argc, void* _argv[], pthread_mutex_t* init ) ;
extern void behaviour_Keyboard( KeyboardPNTR this );
#endif

