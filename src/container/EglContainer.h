#ifndef CONTAINER_H
#define CONTAINER_H

#include "Container.h"

#include <EGL/egl.h>

#ifdef __ANDROID_API__

#include <android_native_app_glue.h>
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

#endif


//
//struct Vertex2 {
//    float x, y, z;
//    unsigned char r, g, b, a;
//};
//
//struct Container {
//    EGLint width, height;
//    EGLSurface surface;
//    EGLContext context;
//    EGLDisplay display;
//    android_app* app;
//    bool running;
//    bool animating;
//
//    GLenum mode;
//    Vertex2* vtxBuffer = NULL;
//    GLushort* idxBuffer = NULL;
//    GLushort vtxBuffSize;
//    GLushort idxBuffSize;
//    float x, y;
//    float rotation;
//    float scale;
//    GLuint vbo;
//    GLuint ibo;
//
//    int init() {
//        LOGI("init");
//        const EGLint attribs[] = {
//                EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
//                EGL_RED_SIZE,        8,
//                EGL_GREEN_SIZE,      8,
//                EGL_BLUE_SIZE,       8,
//
//                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
//
//                EGL_NONE
//        };
//
//        EGLint w, h, format;
//        EGLint numConfigs = 128;
//        EGLConfig config = NULL;
//        EGLSurface surface;
//        EGLContext context;
//
//        EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
//
//        eglInitialize(display, NULL, NULL);
//
//        eglChooseConfig(display, attribs, NULL, 0, &numConfigs);
//        EGLConfig supportedConfigs[128];
//        eglChooseConfig(display, attribs, supportedConfigs, 128, &numConfigs);
//
//        int i = 0;
//        for (; i < numConfigs; i++) {
//            EGLConfig cfg = supportedConfigs[i];
//            EGLint r, g, b, d;
//            if (eglGetConfigAttrib(display, cfg, EGL_RED_SIZE, &r)   &&
//                eglGetConfigAttrib(display, cfg, EGL_GREEN_SIZE, &g) &&
//                eglGetConfigAttrib(display, cfg, EGL_BLUE_SIZE, &b)  &&
//                eglGetConfigAttrib(display, cfg, EGL_DEPTH_SIZE, &d) &&
//                r == 8 && g == 8 && b == 8 && d == 0 ) {
//
//                config = supportedConfigs[i];
//                break;
//            }
//        }
//        if (i == numConfigs) {
//            config = supportedConfigs[0];
//        }
//
//        if (config == NULL) {
//            LOGI("Unable to initialize EGLConfig");
//            return -1;
//        }
//
//        eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
//        surface = eglCreateWindowSurface(display, config, app->window, NULL);
//        context = eglCreateContext(display, config, NULL, NULL);
//
//        if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
//            LOGI("Unable to eglMakeCurrent");
//            return -1;
//        }
//
//        eglQuerySurface(display, surface, EGL_WIDTH, &w);
//        eglQuerySurface(display, surface, EGL_HEIGHT, &h);
//
//        LOGI("OpenGL Info: %s", glGetString(GL_VENDOR));
//        LOGI("OpenGL Info: %s", glGetString(GL_RENDERER));
//        LOGI("OpenGL Info: %s", glGetString(GL_VERSION));
//        LOGI("OpenGL Info: %s", glGetString(GL_EXTENSIONS));
//
//        width = w;
//        height = h;
//        this->display = display;
//        this->surface = surface;
//        this->context = context;
//
//        LOGI("w %d h %d", width, height);
//
//        glEnable(GL_CULL_FACE);
//        glDisable(GL_DEPTH_TEST);
//        glViewport(0, h / 2 - w / 2, w, w);
//
//        glGenBuffers(1, &vbo);
//        glGenBuffers(1, &ibo);
//
//        vtxBuffer = (Vertex2*)malloc(sizeof(Vertex2) * 4);
//        idxBuffer = (GLushort *)malloc(sizeof(GLushort) * 6);
//
//        LOGI("init done");
//
//        return 0;
//    }
//
//    int deinit() {
//        LOGI("deinit %d", running);
//        if (!running) {
//            return 0;
//        }
//        running = false;
//        animating = false;
//
//        free(vtxBuffer);
//        free(idxBuffer);
//        glDeleteBuffers(1, &vbo);
//        glDeleteBuffers(1, &ibo);
//
//        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
//        if (context != EGL_NO_CONTEXT) {
//            eglDestroyContext(display, context);
//        }
//        if (surface != EGL_NO_SURFACE) {
//            eglDestroySurface(display, surface);
//        }
//        eglTerminate(display);
//
//        display = EGL_NO_DISPLAY;
//        context = EGL_NO_CONTEXT;
//        surface = EGL_NO_SURFACE;
//
//        return 0;
//    }
//
//    void draw() {
//        glMatrixMode(GL_MODELVIEW);
//        glPushMatrix();
//
//        glTranslatef(x, y, 0);
//        glRotatef(rotation, 0, 0, 1);
//        glScalef(scale, scale, scale);
//
//        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        glEnableClientState(GL_VERTEX_ARRAY);
//        glEnableClientState(GL_COLOR_ARRAY);
//
//        glVertexPointer(3, GL_FLOAT, sizeof(Vertex2), (void*)offsetof(Vertex2, x));
//        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex2), (void*)offsetof(Vertex2, r));
//
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
//        glDrawElements(mode, idxBuffSize, GL_UNSIGNED_SHORT, 0);
//
//        glDisableClientState(GL_VERTEX_ARRAY);
//        glDisableClientState(GL_COLOR_ARRAY);
//
//        eglSwapBuffers(display, surface);
//    }
//
//    void constructDraw() {
//        x = 0;
//        y = 0;
//        scale = 1;
//        rotation = 0;
//        vtxBuffSize = 0;
//        idxBuffSize = 0;
//
//        uint8_t r = 255, g = 0, b = 255, a = 255;
//        vtxBuffer[vtxBuffSize++] = {-0.3, -0.2, 0, r, g, b, a};
//        vtxBuffer[vtxBuffSize++] = {0.3, -0.2, 0, r, g, b, a};
//        vtxBuffer[vtxBuffSize++] = {0.3, 0.2, 0, r, g, b, a};
//        vtxBuffer[vtxBuffSize++] = {-0.3, 0.2, 0, r, g, b, a};
//        
//        idxBuffer[idxBuffSize++] = 0;
//        idxBuffer[idxBuffSize++] = 1;
//        idxBuffer[idxBuffSize++] = 2;
//        idxBuffer[idxBuffSize++] = 0;
//        idxBuffer[idxBuffSize++] = 2;
//        idxBuffer[idxBuffSize++] = 3;
//
//        mode = GL_TRIANGLES;
//
//        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        glBufferData(GL_ARRAY_BUFFER, vtxBuffSize * sizeof(Vertex2), vtxBuffer, GL_STATIC_DRAW);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxBuffSize * sizeof(GLushort), idxBuffer, GL_STATIC_DRAW);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//        running = true;
//    }
//};
//
//void engine_handle_cmd(struct android_app* app, int32_t cmd) {
//    LOGI("engine_handle_cmd %d", cmd);
//    if (app->userData == NULL) {
//        return;
//    }
//    Container* container1 = (Container*)app->userData;
//    switch (cmd) {
//        case APP_CMD_SAVE_STATE:
//            break;
//        case APP_CMD_INIT_WINDOW:
//            if (container1->app->window != nullptr) {
//                container1->init();
//                container1->constructDraw();
//            }
//            break;
//        case APP_CMD_TERM_WINDOW:
//            container1->deinit();
//            break;
//        case APP_CMD_GAINED_FOCUS:
//            container1->animating = true;
//            break;
//        case APP_CMD_LOST_FOCUS:
//            container1->animating = false;
//            break;
//        default:
//            break;
//    }
//    LOGI("engine_handle_cmd %d done", cmd);
//}


class EglContainer : public Container {
protected:
    virtual void SubInit();

public:
    EGLint width, height;
    EGLSurface surface;
    EGLContext context;
    EGLDisplay display;
    NativeWindowType window;

    EglContainer() : Container() {
        ConstructDraw = NULL;
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
    virtual void OnLoad() { }

};

#endif // CONTAINER_H
