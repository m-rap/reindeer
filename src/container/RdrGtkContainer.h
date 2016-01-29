#ifndef RDRGTKCONTAINER_H
#define RDRGTKCONTAINER_H

#include "GlxContainer.h"
#include "../common/RdrLinuxThread.h"
#include <gtk/gtk.h>
#include <gdk/gdkx.h>

class RdrGtkContainer;

typedef struct {
    RdrGtkContainer* container;
    GtkWidget* gtkWindow;
    GtkWidget* box;
    GtkWidget* drawArea;
    GtkStatusbar* statusBar;
} AppWidgets;

class RdrGtkContainer :
    public GlxContainer
{
protected:
    AppWidgets* appWidgets;
    GtkApplication* app;
    guint timerId;

    virtual void SubInit();
    void LoadNoGlade();
    void LoadGlade();
    virtual void DeinitWindowAndDisplay();

public:
    RdrGtkContainer();
    virtual ~RdrGtkContainer();

    virtual int KEY_LEFT() { return -1; }
    virtual int KEY_RIGHT() { return -1; }
    virtual int KEY_UP() { return -1; }
    virtual int KEY_DOWN() { return -1; }
    virtual int KEY_M() { return -1; }

    virtual int ShouldClose() { return -1; }
    virtual void OnLoad();

    static G_MODULE_EXPORT void OnWindowDestroy(GtkWidget *object, gpointer userData);
    static G_MODULE_EXPORT void OnPanelLoad(GtkWidget *object, cairo_t* cr, gpointer userData);
    static G_MODULE_EXPORT void OnPanelClosed(GtkWidget *object, GdkEvent* event, gpointer userData);
    static G_MODULE_EXPORT gboolean OnTimerTick(gpointer userData);

    virtual void Main();
};

#endif // GTKCONTAINER_H
