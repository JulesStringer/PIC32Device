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
 * File:   ControlledDevice.cpp
 * Author: jules
 * 
 * Created on 05 November 2016, 22:32
 */

#include "ControlledDevice.h"
#include "utils.h"
#include "IODevice.h"

ControlledDevice::ControlledDevice() 
{
}
ControlledDevice::ControlledDevice(const ControlledDevice& orig) 
{
}
ControlledDevice::~ControlledDevice() 
{
}
void ControlledDevice::Initialise(unsigned char nDevice)
{
    m_nDevice = nDevice;
}
void ControlledDevice::SetLevel(unsigned char nLevel){}
void ControlledDevice::SendReading(IODevice* pDevice, unsigned char byCommand, unsigned char nDevice){}
unsigned char ControlledDevice::EncodePin(unsigned char nPin)
{
    if ( nPin < 10 )
    {
        return '0' + nPin;
    }
    else if ( nPin < 36 )
    {
        return 'A' + nPin - 10;
    }
    return '?';
}
void ControlledDevice::SendResult(IODevice* pDevice, unsigned char byCommand, unsigned char nDevice, unsigned short usReading)
{
    unsigned char szCMD[10];
    szCMD[0] = '=';
    szCMD[1] = byCommand;
    szCMD[2] = EncodePin(nDevice);
    szCMD[3] = HexChar(usReading >> 12);
    szCMD[4] = HexChar((usReading >> 8) & 0xF);
    szCMD[5] = HexChar((usReading >> 4) & 0xF);
    szCMD[6] = HexChar( usReading & 0xF);
    szCMD[7] = '\r';
    szCMD[8] = '\n';
    szCMD[9] = 0;
    pDevice->Write(szCMD,9);
}
