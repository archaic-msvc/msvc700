;***
;memmove.asm -
;
;       Copyright (c) 1991-2001, Microsoft Corporation.  All rights reserved.
;
;Purpose:
;       memmove() copies a source memory buffer to a destination buffer.
;       Overlapping buffers are treated specially, to avoid propogation.
;
;       NOTE:  This stub module scheme is compatible with NT build
;       procedure.
;
;*******************************************************************************

MEM_MOVE EQU 1
INCLUDE Intel\MEMCPY.ASM
