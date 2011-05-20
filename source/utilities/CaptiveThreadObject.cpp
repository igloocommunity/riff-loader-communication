/*******************************************************************************
*
*    File name: CaptiveThreadObject.cpp
*     Language: Visual C++
*  Description: Captive Thread Object class definitions
*
*
* Copyright (C) ST-Ericsson SA 2011
* License terms: 3-clause BSD license
*
*******************************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          File CaptiveThreadObject.cpp

#include "CaptiveThreadObject.h"

CCaptiveThreadObject::CCaptiveThreadObject()
    : IsDying(0),
#pragma warning(disable: 4355) // 'this' used before initialized but ok as thread starts in inactive state
      Thread(ThreadEntry, this)
{
}
#pragma warning(default: 4355)

CCaptiveThreadObject::~CCaptiveThreadObject()
{
}

void CCaptiveThreadObject::EndCaptiveThread()
{
    IsDying++;
    SignalDeath();
    Thread.WaitToDie();
}


// ThreadEntry - Entry point, executed by captive thread
#ifdef _WIN32
unsigned int WINAPI CCaptiveThreadObject::ThreadEntry(void *arg)
#else
void *CCaptiveThreadObject::ThreadEntry(void *arg)
#endif
{
    CCaptiveThreadObject *pThis = static_cast<CCaptiveThreadObject *>(arg);
    pThis->InitializeCaptiveThreadObject();
    pThis->MainExecutionLoop();
    return 0;
}
//                                                                                   End of file CaptiveThreadObject.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
