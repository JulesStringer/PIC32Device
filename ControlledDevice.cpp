/* 
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
