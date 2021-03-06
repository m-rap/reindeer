#ifndef GLXCONTAINER_H
#define GLXCONTAINER_H

#include "Container.h"
//#include "../common/RdrLinuxThread.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

class GlxContainer :
    public Container
{
protected:
    Window window;
    Window root;
    Display *display;
    XEvent event;
    //RdrLinuxThread thread;
    int shouldClose;
    int currentKey;
    GLXContext ctx;
    XVisualInfo* vi;
    Visual* visual;
    int screen;
    int depth;
    XSetWindowAttributes swa;
    GLXFBConfig bestFbc;
    Atom wmDeleteMessage;
    bool checkMask;

    virtual void SubInit();
    virtual void InitDisplay();
    inline void InitFb();
    virtual void InitWindow();
    virtual void DeinitWindowAndDisplay();

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

    virtual int ShouldClose();
    virtual void Deinit();

    virtual void PreUpdate() {}
    virtual void PostUpdate();
    virtual void ReadInput();

    void OnClosed();
};

class GlxInputResetThread
//    :
//    public RdrRunnable
{
private:
    //RdrLinuxThread thread;
    GlxContainer *container;

public:
    GlxInputResetThread(GlxContainer* container)
    {
        this->container = container;
        //thread.Start(this);
    }

    virtual void Run()
    {
    }
};

#endif // GLXCONTAINER_H
