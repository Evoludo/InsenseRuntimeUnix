/*
 * Header file for the Insense radio
 *
 * Paul Harvey (modified by Jon Lewis)
 *
 */
#ifndef INSENSE_RADIO_H
#define INSENSE_RADIO_H

//#include "inceos.h"
#include "unix.h"
#include "AnyType.h"



typedef struct insense_radio{
	void (*decRef)(void *this); // JL - decRef function mustbe first field for GC
    int stopped;
    chan_id broadcastSend_comp;
    chan_id unicastSend_comp;
    chan_id received_comp;
    chan_id fromXmac;
    chan_id toXmac;
}insense_radio_struct, *insense_radio_pntr;


extern insense_radio_pntr radio_glob;

void insense_radio_constructor(void *this, int argc, void *argv[]);

void insense_radio_behaviour(void *this);


#endif /* INSENSE_RADIO_H */


