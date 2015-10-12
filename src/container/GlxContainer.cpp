#include "GlxContainer.h"

// Helper to check for extension string presence.  Adapted from:
//   http://www.opengl.org/resources/features/OGLextensions/
static bool isExtensionSupported(const char *extList, const char *extension)
{
    const char *start;
    const char *where, *terminator;

    /* Extension names should not have spaces. */
    where = strchr(extension, ' ');
    if (where || *extension == '\0')
        return false;

    /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
    for (start=extList;;) {
        where = strstr(start, extension);

        if (!where)
            break;

        terminator = where + strlen(extension);

        if ( where == start || *(where - 1) == ' ' )
            if ( *terminator == ' ' || *terminator == '\0' )
                return true;

        start = terminator;
    }

    return false;
}

bool GlxContainer::ctxErrorOccurred = false;

GlxContainer::GlxContainer()
{
    shouldClose = 0;
    currentKey = -1;
    checkMask = false;
}

GlxContainer::~GlxContainer()
{
    glXMakeCurrent(display, 0, 0);
    glXDestroyContext(display, ctx);

    XDestroyWindow(display, window);
    XFreeColormap(display, swa.colormap);
    XCloseDisplay(display);
}

void GlxContainer::FbSetup()
{
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

            printf("  Matching fbconfig %d, visual ID 0x%2x: SAMPLE_BUFFERS = %d, SAMPLES = %d\n", i, vi -> visualid, samp_buf, samples);

            if (best_fbc < 0 || samp_buf && samples > best_num_samp)
                best_fbc = i, best_num_samp = samples;
            if (worst_fbc < 0 || !samp_buf || samples < worst_num_samp)
                worst_fbc = i, worst_num_samp = samples;
        }
        XFree(vi);
    }

    bestFbc = fbc[best_fbc];

    // Be sure to free the FBConfig list allocated by glXChooseFBConfig()
    XFree( fbc );

    // Get a visual
    vi = glXGetVisualFromFBConfig(display, bestFbc);
    visual = vi->visual;
    screen = vi->screen;
    depth = vi->depth;
    printf("Chosen visual ID = 0x%x\n", vi->visualid);

    printf("Creating colormap\n");
    swa.colormap = XCreateColormap(display,
                                   RootWindow(display, vi->screen),
                                   vi->visual, AllocNone);
    swa.background_pixmap = None;
    swa.border_pixel      = 0;

}

void GlxContainer::Init(int argc, char *argv[])
{
    display = XOpenDisplay(NULL);
    screen = 0;
    depth = DefaultDepth(display, 0);

    if (!display) {
        printf("Failed to open X display\n");
        exit(1);
    }

    FbSetup();
    //visual = DefaultVisual(display, 0);

    swa.background_pixel = XWhitePixel(display, 0);
    swa.event_mask       = KeyPressMask | KeyRelease |
                           ExposureMask |
                           PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
                           StructureNotifyMask;

    printf("Creating window\n");
    width = SCREEN_WIDTH;
    height = SCREEN_HEIGHT;
    window = XCreateWindow(display, RootWindow(display, screen),
                           0, 0, width, height, 0, depth, InputOutput,
                           visual,
                           CWBorderPixel | CWColormap | CWEventMask, &swa);
    if (!window) {
        printf("Failed to create window.\n");
        exit(1);
    }

    // Done with the visual info data
    XFree(vi);

    XStoreName(display, window, "GL 3.0 Window");

    printf("Mapping window\n");
    XMapWindow(display, window);

    // Get the default screen's GLX extension list
    const char *glxExts = glXQueryExtensionsString(display, DefaultScreen(display));

    ctx = 0;

    // NOTE: It is not necessary to create or make current to a context before
    // calling glXGetProcAddressARB
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

    // Install an X error handler so the application won't exit if GL 3.0
    // context allocation fails.
    //
    // Note this error handler is global.  All display connections in all threads
    // of a process use the same error handler, so be sure to guard against other
    // threads issuing X commands while this code is running.
    ctxErrorOccurred = false;
    int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);

    // Check for the GLX_ARB_create_context extension string and the function.
    // If either is not present, use GLX 1.3 context creation method.
    if (!isExtensionSupported(glxExts, "GLX_ARB_create_context") ||
        !glXCreateContextAttribsARB) {
        printf( "glXCreateContextAttribsARB() not found"
                " ... using old-style GLX context\n" );
        ctx = glXCreateNewContext( display, bestFbc, GLX_RGBA_TYPE, 0, True );
    }

    // If it does, try to get a GL 3.0 context!
    else
    {
        int context_attribs[] =
        {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 0,
            //GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            None
        };

        printf( "Creating context\n" );
        ctx = glXCreateContextAttribsARB( display, bestFbc, 0,
                                          True, context_attribs );

        // Sync to ensure any errors generated are processed.
        XSync( display, False );
        if ( !ctxErrorOccurred && ctx )
            printf( "Created GL 3.0 context\n" );
        else
        {
            // Couldn't create GL 3.0 context.  Fall back to old-style 2.x context.
            // When a context version below 3.0 is requested, implementations will
            // return the newest context version compatible with OpenGL versions less
            // than version 3.0.
            // GLX_CONTEXT_MAJOR_VERSION_ARB = 1
            context_attribs[1] = 1;
            // GLX_CONTEXT_MINOR_VERSION_ARB = 0
            context_attribs[3] = 0;

            ctxErrorOccurred = false;

            printf( "Failed to create GL 3.0 context"
                    " ... using old-style GLX context\n" );
            ctx = glXCreateContextAttribsARB( display, bestFbc, 0,
                                            True, context_attribs );
        }
    }

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

    wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wmDeleteMessage, 1);

    loaded = false;
    shouldClose = 0;

    //thread.Start(this);
}

int GlxContainer::ShouldClose()
{
    return shouldClose;
}

void GlxContainer::PostUpdate()
{
    glXSwapBuffers(display, window);
}

void GlxContainer::ReadInput()
{
    if (checkMask || XCheckMaskEvent(display, swa.event_mask, &event)) {
        switch (event.type) {
        case Expose: // part of exposure mask
            if (!loaded) {
                OnLoad();
                loaded = true;
            }
            break;

        case KeyPress: // part of keypressmask
            KeyPressed(XLookupKeysym(&event.xkey, 0));
            break;

        case KeyRelease:
            break;

        case ButtonPress:
            switch (event.xbutton.button) {
            case Button1:
                MouseLeftButtonPressed(event.xbutton.x, event.xbutton.y);
                break;
            case Button2:
                MouseMiddleButtonPressed(event.xbutton.x, event.xbutton.y);
                break;
            case Button3:
                MouseRightButtonPressed(event.xbutton.x, event.xbutton.y);
                break;
            case Button4:
                Scrolled(0, 1);
                break;
            case Button5:
                Scrolled(0, -1);
                break;
            }
            break;

        case ButtonRelease:
            switch (event.xbutton.button) {
            case Button1:
                MouseLeftButtonReleased(event.xbutton.x, event.xbutton.y);
                break;
            case Button2:
                MouseMiddleButtonReleased(event.xbutton.x, event.xbutton.y);
                break;
            case Button3:
                MouseRightButtonReleased(event.xbutton.x, event.xbutton.y);
                break;
            }
            break;

        case MotionNotify:
            MouseMoved(event.xmotion.x, event.xmotion.y);
            break;

        case ConfigureNotify: // part of structurenotifymask
            XConfigureEvent xce = event.xconfigure;
            if (xce.width != width || xce.height != height) {
                width = xce.width;
                height = xce.height;
                OnResize();
            }
            break;
        }

        do {
            checkMask = XCheckMaskEvent(display, swa.event_mask, &event);
        } while (checkMask && event.type == MotionNotify);
    }

    XCheckTypedWindowEvent(display, window, ClientMessage, &event);
    switch (event.type) {
    case ClientMessage:
        if (event.xclient.data.l[0] == wmDeleteMessage) {
            shouldClose = 1;
        }
        break;
    }
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

void GlxContainer::OnResize()
{
}
