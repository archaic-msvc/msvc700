/***
* ostruint.cpp - definitions for ostream class operator<<(unsigned int) funct
*
*       Copyright (c) 1991-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Contains the member function definitions for ostream
*       operator<<(unsigned int).
*
*******************************************************************************/

#include <cruntime.h>
#include <internal.h>
#include <stdio.h>
#include <iostream.h>
#pragma hdrstop

ostream& ostream::operator<<(unsigned int n)
{
_WINSTATIC char obuffer[12];
_WINSTATIC char fmt[4] = "%u";
_WINSTATIC char leader[4] = "\0\0";
    if (opfx()) {
        if (n)
            {
            if (x_flags & (hex|oct))
                {
                if (x_flags & hex)
                    {
                    if (x_flags & uppercase)
                        fmt[1] = 'X';
                    else
                        fmt[1] = 'x';
                    leader[1] = fmt[1];   // 0x or 0X  (or \0X)
                    }
                else
                    fmt[1] = 'o';
                if (x_flags & showbase)
                    leader[0] = '0';
                }
            else if (x_flags & showpos)
                {
                leader[0] = '+';
                }
            }
        sprintf(obuffer,fmt,n);
        writepad(leader,obuffer);
        osfx();
    }
    return *this;

}
