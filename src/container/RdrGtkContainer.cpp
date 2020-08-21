#include "RdrGtkContainer.h"

RdrGtkContainer::RdrGtkContainer()
{
    running = false;
}

RdrGtkContainer::~RdrGtkContainer()
{
}

G_MODULE_EXPORT gboolean RdrGtkContainer::TimerTickCb(gpointer userData)
{
    RdrGtkContainer* container = (RdrGtkContainer*)userData;

    if (!container->running)
        return true;

    gtk_widget_queue_draw(container->appWidgets->drawArea);
    return true;
}

G_MODULE_EXPORT void RdrGtkContainer::WindowDestroyCb(GtkWidget* object, gpointer userData)
{
    RdrGtkContainer* container = (RdrGtkContainer*)userData;
    container->running = false;
    g_source_remove(container->timerId);

    gtk_main_quit();
}

G_MODULE_EXPORT void RdrGtkContainer::PanelLoadCb(GtkWidget* object, GdkEventExpose* event, gpointer userData)
{
    ((RdrGtkContainer*)userData)->OnLoad();
}

G_MODULE_EXPORT void RdrGtkContainer::DrawCb(GtkWidget* object, cairo_t* cr, gpointer userData)
{
    ((RdrGtkContainer*)userData)->OnPaint();
}

G_MODULE_EXPORT void RdrGtkContainer::PanelClosedCb(GtkWidget* object, GdkEvent* event, gpointer userData)
{
    ((RdrGtkContainer*)userData)->OnClosing();
}

G_MODULE_EXPORT gboolean RdrGtkContainer::MotionNotifyEventCb(GtkWidget* object, GdkEventMotion* event, gpointer userData)
{
    RdrGtkContainer* container = (RdrGtkContainer*)userData;
    container->MouseMoved(event->x, event->y);
    return true;
}

G_MODULE_EXPORT gboolean RdrGtkContainer::ButtonPressEventCb(GtkWidget* object, GdkEventButton* event, gpointer userData)
{
    RdrGtkContainer* container = (RdrGtkContainer*)userData;
    if (event->button == 1)
        container->MouseLeftButtonPressed(event->x, event->y);
    else if (event->button == 2)
        container->MouseMiddleButtonPressed(event->x, event->y);
    else if (event->button == 3)
        container->MouseRightButtonPressed(event->x, event->y);
    return true;
}

G_MODULE_EXPORT gboolean RdrGtkContainer::ButtonReleaseEventCb(GtkWidget* object, GdkEventButton* event, gpointer userData)
{
    RdrGtkContainer* container = (RdrGtkContainer*)userData;
    if (event->button == 1)
        container->MouseLeftButtonReleased(event->x, event->y);
    else if (event->button == 2)
        container->MouseMiddleButtonReleased(event->x, event->y);
    else if (event->button == 3)
        container->MouseRightButtonReleased(event->x, event->y);
    return true;
}

G_MODULE_EXPORT gboolean RdrGtkContainer::ScrollEventCb(GtkWidget* object, GdkEventScroll* event, gpointer userData)
{
    RdrGtkContainer* container = (RdrGtkContainer*)userData;
    if (event->direction == GDK_SCROLL_UP)
        container->Scrolled(0, 1);
    else if (event->direction == GDK_SCROLL_DOWN)
        container->Scrolled(0, -1);
    return true;
}

G_MODULE_EXPORT gboolean RdrGtkContainer::ConfigureEventCb(GtkWidget* object, GdkEventConfigure* event, gpointer userData)
{
    RdrGtkContainer* container = (RdrGtkContainer*)userData;
    if (event->width != SCREEN_WIDTH || event->height != SCREEN_HEIGHT)
        container->OnResize(event->width, event->height);
    return true;
}

void RdrGtkContainer::Deinit()
{
    g_slice_free(AppWidgets, appWidgets);
}

void RdrGtkContainer::LoadGlade()
{
    GtkBuilder* builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "src/editor/main_form.glade", NULL);

    appWidgets->gtkWindow = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));

    appWidgets->box = GTK_WIDGET(gtk_builder_get_object(builder, "box3"));

    appWidgets->statusBar = GTK_STATUSBAR(gtk_builder_get_object(builder, "statusbar1"));
    g_object_set_data(G_OBJECT(appWidgets->statusBar), "info", (gpointer) "1");
    guint statusCtxId = gtk_statusbar_get_context_id(appWidgets->statusBar, "info");
    gtk_statusbar_push(appWidgets->statusBar, statusCtxId, "Hello world");

    appWidgets->drawArea = gtk_drawing_area_new();
    gtk_widget_set_double_buffered(appWidgets->drawArea, false);
    gtk_box_pack_start(GTK_BOX(appWidgets->box), appWidgets->drawArea, TRUE, TRUE, 0); // already includes gtk_container_add

    g_object_unref(G_OBJECT(builder));
}

void RdrGtkContainer::LoadNoGlade()
{
    appWidgets->gtkWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(appWidgets->gtkWindow), "OpenGL test with GTK");

    appWidgets->drawArea = gtk_drawing_area_new();
    gtk_widget_set_double_buffered(appWidgets->drawArea, FALSE);
    gtk_widget_set_size_request(appWidgets->drawArea, SCREEN_WIDTH, SCREEN_WIDTH);
    gtk_container_add(GTK_CONTAINER(appWidgets->gtkWindow), appWidgets->drawArea);
}

void RdrGtkContainer::ConnectSignals()
{
    timerId = g_timeout_add(1000.0f/60.0f,(GSourceFunc)TimerTickCb, this);
    g_signal_connect(appWidgets->drawArea, "delete-event", G_CALLBACK(PanelClosedCb), this);
    g_signal_connect(appWidgets->drawArea, "expose-event", G_CALLBACK(PanelLoadCb), this);
    g_signal_connect(appWidgets->drawArea, "draw", G_CALLBACK(DrawCb), this);
    g_signal_connect(appWidgets->drawArea, "motion-notify-event", G_CALLBACK(MotionNotifyEventCb), this);
    g_signal_connect(appWidgets->drawArea, "button-press-event", G_CALLBACK(ButtonPressEventCb), this);
    g_signal_connect(appWidgets->drawArea, "button-release-event", G_CALLBACK(ButtonReleaseEventCb), this);
    g_signal_connect(appWidgets->drawArea, "scroll-event", G_CALLBACK(ScrollEventCb), this);
    g_signal_connect(appWidgets->drawArea, "configure-event", G_CALLBACK(ConfigureEventCb), this);
    g_signal_connect(appWidgets->gtkWindow, "destroy", G_CALLBACK(WindowDestroyCb), this);
}

void RdrGtkContainer::SubInit()
{
    appWidgets = g_slice_new(AppWidgets);
    gtk_init(&argc, &argv);

    LoadGlade();

    gtk_widget_set_events(
        appWidgets->drawArea,
        gtk_widget_get_events(appWidgets->drawArea) |
            GDK_BUTTON_PRESS_MASK |
            GDK_BUTTON_RELEASE_MASK |
            GDK_SCROLL_MASK |
            GDK_POINTER_MOTION_MASK
    );

    ConnectSignals();

    GdkDisplay* gtkDisplay = gtk_widget_get_display(appWidgets->gtkWindow);
    display = gdk_x11_display_get_xdisplay(gtkDisplay);
    root = gdk_x11_get_default_root_xwindow();

    GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    vi = glXChooseVisual(display, 0, att);
    if(vi == NULL) {
        printf("No matching visuals\n");
        exit(0);
    }

    swa.colormap = XCreateColormap(display, root, vi->visual, AllocNone);
    if(swa.colormap == 0) {
        printf("Cannot create color map\n");
        exit(0);
    }

    gtk_widget_show_all(appWidgets->gtkWindow);

    ctx = glXCreateContext(display, vi, NULL, GL_TRUE);
    GdkWindow *gdkWindow = gtk_widget_get_window(GTK_WIDGET(appWidgets->drawArea));
    window = gdk_x11_window_get_xid(gdkWindow);
    XMapWindow(display, window);

    if (!glXMakeCurrent(display, window, ctx)) {
        printf("Error make current\n");
        exit(0);
    }
}

void RdrGtkContainer::Main()
{
    if (running)
        return;

    running = true;

    gtk_main();
}
