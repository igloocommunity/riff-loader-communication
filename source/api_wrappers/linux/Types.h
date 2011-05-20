/*******************************************************************************
 * Copyright (C) ST-Ericsson SA 2011
 * License terms: 3-clause BSD license
 ******************************************************************************/

#ifndef _TYPES_H
#define _TYPES_H

const unsigned int os_minus_one = ~0;
#define INFINITE os_minus_one

#define WAIT_OBJECT_0    0
#define WAIT_TIMEOUT     0x00000102L
#define WAIT_FAILED      (DWORD)0xFFFFFFFF

typedef unsigned int DWORD;

#define WINAPI

#endif /* _TYPES_H */

