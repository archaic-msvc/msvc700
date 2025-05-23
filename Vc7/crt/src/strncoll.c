/***
*strncoll.c - Collate locale strings
*
*       Copyright (c) 1994-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Compare two strings using the locale LC_COLLATE information.
*       Compares at most n characters of two strings.
*
*******************************************************************************/

#include <cruntime.h>
#include <string.h>

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <limits.h>
#include <locale.h>
#include <setlocal.h>
#include <mtdll.h>
#include <errno.h>
#include <awint.h>

/***
*int _strncoll() - Collate locale strings
*
*Purpose:
*       Compare two strings using the locale LC_COLLATE information.
*       Compares at most n characters of two strings.
*
*Entry:
*       const char *s1 = pointer to the first string
*       const char *s2 = pointer to the second string
*       size_t count - maximum number of characters to compare
*
*Exit:
*       Less than 0    = first string less than second string
*       0              = strings are equal
*       Greater than 0 = first string greater than second string
*
*Exceptions:
*       _NLSCMPERROR    = error
*       errno = EINVAL
*
*******************************************************************************/

int __cdecl _strncoll (
        const char *_string1,
        const char *_string2,
        size_t count
        )
{

        int ret;
#ifdef _MT
        pthreadlocinfo ptloci;
#endif  /* _MT */

        if ( !count )
            return 0;

        if ( count > INT_MAX ) {
            errno = EINVAL;
            return _NLSCMPERROR;
        }

#ifdef _MT
        ptloci = _getptd()->ptlocinfo;

        if ( ptloci != __ptlocinfo )
            ptloci = __updatetlocinfo();

        if ( ptloci->lc_handle[LC_COLLATE] == _CLOCALEHANDLE )
#else  /* _MT */
        if ( __lc_handle[LC_COLLATE] == _CLOCALEHANDLE )
#endif  /* _MT */
            return strncmp(_string1, _string2, count);

#ifdef _MT
        if ( 0 == (ret = __crtCompareStringA( ptloci->lc_handle[LC_COLLATE],
#else  /* _MT */
        if ( 0 == (ret = __crtCompareStringA( __lc_handle[LC_COLLATE],
#endif  /* _MT */
                                              SORT_STRINGSORT,
                                              _string1,
                                              (int)count,
                                              _string2,
                                              (int)count,
#ifdef _MT
                                              ptloci->lc_collate_cp )) )
#else  /* _MT */
                                              __lc_collate_cp )) )
#endif  /* _MT */
        {
            errno = EINVAL;
            return _NLSCMPERROR;
        }

        return (ret - 2);

}
