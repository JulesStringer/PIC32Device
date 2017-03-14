/*
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
 */
#include <proc/p32mm0064gpl020.h>

#include "hardwareprofile.h"
static volatile DWORD g_dwInternalTicks = 0;

// 6-byte value to store Ticks.  Allows for use over longer periods of time.
static BYTE g_vTickReading[6];

static void GetTickCopy(void);

#define TICKS_PER_SECOND		((GetPeripheralClock()+128ull)/256ull)	// Internal core clock drives timer with 1:256 prescaler


/*****************************************************************************
  Function:
	void _ISR _T1Interrupt(void)

  Description:
	Updates the tick value when an interrupt occurs.

  Precondition:
	None

  Parameters:
	None

  Returns:
  	None
  ***************************************************************************/
#if (defined(__PIC32MX__) || defined(__PIC32MM__) || defined(__PIC32MZ__))

void __attribute__ ((interrupt(IPL4SOFT),vector(_TIMER_1_VECTOR))) _T1Interrupt(void)
{
	// Increment internal high tick counter
	g_dwInternalTicks++;

	// Reset interrupt flag
	IFS0CLR = _IFS0_T1IF_MASK;
}
#else
void _ISR _T1Interrupt(void)
{
	// Increment internal high tick counter
	g_dwInternalTicks++;

	// Reset interrupt flag
	IFS0bits.T1IF = 0;
}
#endif
/*****************************************************************************
  Function:
	void TickInit(void)

  Summary:
	Initializes the Tick manager module.

  Description:
	Configures the Tick module and any necessary hardware resources.

  Precondition:
	None

  Parameters:
	None

  Returns:
  	None
  	
  Remarks:
	This function is called only one during lifetime of the application.
  ***************************************************************************/
void TickInit(void)
{
    __builtin_disable_interrupts();

	// Use Timer 1 for 16-bit and 32-bit processors
	// 1:256 prescale
    T1CON = 0;
    T1CONbits.TCS = 0;
    T1CONbits.TGATE = 0;
	T1CONbits.TCKPS = 3;
	// Base
	PR1 = 0xFFFF;
	// Clear counter
	TMR1 = 0;

	// Enable timer interrupt
    IPC2bits.T1IP = 4;	// Interrupt priority 2 (low)
    IPC2bits.T1IS = 0;
    IFS0CLR = _IFS0_T1IF_MASK;
    IEC0SET = _IEC0_T1IE_MASK;

	// Start timer
//	T1CONbits.TON = 1;
    T1CONbits.ON = 1; 
    __builtin_enable_interrupts();
}
/*****************************************************************************
  Function:
	static void GetTickCopy(void)

  Summary:
	Reads the tick value.

  Description:
	This function performs an interrupt-safe and synchronized read of the 
	48-bit Tick value.

  Precondition:
	None

  Parameters:
	None

  Returns:
  	None
  ***************************************************************************/
static void GetTickCopy(void)
{
	// Perform an Interrupt safe and synchronized read of the 48-bit 
	// tick value
	do
	{
		DWORD dwTempTicks;
		
		IEC0SET = _IEC0_T1IE_MASK;	// Enable interrupt
		Nop();
		IEC0CLR = _IEC0_T1IE_MASK;	// Disable interrupt
		
		// Get low 2 bytes
		((volatile WORD*)g_vTickReading)[0] = TMR1;
		
		// Correct corner case where interrupt increments byte[4+] but 
		// TMR1 hasn't rolled over to 0x0000 yet
		dwTempTicks = g_dwInternalTicks;

		// PIC32MX3XX/4XX devices trigger the timer interrupt when TMR1 == PR1 
		// (TMR1 prescalar is 0x00), requiring us to undo the ISR's increment 
		// of the upper 32 bits of our 48 bit timer in the special case when 
		// TMR1 == PR1 == 0xFFFF.  For other PIC32 families, the ISR is 
		// triggered when TMR1 increments from PR1 to 0x0000, making no special 
		// corner case.
        // TODO check PIC32MM handling of prescalar
        #if defined(__PIC32MX__)
            #if __PIC32_FEATURE_SET__ <= 460 && _PIC32_FEATURE_SET__ > 270
                if(((WORD*)g_vTickReading)[0] == 0xFFFFu)
                    dwTempTicks--;
            #elif !defined(__PIC32_FEATURE_SET__)
                #error __PIC32_FEATURE_SET__ macro must be defined.  You need to download a newer C32 compiler version.
            #endif
        #endif
		// Get high 4 bytes
		g_vTickReading[2] = ((BYTE*)&dwTempTicks)[0];
		g_vTickReading[3] = ((BYTE*)&dwTempTicks)[1];
		g_vTickReading[4] = ((BYTE*)&dwTempTicks)[2];
		g_vTickReading[5] = ((BYTE*)&dwTempTicks)[3];
	} while(IFS0bits.T1IF);
	IEC0SET = _IEC0_T1IE_MASK;		// Enable interrupt
}
/*****************************************************************************
  Function:
	DWORD TickGet(void)

  Summary:
	Obtains the current Tick value.

  Description:
	This function retrieves the current Tick value, allowing timing and
	measurement code to be written in a non-blocking fashion.  This function
	retrieves the least significant 32 bits of the internal tick counter, 
	and is useful for measuring time increments ranging from a few 
	microseconds to a few hours.  Use TickGetDiv256 or TickGetDiv64K for
	longer periods of time.

  Precondition:
	None

  Parameters:
	None

  Returns:
  	Lower 32 bits of the current Tick value.
  ***************************************************************************/
DWORD TickGet(void)
{
	GetTickCopy();
	return *((DWORD*)&g_vTickReading[0]);
}
DWORD TickConvertToMilliseconds(DWORD dwTickValue)
{
	return (dwTickValue+(TICKS_PER_SECOND/2000ul))/((DWORD)(TICKS_PER_SECOND/1000ul));
}
unsigned long GetTickCount()
{
    DWORD dwTick = TickGet();
    // Returns ticks since start in milliseconds    
    return TickConvertToMilliseconds(dwTick);
}
