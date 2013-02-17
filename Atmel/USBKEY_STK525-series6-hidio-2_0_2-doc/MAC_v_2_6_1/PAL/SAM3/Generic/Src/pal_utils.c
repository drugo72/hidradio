/**
 * @file pal_utils.c
 *
 * @brief Utilities for PAL for SAM3 MCUs
 *
 * This file implementes utilities for the PAL module for SAM3 MCUs.
 *
 * $Id: pal_utils.c 24850 2011-01-12 10:10:52Z mahendran.p $
 *
 * @author    Atmel Corporation: http://www.atmel.com
 * @author    Support email: avr@atmel.com
 */
/*
 * Copyright (c) 2010, Atmel Corporation All rights reserved.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */
/* === Includes ============================================================ */

#if (DEBUG > 0)
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "pal.h"
#ifdef TEST_HARNESS
#include "sio_handler.h"
#endif  /* TEST_HARNESS */

/* === Macros ============================================================== */

#ifdef TEST_HARNESS
    #define ASSERT_BUFFER_SIZE          (172)

    #define ASSERT_INDICATION           (0x9F)

    /*
     * Message length is limited to LARGE_BUFFER_SIZE - 5 to include room for
     * SOT, EOT, Length, total buffer length and command code
     */
    #define ASSERT_MESSAGE_LENGTH       (ASSERT_BUFFER_SIZE - 5)
#else
    /**
     * The event payload can be max 255 bytes, 1 byte goes as length byte
     * for octetstring and 1 byte as command code
     */
    #define MAX_OCTETSTRING_SIZE        (253)
#endif  /* TEST_HARNESS */

/* === Globals ============================================================= */

#ifdef TEST_HARNESS
    static uint8_t assert_msg[sizeof(assert_t) + ASSERT_BUFFER_SIZE - sizeof(uint8_t)];
#else
    /* Holds the assert message to be printed. */
    static char tmpbuf[MAX_OCTETSTRING_SIZE];
#endif  /* TEST_HARNESS */

/* === Prototypes ========================================================== */


/* === Implementation ====================================================== */

/**
 * @brief Tests for Assertion
 *
 * This function tests the assertion of a given expression and
 * if the expression fails, a message is printed. This function
 * is implemented similar to the C library function except
 * that the processing will not be aborted if the assertion fails.
 *
 * @param expression To be tested for assertion
 * @param message Data to be printed over SIO
 * @file file File in which assertion has to be tested.
 * @line line Line number on which assertion has to be tested.
 */
void pal_assert(bool expression,
                FLASH_STRING_T message,
                int8_t *file,
                uint16_t line)
{
    /*
     * Assert for the expression. This expression should be true always,
     * false indicates that something went wrong
     */
    if (!expression)
    {
#ifdef TEST_HARNESS
        assert_t *assert_details = (assert_t *)&assert_msg[0];
        uint8_t assert_string[ASSERT_BUFFER_SIZE];

        /* Only used for test environment */
        assert_details->assert_cmdcode = ASSERT_INDICATION;

        /* Copy the assertion message to RAM */
        PGM_STRCPY((char *)assert_string, message);

        /*
         * Put total length of message to be printed in data[0] of echo
         * indication. Data will be copied from failure_msg->data[1]
         * The function snprintf is used for copying variable
         * number of charaters into failure_msg->data
         */
        assert_details->data[0] = snprintf((char *)&(assert_details->data[1]),
            ASSERT_MESSAGE_LENGTH,"%s, line %d: assertion %s failed -",
            file, line, assert_string);

        /*
         * The function snprintf returns the number of characters that
         * would have been printed if there was enough room.
         * Take the actual number of bytes that are printed
         */
        assert_details->data[0] =
          strlen((const char *)&(assert_details->data[1]));

        /* Total size of echo indication */
        assert_details->size = sizeof(assert_t) + assert_details->data[0] -
                                sizeof(assert_details->size);

        /* Write into UART/USB */
        sio_write((uint8_t*)assert_details);
#else
        /* Standard for all applications */
        PGM_STRCPY(tmpbuf, message);
        tmpbuf[PGM_STRLEN(message)+1] = '\0';
        PRINTF("Assertion Failed on File %s, line %d, expression %s\n",
                file, line, tmpbuf);
#endif  /* TEST_HARNESS */
    }
}

#endif  /* (DEBUG > 0) */


#if (defined __GNUC__)
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "pal.h"
#include "armtypes.h"

#ifdef SIO_HUB
#include "sio_handler.h"

/**
 * @brief Read from a file.
 * This function is a reentrant version of read. It takes a pointer to the
 * global data block, which holds errno.
 */
int _read_r (struct _reent *r, int file, char * ptr, int len)
{
    r = r;
    file = file;
#ifndef TEST_HARNESS
    int i;
    int data;

    if(len == 0)
    {
        return 0;
    }

    *ptr++  = _sio_getchar(NULL);

    for(i = 1; i < len; i++)
    {

        data = _sio_getchar_nowait(NULL );
        if(data == -1)
        {
            break;
        }
        *ptr++  = data;
    }

    return i;
#else
    errno = EINVAL;
    return 0;
#endif
}


/**
 * @brief Set position in a file..
 * This function is a reentrant version of lseek.
 */
int _lseek_r (struct _reent *r, int file, int ptr, int dir)
{
    r = r;
    file = file;
    ptr = ptr;
    dir = dir;
    return 0;
}


/**
 * @brief Write to a file.
 * This function is a reentrant version of write.
 */
int _write_r (struct _reent *r, int file, char * ptr, int len)
{
    r = r;
#ifndef TEST_HARNESS
    int i;
#endif

    if(ptr == NULL)
    {
        return -1;
    }

#ifndef TEST_HARNESS
    for ( i = 0; i < len; i++ )
    {
        _sio_putchar(ptr[i], NULL );
    }
#else
    pal_sio_tx(SIO_CHANNEL, (uint8_t*)ptr, (uint8_t)len);
#endif
    return len;
}


/**
 * @brief Close a file.
 * This function is a reentrant version of write.
 */
int _close_r (struct _reent *r, int file)
{
    return 0;
}



/**
 * @brief Status of an open file.
 * This function is a reentrant version of fstat.
 */
int _fstat_r (struct _reent *r, int file, struct stat * st)
{
    r = r;
    file = file;

    memset (st, 0, sizeof (* st));
    st->st_mode = S_IFCHR;
    return 0;
}



/**
 * @brief Query whether output stream is a terminal.
 * This function is a reentrant version of isatty.
 */
int _isatty_r(struct _reent *r, int fd)
{
    r = r;
    fd = fd;
    return 1;
}

#endif /* #ifdef SIO_HUB*/


/**
 * @brief Increase program data space.
 * As malloc and related functions depend on this,
 * it is useful to have a working implementation. The following suffices for a
 * standalone system; it exploits the symbol _end automatically defined by the
 * GNU linker.
 */
register char * stack_ptr asm ("sp");
caddr_t _sbrk_r (struct _reent *r, int incr)
{
    extern char   end asm ("end");
    static char * heap_end;
    char * prev_heap_end;

    if (heap_end == NULL)
        heap_end = & end;

    prev_heap_end = heap_end;

    if (heap_end + incr > stack_ptr)
    {
        errno = ENOMEM;
        return (caddr_t) -1;
    }

    heap_end += incr;

    return (caddr_t) prev_heap_end;
}

#endif
/* EOF */
