/* 
 * File:   UART1.h
 * Author: jules
 *
 * Created on 06 November 2016, 18:10
 */

#ifndef UART1_H
#define	UART1_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "IODevice.h"
    
//bool UART1HasChar();
//unsigned char UART1GetChar();
//void UART1PutChar(unsigned char ch);
//void InitialiseUART1();

class UART1 : public IODevice
{
public:
    virtual void Write(unsigned char* pszData, unsigned int nLen);
    virtual bool HasChar();
    virtual unsigned char GetChar();
    virtual void PutChar(unsigned char ch);
    void Initialise(unsigned int nBaudRate);
};
#ifdef	__cplusplus
}
#endif

#endif	/* UART1_H */

