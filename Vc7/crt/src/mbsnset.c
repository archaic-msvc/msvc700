/***
*mbsnset.c - Sets first n charcaters of string to given character (MBCS)
*
*       Copyright (c) 1985-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Sets first n charcaters of string to given character (MBCS)
*
*******************************************************************************/

#ifdef _MBCS

#include <mtdll.h>
#include <cruntime.h>
#include <string.h>
#include <mbdata.h>
#include <mbctype.h>
#include <mbstring.h>


/***
* _mbsnset - Sets first n charcaters of string to given character (MBCS)
*
*Purpose:
*       Sets the first n characters of string to the supplied
*       character value.  If the length of string is less than n,
*       the length of string is used in place of n.  Handles
*       MBCS chars correctly.
*
*       There are several factors that make this routine complicated:
*               (1) The fill value may be 1 or 2 bytes long.
*               (2) The fill operation may end by hitting the count value
*               or by hitting the end of the string.
*               (3) A null terminating char is NOT placed at the end of
*               the string.
*
*       Cases to be careful of (both of these can occur at once):
*               (1) Leaving an "orphaned" trail byte in the string (e.g.,
*               overwriting a lead byte but not the corresponding trail byte).
*               (2) Writing only the 1st byte of a 2-byte fill value because the
*               end of string was encountered.
*
*Entry:
*       unsigned char *string = string to modify
*       unsigned int val = value to fill string with
*       size_t count = count of characters to fill
*
*
*Exit:
*       Returns string = now filled with char val
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

unsigned char * __cdecl _mbsnset(
        unsigned char *string,
        unsigned int val,
        size_t count
        )
{
        unsigned char  *start = string;
        unsigned int leadbyte = 0;
        unsigned char highval, lowval;
#ifdef _MT
        pthreadmbcinfo ptmbci = _getptd()->ptmbcinfo;

        if ( ptmbci != __ptmbcinfo )
            ptmbci = __updatetmbcinfo();
#endif  /* _MT */

        /*
         * leadbyte flag indicates if the last byte we overwrote was
         * a lead byte or not.
         */
#ifdef _MT
        if ( _ISNOTMBCP_MT(ptmbci) )
#else  /* _MT */
        if ( _ISNOTMBCP )
#endif  /* _MT */
            return _strnset(string, val, count);

        if (highval = (unsigned char)(val>>8)) {

            /* double byte value */

            lowval = (unsigned char)(val & 0x00ff);

            while (count-- && *string) {
#ifdef _MT
                leadbyte = __ismbbtruelead_mt(ptmbci, leadbyte, *string);
#else  /* _MT */
                leadbyte = _ismbbtruelead(leadbyte, *string);
#endif  /* _MT */
                *string++ = highval;

                if (*string) {
#ifdef _MT
                    leadbyte = __ismbbtruelead_mt(ptmbci, leadbyte, *string);
#else  /* _MT */
                    leadbyte = _ismbbtruelead(leadbyte, *string);
#endif  /* _MT */
                    *string++ = lowval;
                }
                else
                    /* overwrite orphaned highval byte */
                    *(string-1) = ' ';
            }
        }

        else {
            /* single byte value */

            while (count-- && *string) {
#ifdef _MT
                leadbyte = __ismbbtruelead_mt(ptmbci, leadbyte, *string);
#else  /* _MT */
                leadbyte = _ismbbtruelead(leadbyte, *string);
#endif  /* _MT */
                *string++ = (unsigned char)val;
            }
        }

        /* overwrite orphaned trailing byte, if necessary */
        if(leadbyte && *string)
            *string = ' ';

        return( start );
}

#endif  /* _MBCS */
