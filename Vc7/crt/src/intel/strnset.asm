        page    ,132
        title   strnset - set first n characters to one char.
;***
;strnset.asm - set first n characters to single character
;
;       Copyright (c) 1985-2001, Microsoft Corporation. All rights reserved.
;
;Purpose:
;       defines _strnset() - sets at most the first n characters of a string
;       to a given character.
;
;*******************************************************************************

        .xlist
        include cruntime.inc
        .list

page
;***
;char *_strnset(string, val, count) - set at most count characters to val
;
;Purpose:
;       Sets the first count characters of string the character value.
;       If the length of string is less than count, the length of
;       string is used in place of n.
;
;       Algorithm:
;       char *
;       _strnset (string, val, count)
;             char *string,val;
;             unsigned int count;
;             {
;             char *start = string;
;
;             while (count-- && *string)
;                     *string++ = val;
;             return(start);
;             }
;
;Entry:
;       char *string - string to set characters in
;       char val - character to fill with
;       unsigned count - count of characters to fill
;
;Exit:
;       returns string, now filled with count copies of val.
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************

        CODESEG

        public  _strnset
_strnset proc \
        uses edi ebx, \
        string:ptr byte, \
        val:byte, \
        count:IWORD


        mov     edi,[string]    ; di = string
        mov     edx,edi         ; dx=string addr; save return value
        mov     ebx,[count]     ; cx = max chars to set
        xor     eax,eax         ; null byte
        mov     ecx,ebx
        jecxz   short done      ; zero length specified

repne   scasb                   ; find null byte & count bytes in cx
        jne     short nonull    ; null not found
        add     ecx,1           ; don't want the null

nonull:
        sub     ebx,ecx         ; bx=strlen (not null)
        mov     ecx,ebx         ; cx=strlen (not null)

        mov     edi,edx         ; restore string pointer
        mov     al,val          ; byte value
rep     stosb                   ; fill 'er up

done:
        mov     eax,edx         ; return value: string addr

        ret                     ; _cdecl return

_strnset endp
        end
