#ifndef AIR_H_INCLUDED
#define AIR_H_INCLUDED

#include <bits/stdc++.h>
#include <gtk/gtk.h>

#define GTK_TABLE_START(A,B) window = gtk_window_new(GTK_WINDOW_TOPLEVEL);\
                             gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);\
                             gtk_container_set_border_width(GTK_CONTAINER(window),10);\
                             gtk_window_set_default_size(GTK_WINDOW(window),A,B);

#define GTK_TABLE_END        gtk_container_add(GTK_CONTAINER(window),table);\
                             gtk_signal_connect(GTK_OBJECT(window),"destroy", G_CALLBACK(destroy),NULL);\
                             gtk_widget_show(window); \
                             gtk_widget_show_all(window);\
                             gtk_main();

#define signal(a,b,c,d)  g_signal_connect(GTK_OBJECT(a),b,G_CALLBACK(c),gpointer(d))
char *rights="©All rights reserved 2014 @rootial";
GtkWidget *hseparator[2],*vseparator[2],* label_rights, *entry;

extern void destroy(GtkWidget*widget,gpointer *data)
{
    gtk_main_quit();
}
extern void separator(void)
{
    int i;
    for(i=0; i<2; i++)
        hseparator[i]=gtk_hseparator_new(),vseparator[i]=gtk_vseparator_new();
    label_rights = gtk_label_new(rights);
}
extern char * _(char *c)
{
    return g_locale_to_utf8(c,-1,0,0,0);
}
#endif // AIR_H_INCLUDED

