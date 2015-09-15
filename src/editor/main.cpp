#include "gtkmm.h"
#include <iostream>

class Window1 :
    public Gtk::Window
{
public:
    Window1(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
        : Gtk::Window(cobject) {}
    virtual ~Window1() {}
};

int main(int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create_from_file("src/editor/main_form.glade");
    //Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    //try {
    //    refBuilder->add_from_file("src/editor/main_form.glade");
    //} catch(const Glib::FileError& ex) {
    //    std::cerr << "FileError: " << ex.what() << std::endl;
    //    return 1;
    //} catch(const Glib::MarkupError& ex) {
    //    std::cerr << "MarkupError: " << ex.what() << std::endl;
    //    return 1;
    //} catch(const Gtk::BuilderError& ex) {
    //    std::cerr << "BuilderError: " << ex.what() << std::endl;
    //    return 1;
    //}

    //Get the GtkBuilder-instantiated Dialog:
    Window1* pDialog = 0;
    refBuilder->get_widget_derived("window1", pDialog);
    if(pDialog) {
        app->run(*pDialog);
    }

    delete pDialog;

    return 0;
}
