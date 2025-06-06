/***
*istream.h - definitions/declarations for the istream class
*
*       Copyright (c) 1990-2001, Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       This file defines the classes, values, macros, and functions
*       used by the istream class.
*       [AT&T C++]
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#ifdef __cplusplus

#ifndef _INC_ISTREAM
#define _INC_ISTREAM

#if !defined (_WIN32)
#error ERROR: Only Win32 target supported!
#endif  /* !defined (_WIN32) */

#ifndef _CRTBLD
/* This version of the header files is NOT for user programs.
 * It is intended for use when building the C runtimes ONLY.
 * The version intended for public use will not have this message.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

#ifdef _MSC_VER
// Currently, all MS C compilers for Win32 platforms default to 8 byte
// alignment.
#pragma pack(push,8)

#include <useoldio.h>

#endif  /* _MSC_VER */

/* Define _CRTIMP */

#ifndef _CRTIMP
#ifdef CRTDLL
#define _CRTIMP __declspec(dllexport)
#else  /* CRTDLL */
#ifdef _DLL
#define _CRTIMP __declspec(dllimport)
#else  /* _DLL */
#define _CRTIMP
#endif  /* _DLL */
#endif  /* CRTDLL */
#endif  /* _CRTIMP */

#ifndef _INTERNAL_IFSTRIP_
/* Define _CRTIMP1 */

#ifndef _CRTIMP1
#ifdef CRTDLL1
#define _CRTIMP1 __declspec(dllexport)
#else  /* CRTDLL1 */
#define _CRTIMP1 _CRTIMP
#endif  /* CRTDLL1 */
#endif  /* _CRTIMP1 */
#endif  /* _INTERNAL_IFSTRIP_ */

#include <ios.h>

#ifdef _MSC_VER
// C4069: "long double != double"
#pragma warning(disable:4069)   // disable C4069 warning
// #pragma warning(default:4069)    // use this to reenable, if desired

// C4514: "unreferenced inline function has been removed"
#pragma warning(disable:4514) // disable C4514 warning
// #pragma warning(default:4514)    // use this to reenable, if desired
#endif  /* _MSC_VER */

#ifndef _INTERNAL_IFSTRIP_
#define MAXLONGSIZ  16
#endif  /* _INTERNAL_IFSTRIP_ */

typedef long streamoff, streampos;

class _CRTIMP1 istream : virtual public ios {

public:
    istream(streambuf*);
    virtual ~istream();

    int  ipfx(int =0);
    void isfx() { unlockbuf(); unlock(); }

    inline istream& operator>>(istream& (__cdecl * _f)(istream&));
    inline istream& operator>>(ios& (__cdecl * _f)(ios&));
    istream& operator>>(char *);
    inline istream& operator>>(unsigned char *);
    inline istream& operator>>(signed char *);
    istream& operator>>(char &);
    inline istream& operator>>(unsigned char &);
    inline istream& operator>>(signed char &);
    istream& operator>>(short &);
    istream& operator>>(unsigned short &);
    istream& operator>>(int &);
    istream& operator>>(unsigned int &);
    istream& operator>>(long &);
    istream& operator>>(unsigned long &);
    istream& operator>>(float &);
    istream& operator>>(double &);
    istream& operator>>(long double &);
    istream& operator>>(streambuf*);

    int get();

    inline istream& get(         char *,int,char ='\n');
    inline istream& get(unsigned char *,int,char ='\n');
    inline istream& get(  signed char *,int,char ='\n');

    istream& get(char &);
    inline istream& get(unsigned char &);
    inline istream& get(  signed char &);

    istream& get(streambuf&,char ='\n');
    inline istream& getline(         char *,int,char ='\n');
    inline istream& getline(unsigned char *,int,char ='\n');
    inline istream& getline(  signed char *,int,char ='\n');

    inline istream& ignore(int =1,int =EOF);
    istream& read(char *,int);
    inline istream& read(unsigned char *,int);
    inline istream& read(signed char *,int);

    int gcount() const { return x_gcount; }
    int peek();
    istream& putback(char);
    int sync();

    istream& seekg(streampos);
    istream& seekg(streamoff,ios::seek_dir);
    streampos tellg();

    void eatwhite();

protected:
    istream();
    istream(const istream&);    // treat as private
    istream& operator=(streambuf* _isb); // treat as private
    istream& operator=(const istream& _is) { return operator=(_is.rdbuf()); }
    istream& get(char *, int, int);
     int do_ipfx(int);

private:
    istream(ios&);
    int getint(char *);
    int getdouble(char *, int);
    int _fGline;
    int x_gcount;
};

    inline istream& istream::operator>>(istream& (__cdecl * _f)(istream&)) { (*_f)(*this); return *this; }
    inline istream& istream::operator>>(ios& (__cdecl * _f)(ios&)) { (*_f)(*this); return *this; }

    inline istream& istream::operator>>(unsigned char * _s) { return operator>>((char *)_s); }
    inline istream& istream::operator>>(  signed char * _s) { return operator>>((char *)_s); }

    inline istream& istream::operator>>(unsigned char & _c) { return operator>>((char &) _c); }
    inline istream& istream::operator>>(  signed char & _c) { return operator>>((char &) _c); }

    inline istream& istream::get(         char * _b, int _lim, char _delim) { return get(        _b, _lim, (int)(unsigned char)_delim); }
    inline istream& istream::get(unsigned char * _b, int _lim, char _delim) { return get((char *)_b, _lim, (int)(unsigned char)_delim); }
    inline istream& istream::get(signed   char * _b, int _lim, char _delim) { return get((char *)_b, _lim, (int)(unsigned char)_delim); }

    inline istream& istream::get(unsigned char & _c) { return get((char &)_c); }
    inline istream& istream::get(  signed char & _c) { return get((char &)_c); }

    inline istream& istream::getline(         char * _b,int _lim,char _delim) { lock(); _fGline++; get(        _b, _lim, (int)(unsigned char)_delim); unlock(); return *this; }
    inline istream& istream::getline(unsigned char * _b,int _lim,char _delim) { lock(); _fGline++; get((char *)_b, _lim, (int)(unsigned char)_delim); unlock(); return *this; }
    inline istream& istream::getline(  signed char * _b,int _lim,char _delim) { lock(); _fGline++; get((char *)_b, _lim, (int)(unsigned char)_delim); unlock(); return *this; }

    inline istream& istream::ignore(int _n,int _delim) { lock(); _fGline++; get((char *)0, _n+1, _delim); unlock(); return *this; }

    inline istream& istream::read(unsigned char * _ptr, int _n) { return read((char *) _ptr, _n); }
    inline istream& istream::read(  signed char * _ptr, int _n) { return read((char *) _ptr, _n); }

class _CRTIMP1 istream_withassign : public istream {
        public:
            istream_withassign();
            istream_withassign(streambuf*);
            ~istream_withassign();
    istream& operator=(const istream& _is) { return istream::operator=(_is); }
    istream& operator=(streambuf* _isb) { return istream::operator=(_isb); }
};

extern _CRTIMP1 istream_withassign cin;

inline _CRTIMP1 istream& __cdecl ws(istream& _ins) { _ins.eatwhite(); return _ins; }

_CRTIMP1 ios&        __cdecl dec(ios&);
_CRTIMP1 ios&        __cdecl hex(ios&);
_CRTIMP1 ios&        __cdecl oct(ios&);

#ifdef _MSC_VER
// Restore default packing
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_ISTREAM */

#endif  /* __cplusplus */
