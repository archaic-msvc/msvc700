/***
*wstat.c - get file status (wchar_t version)
*
*       Copyright (c) 1993-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _wstat() - get file status
*
*******************************************************************************/


#define WPRFLAG 1

#ifndef _UNICODE
#define _UNICODE 1
#endif  /* _UNICODE */

#ifndef UNICODE
#define UNICODE 1
#endif  /* UNICODE */

#undef _MBCS /* UNICODE not _MBCS */

#include "stat.c"

