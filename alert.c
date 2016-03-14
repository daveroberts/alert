#include <gtk/gtk.h>
#include <stdio.h>

static GtkWidget *window;
static GtkWidget *vbox;
static GtkWidget *button;
static GtkWidget *label;
static GtkWidget *debug_button;

// method to call and check if "ack" was typed in
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

// callback function, data argument ignored
static void debug_button_pressed( GtkWidget *widget, GtkWidget* textbox){
  g_print("Debug button pressed\n");
  GdkScreen* screen = gtk_window_get_screen(GTK_WINDOW(window));
  int width = gdk_screen_get_width(screen);
  g_print("The screen is %d pixels wide\n", width);
  int monitors = gdk_screen_get_n_monitors(screen);
  g_print("The screen has %d monitors\n", monitors);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);

  gint x;
  gint y;
  gtk_window_get_position(GTK_WINDOW(window), &x, &y);
  g_print("The window is located at %d, %d\n", x, y);
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

static void configure_callback(GtkWindow *window, GdkEvent *event, gpointer data){
  int x, y;
  GString *buf;

  x = event->configure.x;
  y = event->configure.y;
  buf = g_string_new(NULL);
  g_string_printf(buf, "%d, %d", x, y);
  gtk_window_set_title(window, buf->str);
  g_string_free(buf, TRUE);
}

int main(int argc, char *argv[]){
  GtkWidget *textbox;

  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  vbox = gtk_vbox_new(FALSE, 0);
  label = gtk_label_new(argv[1]);
  textbox = gtk_entry_new();
  button = gtk_button_new_with_label("Type \"ack\" and press Enter to close");
  debug_button = gtk_button_new_with_label("Debug");

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
  gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);
  gtk_widget_show(button);

  // Button
  g_signal_connect(debug_button, "clicked", G_CALLBACK(debug_button_pressed), textbox);
  gtk_box_pack_start(GTK_BOX(vbox), debug_button, TRUE, TRUE, 0);
  gtk_widget_show(debug_button);

  // Main
  gtk_widget_show(window);
  gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_widget_grab_focus(GTK_WIDGET(textbox));

  // Called on mouse move
  //g_signal_connect(G_OBJECT(window), "configure-event", G_CALLBACK(configure_callback), NULL);

  /*
  FILE *fp;
  fp = fopen("/tmp/cdata.txt", "w+");
  fprintf(fp, "The screen is %d pixels wide", width);
  fclose(fp);
  */

  gtk_main();
  return 0;
}
