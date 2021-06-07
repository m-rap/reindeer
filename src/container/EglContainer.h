#ifndef EGLCONTAINER_H
#define EGLCONTAINER_H

#include "Container.h"

#include <EGL/egl.h>


class EglContainer : public Container {
protected:
    virtual void SubInit();

public:
    EGLint width, height;
    EGLSurface surface;
    EGLContext context;
    EGLDisplay display;
    NativeWindowType window;
    bool running;

    EglContainer() : Container() {
    }
    virtual ~EglContainer() { }

    virtual int KEY_LEFT() { return 0; }
    virtual int KEY_RIGHT() { return 0; }
    virtual int KEY_UP() { return 0; }
    virtual int KEY_DOWN() { return 0; }
    virtual int KEY_M() { return 0; }
    virtual int KEY_m() { return 0; }

    virtual int ShouldClose() { return 0; }
    virtual void Deinit();
    virtual void PreUpdate() { }
    virtual void PostUpdate() {
        eglSwapBuffers(display, surface);
    }
    virtual void ReadInput() { }
    //virtual void OnLoad() { }

};

#endif // CONTAINER_H
