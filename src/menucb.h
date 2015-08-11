#ifndef LOGISTICS_MENU_CB_H
#define LOGISTICS_MENU_CB_H

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

#endif
