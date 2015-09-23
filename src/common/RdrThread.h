#ifndef RDRTHREAD_H
#define RDRTHREAD_H

class RdrMutex
{
public:
    virtual int Lock() = 0;
    virtual int Unlock() = 0;
};

class Runnable
{
public:
    virtual void Run() = 0;
};

class RdrThread
{
public:
    virtual int Start() = 0;
    virtual int Join() = 0;
};

class RdrEvent
{
public:
    virtual void Wait() = 0;
    virtual int Set() = 0;
};

#endif // RUNNABLE_H
