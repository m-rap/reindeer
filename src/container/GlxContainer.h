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
    int shouldClose;
    int currentKey;
    GLXContext ctx;
    Visual* visual;
    int screen;
    int depth;
    XSetWindowAttributes swa;
    GLXFBConfig bestFbc;

    void FbSetup();

public:
    GlxContainer();
    virtual ~GlxContainer();

    static bool ctxErrorOccurred;

    static int ctxErrorHandler(Display *dpy, XErrorEvent *ev)
    {
        ctxErrorOccurred = true;
        return 0;
    }

    virtual int KEY_LEFT() { return XK_Left; }
    virtual int KEY_RIGHT() { return XK_Right; }
    virtual int KEY_UP() { return XK_Up; }
    virtual int KEY_DOWN() { return XK_Down; }
    virtual int KEY_M() { return XK_M; }
    virtual int KEY_m() { return XK_m; }

    virtual int ShouldClose() { return shouldClose; }
    virtual void Init(int argc, char *argv[]);
    virtual void PreUpdate() {}
    virtual void PostUpdate() {}
    virtual void ReadInput();

    virtual void Run();

    void OnLoad();
    void OnClosing();
    void OnClosed();
    void OnResize();

protected:
private:
};

class GlxInputResetThread :
    public Runnable
{
private:
    RdrLinuxThread thread;
    GlxContainer *container;

public:
    GlxInputResetThread(GlxContainer* container)
    {
        this->container = container;
        thread.Start(this);
    }

    virtual void Run()
    {

    }
};

#endif // GLXCONTAINER_H
