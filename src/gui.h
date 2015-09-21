#ifndef LOGISTICS_GUI_H
#define LOGISTICS_GUI_H

#include <gtk/gtk.h>
#include <string.h>
#define UIFILE "src/ui/content.ui"


GtkWidget *basewin;
GtkWidget *box;
GtkWidget *content_root;
GtkBuilder *builder;

//HEADER BAR
GtkWidget *header;

//SearchContent
GtkWidget *search_entry;
GtkWidget *list_view;
GtkListStore *package_list;
GtkTreeModelFilter *filter;
GtkTreeIter iter;

GtkListStore *local_package_list;

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
void load_content(char *uifile, char *content);
void update_packagelists(void);
#include "gui_search.h"
#include "gui_main.h"
#include "gui_installed.h"
#include "gui_updates.h"
#include "menucb.h"

gboolean destroy(GtkWidget *w, GdkEvent *e, gpointer p)
{
  gtk_main_quit();
}

void refresh_clicked(GtkWidget *w, GdkEvent *e, gpointer p)
{
  gtk_widget_set_sensitive(basewin, FALSE);
  logwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(logwin), box);

  GtkWidget *scrollwin = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollwin), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrollwin), 300);
  gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrollwin), 400);

  text_view = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
  gtk_container_add(GTK_CONTAINER(scrollwin), text_view);

  gtk_box_pack_start(GTK_BOX(box), scrollwin, TRUE, TRUE, 0);

  pb = gtk_progress_bar_new();
  gtk_progress_bar_pulse(GTK_PROGRESS_BAR(pb));
  gtk_box_pack_end(GTK_BOX(box), pb, FALSE, FALSE, 0);

  char *message = g_strdup_printf("Please enter the root password to update package data base");
  FILE *pacman = popen(g_strdup_printf("sh -c \"gksudo 'pacman -Sy --noconfirm' --message '%s' && echo -n DONE || echo -n DONE\"", message), "r");
  GIOChannel *channel = g_io_channel_unix_new(fileno(pacman));
  g_io_add_watch(channel, G_IO_IN, new_output, pacman);



  gtk_widget_show_all(logwin);


}

void build_base_window(void)
{
  basewin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_resize(GTK_WINDOW(basewin),800, 600);
  gtk_window_set_title(GTK_WINDOW(basewin), "JetSpace Logistics"); //keep as fallback...

  header = gtk_header_bar_new();
  gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header), TRUE);
  gtk_header_bar_set_title(GTK_HEADER_BAR(header), "JetSpace Logistics");

  gtk_window_set_titlebar (GTK_WINDOW(basewin), header);

  GtkWidget *refresh = gtk_button_new_with_label("Refresh");
  gtk_header_bar_pack_start(GTK_HEADER_BAR(header), refresh);
  g_signal_connect(G_OBJECT(refresh), "clicked", G_CALLBACK(refresh_clicked), NULL);

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(basewin), box);

  //create toolbar
  GtkWidget *toolbar = gtk_toolbar_new();
  gtk_box_pack_start(GTK_BOX(box), toolbar, FALSE, FALSE, 0);

  //fill toolbar
    //HOME
    GtkToolItem *home = gtk_tool_button_new(NULL, "Home");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(home), 0);
    g_signal_connect(G_OBJECT(home), "clicked", G_CALLBACK(clicked_home), NULL);

    //SEARCH
    GtkToolItem *search = gtk_tool_button_new(NULL, "Search");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(search), -1);
    g_signal_connect(G_OBJECT(search), "clicked", G_CALLBACK(clicked_search), NULL);

    //INSTALLED
    GtkToolItem *installed = gtk_tool_button_new(NULL, "Installed");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(installed), -1);
    g_signal_connect(G_OBJECT(installed), "clicked", G_CALLBACK(clicked_installed), NULL);

    //UPDATES
    GtkToolItem *updates = gtk_tool_button_new(NULL, "Updates");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(updates), -1);
    g_signal_connect(G_OBJECT(updates), "clicked", G_CALLBACK(clicked_updates), NULL);


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



void create_local_package_db(void)
{

  local_package_list = gtk_list_store_new(PKG_STORE_LENGTH, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_LONG, G_TYPE_LONG, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

  init_alpm_sync();

      alpm_db_t *db = alpm_get_localdb(handle);
      alpm_list_t *i, *cache = alpm_db_get_pkgcache(db);
      for(i = cache; i; i = alpm_list_next(i))
      {
          gtk_list_store_append(GTK_LIST_STORE(local_package_list), &iter);
          gtk_list_store_set(GTK_LIST_STORE(local_package_list), &iter, PKG_NAME, alpm_pkg_get_name(i->data), PKG_VERSION, alpm_pkg_get_version(i->data), PKG_DSIZE, alpm_pkg_get_size(i->data) /1000, PKG_ISIZE, alpm_pkg_get_isize(i->data) /1000, PKG_PACKAGER, alpm_pkg_get_packager(i->data), PKG_DESC, alpm_pkg_get_desc(i->data), PKG_URL, alpm_pkg_get_url(i->data), -1);
      }
}

void update_packagelists(void)
{
	g_object_unref(local_package_list);
	create_local_package_db();
	g_object_unref(package_list);
	init_package_list();
}


void init_gui(int argc, char **argv)
{
  gtk_init(&argc, &argv);

  build_base_window();

  init_package_list();
  create_local_package_db();
  load_main();

  gtk_widget_show_all(basewin);
  gtk_main();
}

#endif
