/* 
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

