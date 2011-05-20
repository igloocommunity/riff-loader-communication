/*******************************************************************************
 * Copyright (C) ST-Ericsson SA 2011
 * License terms: 3-clause BSD license
 ******************************************************************************/

#include <errno.h>
#include <semaphore.h>
#include <sys/time.h>
#include "Types.h"
#include "CSemaphore.h"
#include "OS.h"

CSemaphore::CSemaphore(unsigned int initial_count)
{
    sem_init(&m_semaphore, 0, initial_count);
}

CSemaphore::~CSemaphore()
{
    sem_destroy(&m_semaphore);
}

bool CSemaphore::Release(unsigned int count)
{
    if (!count) {
        return false;
    }

    for (unsigned int i = 0; i < count; ++i) {
        if (sem_post(&m_semaphore)) {
            return false;
        }
    }

    return true;
}

DWORD CSemaphore::Wait(DWORD timeout)
{
    if (INFINITE == timeout) {
        sem_wait(&m_semaphore);
    } else {
        timespec absoulute_time = OS::GetAbsoluteTime(timeout);
        int ret;

        /* coverity[returned_null] */
        while (-1 == (ret = sem_timedwait(&m_semaphore, &absoulute_time)) && errno == EINTR);

        if (0 != ret) {
            return WAIT_TIMEOUT;
        }
    }

    return WAIT_OBJECT_0;
}
