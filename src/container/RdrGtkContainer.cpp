#include "RdrGtkContainer.h"

RdrGtkContainer::RdrGtkContainer()
{
}

RdrGtkContainer::~RdrGtkContainer()
{
}

void RdrGtkContainer::OnLoad()
{

}

G_MODULE_EXPORT void RdrGtkContainer::OnWindowDestroy(GtkWidget *object, gpointer userData)
{
    g_source_remove(((RdrGtkContainer*)userData)->timerId);
    gtk_main_quit();
}

G_MODULE_EXPORT void RdrGtkContainer::OnPanelLoad(GtkWidget *object, cairo_t* cr, gpointer userData)
{
    ((RdrGtkContainer*)userData)->OnLoad();
}

G_MODULE_EXPORT void RdrGtkContainer::OnPanelClosed(GtkWidget *object, GdkEvent* event, gpointer userData)
{
    ((RdrGtkContainer*)userData)->OnClosing();
}

G_MODULE_EXPORT gboolean RdrGtkContainer::OnTimerTick(gpointer userData)
{
    RdrGtkContainer* container = ((RdrGtkContainer*)userData);
    container->OnPaint();
    return true;
}

void RdrGtkContainer::DeinitWindowAndDisplay()
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
    gtk_box_pack_start(GTK_BOX(appWidgets->box), appWidgets->drawArea, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(appWidgets->box), appWidgets->drawArea);

    g_timeout_add(1000.0/60.0,(GSourceFunc)OnTimerTick, this);
    g_signal_connect(appWidgets->drawArea, "delete-event", G_CALLBACK(OnPanelClosed), this);
    g_signal_connect(appWidgets->drawArea, "draw", G_CALLBACK(OnPanelLoad), this);
    g_signal_connect(appWidgets->gtkWindow, "destroy", G_CALLBACK(OnWindowDestroy), this);

    //gtk_builder_connect_signals(builder, appWidgets);

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

    timerId = g_timeout_add(1000.0/60.0,(GSourceFunc)OnTimerTick, this);
    g_signal_connect(appWidgets->drawArea, "delete-event", G_CALLBACK(OnPanelClosed), this);
    g_signal_connect(appWidgets->gtkWindow, "draw", G_CALLBACK(OnPanelLoad), this);
    g_signal_connect(appWidgets->gtkWindow, "destroy", G_CALLBACK(OnWindowDestroy), this);
}

void RdrGtkContainer::SubInit()
{
    appWidgets = g_slice_new(AppWidgets);
    gtk_init(&argc, &argv);

    LoadGlade();
    //LoadNoGlade();

    GdkDisplay* gtkDisplay = gtk_widget_get_display(appWidgets->gtkWindow);
    display = gdk_x11_display_get_xdisplay(gtkDisplay);
    root = gdk_x11_get_default_root_xwindow();

    GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    vi = glXChooseVisual(display, 0, att);
    if(vi == NULL)
        printf("No matching visuals\n");

    swa.colormap = XCreateColormap(display, root, vi->visual, AllocNone);
    if(swa.colormap == 0)
    {
        printf("Cannot create color map\n");
        exit(0);
    }

    gtk_widget_show_all(appWidgets->gtkWindow);

    ctx = glXCreateContext(display, vi, NULL, GL_TRUE);
    GdkWindow *gdkWindow = gtk_widget_get_window(GTK_WIDGET(appWidgets->drawArea));
    window = gdk_x11_window_get_xid(gdkWindow);
    if (!glXMakeCurrent(display, window, ctx))
    {
        printf("Error make current\n");
        exit(0);
    }

    XMapWindow(display, window);
}

void RdrGtkContainer::Main()
{
    gtk_main();
}
