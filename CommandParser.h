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
 * File:   CommandParser.h
 * Author: jules
 *
 * Created on 06 November 2016, 18:14
 */

#ifndef COMMANDPARSER_H
#define	COMMANDPARSER_H

class Server;
class IODevice;

class CommandParser {
public:
    typedef enum
    {
        PARSE_START = 0,
        PARSE_PIN = 1,
        PARSE_BOARD_1 = 2,
        PARSE_BOARD_2 = 3,
        PARSE_BOARD_3 = 4,
        PARSE_BOARD_4 = 5,
        PARSE_LEVEL_1 = 6,
        PARSE_LEVEL_2 = 7,
        PARSE_MASK_1 = 8,
        PARSE_MASK_2 = 9,
        PARSE_COMPLETE = 99                
    }PARSE_STATE;
    CommandParser();
    CommandParser(const CommandParser& orig);
    virtual ~CommandParser();
    void Service(Server* pServer);
    void Initialise(IODevice* pDevice);
    void SendReady();
private:
    bool m_bEcho;
    PARSE_STATE m_parseState;
    unsigned char m_byCommand;
    unsigned char m_byDevice;
    unsigned char m_byLevel;
    unsigned char m_byBoard;
    unsigned long m_tNextReady;
    IODevice* m_pDevice;
};

#endif	/* COMMANDPARSER_H */

