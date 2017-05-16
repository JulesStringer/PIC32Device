/* 
 * File:   OutputDevice.h
 * Author: jules
 *
 * Created on 08 April 2017, 11:28
 */

#ifndef OUTPUTDEVICE_H
#define	OUTPUTDEVICE_H

#include <stdint.h>

class OutputDevice {
public:
    OutputDevice();
    OutputDevice(const OutputDevice& orig);
    virtual ~OutputDevice();
    virtual void Write(const uint8_t* pszData, uint32_t nLen);
    virtual void PutChar(uint8_t ch);
    virtual void Flush();
private:

};

#endif	/* OUTPUTDEVICE_H */

