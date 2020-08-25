#include <gtk/gtk.h>
#include <stdlib.h>
//#include <gtk/gtkgl.h>

typedef struct {
    GtkWidget *window1;
} AppWidgets;

extern "C"
{
    G_MODULE_EXPORT void on_window1_destroy(GtkWidget *object, gpointer user_data)
    {
        gtk_main_quit();
    }
}

int main(int argc, char *argv[])
{
    GtkBuilder      *builder;
    GError *err = NULL;

    AppWidgets *app = g_slice_new(AppWidgets);

    gtk_init(&argc, &argv);
    //gtk_gl_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "src/editor/main_form.glade", &err);
    if (err) {
        g_error(err->message);
        g_error_free(err);
        g_slice_free(AppWidgets, app);
        return 1;
    }

    app->window1 = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));

    gtk_builder_connect_signals(builder, app);
    g_object_unref(G_OBJECT(builder));

    gtk_main();
    g_slice_free(AppWidgets, app);

    return 0;
}
