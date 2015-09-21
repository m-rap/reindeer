#ifndef RUNNABLE_H
#define RUNNABLE_H

class Runnable
{
public:
    virtual void Run() = 0;
};

class RdrThread
{
public:
    virtual void Start() = 0;
    virtual void Join() = 0;
};

#endif // RUNNABLE_H
