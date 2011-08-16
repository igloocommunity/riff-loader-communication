<?xml version="1.0" encoding="UTF-8"?>
<!--
* Copyright (C) ST-Ericsson SA 2011
* License terms: 3-clause BSD license
-->
<stylesheet version="1.0" xmlns="http://www.w3.org/1999/XSL/Transform">

<output method="text"/>
<strip-space elements="*"/>
<param name="target"/>

<template match="/commandspec">/* $Copyright$ */
/* NOTE: This is an automatically generated file. DO NOT EDIT! */

#ifndef _ERRORCODE_H
#define _ERRORCODE_H


/**
 *  @addtogroup ldr_LCM
 *  Error codes for internal loader commands.
 *  @{
 */

/*************************************************************************
* Includes
*************************************************************************/
#include &quot;t_basicdefinitions.h&quot;

#define A2_ERROR_CODES_OFFSET 5000
/**
 *  Internal loader command error codes. Possible range 0x0000 - 0x0FFE (4094).
 */


/**
 * Table for Error groups range
 *
 * General Fatal 0-50
 * General non-fatal 51-99
 *
 * IO Fatal 100-150
 * IO non-fatal 151-199
 *
 * Communication Fatal 200-250
 * Communication non-fatal 251-299
 *
 * Signature Fatal 300-350
 * Signature non-fatal 351-399
 *
 * Authentication Fatal 400-450
 * Authentication non-fatal 451-499
 *
 * COPS General Fatal 500-550
 * COPS General non-fatal 551-599
 *
 * System Fatal 600-650
 * System non-fatal 651-699
 *
 * Flash Fatal 700-750
 * Flash non-fatal 751-799
 *
 * Parameters Fatal 800-850
 * Parameters non-fatal 851-899
 *
 * File management Fatal 900-950
 * File management non-fatal 951-999
 *
 * Command Auditing and execution Fatal 1000-1050
 * Command Auditing and execution non-fatal 1051-1099
 *
 * Emulation Fatal 1100-1150
 * Emulation non-fatal 1151-1199
 *
 * Timers Fatal 1200-1250
 * Timers non-fatal 1251-1299
 *
 * CABS Fatal 1300-1350
 * CABS non-fatal 1351-1399
 *
 * GDFS Fatal 1400-1450
 * GDFS non-fatal 1451-1499
 *
 * Antirollback Fatal 1500-1550
 * Antirollback non-fatal 1551-1599
 *
 * Memory and Boot Fatal 1600-1650
 * Memory and Boot non-fatal 1651-1699
 *
 * @todo this should be removed and error codes should be remaped.
 * The same applies to Emulator errors.
 *
 * Job Handler Fatal 1700-1750
 * Job Handler non-fatal 1751-1799
 *
 * Emulator Fatal 1800-1850
 * Emulator non-fatal 1851-1899
 *
 * Loader utilities Fatal 1900-1950
 * Loader utilities non-fatal 1951-1999
 */

<apply-templates select="status"/>
#endif /* _ERRORCODE_H */
</template>

<template match="status">
typedef enum {
<apply-templates select="value"/>} ErrorCode_e;
</template>

<template match="value">
  <text>  </text><value-of select="@name"/> = <value-of select="@number"/>, /**&lt;  <value-of select="@short"/> */
</template>

<template match="value[last()]">
  <text>  </text><value-of select="@name"/> = <value-of select="@number"/>  /**&lt; <value-of select="@short"/> */
</template>

</stylesheet>
