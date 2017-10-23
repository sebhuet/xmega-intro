/*
 * Copyright (c) 2009, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */
MODULE ccp_write_io
PUBLIC ccp_write_io
RSEG CODE


ccp_write_io:
        in R1, 0x3F
        cli

        movw R31:R30, R17:R16

#ifdef RAMPZ
	ldi R16, 0
        out 0x34, R16
#endif

        ldi R16, 0xD8
        out 0x34, R16

#if (__MEMORY_MODEL__ == 1)
        st z, R17
#elif (__MEMORY_MODEL__ == 2)
        st z, R18
#else /* (__MEMORY_MODEL__ == 3) || (__MEMORY_MODEL__ == 5) */
        st z, R19
#endif /* __MEMORY_MODEL__ */

        out 0x3F, R1
        ret
ENDMOD


MODULE ccp_unprotected_write_io
PUBLIC ccp_unprotected_write_io
RSEG CODE
ccp_unprotected_write_io:
        movw R31:R30, R17:R16

#ifdef RAMPZ
	ldi R16, 0
        out 0x34, R16
#endif

        ldi R16, 0xD8
        out 0x34, R16

#if (__MEMORY_MODEL__ == 1)
        st z, R17
#elif (__MEMORY_MODEL__ == 2)
        st z, R18
#else /* (__MEMORY_MODEL__ == 3) || (__MEMORY_MODEL__ == 5) */
        st z, R19
#endif /* __MEMORY_MODEL__ */

        ret
ENDMOD

END
