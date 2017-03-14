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
 * File:   SPI2.cpp
 * Author: jules
 * 
 * Created on 14 March 2017, 16:49
 */

#include "SPI2.h"
#include "hardwareprofile.h"

SPI2::SPI2() 
{
}

SPI2::SPI2(const SPI2& orig) 
{
}

SPI2::~SPI2()
{
}
void SPI2::Initialise()
// Should this have arguments - speed, 16 bit transfers, polarity
{
    // clear all interrupts
    IEC0CLR = 0x03800000;
    SPI2CONbits.ON = 0;
    SPI2CON=0;
    // Configure pins for SPI2
    InitialiseRPR();
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
//    SPI2CONbits.FRMEN = 1;
    SPI2CONbits.FRMSYNC = 0;
    SPI2CONbits.FRMPOL = 0;
    SPI2CONbits.MSSEN = 1;
//    SPI2CONbits.FRMSYPW = 1;
//    SPI2CONbits.FRMCNT = 4;
    SPI2CONbits.SPIFE = 0;
    SPI2CONbits.ON = 1;
}
void SPI2::InitialiseRPR()
{
    // This need to be implemented in the derived class
}
unsigned long SPI2::Put(unsigned long buf)
{
    while(SPI2STATbits.SPIBUSY);
    SPI2BUF = buf;
    while(!SPI2STATbits.SPITBF);
    return SPI2BUF;
}
