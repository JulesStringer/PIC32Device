/* 
 * File:   Server.h
 * Author: jules
 *
 * Created on 05 November 2016, 22:56
 */

#ifndef SERVER_H
#define	SERVER_H
#include "hardwareprofile.h"
#include "MCP4341.h"
#include "MC14052B.h"
#include "MCP4341Balance.h"
#include "CommandParser.h"
#include "UART1.h"

typedef struct tagDEVICE
{
    unsigned char m_nChannel; // Channel number to pass to handler
    ControlledDevice* m_pHandler;
}DEVICE;

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
    DEVICE m_Devices[MAX_DEVICE];
    virtual void InitialiseDevices();
    virtual void AddDevice(unsigned char nChannel, ControlledDevice* pDevice);
    UART1 m_UART1;
    CommandParser m_commandParser;
    unsigned int m_nMaxDevice;
};
extern "C" void RunServer();
#endif	/* SERVER_H */

