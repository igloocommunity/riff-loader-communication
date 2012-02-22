/*******************************************************************************
 * Copyright (C) ST-Ericsson SA 2011
 * License terms: 3-clause BSD license
 ******************************************************************************/

#ifndef _CSEMAPHORE_H
#define _CSEMAPHORE_H

#include <pthread.h>
#include <semaphore.h>
#include "CWaitableObject.h"

class CSemaphore : public CWaitableObject
{
public:
    CSemaphore(unsigned int initial_count = 0);
    virtual ~CSemaphore();
    bool Release(unsigned int count = 1);
    DWORD Wait(DWORD timeout = INFINITE);

private:
    sem_t *m_semaphore;
};

#endif /* _CSEMAPHORE_H */

