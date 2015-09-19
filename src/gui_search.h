#ifndef LOGISTICS_GUI_SEARCH_H
#define LOGISTICS_GUI_SEARCH_H

void load_search(void);
#include "gui_appview.h"
gboolean querry_change(GtkWidget *b, GdkEvent *e, GtkTreeModelFilter *filter)
{
  gtk_tree_model_filter_refilter(filter);
  return FALSE;
}

gboolean package_filter(GtkTreeModel *pkgs, GtkTreeIter *iter, gpointer *data)
{
  char *name;
  gtk_tree_model_get(pkgs, iter, PKG_NAME, &name, -1);
  char *querry = g_strdup(gtk_entry_get_text(GTK_ENTRY(search_entry)));

  if(strstr(name, querry) != NULL)
  {
    free(name);
    free(querry);
    return TRUE;
  }
  else
  {
    free(name);
    free(querry);
    return FALSE;
  }
}

void app_clicked(GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *col, gpointer data)
{
  GtkTreeIter i;

  gtk_tree_model_get_iter (GTK_TREE_MODEL(filter), &i, path);

  char *name;
  gtk_tree_model_get(GTK_TREE_MODEL(filter), &i, PKG_NAME, &name, -1);

  // now, remove search and add appview content
  gtk_widget_destroy(content_root);
  load_appview(name);
  free(name);
}


void load_search(void)
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
  filter = GTK_TREE_MODEL_FILTER(gtk_tree_model_filter_new(GTK_TREE_MODEL(package_list), NULL));
  gtk_tree_model_filter_set_visible_func(filter, (GtkTreeModelFilterVisibleFunc) package_filter, package_list, NULL);

  //setup model
  gtk_tree_view_set_model(GTK_TREE_VIEW(list_view), GTK_TREE_MODEL(filter));
  gtk_tree_view_expand_all(GTK_TREE_VIEW(list_view));

  //signals
  g_signal_connect(G_OBJECT(search_entry), "key-press-event", G_CALLBACK(querry_change), filter);
  g_signal_connect(G_OBJECT(list_view), "row-activated", G_CALLBACK(app_clicked), NULL);

  //render everything
  gtk_header_bar_set_subtitle (GTK_HEADER_BAR(header), "Search");
  gtk_widget_show_all(basewin);

}
#endif
