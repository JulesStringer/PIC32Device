/* 
 * File:   IODevice.h
 * Author: jules
 *
 * Created on 12 November 2016, 16:20
 */

#ifndef IODEVICE_H
#define	IODEVICE_H

#ifndef __PRODL_DEFINED__
#define __PRODL_DEFINED__ 0
extern int PRODL;
#endif

class IODevice {
public:
    IODevice();
    IODevice(const IODevice& orig);
    virtual ~IODevice();
    virtual void Write(unsigned char* pszData, unsigned int nLen);
    virtual bool HasChar();
    virtual unsigned char GetChar();
    virtual void PutChar(unsigned char ch);
private:

};

#endif	/* IODEVICE_H */

