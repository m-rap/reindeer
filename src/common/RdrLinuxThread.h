#ifndef RDRLINUXTHREAD_H
#define RDRLINUXTHREAD_H

#include "RdrThread.h"
#include <pthread.h>

class RdrLinuxEvent;

class RdrLinuxMutex
{
friend class RdrLinuxEvent;
private:
    pthread_mutex_t mutex;
public:
    RdrLinuxMutex();
    ~RdrLinuxMutex();

    virtual int Lock();
    virtual int Unlock();
};

class RdrLinuxThread
{
protected:
    pthread_t thread;

public:
    RdrLinuxThread();
    virtual ~RdrLinuxThread();

    int Start(RdrRunnable* runnable);
    int Join();
};

class RdrLinuxEvent :
    public RdrEvent
{
private:
    bool flag;
    RdrLinuxMutex protect;
    pthread_cond_t signal;
public:
    RdrLinuxEvent();
    ~RdrLinuxEvent();

    virtual void Wait();
    virtual int Set();
};

#endif // LINUXTHREAD_H
