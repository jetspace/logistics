#ifndef LOGISTICS_GUI_H
#define LOGISTICS_GUI_H

#include <gtk/gtk.h>
#define UIFILE "src/ui/content.ui"


GtkWidget *basewin;
GtkWidget *box;
GtkWidget *content_root;

void build_base_window(void)
{
  basewin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_resize(GTK_WINDOW(basewin),800, 600);
  gtk_window_set_title(GTK_WINDOW(basewin), "Logistics");

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
}

void load_content(char *uifile, char *content)
{
  GtkBuilder *builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, uifile, NULL);
  content_root = GTK_WIDGET(gtk_builder_get_object(builder, content));
  gtk_builder_connect_signals(builder, NULL);
  g_object_unref(G_OBJECT(builder));
  gtk_box_pack_end(GTK_BOX(box), content_root, TRUE, TRUE, 0);
  gtk_widget_show_all(content_root);
}

void init_gui(int argc, char **argv)
{
  gtk_init(&argc, &argv);

  build_base_window();
  load_content(UIFILE, "content_search");

  gtk_widget_show_all(basewin);
  gtk_main();
}

#endif
