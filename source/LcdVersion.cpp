/*******************************************************************************
 * Copyright (C) ST-Ericsson SA 2011
 * License terms: 3-clause BSD license
 ******************************************************************************/
#include "lcdriver_error_codes.h"
#include "LcmInterface.h"
#include "Error.h"
#ifdef _WIN32
#include "WinApiWrappers.h"
#else
#include "LinuxApiWrappers.h"
#include <dlfcn.h>
#define GetProcAddress dlsym
#endif
/**
 * var char *LCD_LCM_CompatibilityList[]
 * brief ASCII string list holding the permitted LCM versions.
 * This table contains compatibility information for the versions of LCM.
 * Current LCM version is defined in file lcm_version.c in LCM code.
 */
char *LCD_LCM_CompatibilityList[] = {"PX5",
                                      NULL
                                    };
