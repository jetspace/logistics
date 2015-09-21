#ifndef LOGISTICS_GUI_TOOLS_H
#define LOGISTICS_GUI_TOOLS_H

enum
{
  FALLBACK_TO_HOME = 0,
  FALLBACK_TO_SEARCH,
  FALLBACK_TO_INSTALLED,
  FALLBACK_TO_UPDATES,
  FALLBACK_TO_NOTHING
};

int fallto = FALLBACK_TO_NOTHING;

GtkWidget *text_view;
GtkWidget *logwin;
GtkWidget *pb;
static gboolean new_output(GIOChannel *channel, GIOCondition condition, gpointer data);
#include "gui_search.h"
#include "gui_main.h"
#include "gui_installed.h"
#include "gui_updates.h"

static gboolean new_output(GIOChannel *channel, GIOCondition condition, gpointer data)
{
	gtk_progress_bar_pulse(GTK_PROGRESS_BAR(pb));
	FILE *cmd = data;
	char out[2048];
	if(fgets(out, 2048, cmd))
	{
		if(strcmp(out, "DONE") == 0)
		{
			fclose(cmd);
      update_packagelists();
			gtk_widget_set_sensitive(basewin, TRUE);
			gtk_widget_destroy(logwin);
      if(fallto != FALLBACK_TO_NOTHING)
			   gtk_widget_destroy(content_root);
			update_packagelists();
      switch(fallto)
      {
        case FALLBACK_TO_HOME:
           load_main();
        break;
        case FALLBACK_TO_SEARCH:
			     load_search();
        break;
        case FALLBACK_TO_INSTALLED:
           load_installed();
        break;
        case FALLBACK_TO_UPDATES:
           load_updates();
        break;
      }
      fallto = FALLBACK_TO_NOTHING;
			return FALSE; //DESTROY WATCH: PROGRAM EXITED
		}

		GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
		GtkTextIter iter;
		gtk_text_buffer_get_end_iter(buffer, &iter);
		gtk_text_buffer_insert(buffer, &iter, out, -1);
		return TRUE;
	}
	else
	{
		g_warning("PACMAN EXITED UNEXPECTED");
		fclose(cmd);
		gtk_widget_set_sensitive(basewin, TRUE);
		gtk_widget_destroy(logwin);
		update_packagelists();
		return FALSE; //DESTROY WATCH: PROGRAM EXITED
	}
}
#endif
