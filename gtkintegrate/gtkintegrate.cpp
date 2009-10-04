#include <gtk/gtk.h>

int main_ex( int argc, char **argv );

int main( int   argc,
          char *argv[] )
{
   // return main_ex(argc, argv);
    
    GtkWidget *window;
    gtk_init (&argc, &argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_show  (window);
    
    gtk_main ();
    return(0);
}


#ifdef WIN32

#include <windows.h>

int WINAPI WinMain(      
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    return main(__argc, __argv);
}

#endif