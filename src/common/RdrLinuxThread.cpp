#include "RdrLinuxThread.h"

void *pthreadFunc(void *param)
{
    RdrRunnable *r = static_cast<RdrRunnable*>(param);
    r->Run();
    return NULL;
}

RdrLinuxMutex::RdrLinuxMutex()
{
    pthread_mutex_init(&mutex, NULL);
}

RdrLinuxMutex::~RdrLinuxMutex()
{
    pthread_mutex_destroy(&mutex);
}

int RdrLinuxMutex::Lock()
{
    return pthread_mutex_lock(&mutex);
}

int RdrLinuxMutex::Unlock()
{
    return pthread_mutex_unlock(&mutex);
}

RdrLinuxThread::RdrLinuxThread()
{
}

RdrLinuxThread::~RdrLinuxThread()
{
}

int RdrLinuxThread::Start(RdrRunnable* runnable)
{
    return pthread_create(&thread, 0, pthreadFunc, runnable);
}

int RdrLinuxThread::Join()
{
    return pthread_join(thread, NULL);
}

RdrLinuxEvent::RdrLinuxEvent()
{
    flag = false;
    pthread_cond_init(&signal, NULL);
}

RdrLinuxEvent::~RdrLinuxEvent()
{
}

void RdrLinuxEvent::Wait()
{
    protect.Lock();
    while (!flag)
        pthread_cond_wait(&signal, &protect.mutex);
    flag = false;
    protect.Unlock();
}

int RdrLinuxEvent::Set()
{
    protect.Lock();
    flag = true;
    protect.Unlock();
    return pthread_cond_signal(&signal);
}
