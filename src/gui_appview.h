#ifndef LOGISTICS_GUI_APPVIEW_H
#define LOGISTICS_GUI_APPVIEW_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "gui.h"
#include "gui_search.h"
#include "gui_tools.h"

void install_app(GtkWidget *w, GdkEvent *e, gpointer p)
{
	char *name = (char *) p;

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

	char *message = g_strdup_printf("Please enter the root password to install %s", name);
	FILE *pacman = popen(g_strdup_printf("sh -c \"gksudo 'pacman -Sy %s --noconfirm' --message '%s' && echo -n DONE || echo -n DONE\"", name, message), "r");
	GIOChannel *channel = g_io_channel_unix_new(fileno(pacman));
	g_io_add_watch(channel, G_IO_IN, new_output, pacman);

	fallto = FALLBACK_TO_SEARCH;

	gtk_widget_show_all(logwin);


}

void remove_app(GtkWidget *w, GdkEvent *e, gpointer p)
{
	char *name = (char *) p;

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

	char *message = g_strdup_printf("Please enter the root password to remove %s", name);
	FILE *pacman = popen(g_strdup_printf("sh -c \"gksudo 'pacman -R %s --noconfirm' --message '%s' && echo -n DONE || echo -n DONE\"", name, message), "r");
	GIOChannel *channel = g_io_channel_unix_new(fileno(pacman));
	g_io_add_watch(channel, G_IO_IN, new_output, pacman);

	fallto = FALLBACK_TO_INSTALLED;

	gtk_widget_show_all(logwin);


}

void load_appview(char *app)
{
  alpm_list_t *i, *cache;
  gboolean match = FALSE;
  size_t p;
  alpm_get_syncdbs(handle);
  init_alpm_sync();
  for(p = 0; p < sizeof(dbs) / sizeof(dbs[0]); p++)
  {
      alpm_db_t *db = alpm_register_syncdb(handle, dbs[p], ALPM_SIG_USE_DEFAULT);
      cache = alpm_db_get_pkgcache(db);
      for(i = cache; i; i = alpm_list_next(i))
      {
          if(strcmp(app, alpm_pkg_get_name(i->data)) == 0)
          {
            match = TRUE;
            break;
          }
          if(match == TRUE)
            break;
      }
      if(match == TRUE)
        break;
  }

  gboolean is_installed = FALSE;


	char i_b[30];
	GtkTreeModel *model = GTK_TREE_MODEL(local_package_list);
	GtkTreeIter iter;
	int x = 0;

	snprintf(i_b, 30, "%d", x);
	while(gtk_tree_model_get_iter_from_string(model, &iter, i_b))
	{
		char *buff;
		gtk_tree_model_get(model, &iter, PKG_NAME, &buff, -1);
		if(strcmp(app, buff) == 0)
			is_installed = TRUE;
		g_free(buff);
		x++;
		snprintf(i_b, 30, "%d", x);
	}





  load_content(UIFILE, "content_app");
  //DataStuff
  GtkWidget *pkgname = GTK_WIDGET(gtk_builder_get_object(builder, "pkgname"));
  GtkWidget *pkgpackager = GTK_WIDGET(gtk_builder_get_object(builder, "pkgpackager"));
  GtkWidget *pkgdesc = GTK_WIDGET(gtk_builder_get_object(builder, "pkgdesc"));
  GtkWidget *pkgver = GTK_WIDGET(gtk_builder_get_object(builder, "pkgver"));
  GtkWidget *pkgsize = GTK_WIDGET(gtk_builder_get_object(builder, "pkgsize"));
  GtkWidget *installbutton = GTK_WIDGET(gtk_builder_get_object(builder, "install_button"));
  GtkWidget *removebutton = GTK_WIDGET(gtk_builder_get_object(builder, "remove_button"));

  gtk_label_set_text(GTK_LABEL(pkgname), alpm_pkg_get_name(i->data));
  gtk_label_set_text(GTK_LABEL(pkgpackager), alpm_pkg_get_packager(i->data));
  gtk_label_set_text(GTK_LABEL(pkgdesc), alpm_pkg_get_desc(i->data));
  gtk_label_set_text(GTK_LABEL(pkgver), alpm_pkg_get_version(i->data));
  char size[20];
  snprintf(size, 20, "%ld kb", alpm_pkg_get_isize(i->data) / 1000);
  gtk_label_set_text(GTK_LABEL(pkgsize), size);

  gtk_header_bar_set_subtitle (GTK_HEADER_BAR(header), "App details:");

  if(is_installed)
    gtk_widget_set_sensitive(installbutton, FALSE);
  else
    gtk_widget_set_sensitive(removebutton, FALSE);

	char *name = g_strdup(alpm_pkg_get_name(i->data));
	g_signal_connect(G_OBJECT(installbutton), "clicked", G_CALLBACK(install_app), name);
	g_signal_connect(G_OBJECT(removebutton), "clicked", G_CALLBACK(remove_app), name);

  gtk_widget_show_all(basewin);
}


#endif
