// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"


#include <iostream>
#include <fstream>
#include <conio.h>
#include <sstream>
#include <windows.h>
#include <map>
#include <time.h> 
#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
#include "opi_uce_win.h"


#define UPDATEPERIOD 30         // number of milliseconds in which to refresh things
#define MAXTIMERCTMISSPKT 120000 //*UPDATEPERIOD*120K=3600" Max timer count before live recording stops
#define LIVEMODE    0           // states of operation
#define SAVEDMODE   1
#define MINSIGDATAPTSTODRAW 64  // minimum number of signal data points to update signal
#define NEWTSEDATACODE 1        // new data code from UCE
#define LIVEFFTLEN   512        // FFT length for live display,
#define LIVEFFTLENOVERLAP 256   // overlap of time data for FFT
#define LIVEFFTDBGAIN   8       // Gain of FFT magnitude dB values
#define LIVEFFTDBOFF    -2       // Offset of FFT magnitude dB values
#define SAMPQUALMASK    0x03    // Bit mask for sample quality in first adc data sample in a packet
#define LEDRED          4
#define LEDORG          3
#define LEDGRN          2
#define LEDBLU          1
#define LEDNON          0       // BLACK or NO display
#define MAXMISSFRMS     15000   // Maximum missing frames to insert in edf >1Hr=14400
#define LIVETSERETRIES  10      // retry times for getting tse data to determine if can use program
#define COMRETRYTIME    10      // milliseconds to wait and retry if usb/com failed

// Relax parameters
// use below as reference and calc/scale to RLEVEL
#define RLEVEL  2 //0 EASY  to 10 HARD
#define THX  1200 //EXCLUSION: 1200 for ReLax APP; 260 for AttN APP
#define THM   260 //M  <THX for ReLax APP; >THX for AttN APP
#define THGM   60 //1G
#define THBM   55 //1B 1x
#define OFFL  300 //LF 1x
#define OFFM   60 //all 4 the same
#define RLSCOREMINPKTS 32 // must get 8"=32 packets to write file/show score


#define FFTSIGNALMIN 1     //the min value of the signal in db
#define FFTSIGNALMAX 254   //the max value of the signal in db
// EDF related
#define EDFDRDURSEC 8   // EDF data record duration in seconds
#define FRMSPERSEC  4   // number of truesense data frames per second
#define EDFEFILETYPE    1
#define UNKNOWNFILETYPE 0


// signal viewer related
#define SIGNALZOOMRATIOINIT     4
#define SIGNALZOOMRATIOMAX      100
#define SIGNALZOOMRATIOMIN      0.25
#define SIGNALMSGX              100 //text location
#define SIGNALMSGY              60  //text location

// activity related
#define SLOWACTWEIGHT   24   // divider of slow activity in activity cal.
#define FASTACTWEIGHT   3   // divider of fast activity
#define SFASTACTWEIGHT  1  // divider of super fast activity in activity calc.
#define ACTOFFSET   0    // offset of activity data when written to file
#define ACTGAIN     1   // gain of activity data when written to file
// savemode
#define NUMPATHFFT 512
#define MAXSECONDINONESCENE 86400 //24Hr
#define MINSECONDINONESCENE 1 //1sec
#define SAMPLERATEADC 256
#define ADCDIGITALMAX 8000
//#define ADCGAINVALUE 2
#define MOUSEINDEXACCURACYRATIO 0.5
#define MOUSEMIDDLEACCURACYRATIO 1.5
#define MOUSEINDEXMOVETHRESHOLD 1
#define MOUSEMIDDLEMOVETHRESHOLD 8
#define FFTKO 8
#define FFTCO -2
// livemode
#define MAX_MIN_ADC 8000 //the absolute value of max and min should be the same 5000
#define INITIALADC  5 //initial value 5 ~ 300sec
#define FFTPENWIDTH 2
#define LEDPENWIDTH 0.5
#define MINSHOWTIME   1    // 1s
#define MAXSHOWTIME 300  // 300s

#define LIVEPVSHOWSECMAX 1200 //PostureViewer shows 20min then clear
#define PVZSCALE 640 // +/-1.25G PostureViewer scaling of z axis

#define ZBWLCHANCT  16  // number of zigbee channels