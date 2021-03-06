#ifndef LOGISTICS_MENU_CB_H
#define LOGISTICS_MENU_CB_H

#include "gui_installed.h"

void clicked_home(GtkWidget *w, gpointer p)
{
    gtk_widget_destroy(content_root);
    load_main();
}

void clicked_search(GtkWidget *w, gpointer p)
{
    gtk_widget_destroy(content_root);
    load_search();
}

void clicked_installed(GtkWidget *w, gpointer p)
{
    gtk_widget_destroy(content_root);
    load_installed();
}

#endif
