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
    virtual void Write(const uint8_t* pszData, uint32_t nLen);
    virtual bool HasChar();
    virtual uint8_t GetChar();
    virtual void PutChar(uint8_t ch);
    void Initialise(uint32_t nBaudRate);
};
#ifdef	__cplusplus
}
#endif

#endif	/* UART1_H */

