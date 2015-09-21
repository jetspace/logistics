#ifndef LOGISTICS_GUI_UPDATES_H
#define LOGISTICS_GUI_UPDATES_H

#include "gui_tools.h"

void load_search(void);
GtkWidget *update_view;
GtkListStore *updates;
GtkTreeIter iter;

enum
{
	UPDATE_NAME=0,
	UPDATE_VERSION_OLD,
	UPDATE_VERSION_NEW
};

void load_updates(void);

void clicked_updates(GtkWidget *w, GdkEvent *e, gpointer p)
{
	gtk_widget_destroy(content_root);
	load_updates();
}

void update_all(GtkWidget *w, GdkEvent *e, gpointer p)
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

	char *message = g_strdup_printf("Please enter the root password to update your system");
	FILE *pacman = popen(g_strdup_printf("sh -c \"gksudo 'pacman -Su --noconfirm' --message '%s' && echo -n DONE || echo -n DONE\"", message), "r");
	GIOChannel *channel = g_io_channel_unix_new(fileno(pacman));
	g_io_add_watch(channel, G_IO_IN, new_output, pacman);


	fallto = FALLBACK_TO_UPDATES;
	gtk_widget_show_all(logwin);


}


void load_updates(void)
{
  load_content(UIFILE, "content_updates");
  update_view = GTK_WIDGET(gtk_builder_get_object(builder, "updates"));
  GtkWidget *updateall = GTK_WIDGET(gtk_builder_get_object(builder, "update_button"));
  g_signal_connect(G_OBJECT(updateall), "clicked", G_CALLBACK(update_all), NULL);
  gtk_widget_set_sensitive(updateall, FALSE);

  //setup renderer
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Name", renderer, "text", UPDATE_NAME, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(update_view), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Version (old)", renderer, "text", UPDATE_VERSION_OLD, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(update_view), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Version (new)", renderer, "text", UPDATE_VERSION_NEW, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(update_view), column);

	updates = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(update_view), GTK_TREE_MODEL(updates));

	FILE *pacman = popen("pacman -Qu", "r");
	char buffer[2048];
	while(fgets(buffer, 2048, pacman))
	{
		char *name = g_strdup(strtok(buffer, " "));
		char *oldv = g_strdup(strtok(NULL, " "));
		strtok(NULL, ">");
		char *newv = g_strdup(strtok(NULL, "\n\0"));

		gtk_widget_set_sensitive(updateall, TRUE);

		gtk_list_store_append(GTK_LIST_STORE(updates), &iter);
          	gtk_list_store_set(GTK_LIST_STORE(updates), &iter, UPDATE_NAME, name, UPDATE_VERSION_OLD, oldv, UPDATE_VERSION_NEW, newv, -1);
	}

  //render everything
  gtk_header_bar_set_subtitle (GTK_HEADER_BAR(header), "Updates");
  gtk_widget_show_all(basewin);

}
#endif
