/*******************************************************************************
 * Copyright (C) ST-Ericsson SA 2011
 * License terms: 3-clause BSD license
 ******************************************************************************/

#include <errno.h>
#include <sys/time.h>

#include "Types.h"
#include "CEventObject.h"
#include "OS.h"

// ******************************************************************************
// Name:  CEventObject()
// Desc:  CEventObject constructor which initializes the m_sem member
// Ret:
// ******************************************************************************
CEventObject::CEventObject()
{
    sem_init(&m_sem, 0, 0);
}

// ******************************************************************************
// Name:  ~CEventObject()
// Desc:  CEventObject destructor
// Ret:
// ******************************************************************************
CEventObject::~CEventObject()
{
    sem_destroy(&m_sem);
}

// ******************************************************************************
// Name:  SetEvent()
// Desc:  Sets an event by post-ing the member semaphore m_sem
// Ret:
// ******************************************************************************
void CEventObject::SetEvent()
{
    sem_post(&m_sem);
}

// ******************************************************************************
// Name:  Wait()
// Desc:  implementation of the pure virtual base class member Wait()
// Ret:   WAIT_OBJECT_0 when event occurs, or WAIT_TIMEOUT on timeout
// ******************************************************************************
DWORD CEventObject::Wait(DWORD dwTimeout)
{
    if (INFINITE == dwTimeout) {
        sem_wait(&m_sem);
        return WAIT_OBJECT_0;
    } else {
        timespec absolute_time = OS::GetAbsoluteTime(dwTimeout);
        int ret;

        /* coverity[returned_null] */
        while (-1 == (ret = sem_timedwait(&m_sem, &absolute_time)) && errno == EINTR);

        if (0 == ret) {
            return WAIT_OBJECT_0;
        } else {
            return WAIT_TIMEOUT;
        }
    }
}
