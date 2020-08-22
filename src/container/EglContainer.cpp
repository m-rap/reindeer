#include "EglContainer.h"

void EglContainer::SubInit() {
    LOGI("init");
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
            EGL_RED_SIZE,        8,
            EGL_GREEN_SIZE,      8,
            EGL_BLUE_SIZE,       8,

            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,

            EGL_NONE
    };

    EGLint w, h, format;
    EGLint numConfigs = 128;
    EGLConfig config = NULL;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, NULL, NULL);

    eglChooseConfig(display, attribs, NULL, 0, &numConfigs);
    EGLConfig supportedConfigs[128];
    eglChooseConfig(display, attribs, supportedConfigs, 128, &numConfigs);

    int i = 0;
    for (; i < numConfigs; i++) {
        EGLConfig cfg = supportedConfigs[i];
        EGLint r, g, b, d;
        if (eglGetConfigAttrib(display, cfg, EGL_RED_SIZE, &r)   &&
            eglGetConfigAttrib(display, cfg, EGL_GREEN_SIZE, &g) &&
            eglGetConfigAttrib(display, cfg, EGL_BLUE_SIZE, &b)  &&
            r == 8 && g == 8 && b == 8 ) {

            config = supportedConfigs[i];
            break;
        }
    }
    if (i == numConfigs) {
        config = supportedConfigs[0];
    }

    if (config == NULL) {
        LOGI("Unable to initialize EGLConfig");
        return;
    }

    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    surface = eglCreateWindowSurface(display, config, window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGI("Unable to eglMakeCurrent");
        return;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    LOGI("OpenGL Info: %s", glGetString(GL_VENDOR));
    LOGI("OpenGL Info: %s", glGetString(GL_RENDERER));
    LOGI("OpenGL Info: %s", glGetString(GL_VERSION));
    LOGI("OpenGL Info: %s", glGetString(GL_EXTENSIONS));

    width = w;
    height = h;
    this->display = display;
    this->surface = surface;
    this->context = context;

    LOGI("w %d h %d", width, height);

    LOGI("init done");
    
    running = true;
}

void EglContainer::Deinit() {
    LOGI("deinit %d", running);
    if (!running) {
        return;
    }
    running = false;

    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (context != EGL_NO_CONTEXT) {
        eglDestroyContext(display, context);
    }
    if (surface != EGL_NO_SURFACE) {
        eglDestroySurface(display, surface);
    }
    eglTerminate(display);

    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
}
