/* 
 * File:   OutputDevice.cpp
 * Author: jules
 * 
 * Created on 08 April 2017, 11:28
 */

#include "OutputDevice.h"

OutputDevice::OutputDevice() 
{
}

OutputDevice::OutputDevice(const OutputDevice& orig) 
{
}

OutputDevice::~OutputDevice() 
{
}
void OutputDevice::Write(const uint8_t* pszData, uint32_t nLen){}
void OutputDevice::PutChar(uint8_t ch){}
void OutputDevice::Flush(){}

