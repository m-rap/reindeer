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
    bool running;

    virtual void SubInit();
    inline void LoadNoGlade();
    inline void LoadGlade();
    inline void ConnectSignals();

public:
    RdrGtkContainer();
    virtual ~RdrGtkContainer();

    virtual int KEY_LEFT() { return -1; }
    virtual int KEY_RIGHT() { return -1; }
    virtual int KEY_UP() { return -1; }
    virtual int KEY_DOWN() { return -1; }
    virtual int KEY_M() { return -1; }
    virtual int ShouldClose() { return -1; }

    virtual void Deinit();

    static G_MODULE_EXPORT void WindowDestroyCb(GtkWidget* object, gpointer userData);
    static G_MODULE_EXPORT void PanelLoadCb(GtkWidget* object, GdkEventExpose *event, gpointer userData);
    static G_MODULE_EXPORT void DrawCb(GtkWidget* object, cairo_t* cr, gpointer userData);
    static G_MODULE_EXPORT void PanelClosedCb(GtkWidget* object, GdkEvent* event, gpointer userData);
    static G_MODULE_EXPORT gboolean TimerTickCb(gpointer userData);
    static G_MODULE_EXPORT gboolean MotionNotifyEventCb(GtkWidget* object, GdkEventMotion* event, gpointer userData);
    static G_MODULE_EXPORT gboolean ButtonPressEventCb(GtkWidget* object, GdkEventButton* event, gpointer userData);
    static G_MODULE_EXPORT gboolean ButtonReleaseEventCb(GtkWidget* object, GdkEventButton* event, gpointer userData);
    static G_MODULE_EXPORT gboolean ScrollEventCb(GtkWidget* object, GdkEventScroll* event, gpointer userData);
    static G_MODULE_EXPORT gboolean ConfigureEventCb(GtkWidget* object, GdkEventConfigure* event, gpointer userData);

    virtual void Main();
};

#endif // GTKCONTAINER_H
