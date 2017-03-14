#include "hardwareprofile.h"
#include "UART1.h"
// Get next character from input buffer
volatile char g_Rx1Buffer[256];
volatile unsigned char g_Rx1Head = 0;
volatile unsigned char g_Rx1Tail = 0;

extern "C" void __attribute__ ((interrupt(IPL3SOFT),vector(_UART1_RX_VECTOR))) _IntUart1Handler(void)
{
    if (IFS0bits.U1RXIF )
    {
        g_Rx1Buffer[g_Rx1Tail] = U1RXREG;
        g_Rx1Tail++;
        IFS0bits.U1RXIF = 0;
    }
}
bool UART1::HasChar()
{
    return g_Rx1Head != g_Rx1Tail;
}
unsigned char UART1::GetChar()
{
    if ( g_Rx1Head != g_Rx1Tail)
    {
        unsigned char ch = g_Rx1Buffer[g_Rx1Head];
        g_Rx1Head++;
        return ch;
    }
    return 0;
}
void UART1::PutChar(unsigned char ch)
{
    U1TXREG = ch;
    while(U1STAbits.TRMT == 0);
}
//#define BAUDRATEREG1        ((GetPeripheralClock()+(BRG_DIV1/2*BAUDRATE1))/BRG_DIV1/BAUDRATE1-1)

void UART1::Initialise(unsigned int nBaudRate)
{
    __builtin_disable_interrupts();
    U1BRG = ((GetPeripheralClock() + (BRG_DIV1/2 * nBaudRate))/BRG_DIV1/nBaudRate -1);
    U1MODE = 0;
    U1MODEbits.BRGH = BRGH1;
    U1STA = 0;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
    IFS0bits.U1RXIF = 0;
    
    U1STAbits.URXEN = 1;
    
    // Set up interrupts
    //IFS0CLR = _IFS0_U1RXIE_MASK;
    IEC0SET = _IEC0_U1RXIE_MASK;

//    IEC0bits.U1RXIE = 1;
    IPC5bits.U1RXIP = 3;
    IPC5bits.U1RXIS = 0;
    __builtin_enable_interrupts();

}
void UART1::Write(unsigned char* psz, unsigned int nChars)
{
    while(nChars > 0)
    {
        PutChar(*psz);
        psz++;
        nChars--;
    }
}

