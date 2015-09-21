#include "RdrLinuxThread.h"

void *pthreadFunc(void *param)
{
    Runnable *r = static_cast<Runnable*>(param);
    r->Run();
}

RdrLinuxThread::RdrLinuxThread()
{
    //ctor
}

RdrLinuxThread::~RdrLinuxThread()
{
    //dtor
}

void RdrLinuxThread::Start(Runnable* runnable)
{
    pthread_create(&thread, 0, pthreadFunc, (void*)runnable);
}

void RdrLinuxThread::Join()
{
    pthread_join(thread, NULL);
}
