#ifndef LOGISTICS_GUI_APPVIEW_H
#define LOGISTICS_GUI_APPVIEW_H

#include <string.h>
#include "gui.h"

void load_appview(char *app)
{
  alpm_list_t *i, *cache;
  gboolean match = FALSE;
  size_t p;
  alpm_get_syncdbs(handle);
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


  load_content(UIFILE, "content_app");
  //DataStuff
  GtkWidget *pkgname = GTK_WIDGET(gtk_builder_get_object(builder, "pkgname"));
  GtkWidget *pkgpackager = GTK_WIDGET(gtk_builder_get_object(builder, "pkgpackager"));
  GtkWidget *pkgdesc = GTK_WIDGET(gtk_builder_get_object(builder, "pkgdesc"));
  GtkWidget *pkgver = GTK_WIDGET(gtk_builder_get_object(builder, "pkgver"));
  GtkWidget *pkgsize = GTK_WIDGET(gtk_builder_get_object(builder, "pkgsize"));

  gtk_label_set_text(GTK_LABEL(pkgname), alpm_pkg_get_name(i->data));
  gtk_label_set_text(GTK_LABEL(pkgpackager), alpm_pkg_get_packager(i->data));
  gtk_label_set_text(GTK_LABEL(pkgdesc), alpm_pkg_get_desc(i->data));
  gtk_label_set_text(GTK_LABEL(pkgver), alpm_pkg_get_version(i->data));
  char size[20];
  snprintf(size, 20, "&ld kb", alpm_pkg_get_isize(i->data) / 1000);
  gtk_label_set_text(GTK_LABEL(pkgsize), size);

  gtk_widget_show_all(basewin);
}


#endif
