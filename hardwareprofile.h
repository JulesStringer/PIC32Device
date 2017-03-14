/* ************************************************************************** */
/** Descriptive File Name
 MIT License

Copyright (c) 2017 Jules Stringer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

  @Company
    Jules Stringer

  @File Name
    HardwareProfile.h

  @Summary
    Pin definitions for analog audio board using PIC32MM 20 pin.

  @Description
    This file maps pins to application functions
 */
/* ************************************************************************** */

#ifndef _HARDWARE_PROFILE_H    /* Guard against multiple inclusion */
#define _HARDWARD_PROFILE_H

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
#include <GenericTypeDefs.h>
#include <xc.h>

/* Oscillator constants */

#define CLOCK_FREQ          (8000000ul)
#define GetSystemClock()    CLOCK_FREQ			// Hz
// need to check clocks
#define GetInstructionClock()	(GetSystemClock()/1)	// GetSystemClock()/1 for PIC32.  Might need changing if using Doze modes.
#define GetPeripheralClock()	(GetSystemClock()/1)	// GetSystemClock()/1 for PIC32.  Divisor may be different if using a PIC32 since it's configurable.

#define BAUDRATE1 19200
#define BRG_DIV1        4 
#define BRGH1           1 

    
// POT SDI
#define SPI2_SDI_TRIS      (TRISBbits.TRISB8)
#define SPI2_SDI_ANSEL     (PRODL)
#define SPI2_SDI_RPR       (RPINR11bits.SDI2R)
#define SPI2_SDI_RPRVAL    7
// POT SDO
#define SPI2_SDO_TRIS      (TRISBbits.TRISB9)
#define SPI2_SDO_ANSEL     (PRODL)
#define SPI2_SDO_RPR       (RPOR1bits.RP8R)
#define SPI2_SDO_RPRVAL    3
// POT SCK
#define SPI2_SCK_TRIS      (TRISBbits.TRISB12)
#define SPI2_SCK_ANSEL     (ANSELBbits.ANSB12)
#define SPI2_SCK_RPR       (RPOR2bits.RP12R)
#define SPI2_SCK_RPRVAL    4
// POT SS
#define SPI2_SS_TRIS       (TRISBbits.TRISB13)
#define SPI2_SS_ANSEL      (ANSELBbits.ANSB13)
#define SPI2_SS_LAT        (LATBbits.LATB13)
#define SPI2_SS_RPR        (RPOR3bits.RP13R)
#define SPI2_SS_RPRVAL     5
// TX
#define COMM_TX_TRIS      (TRISBbits.TRISB14)
#define COMM_TX_ANSEL     (ANSELBbits.ANSB14)
// RX
#define COMM_RX_TRIS      (TRISBbits.TRISB15)
#define COMM_RX_ANSEL     (ANSELBbits.ANSB15)

// Number of controlled devices
#define MIN_DEVICE 0
void InitialiseBoard();
void InitialiseSPI2();
unsigned long WriteSPI2(unsigned long buf);
unsigned long GetTickCount();
void TickInit();
int GetBoardID();
//void inline ClrWdt();
#define ClrWdt(){ WDTCONSET = 0x1; }
/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
