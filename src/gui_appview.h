#ifndef LOGISTICS_GUI_APPVIEW_H
#define LOGISTICS_GUI_APPVIEW_H

#include <string.h>
#include "gui.h"
#include "gui_search.h"

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

 /* if(match == FALSE)
  {
    g_warning("Can't find requested package: %s\n", app);
    load_search();
    return;
  }*/
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

  gtk_widget_show_all(basewin);
}


#endif
