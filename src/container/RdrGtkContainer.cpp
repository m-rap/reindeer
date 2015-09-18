#include "RdrGtkContainer.h"
#include <gtk/gtk.h>

extern "C"
{
    void on_window1_destroy(GtkWidget *object, gpointer user_data)
    {
        gtk_main_quit();
    }
}

RdrGtkContainer::RdrGtkContainer()
{
    //ctor
}

RdrGtkContainer::~RdrGtkContainer()
{
    //dtor
}

void RdrGtkContainer::Init(int argc, char *argv[])
{
    GtkBuilder *builder;
    GtkWidget  *window;
    GtkWidget  *box;
    GtkWidget  *glArea;

    gtk_init(&argc, &argv);
    gtk_gl_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "src/editor/main_form.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));

    box = GTK_WIDGET(gtk_builder_get_object(builder, "box3"));

    /*
    * Drawing area for drawing OpenGL scene.
    */
    glArea = gtk_drawing_area_new();

    /* Set OpenGL-capability to the widget. */
    gtk_widget_set_gl_capability(
        drawing_area,
        glconfig,
        NULL,
        TRUE,
        GDK_GL_RGBA_TYPE
    );

    gtk_box_pack_start(GTK_BOX(box), glArea, TRUE, TRUE, 0);

    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(G_OBJECT(builder));

    gtk_widget_show(window);
    gtk_main();
}
