#include <gtk/gtk.h>

static GtkWidget *window;
static GtkWidget *vbox;
static GtkWidget *button;
static GtkWidget *label;

static void check_end(GtkWidget* textbox){
  const gchar* data;
  data = gtk_entry_get_text(GTK_ENTRY(textbox));
  if (g_strcmp0(data, "ack")) {
    // Does not match
  } else {
    // matches
    gtk_widget_destroy(window);
  }
}

// callback function, data argument ignored
static void button_pressed( GtkWidget *widget, GtkWidget* textbox){
  check_end(textbox);
}

static gboolean key_press_event(GtkWidget *widget, GdkEventKey *event, GtkWidget* textbox){
  if (event->keyval == 65293){ // Enter pressed
    check_end(textbox);
  }
  return FALSE;
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data){
  //g_print("delete event occurred\n");
  return TRUE; // Keeps window open
  //return FALSE; // Closes the window
}

static void destroy(GtkWidget *widget, gpointer data){
  gtk_main_quit();
}

int main(int argc, char *argv[]){
  GtkWidget *textbox;

  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  vbox = gtk_vbox_new(FALSE, 0);
  label = gtk_label_new(argv[1]);
  textbox = gtk_entry_new();
  button = gtk_button_new_with_label("Type \"ack\" and press Enter to close");

  // Window
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  // delete-event is usually called by the window manager, when the user clicks close
  g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);
  // destroy is triggered when gtk_widget_destroy() is called
  // Either by calling that directly, or when FALSE is returned from the delete-event callback
  g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);

  // Vertical layout box
  gtk_container_add(GTK_CONTAINER(window), vbox);
  gtk_widget_show(vbox);

  // Label
  //label = gtk_label_new("This is a normal label");
  gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);
  gtk_widget_show(label);

  // Textbox
  gtk_entry_set_text(GTK_ENTRY(textbox), "");
  g_signal_connect(window, "key-press-event", G_CALLBACK(key_press_event), textbox);
  gtk_box_pack_start(GTK_BOX(vbox), textbox, TRUE, TRUE, 0);
  gtk_widget_show(textbox);

  // Button
  g_signal_connect(button, "clicked", G_CALLBACK(button_pressed), textbox);
  //g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
  gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);
  gtk_widget_show(button);

  // Main
  gtk_widget_show(window);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_widget_grab_focus(GTK_WIDGET(textbox));
  gtk_main();
  return 0;
}
