/***
*output.c - printf style output to a FILE
*
*       Copyright (c) 1989-2001, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains the code that does all the work for the
*       printf family of functions.  It should not be called directly, only
*       by the *printf functions.  We don't make any assumtions about the
*       sizes of ints, longs, shorts, or long doubles, but if types do overlap,
*       we also try to be efficient.  We do assume that pointers are the same
*       size as either ints or longs.
*       If CPRFLAG is defined, defines _cprintf instead.
*       **** DOESN'T CURRENTLY DO MTHREAD LOCKING ****
*
*******************************************************************************/

#if defined (_M_MRX000) || defined (_M_ALPHA) || defined (_M_PPC)
#define DOUBLE double
#endif  /* defined (_M_MRX000) || defined (_M_ALPHA) || defined (_M_PPC) */


/* temporary work-around for compiler without 64-bit support */

#ifndef _INTEGRAL_MAX_BITS
#define _INTEGRAL_MAX_BITS  64
#endif  /* _INTEGRAL_MAX_BITS */


#include <cruntime.h>
#include <limits.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <cvt.h>
#include <conio.h>
#include <internal.h>
#include <fltintrn.h>
#include <stdlib.h>
#include <ctype.h>
#include <dbgint.h>

/* inline keyword is non-ANSI C7 extension */
#if !defined (_MSC_VER) || defined (__STDC__)
#define __inline static
#else  /* !defined (_MSC_VER) || defined (__STDC__) */
/* UNDONE: compiler is broken */
#define __inline static
#endif  /* !defined (_MSC_VER) || defined (__STDC__) */

#ifdef _MBCS
#undef  _MBCS
#endif  /* _MBCS */
#include <tchar.h>

/* this macro defines a function which is private and as fast as possible: */
/* for example, in C 6.0, it might be static _fastcall <type> near. */
#define LOCAL(x) static x __cdecl

/* int/long/short/pointer sizes */

/* the following should be set depending on the sizes of various types */
#define LONG_IS_INT      1      /* 1 means long is same size as int */
#define SHORT_IS_INT     0      /* 1 means short is same size as int */
#define LONGDOUBLE_IS_DOUBLE 1  /* 1 means long double is same as double */
#if defined (_WIN64)
#define PTR_IS_INT       0      /* 1 means ptr is same size as int */
#define PTR_IS_LONG      0      /* 1 means ptr is same size as long */
#define PTR_IS_INT64     1      /* 1 means ptr is same size as int64 */
#else  /* defined (_WIN64) */
#define PTR_IS_INT       1      /* 1 means ptr is same size as int */
#define PTR_IS_LONG      1      /* 1 means ptr is same size as long */
#define PTR_IS_INT64     0      /* 1 means ptr is same size as int64 */
#endif  /* defined (_WIN64) */

#if LONG_IS_INT
    #define get_long_arg(x) (long)get_int_arg(x)
#endif  /* LONG_IS_INT */

#ifndef _UNICODE
#if SHORT_IS_INT
    #define get_short_arg(x) (short)get_int_arg(x)
#endif  /* SHORT_IS_INT */
#endif  /* _UNICODE */

#if PTR_IS_INT
    #define get_ptr_arg(x) (void *)(intptr_t)get_int_arg(x)
#elif PTR_IS_LONG
    #define get_ptr_arg(x) (void *)(intptr_t)get_long_arg(x)
#elif PTR_IS_INT64
    #define get_ptr_arg(x) (void *)get_int64_arg(x)
#else  /* PTR_IS_INT64 */
    #error Size of pointer must be same as size of int or long
#endif  /* PTR_IS_INT64 */



/* CONSTANTS */

/* size of conversion buffer (ANSI-specified minimum is 509) */

#define BUFFERSIZE    512
#define MAXPRECISION  BUFFERSIZE

#if BUFFERSIZE < CVTBUFSIZE + 6
/*
 * Buffer needs to be big enough for default minimum precision
 * when converting floating point needs bigger buffer, and malloc
 * fails
 */
#error Conversion buffer too small for max double.
#endif  /* BUFFERSIZE < CVTBUFSIZE + 6 */

/* flag definitions */
#define FL_SIGN       0x00001   /* put plus or minus in front */
#define FL_SIGNSP     0x00002   /* put space or minus in front */
#define FL_LEFT       0x00004   /* left justify */
#define FL_LEADZERO   0x00008   /* pad with leading zeros */
#define FL_LONG       0x00010   /* long value given */
#define FL_SHORT      0x00020   /* short value given */
#define FL_SIGNED     0x00040   /* signed data given */
#define FL_ALTERNATE  0x00080   /* alternate form requested */
#define FL_NEGATIVE   0x00100   /* value is negative */
#define FL_FORCEOCTAL 0x00200   /* force leading '0' for octals */
#define FL_LONGDOUBLE 0x00400   /* long double value given */
#define FL_WIDECHAR   0x00800   /* wide characters */
#define FL_I64        0x08000   /* __int64 value given */

/* state definitions */
enum STATE {
    ST_NORMAL,          /* normal state; outputting literal chars */
    ST_PERCENT,         /* just read '%' */
    ST_FLAG,            /* just read flag character */
    ST_WIDTH,           /* just read width specifier */
    ST_DOT,             /* just read '.' */
    ST_PRECIS,          /* just read precision specifier */
    ST_SIZE,            /* just read size specifier */
    ST_TYPE             /* just read type specifier */
};
#define NUMSTATES (ST_TYPE + 1)

/* character type values */
enum CHARTYPE {
    CH_OTHER,           /* character with no special meaning */
    CH_PERCENT,         /* '%' */
    CH_DOT,             /* '.' */
    CH_STAR,            /* '*' */
    CH_ZERO,            /* '0' */
    CH_DIGIT,           /* '1'..'9' */
    CH_FLAG,            /* ' ', '+', '-', '#' */
    CH_SIZE,            /* 'h', 'l', 'L', 'N', 'F', 'w' */
    CH_TYPE             /* type specifying character */
};

/* static data (read only, since we are re-entrant) */
#if defined (_UNICODE) || defined (CPRFLAG)
extern char *__nullstring;  /* string to print on null ptr */
extern wchar_t *__wnullstring;  /* string to print on null ptr */
#else  /* defined (_UNICODE) || defined (CPRFLAG) */
char *__nullstring = "(null)";  /* string to print on null ptr */
wchar_t *__wnullstring = L"(null)";/* string to print on null ptr */
#endif  /* defined (_UNICODE) || defined (CPRFLAG) */

/* The state table.  This table is actually two tables combined into one. */
/* The lower nybble of each byte gives the character class of any         */
/* character; while the uper nybble of the byte gives the next state      */
/* to enter.  See the macros below the table for details.                 */
/*                                                                        */
/* The table is generated by maketabc.c -- use this program to make       */
/* changes.                                                               */

#if defined (_UNICODE) || defined (CPRFLAG)

extern const char __lookuptable[];

#else  /* defined (_UNICODE) || defined (CPRFLAG) */

/* Table generated by maketabc.c built with -D_WIN32_. Defines additional */
/* format code %Z for counted string.                                     */

const char __lookuptable[] = {
         0x06, 0x00, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00,
         0x10, 0x00, 0x03, 0x06, 0x00, 0x06, 0x02, 0x10,
         0x04, 0x45, 0x45, 0x45, 0x05, 0x05, 0x05, 0x05,
         0x05, 0x35, 0x30, 0x00, 0x50, 0x00, 0x00, 0x00,
         0x00, 0x20, 0x28, 0x38, 0x50, 0x58, 0x07, 0x08,
         0x00, 0x37, 0x30, 0x30, 0x57, 0x50, 0x07, 0x00,
         0x00, 0x20, 0x20, 0x08, 0x00, 0x00, 0x00, 0x00,
         0x08, 0x60,
         0x68, /* 'Z' (extension for NT development) */
                           0x60, 0x60, 0x60, 0x60, 0x00,
         0x00, 0x70, 0x70, 0x78, 0x78, 0x78, 0x78, 0x08,
         0x07, 0x08, 0x00, 0x00, 0x07, 0x00, 0x08, 0x08,
         0x08, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00,
         0x07, /* 'w' (extension for NT development) */
         0x08
};

#endif  /* defined (_UNICODE) || defined (CPRFLAG) */

#define find_char_class(c)      \
        ((c) < _T(' ') || (c) > _T('x') ? \
            CH_OTHER            \
            :               \
        __lookuptable[(c)-_T(' ')] & 0xF)

#define find_next_state(class, state)   \
        (__lookuptable[(class) * NUMSTATES + (state)] >> 4)


/*
 * Note: CPRFLAG and _UNICODE cases are currently mutually exclusive.
 */

/* prototypes */

#ifdef CPRFLAG

#define WRITE_CHAR(ch, pnw)         write_char(ch, pnw)
#define WRITE_MULTI_CHAR(ch, num, pnw)  write_multi_char(ch, num, pnw)
#define WRITE_STRING(s, len, pnw)   write_string(s, len, pnw)
#define WRITE_WSTRING(s, len, pnw)  write_wstring(s, len, pnw)

LOCAL(void) write_char(_TCHAR ch, int *pnumwritten);
LOCAL(void) write_multi_char(_TCHAR ch, int num, int *pnumwritten);
LOCAL(void) write_string(_TCHAR *string, int len, int *numwritten);
LOCAL(void) write_wstring(wchar_t *string, int len, int *numwritten);

#else  /* CPRFLAG */

#define WRITE_CHAR(ch, pnw)         write_char(ch, stream, pnw)
#define WRITE_MULTI_CHAR(ch, num, pnw)  write_multi_char(ch, num, stream, pnw)
#define WRITE_STRING(s, len, pnw)   write_string(s, len, stream, pnw)
#define WRITE_WSTRING(s, len, pnw)  write_wstring(s, len, stream, pnw)

LOCAL(void) write_char(_TCHAR ch, FILE *f, int *pnumwritten);
LOCAL(void) write_multi_char(_TCHAR ch, int num, FILE *f, int *pnumwritten);
LOCAL(void) write_string(_TCHAR *string, int len, FILE *f, int *numwritten);
LOCAL(void) write_wstring(wchar_t *string, int len, FILE *f, int *numwritten);

#endif  /* CPRFLAG */

__inline int __cdecl get_int_arg(va_list *pargptr);

#ifndef _UNICODE
#if !SHORT_IS_INT
__inline short __cdecl get_short_arg(va_list *pargptr);
#endif  /* !SHORT_IS_INT */
#endif  /* _UNICODE */

#if !LONG_IS_INT
__inline long __cdecl get_long_arg(va_list *pargptr);
#endif  /* !LONG_IS_INT */

#if _INTEGRAL_MAX_BITS >= 64   
__inline __int64 __cdecl get_int64_arg(va_list *pargptr);
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#ifdef CPRFLAG
LOCAL(int) output(const _TCHAR *, va_list);

/***
*int _cprintf(format, arglist) - write formatted output directly to console
*
*Purpose:
*   Writes formatted data like printf, but uses console I/O functions.
*
*Entry:
*   char *format - format string to determine data formats
*   arglist - list of POINTERS to where to put data
*
*Exit:
*   returns number of characters written
*
*Exceptions:
*
*******************************************************************************/
#ifdef _UNICODE
int __cdecl _cwprintf (
#else  /* _UNICODE */
int __cdecl _cprintf (
#endif  /* _UNICODE */
        const _TCHAR * format,
        ...
        )
{
        va_list arglist;

        va_start(arglist, format);

        return output(format, arglist);
}

#endif  /* CPRFLAG */


/***
*int _output(stream, format, argptr), static int output(format, argptr)
*
*Purpose:
*   Output performs printf style output onto a stream.  It is called by
*   printf/fprintf/sprintf/vprintf/vfprintf/vsprintf to so the dirty
*   work.  In multi-thread situations, _output assumes that the given
*   stream is already locked.
*
*   Algorithm:
*       The format string is parsed by using a finite state automaton
*       based on the current state and the current character read from
*       the format string.  Thus, looping is on a per-character basis,
*       not a per conversion specifier basis.  Once the format specififying
*       character is read, output is performed.
*
*Entry:
*   FILE *stream   - stream for output
*   char *format   - printf style format string
*   va_list argptr - pointer to list of subsidiary arguments
*
*Exit:
*   Returns the number of characters written, or -1 if an output error
*   occurs.
*ifdef _UNICODE
*   The wide-character flavour returns the number of wide-characters written.
*endif
*
*Exceptions:
*
*******************************************************************************/

#ifdef CPRFLAG
LOCAL(int) output (
#else  /* CPRFLAG */

#ifdef _UNICODE
int __cdecl _woutput (
    FILE *stream,
#else  /* _UNICODE */
int __cdecl _output (
    FILE *stream,
#endif  /* _UNICODE */

#endif  /* CPRFLAG */
    const _TCHAR *format,
    va_list argptr
    )
{
    int hexadd;     /* offset to add to number to get 'a'..'f' */
    TCHAR ch;       /* character just read */
    int flags;      /* flag word -- see #defines above for flag values */
    enum STATE state;   /* current state */
    enum CHARTYPE chclass; /* class of current character */
    int radix;      /* current conversion radix */
    int charsout;   /* characters currently written so far, -1 = IO error */
    int fldwidth;   /* selected field width -- 0 means default */
    int precision;  /* selected precision  -- -1 means default */
    TCHAR prefix[2];    /* numeric prefix -- up to two characters */
    int prefixlen;  /* length of prefix -- 0 means no prefix */
    int capexp;     /* non-zero = 'E' exponent signifient, zero = 'e' */
    int no_output;  /* non-zero = prodcue no output for this specifier */
    union {
        char *sz;   /* pointer text to be printed, not zero terminated */
        wchar_t *wz;
        } text;

    int textlen;    /* length of the text in bytes/wchars to be printed.
                       textlen is in multibyte or wide chars if _UNICODE */
    union {
        char sz[BUFFERSIZE];
#ifdef _UNICODE
        wchar_t wz[BUFFERSIZE];
#endif  /* _UNICODE */
        } buffer;
    wchar_t wchar;      /* temp wchar_t */
    int bufferiswide;   /* non-zero = buffer contains wide chars already */
    char *heapbuf;      /* non-zero = test.sz using heap buffer to be freed */

    textlen = 0;        /* no text yet */
    charsout = 0;       /* no characters written yet */
    state = ST_NORMAL;  /* starting state */
    heapbuf = NULL;     /* not using heap-allocated buffer */

    /* main loop -- loop while format character exist and no I/O errors */
    while ((ch = *format++) != _T('\0') && charsout >= 0) {
        chclass = find_char_class(ch);  /* find character class */
        state = find_next_state(chclass, state); /* find next state */

        /* execute code for each state */
        switch (state) {

        case ST_NORMAL:

        NORMAL_STATE:

            /* normal state -- just write character */
#ifdef _UNICODE
            bufferiswide = 1;
#else  /* _UNICODE */
            bufferiswide = 0;
            if (isleadbyte((int)(unsigned char)ch)) {
                WRITE_CHAR(ch, &charsout);
                ch = *format++;
                _ASSERTE (ch != _T('\0')); /* UNDONE: don't fall off format string */
            }
#endif  /* _UNICODE */
            WRITE_CHAR(ch, &charsout);
            break;

        case ST_PERCENT:
            /* set default value of conversion parameters */
            prefixlen = fldwidth = no_output = capexp = 0;
            flags = 0;
            precision = -1;
            bufferiswide = 0;   /* default */
            break;

        case ST_FLAG:
            /* set flag based on which flag character */
            switch (ch) {
            case _T('-'):
                flags |= FL_LEFT;   /* '-' => left justify */
                break;
            case _T('+'):
                flags |= FL_SIGN;   /* '+' => force sign indicator */
                break;
            case _T(' '):
                flags |= FL_SIGNSP; /* ' ' => force sign or space */
                break;
            case _T('#'):
                flags |= FL_ALTERNATE;  /* '#' => alternate form */
                break;
            case _T('0'):
                flags |= FL_LEADZERO;   /* '0' => pad with leading zeros */
                break;
            }
            break;

        case ST_WIDTH:
            /* update width value */
            if (ch == _T('*')) {
                /* get width from arg list */
                fldwidth = get_int_arg(&argptr);
                if (fldwidth < 0) {
                    /* ANSI says neg fld width means '-' flag and pos width */
                    flags |= FL_LEFT;
                    fldwidth = -fldwidth;
                }
            }
            else {
                /* add digit to current field width */
                fldwidth = fldwidth * 10 + (ch - _T('0'));
            }
            break;

        case ST_DOT:
            /* zero the precision, since dot with no number means 0
               not default, according to ANSI */
            precision = 0;
            break;

        case ST_PRECIS:
            /* update precison value */
            if (ch == _T('*')) {
                /* get precision from arg list */
                precision = get_int_arg(&argptr);
                if (precision < 0)
                    precision = -1; /* neg precision means default */
            }
            else {
                /* add digit to current precision */
                precision = precision * 10 + (ch - _T('0'));
            }
            break;

        case ST_SIZE:
            /* just read a size specifier, set the flags based on it */
            switch (ch) {
            case _T('l'):
                flags |= FL_LONG;   /* 'l' => long int or wchar_t */
                break;
#if !LONGDOUBLE_IS_DOUBLE || defined (_M_ALPHA)
            /*
             * Alpha has native 64-bit integer registers and operations.
             * The int and long types are 32 bits and an Alpha specific
             * __int64 type is 64 bits.  We also use the 'L' flag for
             * integer arguments to indicate 64-bit conversions (%Lx).
             */

            case _T('L'):
                flags |= FL_I64;    /* 'L' => __int64 */
                break;
#endif  /* !LONGDOUBLE_IS_DOUBLE || defined (_M_ALPHA) */

            case _T('I'):
                /*
                 * In order to handle the I, I32, and I64 size modifiers, we
                 * depart from the simple deterministic state machine. The
                 * code below scans for characters following the 'I',
                 * and defaults to 64 bit on WIN64 and 32 bit on WIN32
                 */
#if PTR_IS_INT64
                flags |= FL_I64;    /* 'I' => __int64 on WIN64 systems */
#endif  /* PTR_IS_INT64 */
                if ( (*format == _T('6')) && (*(format + 1) == _T('4')) )
                {
                    format += 2;
                    flags |= FL_I64;    /* I64 => __int64 */
                }
                else if ( (*format == _T('3')) && (*(format + 1) == _T('2')) )
                {
                    format += 2;
                    flags &= ~FL_I64;   /* I32 => __int32 */
                }
                else if ( (*format == _T('d')) ||
                          (*format == _T('i')) ||
                          (*format == _T('o')) ||
                          (*format == _T('u')) ||
                          (*format == _T('x')) ||
                          (*format == _T('X')) )
                {
                   /*
                    * Nothing further needed.  %Id (et al) is
                    * handled just like %d, except that it defaults to 64 bits
                    * on WIN64.  Fall through to the next iteration.
                    */
                }
                else {
                    state = ST_NORMAL;
                    goto NORMAL_STATE;
                }
                break;

            case _T('h'):
                flags |= FL_SHORT;  /* 'h' => short int or char */
                break;

/* UNDONE: support %wc and %ws for now only for compatibility */
            case _T('w'):
                flags |= FL_WIDECHAR;  /* 'w' => wide character */
                break;

            }
            break;

        case ST_TYPE:
            /* we have finally read the actual type character, so we       */
            /* now format and "print" the output.  We use a big switch     */
            /* statement that sets 'text' to point to the text that should */
            /* be printed, and 'textlen' to the length of this text.       */
            /* Common code later on takes care of justifying it and        */
            /* other miscellaneous chores.  Note that cases share code,    */
            /* in particular, all integer formatting is done in one place. */
            /* Look at those funky goto statements!                        */

            switch (ch) {

            case _T('C'):   /* ISO wide character */
                if (!(flags & (FL_SHORT|FL_LONG|FL_WIDECHAR)))
#ifdef _UNICODE
                    flags |= FL_SHORT;
#else  /* _UNICODE */
                    flags |= FL_WIDECHAR;   /* ISO std. */
#endif  /* _UNICODE */
                /* fall into 'c' case */

            case _T('c'): {
                /* print a single character specified by int argument */
#ifdef _UNICODE
                bufferiswide = 1;
                wchar = (wchar_t) get_int_arg(&argptr);
                if (flags & FL_SHORT) {
                    /* format multibyte character */
                    /* this is an extension of ANSI */
                    char tempchar[2];
                    {
                        tempchar[0] = (char)(wchar & 0x00ff);
                        tempchar[1] = '\0';
                    }

                    if (mbtowc(buffer.wz,tempchar,MB_CUR_MAX) < 0) {
                        /* ignore if conversion was unsuccessful */
                        no_output = 1;
                    }
                } else {
                    buffer.wz[0] = wchar;
                }
                text.wz = buffer.wz;
                textlen = 1;    /* print just a single character */
#else  /* _UNICODE */
                if (flags & (FL_LONG|FL_WIDECHAR)) {
                    wchar = (wchar_t) get_short_arg(&argptr);
                    /* convert to multibyte character */
                    textlen = wctomb(buffer.sz, wchar);

                    /* check that conversion was successful */
                    if (textlen < 0)
                        no_output = 1;
                } else {
                    /* format multibyte character */
                    /* this is an extension of ANSI */
                    unsigned short temp;
                    temp = (unsigned short) get_int_arg(&argptr);
                    {
                        buffer.sz[0] = (char) temp;
                        textlen = 1;
                    }
                }
                text.sz = buffer.sz;
#endif  /* _UNICODE */
            }
            break;

            case _T('Z'): {
                /* print a Counted String

                int i;
                char *p;       /* temps */
                struct _count_string {
                    short Length;
                    short MaximumLength;
                    char *Buffer;
                } *pstr;

                pstr = get_ptr_arg(&argptr);
                if (pstr == NULL || pstr->Buffer == NULL) {
                    /* null ptr passed, use special string */
                    text.sz = __nullstring;
                    textlen = (int)strlen(text.sz);
                } else {
                    if (flags & FL_WIDECHAR) {
                        text.wz = (wchar_t *)pstr->Buffer;
                        textlen = pstr->Length / (int)sizeof(wchar_t);
                        bufferiswide = 1;
                    } else {
                        bufferiswide = 0;
                        text.sz = pstr->Buffer;
                        textlen = pstr->Length;
                    }
                }
            }
            break;

            case _T('S'):   /* ISO wide character string */
#ifndef _UNICODE
                if (!(flags & (FL_SHORT|FL_LONG|FL_WIDECHAR)))
                    flags |= FL_WIDECHAR;
#else  /* _UNICODE */
                if (!(flags & (FL_SHORT|FL_LONG|FL_WIDECHAR)))
                    flags |= FL_SHORT;
#endif  /* _UNICODE */

            case _T('s'): {
                /* print a string --                            */
                /* ANSI rules on how much of string to print:   */
                /*   all if precision is default,               */
                /*   min(precision, length) if precision given. */
                /* prints '(null)' if a null string is passed   */

                int i;
                char *p;       /* temps */
                wchar_t *pwch;

                /* At this point it is tempting to use strlen(), but */
                /* if a precision is specified, we're not allowed to */
                /* scan past there, because there might be no null   */
                /* at all.  Thus, we must do our own scan.           */

                i = (precision == -1) ? INT_MAX : precision;
                text.sz = get_ptr_arg(&argptr);

/* UNDONE: handle '#' case properly */
                /* scan for null upto i characters */
#ifdef _UNICODE
                if (flags & FL_SHORT) {
                    if (text.sz == NULL) /* NULL passed, use special string */
                        text.sz = __nullstring;
                    p = text.sz;
                    for (textlen=0; textlen<i && *p; textlen++) {
                        if (isleadbyte((int)*p))
                            ++p;
                        ++p;
                    }
                    /* textlen now contains length in multibyte chars */
                } else {
                    if (text.wz == NULL) /* NULL passed, use special string */
                        text.wz = __wnullstring;
                    bufferiswide = 1;
                    pwch = text.wz;
                    while (i-- && *pwch)
                        ++pwch;
                    textlen = (int)(pwch - text.wz);       /* in wchar_ts */
                    /* textlen now contains length in wide chars */
                }
#else  /* _UNICODE */
                if (flags & (FL_LONG|FL_WIDECHAR)) {
                    if (text.wz == NULL) /* NULL passed, use special string */
                        text.wz = __wnullstring;
                    bufferiswide = 1;
                    pwch = text.wz;
                    while ( i-- && *pwch )
                        ++pwch;
                    textlen = (int)(pwch - text.wz);
                    /* textlen now contains length in wide chars */
                } else {
                    if (text.sz == NULL) /* NULL passed, use special string */
                        text.sz = __nullstring;
                    p = text.sz;
                    while (i-- && *p)
                        ++p;
                    textlen = (int)(p - text.sz);    /* length of the string */
                }

#endif  /* _UNICODE */
            }
            break;


            case _T('n'): {
                /* write count of characters seen so far into */
                /* short/int/long thru ptr read from args */

                void *p;        /* temp */

                p = get_ptr_arg(&argptr);

                /* store chars out into short/long/int depending on flags */
#if !LONG_IS_INT
                if (flags & FL_LONG)
                    *(long *)p = charsout;
                else
#endif  /* !LONG_IS_INT */

#if !SHORT_IS_INT
                if (flags & FL_SHORT)
                    *(short *)p = (short) charsout;
                else
#endif  /* !SHORT_IS_INT */
                    *(int *)p = charsout;

                no_output = 1;              /* force no output */
            }
            break;


            case _T('E'):
            case _T('G'):
                capexp = 1;                 /* capitalize exponent */
                ch += _T('a') - _T('A');    /* convert format char to lower */
                /* DROP THROUGH */
            case _T('e'):
            case _T('f'):
            case _T('g'): {
                /* floating point conversion -- we call cfltcvt routines */
                /* to do the work for us.                                */
                flags |= FL_SIGNED;         /* floating point is signed conversion */
                text.sz = buffer.sz;        /* put result in buffer */

                /* compute the precision value */
                if (precision < 0)
                    precision = 6;          /* default precision: 6 */
                else if (precision == 0 && ch == _T('g'))
                    precision = 1;          /* ANSI specified */
                else if (precision > MAXPRECISION)
                    precision = MAXPRECISION;

                if (precision > BUFFERSIZE - CVTBUFSIZE) {
                    /* conversion will potentially overflow local buffer */
                    /* so we need to use a heap-allocated buffer.        */
                    heapbuf = (char *)_malloc_crt(CVTBUFSIZE + precision);
                    if (heapbuf != NULL)
                        text.sz = heapbuf;
                    else
                        /* malloc failed, cap precision further */
                        precision = BUFFERSIZE - CVTBUFSIZE;
                }

#if !LONGDOUBLE_IS_DOUBLE
                /* do the conversion */
                if (flags & FL_LONGDOUBLE) {
                    LONGDOUBLE tmp;
                    tmp=va_arg(argptr, LONGDOUBLE);
                    /* Note: assumes ch is in ASCII range */
                    _cldcvt(&tmp, text.sz, (char)ch, precision, capexp);
                } else
#endif  /* !LONGDOUBLE_IS_DOUBLE */
                {
                    DOUBLE tmp;
                    tmp=va_arg(argptr, DOUBLE);
                    /* Note: assumes ch is in ASCII range */
                    _cfltcvt(&tmp,text.sz, (char)ch, precision, capexp);
                }

                /* '#' and precision == 0 means force a decimal point */
                if ((flags & FL_ALTERNATE) && precision == 0)
                    _forcdecpt(text.sz);

                /* 'g' format means crop zero unless '#' given */
                if (ch == _T('g') && !(flags & FL_ALTERNATE))
                    _cropzeros(text.sz);

                /* check if result was negative, save '-' for later */
                /* and point to positive part (this is for '0' padding) */
                if (*text.sz == '-') {
                    flags |= FL_NEGATIVE;
                    ++text.sz;
                }

                textlen = (int)strlen(text.sz);     /* compute length of text */
            }
            break;

            case _T('d'):
            case _T('i'):
                /* signed decimal output */
                flags |= FL_SIGNED;
                radix = 10;
                goto COMMON_INT;

            case _T('u'):
                radix = 10;
                goto COMMON_INT;

            case _T('p'):
                /* write a pointer -- this is like an integer or long */
                /* except we force precision to pad with zeros and */
                /* output in big hex. */

                precision = 2 * sizeof(void *);     /* number of hex digits needed */
#if PTR_IS_INT64
                flags |= FL_I64;                    /* assume we're converting an int64 */
#elif !PTR_IS_INT
                flags |= FL_LONG;                   /* assume we're converting a long */
#endif  /* !PTR_IS_INT */
                /* DROP THROUGH to hex formatting */

            case _T('X'):
                /* unsigned upper hex output */
                hexadd = _T('A') - _T('9') - 1;     /* set hexadd for uppercase hex */
                goto COMMON_HEX;

            case _T('x'):
                /* unsigned lower hex output */
                hexadd = _T('a') - _T('9') - 1;     /* set hexadd for lowercase hex */
                /* DROP THROUGH TO COMMON_HEX */

            COMMON_HEX:
                radix = 16;
                if (flags & FL_ALTERNATE) {
                    /* alternate form means '0x' prefix */
                    prefix[0] = _T('0');
                    prefix[1] = (TCHAR)(_T('x') - _T('a') + _T('9') + 1 + hexadd);  /* 'x' or 'X' */
                    prefixlen = 2;
                }
                goto COMMON_INT;

            case _T('o'):
                /* unsigned octal output */
                radix = 8;
                if (flags & FL_ALTERNATE) {
                    /* alternate form means force a leading 0 */
                    flags |= FL_FORCEOCTAL;
                }
                /* DROP THROUGH to COMMON_INT */

            COMMON_INT: {
                /* This is the general integer formatting routine. */
                /* Basically, we get an argument, make it positive */
                /* if necessary, and convert it according to the */
                /* correct radix, setting text and textlen */
                /* appropriately. */

#if _INTEGRAL_MAX_BITS >= 64       
                unsigned __int64 number;    /* number to convert */
                int digit;              /* ascii value of digit */
                __int64 l;              /* temp long value */
#else  /* _INTEGRAL_MAX_BITS >= 64        */
                unsigned long number;   /* number to convert */
                int digit;              /* ascii value of digit */
                long l;                 /* temp long value */
#endif  /* _INTEGRAL_MAX_BITS >= 64        */

                /* 1. read argument into l, sign extend as needed */
#if _INTEGRAL_MAX_BITS >= 64       
                if (flags & FL_I64)
                    l = get_int64_arg(&argptr);
                else
#endif  /* _INTEGRAL_MAX_BITS >= 64        */

#if !LONG_IS_INT
                if (flags & FL_LONG)
                    l = get_long_arg(&argptr);
                else
#endif  /* !LONG_IS_INT */

#if !SHORT_IS_INT
                if (flags & FL_SHORT) {
                    if (flags & FL_SIGNED)
                        l = (short) get_int_arg(&argptr); /* sign extend */
                    else
                        l = (unsigned short) get_int_arg(&argptr);    /* zero-extend*/
                } else
#endif  /* !SHORT_IS_INT */
                {
                    if (flags & FL_SIGNED)
                        l = get_int_arg(&argptr); /* sign extend */
                    else
                        l = (unsigned int) get_int_arg(&argptr);    /* zero-extend*/
                }

                /* 2. check for negative; copy into number */
                if ( (flags & FL_SIGNED) && l < 0) {
                    number = -l;
                    flags |= FL_NEGATIVE;   /* remember negative sign */
                } else {
                    number = l;
                }

#if _INTEGRAL_MAX_BITS >= 64       
                if ( (flags & FL_I64) == 0 ) {
                    /*
                     * Unless printing a full 64-bit value, insure values
                     * here are not in cananical longword format to prevent
                     * the sign extended upper 32-bits from being printed.
                     */
                    number &= 0xffffffff;
                }
#endif  /* _INTEGRAL_MAX_BITS >= 64        */

                /* 3. check precision value for default; non-default */
                /*    turns off 0 flag, according to ANSI. */
                if (precision < 0)
                    precision = 1;  /* default precision */
                else {
                    flags &= ~FL_LEADZERO;
                    if (precision > MAXPRECISION)
                        precision = MAXPRECISION;
                }

                /* 4. Check if data is 0; if so, turn off hex prefix */
                if (number == 0)
                    prefixlen = 0;

                /* 5. Convert data to ASCII -- note if precision is zero */
                /*    and number is zero, we get no digits at all.       */

                text.sz = &buffer.sz[BUFFERSIZE-1];    /* last digit at end of buffer */

                while (precision-- > 0 || number != 0) {
                    digit = (int)(number % radix) + '0';
                    number /= radix;                /* reduce number */
                    if (digit > '9') {
                        /* a hex digit, make it a letter */
                        digit += hexadd;
                    }
                    *text.sz-- = (char)digit;       /* store the digit */
                }

                textlen = (int)((char *)&buffer.sz[BUFFERSIZE-1] - text.sz); /* compute length of number */
                ++text.sz;          /* text points to first digit now */


                /* 6. Force a leading zero if FORCEOCTAL flag set */
                if ((flags & FL_FORCEOCTAL) && (text.sz[0] != '0' || textlen == 0)) {
                    *--text.sz = '0';
                    ++textlen;      /* add a zero */
                }
            }
            break;
            }

            /* At this point, we have done the specific conversion, and */
            /* 'text' points to text to print; 'textlen' is length.  Now we */
            /* justify it, put on prefixes, leading zeros, and then */
            /* print it. */

            if (!no_output) {
                int padding;    /* amount of padding, negative means zero */

                if (flags & FL_SIGNED) {
                    if (flags & FL_NEGATIVE) {
                        /* prefix is a '-' */
                        prefix[0] = _T('-');
                        prefixlen = 1;
                    }
                    else if (flags & FL_SIGN) {
                        /* prefix is '+' */
                        prefix[0] = _T('+');
                        prefixlen = 1;
                    }
                    else if (flags & FL_SIGNSP) {
                        /* prefix is ' ' */
                        prefix[0] = _T(' ');
                        prefixlen = 1;
                    }
                }

                /* calculate amount of padding -- might be negative, */
                /* but this will just mean zero */
                padding = fldwidth - textlen - prefixlen;

                /* put out the padding, prefix, and text, in the correct order */

                if (!(flags & (FL_LEFT | FL_LEADZERO))) {
                    /* pad on left with blanks */
                    WRITE_MULTI_CHAR(_T(' '), padding, &charsout);
                }

                /* write prefix */
                WRITE_STRING(prefix, prefixlen, &charsout);

                if ((flags & FL_LEADZERO) && !(flags & FL_LEFT)) {
                    /* write leading zeros */
                    WRITE_MULTI_CHAR(_T('0'), padding, &charsout);
                }

                /* write text */
#ifndef _UNICODE
                if (bufferiswide && (textlen > 0)) {
                    wchar_t *p;
                    int retval, count;
                    char buffer[MB_LEN_MAX+1];

                    p = text.wz;
                    count = textlen;
                    while (count--) {
                        retval = wctomb(buffer, *p++);
                        if (retval <= 0)
                            break;
                        WRITE_STRING(buffer, retval, &charsout);
                    }
                } else {
                    WRITE_STRING(text.sz, textlen, &charsout);
                }
#else  /* _UNICODE */
                if (!bufferiswide && textlen > 0) {
                    char *p;
                    int retval, count;

                    p = text.sz;
                    count = textlen;
                    while (count-- > 0) {
                        retval = mbtowc(&wchar, p, MB_CUR_MAX);
                        if (retval <= 0)
                            break;
                        WRITE_CHAR(wchar, &charsout);
                        p += retval;
                    }
                } else {
                    WRITE_STRING(text.wz, textlen, &charsout);
                }
#endif  /* _UNICODE */

                if (flags & FL_LEFT) {
                    /* pad on right with blanks */
                    WRITE_MULTI_CHAR(_T(' '), padding, &charsout);
                }

                /* we're done! */
            }

            if (heapbuf) {
                _free_crt(heapbuf);
                heapbuf = NULL;
            }

            break;
        }
    }

    return charsout;        /* return value = number of characters written */
}

/*
 *  Future Optimizations for swprintf:
 *  - Don't free the memory used for converting the buffer to wide chars.
 *    Use realloc if the memory is not sufficient.  Free it at the end.
 */

/***
*void write_char(char ch, int *pnumwritten)
*ifdef _UNICODE
*void write_char(wchar_t ch, FILE *f, int *pnumwritten)
*endif
*void write_char(char ch, FILE *f, int *pnumwritten)
*
*Purpose:
*   Writes a single character to the given file/console.  If no error occurs,
*   then *pnumwritten is incremented; otherwise, *pnumwritten is set
*   to -1.
*
*Entry:
*   _TCHAR ch        - character to write
*   FILE *f          - file to write to
*   int *pnumwritten - pointer to integer to update with total chars written
*
*Exit:
*   No return value.
*
*Exceptions:
*
*******************************************************************************/

#ifdef CPRFLAG

LOCAL(void) write_char (
    _TCHAR ch,
    int *pnumwritten
    )
{
#ifdef _UNICODE
    if (_putwch_lk(ch) == WEOF)
#else  /* _UNICODE */
    if (_putch_lk(ch) == EOF)
#endif  /* _UNICODE */
        *pnumwritten = -1;
    else
        ++(*pnumwritten);
}

#else  /* CPRFLAG */

LOCAL(void) write_char (
    _TCHAR ch,
    FILE *f,
    int *pnumwritten
    )
{
    if ( (f->_flag & _IOSTRG) && f->_base == NULL)
    {
        ++(*pnumwritten);
        return;
    }
#ifdef _UNICODE
    if (_putwc_lk(ch, f) == WEOF)
#else  /* _UNICODE */
    if (_putc_lk(ch, f) == EOF)
#endif  /* _UNICODE */
        *pnumwritten = -1;
    else
        ++(*pnumwritten);
}

#endif  /* CPRFLAG */

/***
*void write_multi_char(char ch, int num, int *pnumwritten)
*ifdef _UNICODE
*void write_multi_char(wchar_t ch, int num, FILE *f, int *pnumwritten)
*endif
*void write_multi_char(char ch, int num, FILE *f, int *pnumwritten)
*
*Purpose:
*   Writes num copies of a character to the given file/console.  If no error occurs,
*   then *pnumwritten is incremented by num; otherwise, *pnumwritten is set
*   to -1.  If num is negative, it is treated as zero.
*
*Entry:
*   _TCHAR ch        - character to write
*   int num          - number of times to write the characters
*   FILE *f          - file to write to
*   int *pnumwritten - pointer to integer to update with total chars written
*
*Exit:
*   No return value.
*
*Exceptions:
*
*******************************************************************************/

#ifdef CPRFLAG
LOCAL(void) write_multi_char (
    _TCHAR ch,
    int num,
    int *pnumwritten
    )
{
    while (num-- > 0) {
        write_char(ch, pnumwritten);
        if (*pnumwritten == -1)
            break;
    }
}

#else  /* CPRFLAG */

LOCAL(void) write_multi_char (
    _TCHAR ch,
    int num,
    FILE *f,
    int *pnumwritten
    )
{
    while (num-- > 0) {
        write_char(ch, f, pnumwritten);
        if (*pnumwritten == -1)
            break;
    }
}

#endif  /* CPRFLAG */

/***
*void write_string(char *string, int len, int *pnumwritten)
*void write_string(char *string, int len, FILE *f, int *pnumwritten)
*ifdef _UNICODE
*void write_string(wchar_t *string, int len, FILE *f, int *pnumwritten)
*endif
*void write_wstring(wchar_t *string, int len, int *pnumwritten)
*void write_wstring(wchar_t *string, int len, FILE *f, int *pnumwritten)
*
*Purpose:
*   Writes a string of the given length to the given file.  If no error occurs,
*   then *pnumwritten is incremented by len; otherwise, *pnumwritten is set
*   to -1.  If len is negative, it is treated as zero.
*
*Entry:
*   _TCHAR *string   - string to write (NOT null-terminated)
*   int len          - length of string
*   FILE *f          - file to write to
*   int *pnumwritten - pointer to integer to update with total chars written
*
*Exit:
*   No return value.
*
*Exceptions:
*
*******************************************************************************/

#ifdef CPRFLAG

LOCAL(void) write_string (
    _TCHAR *string,
    int len,
    int *pnumwritten
    )
{
    while (len-- > 0) {
        write_char(*string++, pnumwritten);
        if (*pnumwritten == -1)
            break;
    }
}

#else  /* CPRFLAG */

LOCAL(void) write_string (
    _TCHAR *string,
    int len,
    FILE *f,
    int *pnumwritten
    )
{
    if ( (f->_flag & _IOSTRG) && f->_base == NULL)
    {
        (*pnumwritten) += len;
        return;
    }
    while (len-- > 0) {
        write_char(*string++, f, pnumwritten);
        if (*pnumwritten == -1)
            break;
    }
}
#endif  /* CPRFLAG */


/***
*int get_int_arg(va_list *pargptr)
*
*Purpose:
*   Gets an int argument off the given argument list and updates *pargptr.
*
*Entry:
*   va_list *pargptr - pointer to argument list; updated by function
*
*Exit:
*   Returns the integer argument read from the argument list.
*
*Exceptions:
*
*******************************************************************************/

__inline int __cdecl get_int_arg (
    va_list *pargptr
    )
{
    return va_arg(*pargptr, int);
}

/***
*long get_long_arg(va_list *pargptr)
*
*Purpose:
*   Gets an long argument off the given argument list and updates *pargptr.
*
*Entry:
*   va_list *pargptr - pointer to argument list; updated by function
*
*Exit:
*   Returns the long argument read from the argument list.
*
*Exceptions:
*
*******************************************************************************/

#if !LONG_IS_INT
__inline long __cdecl get_long_arg (
    va_list *pargptr
    )
{
    return va_arg(*pargptr, long);
}
#endif  /* !LONG_IS_INT */

#if _INTEGRAL_MAX_BITS >= 64   
__inline __int64 __cdecl get_int64_arg (
    va_list *pargptr
    )
{
    return va_arg(*pargptr, __int64);
}
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

#ifndef _UNICODE
/***
*short get_short_arg(va_list *pargptr)
*
*Purpose:
*   Gets a short argument off the given argument list and updates *pargptr.
*   *** CURRENTLY ONLY USED TO GET A WCHAR_T, IFDEF _INTL ***
*
*Entry:
*   va_list *pargptr - pointer to argument list; updated by function
*
*Exit:
*   Returns the short argument read from the argument list.
*
*Exceptions:
*
*******************************************************************************/

#if !SHORT_IS_INT
__inline short __cdecl get_short_arg (
    va_list *pargptr
    )
{
    return va_arg(*pargptr, short);
}
#endif  /* !SHORT_IS_INT */
#endif  /* _UNICODE */
