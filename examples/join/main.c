/*******************************************************************************
 * Copyright (c) 2014-2015 IBM Corporation.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    IBM Zurich Research Lab - initial API, implementation and documentation
 *******************************************************************************/

#include "../../lmic/lmic.h"
#include "../../hal/debug.h"
#include <stdio.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "retargetserial.h"

//////////////////////////////////////////////////
// CONFIGURATION (FOR APPLICATION CALLBACKS BELOW)
//////////////////////////////////////////////////

// application router ID (LSBF)
static const u1_t DEVEUI[8]  = { 0xCD, 0x51, 0x02, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };//4000a0e70b017cbe : TTN DEVICE EUI

// unique device ID (LSBF)
static const u1_t APPEUI[8]  = { 0xBE, 0x7C, 0x01, 0x0B, 0xE7, 0xA0, 0x00, 0x45 };//4500a0e70b017cbe : TTN DEVICE ID

// device-specific AES key (derived from device EUI)
static const u1_t DEVKEY[16] = { 0x67, 0xB1, 0xC9, 0xAF, 0x50, 0x74, 0xF6, 0x40, 0xB4, 0x27, 0x34, 0x1A, 0xC8, 0xC2, 0xC3, 0x62 };


//////////////////////////////////////////////////
// APPLICATION CALLBACKS
//////////////////////////////////////////////////

// provide application router ID (8 bytes, LSBF)
void os_getArtEui (u1_t* buf) {
    memcpy(buf, DEVEUI, 8);
}

// provide device ID (8 bytes, LSBF)
void os_getDevEui (u1_t* buf) {
    memcpy(buf, APPEUI, 8);
}

// provide device key (16 bytes)
void os_getDevKey (u1_t* buf) {
    memcpy(buf, DEVKEY, 16);
}


//////////////////////////////////////////////////
// MAIN - INITIALIZATION AND STARTUP
//////////////////////////////////////////////////

// initial job
static void initfunc (osjob_t* j) {
    // reset MAC state
    LMIC_reset();
    // start joining
    LMIC_startJoining();
    // init done - onEvent() callback will be invoked...
}


// application entry point
int main () {
    osjob_t initjob;

    /* Chip errata */
    CHIP_Init();
    RETARGET_SerialInit();
    /* Infinite loop */

  	printf("\n Welcome Lora Sample program \n");


    // initialize runtime env
    os_init();
    // initialize debug library
    debug_init();
    // setup initial job
    os_setCallback(&initjob, initfunc);
    // execute scheduled jobs and events
    os_runloop();
    // (not reached)
    return 0;
}


//////////////////////////////////////////////////
// UTILITY JOB
//////////////////////////////////////////////////

static osjob_t blinkjob;
static u1_t ledstate = 0;

static void blinkfunc (osjob_t* j) {
    // toggle LED
    ledstate = !ledstate;
    debug_led(ledstate);
    // reschedule blink job
    os_setTimedCallback(j, os_getTime()+ms2osticks(100), blinkfunc);
}


//////////////////////////////////////////////////
// LMIC EVENT CALLBACK
//////////////////////////////////////////////////

void onEvent (ev_t ev) {
    debug_event(ev);

    printf("onEvent Enter= %d\n",ev);

    switch(ev) {

      // starting to join network
      case EV_JOINING:
          // start blinking
          //blinkfunc(&blinkjob);
          break;
          
      // network joined, session established
      case EV_JOINED:
    	  printf("\r\n ############################## EV_JOINED ############################## \n",ev);

          // cancel blink job
          //os_clearCallback(&blinkjob);
          // switch on LED
          //debug_led(1);
          // (don't schedule any new actions)
    	    LMIC.frame[0] = 'A';
    	    LMIC.frame[1] = 'B';
    	    LMIC.frame[1] = 'C';
    	    LMIC_setTxData2(1, LMIC.frame, 3, 1); // (port 1, 2 bytes, unconfirmed)
          break;
      case EV_JOIN_FAILED:
    	  printf("\r\n EV_JOIN_FAILED \n",ev);
    	  printf("\r\n EV_JOIN_FAILED \n",ev);
    	  printf("\r\n EV_JOIN_FAILED \n",ev);
    	  printf("\r\n EV_JOIN_FAILED \n",ev);
    	  break;
    }
}
