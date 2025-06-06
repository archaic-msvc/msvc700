/***
*_open.c - open a stream, with string mode
*
*       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _openfile() - opens a stream, with string arguments for mode
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <fcntl.h>
#include <file2.h>
#include <io.h>
#include <dbgint.h>
#include <internal.h>
#include <tchar.h>

#define CMASK   0644    /* rw-r--r-- */
#define P_CMASK 0666    /* different for Posix */

/***
*FILE *_openfile(filename, mode, shflag, stream) - open a file with string
*       mode and file sharing flag.
*
*Purpose:
*       parse the string, looking for exactly one of {rwa}, at most one '+',
*       at most one of {tb}, at most one of {cn}, at most one of {SR}, at most
*       one 'T', and at most one 'D'. pass the result on as an int containing
*       flags of what was found. open a file with proper mode if permissions
*       allow. buffer not allocated until first i/o call is issued. intended
*       for use inside library only
*
*Entry:
*       char *filename - file to open
*       char *mode - mode to use (see above)
*       int shflag - file sharing flag
*       FILE *stream - stream to use for file
*
*Exit:
*       set stream's fields, and causes system file management by system calls
*       returns stream or NULL if fails
*
*Exceptions:
*
*******************************************************************************/

#ifdef _UNICODE
FILE * __cdecl _wopenfile (
#else  /* _UNICODE */
FILE * __cdecl _openfile (
#endif  /* _UNICODE */
        const _TSCHAR *filename,
        REG3 const _TSCHAR *mode,
        int shflag,
        FILE *str
        )
{
        REG2 int modeflag;
        int streamflag = _commode;
        int commodeset = 0;
        int scanset    = 0;
        int whileflag;
        int filedes;
        REG1 FILE *stream;

        _ASSERTE(filename != NULL);
        _ASSERTE(mode != NULL);
        _ASSERTE(str != NULL);

        /* Parse the user's specification string as set flags in
               (1) modeflag - system call flags word
               (2) streamflag - stream handle flags word. */

        /* First mode character must be 'r', 'w', or 'a'. */

        switch (*mode) {
        case _T('r'):
                modeflag = _O_RDONLY;
                streamflag |= _IOREAD;
                break;
        case _T('w'):
                modeflag = _O_WRONLY | _O_CREAT | _O_TRUNC;
                streamflag |= _IOWRT;
                break;
        case _T('a'):
                modeflag = _O_WRONLY | _O_CREAT | _O_APPEND;
                streamflag |= _IOWRT;
                break;
        default:
                return(NULL);
                break;
        }

        /* There can be up to three more optional mode characters:
           (1) A single '+' character,
           (2) One of 't' and 'b' and
           (3) One of 'c' and 'n'.
        */

        whileflag=1;

        while(*++mode && whileflag)
                switch(*mode) {

                case _T('+'):
                        if (modeflag & _O_RDWR)
                                whileflag=0;
                        else {
                                modeflag |= _O_RDWR;
                                modeflag &= ~(_O_RDONLY | _O_WRONLY);
                                streamflag |= _IORW;
                                streamflag &= ~(_IOREAD | _IOWRT);
                        }
                        break;

                case _T('b'):
                        if (modeflag & (_O_TEXT | _O_BINARY))
                                whileflag=0;
                        else
                                modeflag |= _O_BINARY;
                        break;

                case _T('t'):
                        if (modeflag & (_O_TEXT | _O_BINARY))
                                whileflag=0;
                        else
                                modeflag |= _O_TEXT;
                        break;

                case _T('c'):
                        if (commodeset)
                                whileflag=0;
                        else {
                                commodeset = 1;
                                streamflag |= _IOCOMMIT;
                        }
                        break;

                case _T('n'):
                        if (commodeset)
                                whileflag=0;
                        else {
                                commodeset = 1;
                                streamflag &= ~_IOCOMMIT;
                        }
                        break;

                case _T('S'):
                        if (scanset)
                                whileflag=0;
                        else {
                                scanset = 1;
                                modeflag |= _O_SEQUENTIAL;
                        }
                        break;

                case _T('R'):
                        if (scanset)
                                whileflag=0;
                        else {
                                scanset = 1;
                                modeflag |= _O_RANDOM;
                        }
                        break;

                case _T('T'):
                        if (modeflag & _O_SHORT_LIVED)
                                whileflag=0;
                        else
                                modeflag |= _O_SHORT_LIVED;
                        break;

                case _T('D'):
                        if (modeflag & _O_TEMPORARY)
                                whileflag=0;
                        else
                                modeflag |= _O_TEMPORARY;
                        break;

                default:
                        whileflag=0;
                        break;
                }

        /* Try to open the file.  Note that if neither 't' nor 'b' is
           specified, _sopen will use the default. */

        if ((filedes = _tsopen(filename, modeflag, shflag, CMASK)) < 0)
                return(NULL);

        /* Set up the stream data base. */
#ifndef CRTDLL
        _cflush++;  /* force library pre-termination procedure */
#endif  /* CRTDLL */
        /* Init pointers */
        stream = str;

        stream->_flag = streamflag;
        stream->_cnt = 0;
        stream->_tmpfname = stream->_base = stream->_ptr = NULL;

        stream->_file = filedes;

        return(stream);
}
