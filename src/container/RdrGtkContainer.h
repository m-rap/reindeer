#ifndef RDRGTKCONTAINER_H
#define RDRGTKCONTAINER_H

#include "Container.h"

class RdrGtkContainer :
    public Container
{
public:
    RdrGtkContainer();
    virtual ~RdrGtkContainer();

    virtual int KEY_LEFT() { return -1; }
    virtual int KEY_RIGHT() { return -1; }
    virtual int KEY_UP() { return -1; }
    virtual int KEY_DOWN() { return -1; }
    virtual int KEY_M() { return -1; }

    virtual int ShouldClose() { return -1; }
    virtual void Init(int argc, char *argv[]);
    virtual void PreUpdate() {}
    virtual void PostUpdate() {}
    virtual void ReadInput() {}
};

#endif // GTKCONTAINER_H
