#ifndef LINUXTHREAD_H
#define LINUXTHREAD_H

#include "RdrThread.h"
#include <pthread.h>

class RdrLinuxThread
{
protected:
    pthread_t thread;

public:
    RdrLinuxThread();
    virtual ~RdrLinuxThread();

    void Start(Runnable* runnable);
    void Join();
};

#endif // LINUXTHREAD_H
