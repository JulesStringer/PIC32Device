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

