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
 * File:   Server.h
 * Author: jules
 *
 * Created on 05 November 2016, 22:56
 */

#ifndef SERVER_H
#define	SERVER_H
#include "hardwareprofile.h"
#include "ControlledDevice.h"
#include "CommandParser.h"
#include "UART1.h"

#define MIN_DEVICE 0
#define MAX_DEVICE 6

class Server 
{
public:
    Server();
    Server(const Server& orig);
    virtual ~Server();
    virtual void Run();
    void SetLevel(unsigned char nDevice, unsigned char nLevel);
    void SendReading(IODevice* pDevice, unsigned char byCommand, unsigned char nDevice);
protected:
    ControlledDevice* m_Devices[MAX_DEVICE];
    virtual void InitialiseDevices();
    virtual void AddDevice(ControlledDevice* pDevice);
    UART1 m_UART1;
    CommandParser m_commandParser;
    unsigned int m_nMaxDevice;
};
extern "C" void RunServer();
#endif	/* SERVER_H */

