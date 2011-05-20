/******************************************************************************
*
* Copyright (C) ST-Ericsson SA 2011
* License terms: 3-clause BSD license
*
******************************************************************************/

#ifndef _C_COMPILER_H
#define _C_COMPILER_H

/*************************************************************************
*
*     HEADER SPECIFICATION
*
*     $Workfile: c_compiler.h $
*
**************************************************************************
*
* DESCRIPTION:
*
* This file contains macros that determines current compiler
*
*
**************************************************************************
*
* REVISION HISTORY:
*
* $Log: \KIRND_FuncBlocks_Utilities_003\cnh1606432\dependencies\Utils\c_compiler.h $
 *
 *    Version: ...\cnh1606432\cnh1606432_r1\7 18 Nov 2009 10:18 (GMT) qsjomik
 * Overwrite merge.
 * Compiled with _INTERCEPTCALLBACKS pre-compiler directive.
 * Stability, Multi-Dut and storing BufferVector updates.
 * Updated for LCM R1B.
 * R1A027.
 *
 *    Version: ...\cnh160692_r1a_dev\3 12 Mar 2002 14:21 (GMT) QCSSTZI
 * rename p_compiler to c_compiler.
 *
 *    Version: ...\cnh160692_r1a_dev\2 10 Jan 2002 12:35 (GMT) QCSSTZI
 * Updated to reflect R4A013 on the old module (cnh1010024)
 *
 *    Version: ...\cnh1010024_r4a_dev\1 11 Sep 2001 07:34 (GMT) QCSPAAB
 * Merge to Valentina (R4)
 *
 *    Version: ...\cnh1010024\1 14 Nov 2000 15:30 (GMT) ECSJENE
 * Merge to start new dev-branch for the Mia project.
 *
 *    Version: ...\cnh1010024_r2a_dev\4 05 Oct 2000 06:25 (GMT) ECSSTMA
 * Macro to remove non_banked directive.
 *
 *    Version: ...\cnh1010024_r2a_dev\3 19 Sep 2000 12:38 (GMT) QCSLAIS
 * Compiler warnings work around.
 *
 *    Version: ...\cnh1010024_r2a_dev\2 22 May 2000 08:25 (GMT) ECSSTMA
 * Accept LINT as being one among the compilers.
 *
 *    Version: ...\cnh1010024_r2a_dev\1 16 May 2000 12:19 (GMT) QCSLAIS
 * Created file compiler.h which determines current compiler and target
*
**************************************************************************/

#if defined(__IAR_SYSTEMS_ICC) || defined(__IAR_SYSTEMS_ICC__)
/* COMPILER IS FROM IAR */
#if   ((__TID__ >> 8) & 0x7f) == 23
/* TARGET IS Z80 */
#define COMPILER_IAR_Z80
#elif ((__TID__ >> 8) & 0x7f) == 90
/* TARGET IS AVR */
#define COMPILER_IAR_AVR /* Preferred */
#define COMPILER_AVR
#define AT90S
#elif ((__TID__ >> 8) & 0x7f) == 0x4f
/* TARGET IS ARM */
#define COMPILER_IAR_ARM /* Preferred */
#define COMPILER_ARM
#define ARM
#else
#error Unknown IAR compiler
#endif
#elif defined(_WIN32)
/* TARGET IS Win32 */
#define COMPILER_MSDEV
#define __arm
#define __pcs
#define __no_init
#elif defined(__arm)
/* TARGET IS ARM */
#define COMPILER_ARM_ARM /* Preferred */
#define COMPILER_ARM
#define ARM
#elif defined(__IAR_SYSTEMS_ASM__)
#elif defined(_lint)
#elif defined(__GNUC__)
#define COMPILER_GNUC
#else
#error Unknown compiler
#endif

/*
 * Some compiler bug work arounds.
 */

/*
 * Avoid ARM compiler error when comparing NULL with a pointer
 * by defining NULL as NULL pointer.
 */
#if defined(COMPILER_ARM_ARM) && !defined(NULL)
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

/*
 * Some modules still use the unused banked prefix.
 */
#ifndef banked
#define banked
#endif

#ifndef non_banked
#define non_banked
#endif

#endif // _C_COMPILER_H
