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
 * File:   Server.cpp
 * Author: jules
 * 
 * Created on 05 November 2016, 22:56
 */

#include "Server.h"
#include "UART1.h"

Server::Server() : m_nMaxDevice(0)
{
}

Server::Server(const Server& orig) 
{
}

Server::~Server() 
{
}
void Server::InitialiseDevices()
{
}
void Server::Run()
{
    InitialiseBoard();
    InitialiseSPI2();
    m_UART1.Initialise(19200);
    TickInit();
    
    InitialiseDevices();
    m_commandParser.Initialise(&m_UART1);
    m_commandParser.SendReady();
    while(1)
    {
        ClrWdt();
        m_commandParser.Service(this);
    }
}
void Server::SetLevel(unsigned char nDevice, unsigned char nLevel)
{
    if ( nDevice >= 1 && nDevice <= m_nMaxDevice)
    {
        m_Devices[nDevice-1].m_pHandler->SetLevel(nLevel);
    }
}
void Server::SendReading(IODevice* pDevice, unsigned char byCommand, unsigned char nDevice)
{
    if ( nDevice >= 1 && nDevice <= m_nMaxDevice)
    {
        m_Devices[nDevice-1].m_pHandler->SendReading(pDevice, byCommand, nDevice);
    }
}
// TODO write interrupt handlers
void Server::AddDevice(unsigned char nChannel, ControlledDevice* pDevice)
{
    m_Devices[m_nMaxDevice].m_nChannel = nChannel;
    m_Devices[m_nMaxDevice].m_pHandler = pDevice;
    m_nMaxDevice++;
}
