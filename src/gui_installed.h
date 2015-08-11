#ifndef LOGISTICS_GUI_INSTALLED_H
#define LOGISTICS_GUI_INSTALLED_H

#include "gui_appview.h"
#include "gui_search.h"
#include "gui.h"


void load_installed(void)
{
  load_content(UIFILE, "content_search");
  search_entry = GTK_WIDGET(gtk_builder_get_object(builder, "searchbox"));
  list_view = GTK_WIDGET(gtk_builder_get_object(builder, "results"));

  //setup renderer
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Name", renderer, "text", PKG_NAME, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Version", renderer, "text", PKG_VERSION, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Size (kb)", renderer, "text", PKG_ISIZE, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("URL", renderer, "text", PKG_URL, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  //create filter
  filter = GTK_TREE_MODEL_FILTER(gtk_tree_model_filter_new(GTK_TREE_MODEL(local_package_list), NULL));
  gtk_tree_model_filter_set_visible_func(filter, (GtkTreeModelFilterVisibleFunc) package_filter, local_package_list, NULL);

  //setup model
  gtk_tree_view_set_model(GTK_TREE_VIEW(list_view), GTK_TREE_MODEL(filter));
  gtk_tree_view_expand_all(GTK_TREE_VIEW(list_view));

  //signals
  g_signal_connect(G_OBJECT(search_entry), "key-press-event", G_CALLBACK(querry_change), filter);
  g_signal_connect(G_OBJECT(list_view), "row-activated", G_CALLBACK(app_clicked), NULL);

  //render everything
  gtk_widget_show_all(basewin);

}
#endif
