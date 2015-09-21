#ifndef GLXCONTAINER_H
#define GLXCONTAINER_H

#include "Container.h"
#include "../common/RdrLinuxThread.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>

class GlxContainer :
    public Container,
    public Runnable
{
private:
    Window window;
    Display *display;
    XEvent event;
    int width, height;
    RdrLinuxThread thread;

public:
    GlxContainer();
    virtual ~GlxContainer();

    static bool ctxErrorOccurred;

    static int ctxErrorHandler(Display *dpy, XErrorEvent *ev)
    {
        ctxErrorOccurred = true;
        return 0;
    }

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

    virtual void Run();

    void OnLoad();
    void OnClosing();
    void OnClosed();
    void OnKeyPress(XEvent* e);
    void OnResize();

protected:
private:
};

#endif // GLXCONTAINER_H
