/* ************************************************************************** */
/** PIC32MM hardware handling
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
    hardware.c

  @Summary
    Functions that interface directly with the hardware.

 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
//#include <proc/p32mm0064gpl020.h>

#include "hardwareprofile.h"
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */
int PRODL = 0;  // Dummy result for assignment
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */
//static int ExampleLocalFunction(int param1, int param2) 
//{
//    return 0;
//}
/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */
//#include <plib.h>
/*  A brief description of a section can be given directly below the section
    banner.
 */
extern void __attribute__((nomips16, noreturn, far, weak)) __pic32_software_reset();
void __attribute__((weak)) __exception_handler_break(void);
extern void _DEBUGGER  __attribute__((weak));

void __attribute__((weak, naked, nomips16, noreturn, section(".vector_default"))) _DefaultInterrupt (void)
{
  if (&_DEBUGGER != 0 && __exception_handler_break)
    __asm__ volatile ("sdbbp 0");
  else if (__pic32_software_reset) 
    __pic32_software_reset();
}

void __attribute__ ((nomips16)) INTEnableInterrupts(void)
{
    __builtin_enable_interrupts();
}
void __attribute__ ((nomips16)) INTEnableSystemMultiVectoredInt(void)
{
    unsigned int val;
    // set the CP0 cause IV bit high
    asm volatile("mfc0   %0,$13" : "=r"(val));
    val |= 0x00800000;
    asm volatile("mtc0   %0,$13" : "+r"(val));
    INTCONSET = _INTCON_MVEC_MASK;
    // set the CP0 status IE bit high to turn on interrupts
    INTEnableInterrupts();
}
// *****************************************************************************

/** 
  @Function
    int InitialiseBoard() 

  @Summary
    Hardware initialisation.
    Configures PIC

  @Remarks
 */
void InitialiseBoard()
{
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    //CFGCONbits.IOLOCK = 0;   // Allow configuration to be set
    //
    //  Oscillator setup - set to 8Mhz
    //
    OSCCONbits.COSC=0;
    OSCCONbits.FRCDIV = 0;
    OSCCONbits.NOSC =   0;
    OSCCONbits.OSWEN = 1;
    while(OSCCONbits.OSWEN);
    // TX
    COMM_TX_TRIS = 0;
    COMM_TX_ANSEL = 0;
    // RX
    COMM_RX_TRIS = 1;
    COMM_RX_ANSEL = 0;

    // TODO - Set up and initialise interrupts
    // Enable multi-vectored interrupts
    // Individual interrupts are enabled in 
//    INTCON = 0;
//    INTCONbits.MVEC=1;  // Enable multivectored interrupts
//    INTCONbits.VS = 0x4;  // Vector spacing 32 bytes
//    INTCONbits.VS = _vector_spacing;
//    INTCONbits.TPC = 0; // disable Interrupt Proximity Timer control bits
    // Individual interrupts are enabled in device initialisation functions
    INTEnableSystemMultiVectoredInt();
}
#if 0
void InitialiseSPI2()
{
    // clear all interrupts
    IEC0CLR = 0x03800000;
    SPI2CONbits.ON = 0;
    SPI2CON=0;
    // Configure pins for SPI2
    // POT SDI
    SPI2_SDI_TRIS = 1;
    SPI2_SDI_ANSEL = 0;
    SPI2_SDI_RPR = SPI2_SDI_RPRVAL;
    // POT SDO
    SPI2_SDO_TRIS = 0;
    SPI2_SDO_ANSEL = 0;
    SPI2_SDO_RPR = SPI2_SDO_RPRVAL;
    // POT SCK
    SPI2_SCK_TRIS = 0;
    SPI2_SCK_ANSEL = 0;
    SPI2_SCK_RPR = SPI2_SCK_RPRVAL;
    // POT SS
    SPI2_SS_TRIS = 0;
    SPI2_SS_ANSEL = 0;
    SPI2_SS_LAT=1;     // normally chip is not selected
    // clear receive buffer
    unsigned long l = SPI2BUF;
    SPI2CON2 = 0;
    SPI2STAT = 0;
    // Set SPI baud rate
    // Initially set this to 97.66 khz so it is easier to debug
    // may want to increase this once working
    // maximum is 10mhz which is 1.
    SPI2BRG = 127;
    SPI2CONbits.MSTEN = 1;
    SPI2CONbits.CKE = 1;
    SPI2CONbits.CKP = 1;
    // configure for 16 bit transfers
    SPI2CONbits.MODE16 = 1;
    SPI2CONbits.SMP = 1;
    SPI2_SS_LAT = 1;
    SPI2_SS_RPR = SPI2_SS_RPRVAL;
//    SPI2CONbits.FRMEN = 1;
    SPI2CONbits.FRMSYNC = 0;
    SPI2CONbits.FRMPOL = 0;
    SPI2CONbits.MSSEN = 1;
//    SPI2CONbits.FRMSYPW = 1;
//    SPI2CONbits.FRMCNT = 4;
    SPI2CONbits.SPIFE = 0;
    SPI2CONbits.ON = 1;
}
#endif
#if 0
unsigned long WriteSPI2(unsigned long buf)
{
//    SPI2_SS_LAT = 0;
    while(SPI2STATbits.SPIBUSY);
    SPI2BUF = buf;
    while(!SPI2STATbits.SPITBF);
//    SPI2_SS_LAT = 1;
    return SPI2BUF;
}
#endif
//void ClrWdt(){ WDTCONSET=0x1;}

/* *****************************************************************************
 End of File
 */
