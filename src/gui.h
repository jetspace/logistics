#ifndef LOGISTICS_GUI_H
#define LOGISTICS_GUI_H

#include <gtk/gtk.h>
#define UIFILE "src/ui/content.ui"


GtkWidget *basewin;
GtkWidget *box;
GtkWidget *content_root;
GtkBuilder *builder

//SearchContent
GtkWidget *search_entry;
GtkWidget *list_view;
GtkListStore *package_list;
GtkTreeIter iter;

//PACKAGE LIST STORE
enum {
  PKG_NAME = 0,
  PKG_VERSION,
  PKG_DSIZE,
  PKG_ISIZE,
  PKG_PACKAGER,
  PKG_DESC,
  PKG_URL,
  PKG_STORE_LENGTH
};

gboolean destroy(GtkWidget *w, GdkEvent *e, gpointer p)
{
  gtk_main_quit();
}

void build_base_window(void)
{
  basewin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_resize(GTK_WINDOW(basewin),800, 600);
  gtk_window_set_title(GTK_WINDOW(basewin), "Logistics");

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(basewin), box);

  g_signal_connect(G_OBJECT(basewin), "destroy", G_CALLBACK(destroy), NULL);
}

void load_content(char *uifile, char *content)
{
  builder = gtk_builder_new_from_file(uifile);
  content_root = GTK_WIDGET(gtk_builder_get_object(builder, content));
  gtk_box_pack_end(GTK_BOX(box), content_root, TRUE, TRUE, 0);
  gtk_widget_show_all(content_root);

}

void init_package_list(void)
{
  package_list = gtk_list_store_new(PKG_STORE_LENGTH, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_LONG, G_TYPE_LONG, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

  init_alpm_sync();

  size_t i;
  for(i = 0; i < sizeof(dbs) / sizeof(dbs[0]); i++)
  {
      alpm_db_t *db = alpm_register_syncdb(handle, dbs[i], ALPM_SIG_USE_DEFAULT);
      alpm_list_t *i, *cache = alpm_db_get_pkgcache(db);
      for(i = cache; i; i = alpm_list_next(i))
      {
          gtk_list_store_append(GTK_LIST_STORE(package_list), &iter);
          gtk_list_store_set(GTK_LIST_STORE(package_list), &iter, PKG_NAME, alpm_pkg_get_name(i->data), PKG_VERSION, alpm_pkg_get_version(i->data), PKG_DSIZE, alpm_pkg_get_size(i->data) /1000, PKG_ISIZE, alpm_pkg_get_isize(i->data) /1000, PKG_PACKAGER, alpm_pkg_get_packager(i->data), PKG_DESC, alpm_pkg_get_desc(i->data), PKG_URL, alpm_pkg_get_url(i->data), -1);
      }
  }

}

void load_search(void)
{
  load_content(UIFILE, "content_search");
  search_entry = GTK_WIDGET(gtk_builder_get_object(builder, "searchbox"));
  list_view = GTK_WIDGET(gtk_builder_get_object(builder, "results"));

  init_package_list();

  //setup renderer
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Name", renderer, "text", PKG_NAME, NULL)
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Version", renderer, "text", PKG_VERSION, NULL)
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Size (kb)", renderer, "text", PKG_ISIZE, NULL)
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("URL", renderer, "text", PKG_URL, NULL)
  gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

  //setup model
  gtk_tree_view_set_model(GTK_TREE_VIEW(list_view), GTK_TREE_MODEL(package_list));
  gtk_tree_view_expand_all(GTK_TREE_VIEW(list_view));

  //render everything
  gtk_widget_show_all(basewin);

}

void init_gui(int argc, char **argv)
{
  gtk_init(&argc, &argv);

  build_base_window();
  load_search();

  gtk_widget_show_all(basewin);
  gtk_main();
}

#endif
