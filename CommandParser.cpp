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
 * File:   CommandParser.cpp
 * Author: Jules Stringer
 * 
 * Created on 06 November 2016, 18:14
 */

#include "CommandParser.h"
#include "server.h"
//#include "UART1.h"
#include "utils.h"
#include "IODevice.h"

CommandParser::CommandParser() 
    : m_bEcho(true)
    , m_parseState(PARSE_START)
    , m_byCommand(0)
    , m_pDevice(NULL)
{
}

CommandParser::CommandParser(const CommandParser& orig) 
{
}

CommandParser::~CommandParser() 
{
}
void CommandParser::Initialise(IODevice* pDevice)
{
    m_pDevice = pDevice;
}
void CommandParser::Service(Server* pServer)
{
    if ( m_pDevice->HasChar())
    {
        char ch = m_pDevice->GetChar();
        char sz[2];
        sz[0] = ch;
        sz[1] = 0;
        if ( m_bEcho )
        {
            m_pDevice->PutChar(ch);
        }
        switch( m_parseState )
        { 
        case PARSE_START:
                // L set light level
                // I get current
                // V get voltage
            if ( ch == 'L' || ch == 'I')
            {
                m_parseState = PARSE_PIN;
                m_byCommand = ch;
            }
            else if ( ch == 'E' )
            {
                m_byCommand = ch;
                m_parseState = PARSE_BOARD_1;
            }
            else if ( ch == 'S' || ch == 'V' )
            {
                m_byCommand = ch;
                m_parseState = PARSE_BOARD_1;
            }
            break;
        case PARSE_PIN:
            if ( ch >= '1' && ch <= '9')
            {
                m_byDevice = ch - '0';
            }
            else if ( ch >= 'A' && ch <= 'Z')
            {
                m_byDevice = ch - 'A' + 10;
            }
            else
            {
                m_parseState = PARSE_START;
            }
            if ( m_parseState != PARSE_START && 
                 m_byDevice > MIN_DEVICE && m_byDevice <= MAX_DEVICE )
            {
                m_byDevice -= MIN_DEVICE;
                m_byLevel = 0;
                if ( m_byCommand == 'L' )
                {
                    m_parseState = PARSE_LEVEL_1;
                }
                else
                {
                    m_parseState = PARSE_COMPLETE;
                }
            }
            else
            {
                m_parseState = PARSE_START;
            }
            break;
        case PARSE_LEVEL_1:
            if ( ch >= '0' && ch <= '9' )
            {
                m_byLevel =  ( ch - '0' ) << 4;
                m_parseState = PARSE_LEVEL_2;
            }
            else if ( ch >= 'A' && ch <= 'F' )
            {
                m_byLevel = (ch - 'A' + 10) << 4;
                m_parseState = PARSE_LEVEL_2;
            }
            else
            {
                m_parseState = PARSE_START;
            }
            break;
        case PARSE_LEVEL_2:
            if ( ch >= '0' && ch <= '9' )
            {
                m_byLevel |=  (( ch - '0' ) & 0xF);
                m_parseState = PARSE_COMPLETE;
            }
            else if ( ch >= 'A' && ch <= 'F' )
            {
                m_byLevel |= ((ch - 'A' + 10) & 0xF);
                m_parseState = PARSE_COMPLETE;
            }
            else
            {
                m_parseState = PARSE_START;
            }
            break;
        case PARSE_MASK_1:
            if ( ch >= '0' && ch <= '9')
            {
                m_byDevice = (ch - '0') << 4;
                m_parseState = PARSE_MASK_2;
            }
            else if ( ch >= 'A' && ch <= 'F')
            {
                m_byDevice = (ch - 'A' + 10) << 4;
                m_parseState = PARSE_MASK_2;
            }
            else
            {
                m_parseState = PARSE_START;
            }
            break;
        case PARSE_MASK_2:
            if ( ch >= '0' && ch <= '9')
            {
                m_byDevice |= (ch - '0');
                m_parseState = PARSE_LEVEL_1;
            }
            else if ( ch >= 'A' && ch <= 'F')
            {
                m_byDevice |= (ch - 'A' + 10);
                m_parseState = PARSE_LEVEL_1;
            }
            else
            {
                m_parseState = PARSE_START;
            }
            break;
        case PARSE_BOARD_1:
            if ( ch >= '0' && ch <= '9')
            {
                m_byBoard = (ch - '0') << 4;
                m_parseState = PARSE_BOARD_2;
            }
            else if ( ch >= 'A' && ch <= 'F')
            {
                m_byBoard = (ch - 'A' + 10) << 4;
                m_parseState = PARSE_BOARD_2;
            }
            else
            {
                m_parseState = PARSE_START;
            }
            break;
        case PARSE_BOARD_2:
            if ( ch >= '0' && ch <= '9')
            {
                m_byBoard |= (ch - '0');
                m_parseState = PARSE_BOARD_3;
            }
            else if ( ch >= 'A' && ch <= 'F')
            {
                m_byBoard |= (ch - 'A' + 10);
                m_parseState = PARSE_BOARD_3;
            }
            else
            {
                m_parseState = PARSE_START;
            }
            if ( m_parseState == PARSE_BOARD_3)
            {
                if ( m_byBoard != GetBoardID())
                {
                    m_parseState = PARSE_START;
                }
                else
                {
                    if ( m_byCommand == 'E')
                    {
                        m_parseState = PARSE_MASK_1;
                    }
                    else
                    {
                        m_parseState = PARSE_COMPLETE;
                    }
                }
            }
            break;
        }
        if ( ch == '\r' )
        {
            // only acknowledge and act is LED is ours.
            int bReady = 0;
            switch(m_byCommand)
            {
            case 'E':
                if ( m_parseState == PARSE_COMPLETE)
                {
                    BYTE n;
                    for( n = 0; n < MAX_DEVICE; n++ )
                    {
                        if ( m_byDevice & 1)
                        {
                            pServer->SetLevel(n+1, m_byLevel);    
                        }
                        m_byLevel = m_byLevel >> 1;
                    }
                }
                break;
            case 'L':
                if ( m_parseState == PARSE_COMPLETE )
                {
                    pServer->SetLevel(m_byDevice, m_byLevel);
                    // Report reading
                    pServer->SendReading(m_pDevice,'S',m_byDevice);
                    pServer->SendReading(m_pDevice,'I',m_byDevice);
                    bReady = 1;
                }
                break;
            case 'I':
                if ( m_parseState == PARSE_COMPLETE )
                {
                    pServer->SendReading(m_pDevice, m_byCommand, m_byDevice);
                }
                break;
            case 'S':
                // allow 100ms as can be 104 characters
                if ( m_parseState == PARSE_COMPLETE )
                {
                    pServer->SendReading(m_pDevice, m_byCommand, m_byDevice);
                }
                break;        
            case 'V':
                if ( m_parseState == PARSE_COMPLETE )
                {
                    pServer->SendReading(m_pDevice, m_byCommand, m_byDevice);
                }
                break;
            }
            if ( !bReady )
            {
                if ( GetTickCount() > m_tNextReady )
                {
                    bReady = 1;
                }
            }
            m_parseState = PARSE_START;
            if ( bReady )
            {
                SendReady();
                m_tNextReady = GetTickCount() + 1000;
            }
        }
    }
}
void CommandParser::SendReady()
{
    unsigned char sz[8];
    sz[0] = '\r';
    sz[1] = '\n';
    sz[2] = '$';
    sz[3] = '0';
    sz[4] = 'x';
    sz[5] = HexChar(GetBoardID() >> 4);
    sz[6] = HexChar(GetBoardID() & 0xF);
    sz[7] = '>';
    m_pDevice->Write(sz,8);
}
