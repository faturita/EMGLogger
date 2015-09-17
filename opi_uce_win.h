/*** ****************************
  * HW interface functions header file
  * OP Innovations
  *
  * v1.10 - 20131121	mpeng
  * --------------------------
  * 1. Added "on" and "off" mode setting for controller (UC).
  * 2. Added micro SD slot SPI interface on and off commands.
  * 3. Added opitsign trigger command (needs uSD SPI interface to be turned off
  * 4. Added battery cycling command
  *
  * v1.00 - 20130520	mpeng
  * --------------------------
  * 1. Release version
  *
  * ****************************/

#ifndef OPI_UCE_WIN_H
#define OPI_UCE_WIN_H

/***
  * Include Files
  */
#include <windows.h>	// for com port opening related functions
#include <stdio.h>	


/***
  * Definitions
  */
#define DSNLEN		5	// Device Serial Number length in bytes
#define TSLEN		6	// Time stamp length in bytes
#define FWVLEN		2 	// Firmware version length in bytes
#define PDNLISTLEN	8	// Number of PDNs that can be paired
#define PDNINFOLEN  12	// Length of info bytes for each PDN stored in UC
#define WLFRMHDRLEN	2	// Length of the wireless frame header in bytes
#define WFRMHDRLEN	2   // Length of the wired frame header in bytes
#define TSEFRMLEN	145 // Usual length of the opipkt with truesense education data in bytes
#define EVFRMLEN    9   // Length of an wired frame with single event in bytes
#define OPIUCESTLEN 128 // Number of bytes in a UC status packet including dataCode
#define ADCLEN		64	// Usual number of ADC samples
#define TMPLEN		1   // Length of temperature data in bytes
#define	ACCDLEN		6   // Length of accelerometer data in bytes (1 x, 1 y, 4 z)
#define ACCLEN		4   // Length of z-axis accelerometer data in bytes
#define OPIHDRLEN   512 // Number of bytes at the beginning of OPI file for header
#define SYNCBYTE	0x33	// Byte value for sync bytes
#define UCERTCFREQ  4096    // Frequency of real-time clock on Unicon
#define TSERTCFREQ  256     // Frequency of real-time clock on TrueSense
#define UCEFFTLEN   32  // FFT size done on UCE divide by 2 since symmetrical


/***
  * Structure definition
  */
typedef struct OPIPKT_struct
{
    unsigned char dataCode;  	// dataCode, 1 byte
    unsigned short length;		// length of payload, 2 bytes
    unsigned char payload[1024];	// payload array, 1024 byte array
                                    //(should never be greater than 1K)
} OPIPKT_t;


/***
  * Prototypes
  */
int opipkt_get_com(OPIPKT_t* pktptr, HANDLE *comportptr);
int opipkt_put_com(OPIPKT_t* pktptr, HANDLE* comportptr);
int opi_openuce_com(HANDLE* comportptr);
void opi_closeuce_com(HANDLE* comportptr);
void opipkt_dump(OPIPKT_t* pktptr);
int opiuce_status(HANDLE *comportptr, OPIPKT_t* pktptr);
int opiuce_tsestatus(HANDLE *comportptr, OPIPKT_t* pktptr);
int opiuce_settsepdn(HANDLE *comportptr, int pdn);
int opiuce_settsezbchan(HANDLE *comportptr, int zbChan);
int opiuce_settserfmode(HANDLE *comportptr, int rfmode);
int opiuce_settserftxpwr(HANDLE *comportptr, int rfmode);
int opiuce_settsemmwrite(HANDLE *comportptr, int mmwrite);
int opiuce_settserftxtimeout(HANDLE *comportptr, int rftxtimeout);
int opiuce_settsertc(HANDLE *comportptr);
int opiuce_setzbchan(HANDLE *comportptr, int zbChan);
int opiuce_setpktts(HANDLE *comportptr, int* timeStamp);
int opiuce_copytsesettings(HANDLE *comportptr, int pdnslot);
int opiuce_forgettsesettings(HANDLE *comportptr, int pdnslot);
int opiuce_getwltsedata(HANDLE *comportptr, OPIPKT_t* pktptr);
int opiuce_wlmeasure(HANDLE *comportptr, OPIPKT_t* pktptr);
int opiuce_evcapread(HANDLE *comportptr, OPIPKT_t* pktptr);
int opiuce_evcaperase(HANDLE *comportptr);
int opiuce_mmerasest(HANDLE *comportptr);
int opiuce_mmeraseend(HANDLE *comportptr);
int opiuce_get5mmtsdata(HANDLE *comportptr, int pktNumber, OPIPKT_t* pktptr);
int opiuce_turnmodoff(HANDLE *comportptr);
int opiuce_turnmodon(HANDLE *comportptr);
int opiuce_shutdown(HANDLE *comportptr);
int opiuce_getrelaxdata(HANDLE *comportptr, OPIPKT_t *pktptr);
int opiuce_resetrelaxdata(HANDLE *comportptr);
int opiuce_getwltsefft(HANDLE *comporptr, OPIPKT_t *pktptr);
int opiuce_getrelaxparams(HANDLE *comportptr, OPIPKT_t *pktptr);
int opiuce_setrelaxparams(HANDLE *comportptr, int thx, int thm, int offl, int th3gm, int th3bm, int th2gm, int th2bm, int th1gm, int th1bm, int offm);
int opiuce_offmode(HANDLE *comportptr);
int opiuce_onmode(HANDLE *comportptr);
int opiuce_turnusdspion(HANDLE *comportptr);
int opiuce_turnusdspioff(HANDLE *comportptr);
int opiuce_triggertsign(HANDLE *comporptr, OPIPKT_t *pktptr);
int opiuce_battcycle(HANDLE *comportptr);

#endif // opi_uce_win_H
