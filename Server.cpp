/* 
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
