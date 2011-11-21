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
 * var char *LCD_VersionList[]
 * brief ASCII string list variable holding the LCD version/build time/product number..
 */
char *LCD_VersionList[] = {"PX2",
                           NULL
                          };
