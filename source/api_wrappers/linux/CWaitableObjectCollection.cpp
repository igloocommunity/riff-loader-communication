/*******************************************************************************
 * Copyright (C) ST-Ericsson SA 2011
 * License terms: 3-clause BSD license
 ******************************************************************************/

#include <time.h>
#include <errno.h>

#include "Types.h"
#include "CWaitableObjectCollection.h"
#include <assert.h>

CWaitableObjectCollection::CWaitableObjectCollection()
{
    m_objs.clear();
}

CWaitableObjectCollection::~CWaitableObjectCollection()
{
    m_objs.clear();
}

void CWaitableObjectCollection::Add(CWaitableObject *obj)
{
    m_objs.push_back(obj);
}

CWaitableObject *CWaitableObjectCollection::Wait(DWORD dwTimeout)
{
    vector<CWaitableObject *>::iterator it;
    DWORD dwTimePassed = 0;
    struct timespec ts;
    struct timespec curr_time, start_time;

    if (-1 == clock_gettime(CLOCK_REALTIME, &start_time)) {
        return NULL;
    }

    do {
        for (it = m_objs.begin(); it != m_objs.end(); ++it) {
            assert(*it);

            if (WAIT_OBJECT_0 == (*it)->Wait(0)) {
                return (*it);
            }
        }

        ts.tv_sec = 0;
        ts.tv_nsec = 10000000L;  // 10 milliseconds

        // There isn't any possiblity of errno returning NULL, mean is not defined. Even if
        // errno is not defined this is the least thing that we should care for.

        // coverity[returned_null]
        while (-1 == nanosleep(&ts, &ts) && EINTR == errno);

        if (-1 == clock_gettime(CLOCK_REALTIME, &curr_time)) {
            return NULL;
        }

        dwTimePassed = 1000 * (curr_time.tv_sec - start_time.tv_sec) + \
                       (curr_time.tv_nsec - start_time.tv_nsec) / 1000000;

    } while (dwTimePassed < dwTimeout);

    return NULL;
}
