#ifndef LOGISTICS_GUI_MAIN_H
#define LOGISTICS_GUI_MAIN_H

#define START_PAGE "http://jetspace.github.io/logistics/index.html"

#include <webkit2/webkit2.h>
#include "gui_search.h"

void check_link (WebKitWebView *web_view, WebKitWebResource *resource, WebKitURIRequest *request, gpointer user_data)
{
  char *link = g_strdup(webkit_uri_request_get_uri(request));

  if(strcmp(link, "logistics://search") == 0)
  {
    gtk_widget_destroy(content_root);
    load_search();
  }

}

void load_main(void)
{// main is not in the UI file, because I can't get WebKitWebView to work with GLADE...
  content_root = gtk_srolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(content_root), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(content_root), 400);
  gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(content_root), 400);

  GtkWidget *webview = webkit_web_view_new()
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), START_PAGE);
  g_signal_connect(G_OBJECT(webview), "resource-load-started", G_CALLBACK(check_link), NULL);

  gtk_container_add(GTK_CONTAINER(content_root), webview);
}

#endif
