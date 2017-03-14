
/* 
 * File:   IODevice.cpp
 * Author: jules
 * 
 * Created on 12 November 2016, 16:20
 */

#include "IODevice.h"

IODevice::IODevice() 
{
}

IODevice::IODevice(const IODevice& orig) 
{
}

IODevice::~IODevice() 
{
}

void IODevice::Write(unsigned char* pszData, unsigned int nLen)
{
    
}
bool IODevice::HasChar(){ return false;}
unsigned char IODevice::GetChar(){ return 0;}
void IODevice::PutChar(unsigned char ch){}
