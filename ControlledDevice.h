/* 
 * File:   ControlledDevice.h
 * Author: jules
 *
 * Created on 05 November 2016, 22:32
 */

#ifndef CONTROLLEDDEVICE_H
#define	CONTROLLEDDEVICE_H

#ifndef __PRODL_DEFINED__
#define __PRODL_DEFINED__ 0
extern int PRODL;
#endif

class IODevice;

class ControlledDevice {
public:
    ControlledDevice();
    ControlledDevice(const ControlledDevice& orig);
    virtual ~ControlledDevice();
    virtual void Initialise(unsigned char nChannel);
    virtual void SetLevel(unsigned char nLevel);
    virtual void SendReading(IODevice* pDevice, unsigned char byCommand, unsigned char nDevice);
private:
    unsigned char EncodePin(unsigned char nPin);
protected:
    unsigned char m_nDevice;
    void SendResult(IODevice* pDevice, unsigned char byCommand, unsigned char nDevice, unsigned short usResult);
};

#endif	/* CONTROLLEDDEVICE_H */

