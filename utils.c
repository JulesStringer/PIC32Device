#include "utils.h"

unsigned char HexChar(unsigned char by)
{
    if ( by >= 0 && by < 10 )
    {
        return by + '0';
    }
    else if ( by >= 10 && by < 16 )
    {
        return by - 10 + 'A';
    }
    return '?';
}

