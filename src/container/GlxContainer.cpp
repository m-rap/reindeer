#include "GlxContainer.h"

bool GlxContainer::ctxErrorOccurred = false;

GlxContainer::GlxContainer()
{
}

GlxContainer::~GlxContainer()
{
    //dtor
}

void GlxContainer::Init(int argc, char *argv[])
{
    display = XOpenDisplay(NULL);

    if (!display) {
        printf("Failed to open X display\n");
        exit(1);
    }

    // Get a matching FB config
    static int visual_attribs[] = {
        GLX_X_RENDERABLE    , True,
        GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
        GLX_RENDER_TYPE     , GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
        GLX_RED_SIZE        , 8,
        GLX_GREEN_SIZE      , 8,
        GLX_BLUE_SIZE       , 8,
        GLX_ALPHA_SIZE      , 8,
        GLX_DEPTH_SIZE      , 24,
        GLX_STENCIL_SIZE    , 8,
        GLX_DOUBLEBUFFER    , True,
        //GLX_SAMPLE_BUFFERS  , 1,
        //GLX_SAMPLES         , 4,
        None
    };

    int glx_major, glx_minor;

    // FBConfigs were added in GLX version 1.3.
    if (!glXQueryVersion(display, &glx_major, &glx_minor) ||
        ((glx_major == 1) && (glx_minor < 3)) ||
        (glx_major < 1)) {
        printf("Invalid GLX version");
        exit(1);
    }

    printf("Getting matching framebuffer configs\n");
    int fbcount;
    GLXFBConfig* fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);
    if (!fbc) {
        printf("Failed to retrieve a framebuffer config\n");
        exit(1);
    }
    printf("Found %d matching FB configs.\n", fbcount);

    // Pick the FB config/visual with the most samples per pixel
    printf("Getting XVisualInfos\n");
    int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

    int i;
    for (i = 0; i < fbcount; ++i) {
        XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc[i]);
        if (vi) {
            int samp_buf, samples;
            glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
            glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLES       , &samples );

            printf("  Matching fbconfig %d, visual ID 0x%2x: SAMPLE_BUFFERS = %d,"
                   " SAMPLES = %d\n",
                   i, vi -> visualid, samp_buf, samples);

            if (best_fbc < 0 || samp_buf && samples > best_num_samp)
                best_fbc = i, best_num_samp = samples;
            if (worst_fbc < 0 || !samp_buf || samples < worst_num_samp)
                worst_fbc = i, worst_num_samp = samples;
        }
        XFree(vi);
    }

    GLXFBConfig bestFbc = fbc[best_fbc];

    // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
    XFree( fbc );

    // Get a visual
    XVisualInfo *vi = glXGetVisualFromFBConfig(display, bestFbc);
    printf("Chosen visual ID = 0x%x\n", vi->visualid);

    printf("Creating colormap\n");
    XSetWindowAttributes swa;
    Colormap cmap;
    swa.colormap = cmap = XCreateColormap(display,
                                          RootWindow(display, vi->screen),
                                          vi->visual, AllocNone);
    swa.background_pixmap = None ;
    swa.border_pixel      = 0;
    swa.event_mask        = StructureNotifyMask;

    printf("Creating window\n");
    width = SCREEN_WIDTH;
    height = SCREEN_HEIGHT;
    window = XCreateWindow(display, RootWindow(display, vi->screen),
                           0, 0, width, height, 0, vi->depth, InputOutput,
                           vi->visual,
                           CWBorderPixel | CWColormap | CWEventMask, &swa);
    if (!window) {
        printf( "Failed to create window.\n" );
        exit(1);
    }

    // Done with the visual info data
    XFree(vi);

    XStoreName(display, window, "GL 3.0 Window");

    printf("Mapping window\n");
    XMapWindow(display, window);

    // Get the default screen's GLX extension list
    const char *glxExts = glXQueryExtensionsString(display, DefaultScreen(display));

    GLXContext ctx = 0;

    // Install an X error handler so the application won't exit if GL 3.0
    // context allocation fails.
    //
    // Note this error handler is global.  All display connections in all threads
    // of a process use the same error handler, so be sure to guard against other
    // threads issuing X commands while this code is running.
    ctxErrorOccurred = false;
    int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);

    ctx = glXCreateNewContext(display, bestFbc, GLX_RGBA_TYPE, 0, True);

    // Sync to ensure any errors generated are processed.
    XSync(display, False);

    // Restore the original error handler
    XSetErrorHandler(oldHandler);

    if (ctxErrorOccurred || !ctx) {
        printf("Failed to create an OpenGL context\n");
        exit(1);
    }

    // Verifying that context is a direct context
    if (!glXIsDirect(display, ctx)) {
        printf( "Indirect GLX rendering context obtained\n" );
    } else {
        printf( "Direct GLX rendering context obtained\n" );
    }

    printf("Making context current\n");
    glXMakeCurrent(display, window, ctx);

    thread.Start(this);
}

void GlxContainer::Run()
{
    Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wmDeleteMessage, 1);

    bool opened = true;
    bool loaded = false;

    while (opened) {
        XNextEvent(display, &event);

        switch (event.type) {

        case Expose:
            if (!loaded) {
                OnLoad();
                loaded = true;
            }
            break;

        case KeyPress:
            OnKeyPress(&event);
            break;

        case ClientMessage:
            if (event.xclient.data.l[0] == wmDeleteMessage) {
                opened = false;
                XDestroyWindow(display, event.xclient.window);
            }
            break;

        case ConfigureNotify:
            XConfigureEvent xce = event.xconfigure;
            if (xce.width != width || xce.height != height) {
                width = xce.width;
                height = xce.height;
                OnResize();
            }
            break;
        }
    }

    OnClosing();
    XCloseDisplay(display);
    OnClosed();
}

void GlxContainer::OnLoad()
{
}

void GlxContainer::OnClosing()
{
}

void GlxContainer::OnClosed()
{
}

void GlxContainer::OnKeyPress(XEvent* e)
{
}

void GlxContainer::OnResize()
{
}
