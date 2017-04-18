/** ***********************************************************************************
  * it-edit the Integrated Terminal Editor: a text editor with severals               *
  * integrated functionalities.                                                       *
  *                                                                                   *
  * Copyright (C) 2015,2016 Brüggemann Eddie.                                         *
  *                                                                                   *
  * This file is part of it-edit.                                                     *
  * it-edit is free software: you can redistribute it and/or modify                   *
  * it under the terms of the GNU General Public License as published by              *
  * the Free Software Foundation, either version 3 of the License, or                 *
  * (at your option) any later version.                                               *
  *                                                                                   *
  * it-edit is distributed in the hope that it will be useful,                        *
  * but WITHOUT ANY WARRANTY; without even the implied warranty of                    *
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                      *
  * GNU General Public License for more details.                                      *
  *                                                                                   *
  * You should have received a copy of the GNU General Public License                 *
  * along with it-edit. If not, see <http://www.gnu.org/licenses/>                    *
  *                                                                                   *
  ************************************************************************************/


#include "./dialogs.h"

static void toggle_mode_bit(GtkWidget *widget, File_Editor *file_editor) ;

static GtkWidget* generate_calendar(GStatBuf *datetime, int16_t type) ;

static void generate_calendar_and_timer(GtkWidget *widget, GStatBuf *datetime) ;

static void rename_file(GtkWidget *widget, File_Editor *file_editor) ;

static void copy_file(GtkWidget *widget, File_Editor *file_editor) ;

static char *g_get_wc(char *filepath, char *option) ;

static void destroy_toplevel_terminal(GtkWidget *widget, GtkWidget *itterm) ;

static void display_execute_command_terminal(gchar *execute_command_string) ;

void go_to_line(gint line_count) {
  /** Go to line callback. **/

  #ifdef DEBUG
    DEBUG_FUNC_MARK
  #endif

  /** Getting the user entry. **/
  const gchar *line_number = gtk_entry_get_text(GTK_ENTRY(gui->go_to_line_dialog->go_to_line_entry)) ;

  guint8 c=0 ;

  for ( ; c < strlen(line_number) ; c++) {
    /** Check if the user entry is a valid line number. **/
    if (! g_ascii_isdigit(line_number[c]) ) {
      gtk_widget_destroy(gui->go_to_line_dialog->go_to_line_dialog);
      return ;
    }
  }

  gint line ;
  sscanf(line_number,"%d", &line) ;

  /** Check if th user entry is a valid line number. **/
  if (line > line_count || line_count < 1) {
    gtk_widget_destroy(gui->go_to_line_dialog->go_to_line_dialog);
    return ;
  }

  /** Move to the wanted line number. **/
  GtkTextIter line_iter ;
  gtk_text_buffer_get_iter_at_line(GTK_TEXT_BUFFER(current_editor.current_buffer), &line_iter, line-1) ;
  gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(current_editor.current_textview),     &line_iter, 0.25,  TRUE, 0.5, 0.5) ;
  gtk_text_buffer_place_cursor(GTK_TEXT_BUFFER(current_editor.current_buffer),     &line_iter) ;

  /** Highlight the line. **/
  gtk_source_view_set_highlight_current_line(GTK_SOURCE_VIEW(current_editor.current_textview), TRUE);


  gtk_widget_destroy(gui->go_to_line_dialog->go_to_line_dialog);

  return ;
}



void display_go_to_line_dialog(GtkWidget *widget) {
  /** Generate the line number asking toplevel dialog window. **/

  #ifdef DEBUG
    DEBUG_FUNC_MARK
  #endif

  gui->go_to_line_dialog->go_to_line_dialog  = gtk_dialog_new() ;

  gtk_window_set_transient_for(GTK_WINDOW(gui->go_to_line_dialog->go_to_line_dialog), GTK_WINDOW(gui->main_window)) ;

  gtk_window_set_icon_from_file(GTK_WINDOW(gui->go_to_line_dialog->go_to_line_dialog), PATH_TO_BUTTON_ICON "edit-text-frame-update.png", NULL) ;
  gtk_window_set_decorated(GTK_WINDOW(gui->go_to_line_dialog->go_to_line_dialog), FALSE) ;
  gtk_window_set_position(GTK_WINDOW(gui->go_to_line_dialog->go_to_line_dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(gui->go_to_line_dialog->go_to_line_dialog), _("Go to line...") );

  gui->go_to_line_dialog->go_to_line_content = gtk_dialog_get_content_area(GTK_DIALOG(gui->go_to_line_dialog->go_to_line_dialog));

  gui->go_to_line_dialog->go_to_line_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  /** Window content label in form: " Go to line (1 - line_count) ". **/
  char *label_text = g_strdup_printf( _(" Go to line: (1- %d) "), gtk_text_buffer_get_line_count(current_editor.current_buffer) ) ;
  gui->go_to_line_dialog->go_to_line_label = gtk_label_new(label_text);
  free(label_text) ;

  /** Line number entry field. **/
  gui->go_to_line_dialog->go_to_line_entry = gtk_entry_new() ;


  GFile *gicon_file = g_file_new_for_path(PATH_TO_BUTTON_ICON "format-line-spacing-normal.png");
  GIcon *icon       = g_file_icon_new(gicon_file) ;
  g_object_set(G_OBJECT(gui->go_to_line_dialog->go_to_line_entry), "primary-icon-gicon", icon, NULL) ;


  gtk_entry_set_input_purpose(GTK_ENTRY(gui->go_to_line_dialog->go_to_line_entry), GTK_INPUT_PURPOSE_DIGITS);


  char *len_chars ;
  len_chars = g_strdup_printf("%d", gtk_text_buffer_get_line_count(current_editor.current_buffer)) ;

  int length = (int) strlen(len_chars) ;

  g_free(len_chars) ;

  GtkEntryBuffer *entry_buffer = gtk_entry_buffer_new(NULL, -1) ;
  g_object_set(G_OBJECT(entry_buffer), "max-length", length, NULL) ;
  gtk_entry_set_buffer(GTK_ENTRY(gui->go_to_line_dialog->go_to_line_entry), entry_buffer) ;



  gtk_box_pack_start(GTK_BOX(gui->go_to_line_dialog->go_to_line_vbox), gui->go_to_line_dialog->go_to_line_label, FALSE, FALSE, 8 ) ;
  gtk_box_pack_start(GTK_BOX(gui->go_to_line_dialog->go_to_line_vbox), gui->go_to_line_dialog->go_to_line_entry, FALSE, FALSE, 8 ) ;

  gtk_container_add(GTK_CONTAINER (gui->go_to_line_dialog->go_to_line_content), gui->go_to_line_dialog->go_to_line_vbox);


  /** Dialog window buttons: **/
  gui->go_to_line_dialog->go_to_line_close = gtk_dialog_add_button(GTK_DIALOG(gui->go_to_line_dialog->go_to_line_dialog), _("Close"), GTK_RESPONSE_CLOSE) ;
  gui->go_to_line_dialog->go_to_line_apply = gtk_dialog_add_button(GTK_DIALOG(gui->go_to_line_dialog->go_to_line_dialog), _("Apply"), GTK_RESPONSE_APPLY) ;

  /** Apply the Return key shortcut to the Apply button: **/
  GtkAccelGroup *accel_group = gtk_accel_group_new();

  gtk_widget_add_accelerator(gui->go_to_line_dialog->go_to_line_apply,
                            "activate",
                            accel_group,
                            GDK_KEY_Return,
                            0,
                            GTK_ACCEL_VISIBLE);

  gtk_widget_add_accelerator(gui->go_to_line_dialog->go_to_line_close,
                            "activate",
                            accel_group,
                            GDK_KEY_Escape,
                            0,
                            GTK_ACCEL_VISIBLE);

  gtk_window_add_accel_group(GTK_WINDOW(gui->go_to_line_dialog->go_to_line_dialog), accel_group) ;



  gtk_widget_show_all(gui->go_to_line_dialog->go_to_line_dialog) ;

  gint result = gtk_dialog_run (GTK_DIALOG (gui->go_to_line_dialog->go_to_line_dialog));

  switch (result) {

    case GTK_RESPONSE_APPLY :

       go_to_line(gtk_text_buffer_get_line_count(current_editor.current_buffer)) ;
       break;

    case GTK_RESPONSE_CLOSE :

       gtk_widget_destroy(gui->go_to_line_dialog->go_to_line_dialog);
       break;
  }

  return ;
}


static void destroy_toplevel_terminal(GtkWidget *widget, GtkWidget *itterm) {

  GPid *shell_pid = g_object_get_data(G_OBJECT(gtk_itterm_get_vte_terminal(itterm)), "Pid") ;

  g_spawn_close_pid(*shell_pid) ;

  gtk_widget_destroy(itterm) ;

  gtk_widget_destroy(widget) ;

  return ;

}


static void display_execute_command_terminal(gchar *execute_command_string) {
  /** Generate the terminal include window for executing the wanted command. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if ( g_strcmp0(execute_command_string,"") == 0) {
    return ;
  }

  GtkWidget *itterm = gtk_itterm_new(TRUE, FALSE) ;


  GtkWidget *execute_command_terminal_dialog  = gtk_window_new(GTK_WINDOW_TOPLEVEL) ;


  gtk_window_set_icon_from_file(GTK_WINDOW(execute_command_terminal_dialog), PATH_TO_BUTTON_ICON "utilities-terminal.png", NULL) ;
  gtk_window_set_decorated(GTK_WINDOW(execute_command_terminal_dialog), TRUE) ;
  gtk_window_set_position(GTK_WINDOW(execute_command_terminal_dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_destroy_with_parent(GTK_WINDOW(execute_command_terminal_dialog), TRUE);
  gtk_window_set_title(GTK_WINDOW(execute_command_terminal_dialog), _("Execute command") );
  gtk_window_set_transient_for(GTK_WINDOW(execute_command_terminal_dialog), GTK_WINDOW(gui->main_window)) ;

  gtk_window_set_application(GTK_WINDOW(execute_command_terminal_dialog), app);

  g_signal_connect(G_OBJECT(execute_command_terminal_dialog), "destroy", G_CALLBACK(destroy_toplevel_terminal), itterm) ;

  gtk_widget_set_size_request(execute_command_terminal_dialog, 80 * 8 + 16, 24 *16) ;

  gtk_container_add(GTK_CONTAINER(execute_command_terminal_dialog),  itterm);

  gtk_widget_show_all(execute_command_terminal_dialog) ;

  gchar *command_string = g_strdup_printf("%s \r", execute_command_string) ;
  free(execute_command_string) ;

  while (gtk_events_pending()) {

    /** Wait enough time to get no displaying problems on my machine.
      *
      * Else the given command to the top-level terminal appears twice:
      * once before the prompt is displayed.
      * another time after the prompt is displayed.
      ****************************************************************/

    gtk_main_iteration_do(FALSE) ;
    g_usleep(5000) ; /** Wait enough time to have no displaying problems on my machine. **/

  }

  /** Launch the wanted command in the terminal. **/
  vte_terminal_feed_child(VTE_TERMINAL(gtk_itterm_get_vte_terminal(itterm)), command_string, strlen(command_string));

  free(command_string) ;

  return ;

}

void display_execute_command_dialog(GtkWidget *widget) {
  /** Generate the execute command dialog, asking the user for the command to execute. **/

  #ifdef DEBUG
    DEBUG_FUNC_MARK
  #endif

  GtkWidget *execute_command_dialog  = gtk_dialog_new_with_buttons ( _("Execute a command"),
                                                GTK_WINDOW(gui->main_window),
                                                GTK_DIALOG_USE_HEADER_BAR | GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                NULL,
                                                NULL);



  gtk_window_set_decorated(GTK_WINDOW(execute_command_dialog), TRUE) ;
  gtk_window_set_destroy_with_parent(GTK_WINDOW(execute_command_dialog), TRUE);
  gtk_window_set_position(GTK_WINDOW(execute_command_dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_modal(GTK_WINDOW(execute_command_dialog), TRUE);
  gtk_window_set_icon_from_file(GTK_WINDOW(execute_command_dialog), PATH_TO_BUTTON_ICON "system-run.png", NULL) ;
  gtk_window_set_title(GTK_WINDOW(execute_command_dialog), _("Execute command") );
  gtk_window_set_application(GTK_WINDOW(execute_command_dialog), app);

  GtkWidget *execute_command_dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(execute_command_dialog));

  GtkWidget *execute_command_dialog_vbox    = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;


  GtkWidget *execute_command_dialog_label = gtk_label_new( _(" Enter a command (per example: man [section] page) ") );

  GtkWidget *execute_command_dialog_entry = gtk_entry_new() ;



  GFile *gicon_file = g_file_new_for_path(PATH_TO_BUTTON_ICON "system-run.png");
  GIcon *icon       = g_file_icon_new(gicon_file) ;
  g_object_set(G_OBJECT(execute_command_dialog_entry), "primary-icon-gicon", icon, NULL) ;


  gtk_box_pack_start(GTK_BOX(execute_command_dialog_vbox), execute_command_dialog_label, FALSE, FALSE, 8 ) ;
  gtk_box_pack_start(GTK_BOX(execute_command_dialog_vbox), execute_command_dialog_entry, FALSE, FALSE, 8 ) ;


  gtk_container_add(GTK_CONTAINER (execute_command_dialog_content), execute_command_dialog_vbox);



  GtkWidget *execute_command_dialog_cancel  = gtk_dialog_add_button(GTK_DIALOG(execute_command_dialog), _("Cancel"),  GTK_RESPONSE_CANCEL) ;
  GtkWidget *execute_command_dialog_apply   = gtk_dialog_add_button(GTK_DIALOG(execute_command_dialog), _("Execute"), GTK_RESPONSE_APPLY) ;

  /** Assign the Return key shortcut to the Apply button. **/
  GtkAccelGroup *accel_group = gtk_accel_group_new();
  gtk_widget_add_accelerator(execute_command_dialog_apply,
                           "activate",
                           accel_group,
                           GDK_KEY_Return,
                           0,
                           GTK_ACCEL_VISIBLE);

  gtk_widget_add_accelerator(execute_command_dialog_cancel,
                           "activate",
                           accel_group,
                           GDK_KEY_Escape,
                           0,
                           GTK_ACCEL_VISIBLE);

  gtk_window_add_accel_group(GTK_WINDOW(execute_command_dialog), accel_group) ;


  gtk_widget_show_all(execute_command_dialog) ;



  gint result = gtk_dialog_run (GTK_DIALOG (execute_command_dialog));

  char *exec_cmd ;

  switch (result) {

    case GTK_RESPONSE_APPLY :

       exec_cmd = g_strdup(gtk_entry_get_text(GTK_ENTRY(execute_command_dialog_entry)) ) ;

       gtk_widget_destroy(execute_command_dialog) ;
       display_execute_command_terminal(exec_cmd) ;

       break;

    case GTK_RESPONSE_CANCEL :

       gtk_widget_destroy(execute_command_dialog);

       break;
  }

  return ;


}


static void application_selected(GtkAppChooserWidget *self, GAppInfo *application) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GError *error = NULL ;

  g_app_info_launch_uris(application, NULL, g_app_launch_context_new(), &error) ;

  if (error != NULL) {

    display_message_dialog( _("Error launching application !"), error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_error_free(error) ;

  }

  return ;

}


void display_application_laucher_dialog(GtkWidget *widget) {
  /** Generate application launcher dialog window. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *app_launcher_dialog = gtk_app_chooser_dialog_new_for_content_type(GTK_WINDOW(gui->main_window), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, "." ) ;

  gtk_app_chooser_dialog_set_heading(GTK_APP_CHOOSER_DIALOG(app_launcher_dialog), _("Select application to launch") ) ;

  GtkWidget *app_launcher_widget = gtk_app_chooser_dialog_get_widget(GTK_APP_CHOOSER_DIALOG(app_launcher_dialog) );

  g_signal_connect(G_OBJECT(app_launcher_widget), "application-selected", G_CALLBACK(application_selected), NULL) ;

  gtk_app_chooser_widget_set_show_all(GTK_APP_CHOOSER_WIDGET(app_launcher_widget), TRUE) ;



  gtk_dialog_run(GTK_DIALOG(app_launcher_dialog));

  gtk_widget_destroy(app_launcher_dialog) ;

}

static gboolean activate_link(GtkLinkButton *button) {

  if (! settings.session_mode) {

    goto launching ;

  }

  guint16 length = 0 ;

  if (session->session_doc != NULL) {

   length = g_strv_length(session->session_doc);

  }

  gchar **strv_doc = g_strdupv(session->session_doc) ;

  g_strfreev(session->session_doc) ;

  session->session_doc = calloc(length+2, sizeof(gchar *)) ;

  guint16 i = 0 ;

  for ( ; i < length ; ++i) {

    g_free(session->session_doc[i]) ;

    session->session_doc[i] = g_strdup(strv_doc[i]) ;

  }

  g_strfreev(strv_doc) ;

  session->session_doc[i] = g_strdup(gtk_link_button_get_uri(button)) ;

  session->session_doc[++i] = NULL ;

  session->session_doc_nb = i ;

  launching : {

    GError *error = NULL ;

    gtk_show_uri(NULL, gtk_link_button_get_uri(button), GDK_CURRENT_TIME, &error);


    if (error != NULL) {

      display_message_dialog( _("Error launching file !"), error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

      g_error_free(error) ;

    }

  }

  return TRUE ;

}




void display_file_handler_dialog(GtkWidget *widget) {
  /** Generate "File Handler" feature dialog window. **/

  #ifdef DEBUG
    DEBUG_FUNC_MARK
  #endif

  /** Read the file which containing the titles (the buttons labels) and the corresponding file URI.
    * And construct the window.
  **/

  GError *error = NULL ;

  GIOChannel *gio_channel = g_io_channel_new_file(PATH_TO_LINKS_FILE, "r", &error) ;

  if (error != NULL) {

    display_message_dialog( _("Error open file !"), error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_error_free(error) ;

    return ;

  }


  #ifdef MAC_INTEGRATION  /** The library gtk-mac-integration-gtk3 define this. **/
  g_io_channel_set_line_term(gio_channel, "\r", 1);
  #else
  g_io_channel_set_line_term(gio_channel, "\n", 1);
  #endif

  GtkWidget *file_handler_dialog_main_window = gtk_dialog_new() ;

  gtk_window_set_transient_for(GTK_WINDOW(file_handler_dialog_main_window), GTK_WINDOW(gui->main_window)) ;

  gtk_window_set_destroy_with_parent(GTK_WINDOW(file_handler_dialog_main_window), TRUE)    ;
  gtk_window_set_resizable(GTK_WINDOW(file_handler_dialog_main_window),           FALSE)   ;
  gtk_window_set_position(GTK_WINDOW(file_handler_dialog_main_window), GTK_WIN_POS_CENTER) ;
  gtk_window_set_decorated(GTK_WINDOW(file_handler_dialog_main_window),           TRUE)    ;

  gtk_window_set_title(GTK_WINDOW(file_handler_dialog_main_window),              _("Files Handler") );
  gtk_widget_set_size_request(GTK_WIDGET(file_handler_dialog_main_window),        400, 600);


  GtkWidget *file_handler_scrolled_window    = gtk_scrolled_window_new(NULL, NULL) ;

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(file_handler_scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );

  gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(file_handler_scrolled_window),  400) ;
  gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(file_handler_scrolled_window), 600) ;

  GtkWidget *file_handler_vbox = gtk_box_new( GTK_ORIENTATION_VERTICAL, 0) ;

  while (TRUE) {

    gchar *label = NULL ;

    /** Getting the title from the entry (button label text). **/
    if (g_io_channel_read_line(gio_channel, &label, NULL, NULL, NULL) != G_IO_STATUS_NORMAL) {

      break ;

    }

    char *label_linefeed = strrchr(label,'\n') ;

    if ( label_linefeed != NULL  ) {
      label[label_linefeed - label] = '\0' ;
    }

    gchar *uri = NULL ;
    /** Getting the URI corresponding to the file to be launched. **/
    if (g_io_channel_read_line(gio_channel, &uri, NULL, NULL, NULL) != G_IO_STATUS_NORMAL) {

      break ;

    }

    label_linefeed = strrchr(uri,'\n') ;

    if ( label_linefeed != NULL  ) {
      uri[label_linefeed - uri] = '\0' ;
    }

    /** Construct a "link button" with the URI and the label (title). **/
    GtkWidget *file_handler_button =  gtk_link_button_new_with_label(uri, label);

    g_signal_connect(G_OBJECT(file_handler_button), "activate-link", G_CALLBACK(activate_link), NULL) ;

    gtk_box_pack_start(GTK_BOX(file_handler_vbox),  file_handler_button, FALSE, FALSE, 5) ;

    g_free(label) ;
    g_free(uri)   ;

  }

  g_io_channel_shutdown(gio_channel, FALSE, NULL) ;




  gtk_container_set_border_width(GTK_CONTAINER(file_handler_vbox), 5);

  gtk_container_add(GTK_CONTAINER(file_handler_scrolled_window), file_handler_vbox) ;


  GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(file_handler_dialog_main_window));

  gtk_container_add(GTK_CONTAINER(content_area), file_handler_scrolled_window) ;


  gtk_widget_show_all(file_handler_dialog_main_window) ;

  return ;

}



gboolean display_unsaved_files_dialog(void) {
  /** Generate the unsaved files at application exit dialog window. **/

  #ifdef DEBUG
    DEBUG_FUNC_MARK
  #endif

  GtkWidget *unsaved_files_dialog  = gtk_dialog_new_with_buttons( _("Some files still are unsaved..."),
                                                  GTK_WINDOW(gui->main_window),
                                                  GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                  _("Cancel quit"),
                                                  GTK_RESPONSE_CANCEL,
                                                  _("Save selected files and quit"),
                                                  GTK_RESPONSE_APPLY,
                                                  _("Quit without saving"),
                                                  GTK_RESPONSE_CLOSE,
                                                  NULL);

  gtk_window_set_destroy_with_parent(GTK_WINDOW(unsaved_files_dialog), TRUE)  ;
  gtk_window_set_decorated(GTK_WINDOW(unsaved_files_dialog),           FALSE)  ;
  gtk_window_set_resizable(GTK_WINDOW(unsaved_files_dialog),           FALSE) ;
  gtk_window_set_position(GTK_WINDOW(unsaved_files_dialog),            GTK_WIN_POS_CENTER) ;
  gtk_window_set_title(GTK_WINDOW(unsaved_files_dialog),               _("Some files still are unsaved...") ) ;




  GtkWidget *unsaved_files_content_area = gtk_dialog_get_content_area(GTK_DIALOG(unsaved_files_dialog)) ;

  gtk_container_set_border_width(GTK_CONTAINER(unsaved_files_content_area), 5) ;


  GtkWidget *unsaved_files_scrolled_window = gtk_scrolled_window_new(NULL, NULL) ;

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(unsaved_files_scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC) ;



  GtkWidget *unsaved_files_listbox = gtk_list_box_new() ;

  gtk_list_box_set_selection_mode(GTK_LIST_BOX(unsaved_files_listbox), GTK_SELECTION_NONE);

  gint c = 0 ;

  /** The unsaved_files GSList is yet filled with the unsaved files list from the function: files_not_saved_check(). **/

  while (unsaved_files->data != NULL) {

    gchar *filepath = (gchar *) ((struct Unsaved_File *) (unsaved_files->data))->filepath ;

    /** Generate an entry in the GtkListBox as a checked checkbutton. **/
    GtkWidget *file_checkbox = gtk_check_button_new_with_label(g_path_get_basename(filepath)) ;

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(file_checkbox), TRUE) ;


    gint *num = &((struct Unsaved_File *) (unsaved_files->data))->page_number ;

    /** Setting datas for further saving processing. **/
    g_object_set_data(G_OBJECT(file_checkbox), "page_number", num) ;
    g_object_set_data(G_OBJECT(file_checkbox), "filepath", filepath) ;


    gtk_list_box_insert(GTK_LIST_BOX(unsaved_files_listbox), file_checkbox, c) ;

    c++ ;



    if (unsaved_files->next == NULL) {
      break ;
    }

    unsaved_files = unsaved_files->next ;



  }

  gint cc = 0  ;

  while (cc < c) {
    /** Clean the unsaved_files GSList, to not have doubles if the user choose to not close the application and try to quit. **/
    unsaved_files = g_slist_remove(unsaved_files, g_slist_nth_data(unsaved_files, cc)) ;
    cc++ ;

    /** @NOTE: the files saving, if the user choose it will happend throught the checkbutton datas. **/
  }


  gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(unsaved_files_scrolled_window), c < 9 ? c * 32 : 256) ;

  gtk_container_add(GTK_CONTAINER(unsaved_files_scrolled_window), unsaved_files_listbox) ;
  gtk_container_add(GTK_CONTAINER(unsaved_files_content_area),    unsaved_files_scrolled_window) ;

  gtk_widget_show_all(unsaved_files_dialog) ;

  gint result = gtk_dialog_run (GTK_DIALOG (unsaved_files_dialog));


  cc = 0 ;

  /** @NEW: Usage of the @GtkSourceFileSaver !!! **/

  switch (result) {

    case GTK_RESPONSE_APPLY :

      while (cc < c) {

        GtkListBoxRow *listbox_row = gtk_list_box_get_row_at_index(GTK_LIST_BOX(unsaved_files_listbox), cc);

        GtkWidget *file_checkbox = gtk_bin_get_child(GTK_BIN(listbox_row)) ;

        if ( gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(file_checkbox)) ) {

          gint page_number = *(gint *) g_object_get_data(G_OBJECT(file_checkbox), "page_number") ;

          gchar *filepath  = g_object_get_data(G_OBJECT(file_checkbox), "filepath") ;


          /** Getting the notebook page to save. **/
          GtkWidget *notebook_page        = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->editor_notebook), page_number) ;
          GtkWidget *current_textview     = gtk_bin_get_child(GTK_BIN(notebook_page)) ;
          GtkTextBuffer *current_buffer   = gtk_text_view_get_buffer(GTK_TEXT_VIEW(current_textview)) ;

          GtkSourceFile *source_file = gtk_source_file_new() ;

          GFile *g_file = g_file_new_for_path(filepath) ;

          gtk_source_file_set_location(source_file, g_file) ;

          source_file_saver(GTK_SOURCE_BUFFER(current_buffer), source_file) ;

          g_free(filepath) ;

        }

        cc++ ;

      }


      gtk_widget_destroy(unsaved_files_dialog) ;

      return FALSE ;

    case GTK_RESPONSE_CANCEL :

      gtk_widget_destroy(unsaved_files_dialog) ;
      return TRUE ;  /** Do not propagate the signal to the destroy signal handler wich will exit the application. **/

    case GTK_RESPONSE_CLOSE :

      gtk_widget_destroy(unsaved_files_dialog) ;
      exit(EXIT_SUCCESS) ;

  }

  return FALSE ;
}

void display_about_dialog(GtkWidget *widget) {


  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif


  GtkWidget *about_dialog = gtk_about_dialog_new() ;

  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(about_dialog), PRGNAME) ;

  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about_dialog),      VERSION)   ;

  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(about_dialog),    COPYRIGHT)   ;

  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(about_dialog),     _("it-edit the terminals integrated text-editor,\n"
                                                                    "with severals functionalities.")) ;
  gchar *file_content ;

  g_file_get_contents(PATH_TO_GPL_TEXT, &file_content, NULL, NULL) ;

  gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(about_dialog), file_content) ;

  g_free(file_content) ;

  gtk_about_dialog_set_wrap_license(GTK_ABOUT_DIALOG(about_dialog), FALSE);

  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(about_dialog), PRGWEBURL) ;

  gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(about_dialog), PRGNAME " Website") ;

  const gchar *author[2] = {AUTHOR " " MAIL, NULL} ;

  gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(about_dialog), author);

  gtk_about_dialog_set_documenters(GTK_ABOUT_DIALOG(about_dialog), author);

  gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(about_dialog), NULL) ;

  gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(about_dialog), NULL);

  const gchar *thanks[2] = { _("Thanks to my beloved mother, my family and the doctors."), NULL} ;

  gtk_about_dialog_add_credit_section(GTK_ABOUT_DIALOG(about_dialog), "Thank's", thanks);

  const gchar *advice[2] = { _("Stay away from drugs: drugs destroy your brain and your life."), NULL} ;

  gtk_about_dialog_add_credit_section(GTK_ABOUT_DIALOG(about_dialog), _("Advice:"), advice);

  gtk_dialog_run(GTK_DIALOG(about_dialog)) ;

  gtk_widget_destroy(about_dialog) ;

  return ;

}

static GtkWidget *ctime_entry ;
static GtkWidget *atime_entry ;
static GtkWidget *mtime_entry ;

static GtkWidget *file_name_entry ;

static GtkWidget* generate_calendar(GStatBuf *datetime, gint16 type) {

  #ifdef DEBUG
    DEBUG_FUNC_MARK
  #endif

  GtkWidget *calendar = gtk_calendar_new() ;

  GDateTime *datetime_file = g_date_time_new_from_unix_local( (type == -1) ?  datetime->st_atime : (type == 0) ? datetime->st_ctime : datetime->st_mtime) ;

  gtk_calendar_select_month(GTK_CALENDAR(calendar), (guint) g_date_time_get_month(datetime_file)-1, (guint) g_date_time_get_year(datetime_file));

  gtk_calendar_mark_day(GTK_CALENDAR(calendar), g_date_time_get_day_of_month(datetime_file)) ;

  gtk_calendar_set_display_options(GTK_CALENDAR(calendar), GTK_CALENDAR_SHOW_HEADING | GTK_CALENDAR_SHOW_DAY_NAMES) ;

  gtk_calendar_set_detail_width_chars(GTK_CALENDAR(calendar), 63) ;

  gtk_calendar_set_detail_height_rows(GTK_CALENDAR(calendar), 63) ;

  g_date_time_unref(datetime_file) ;

  return calendar ;
}

static void generate_calendar_and_timer(GtkWidget *widget, GStatBuf *datetime) {

  #ifdef DEBUG
    DEBUG_FUNC_MARK
  #endif

  if ( g_strcmp0(gtk_widget_get_name(widget), "ctime") == 0 ) {

    return ;

  }

  GtkWidget *dialog = gtk_dialog_new_with_buttons( _("New timestamp"),
                                                  GTK_WINDOW(gui->main_window),
                                                  GTK_DIALOG_USE_HEADER_BAR | GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                  _("OK"),
                                                  GTK_RESPONSE_ACCEPT,
                                                  _("Cancel"),
                                                  GTK_RESPONSE_REJECT,
                                                  NULL) ;

  GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog)) ;

  GtkWidget *calendar = NULL ;

  GtkWidget *label_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;
  GtkWidget *time_hbox  = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  GtkWidget *hour_label = gtk_label_new( _("Hours")   ) ;
  GtkWidget *min_label  = gtk_label_new( _("Minutes") ) ;
  GtkWidget *sec_label  = gtk_label_new( _("Seconds") ) ;

  GtkWidget *hour = gtk_spin_button_new_with_range(0, 23, 1);
  GtkWidget *min  = gtk_spin_button_new_with_range(0, 59, 1);
  GtkWidget *sec  = gtk_spin_button_new_with_range(0, 59, 1);

  gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(hour), FALSE) ;
  gtk_spin_button_set_update_policy(GTK_SPIN_BUTTON(hour), GTK_UPDATE_IF_VALID) ;
  gtk_spin_button_set_wrap(GTK_SPIN_BUTTON(hour), TRUE) ;

  gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(min), FALSE) ;
  gtk_spin_button_set_update_policy(GTK_SPIN_BUTTON(min), GTK_UPDATE_IF_VALID) ;
  gtk_spin_button_set_wrap(GTK_SPIN_BUTTON(min), TRUE) ;

  gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(sec), FALSE) ;
  gtk_spin_button_set_update_policy(GTK_SPIN_BUTTON(sec), GTK_UPDATE_IF_VALID) ;
  gtk_spin_button_set_wrap(GTK_SPIN_BUTTON(sec), TRUE) ;



  if ( g_strcmp0(gtk_widget_get_name(widget), "ctime") == 0 ) {

    calendar = generate_calendar(datetime, 0) ;

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(hour), g_date_time_get_hour(g_date_time_new_from_unix_local(datetime->st_ctime))   ) ;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(min),  g_date_time_get_minute(g_date_time_new_from_unix_local(datetime->st_ctime)) ) ;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(sec),  g_date_time_get_second(g_date_time_new_from_unix_local(datetime->st_ctime)) ) ;


  }
  else if ( g_strcmp0(gtk_widget_get_name(widget), "atime") == 0 ) {

    calendar = generate_calendar(datetime, -1) ;

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(hour), g_date_time_get_hour(g_date_time_new_from_unix_local(datetime->st_atime))   ) ;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(min),  g_date_time_get_minute(g_date_time_new_from_unix_local(datetime->st_atime)) ) ;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(sec),  g_date_time_get_second(g_date_time_new_from_unix_local(datetime->st_atime)) ) ;


  }
  else if ( g_strcmp0(gtk_widget_get_name(widget), "mtime") == 0 ) {

    calendar = generate_calendar(datetime, 1) ;

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(hour), g_date_time_get_hour(g_date_time_new_from_unix_local(datetime->st_mtime))   ) ;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(min),  g_date_time_get_minute(g_date_time_new_from_unix_local(datetime->st_mtime)) ) ;
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(sec),  g_date_time_get_second(g_date_time_new_from_unix_local(datetime->st_mtime)) ) ;


  }


  gtk_box_pack_start(GTK_BOX(label_hbox), hour_label, TRUE, TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(label_hbox), min_label,  TRUE, TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(label_hbox), sec_label,  TRUE, TRUE, 0) ;

  gtk_box_pack_start(GTK_BOX(time_hbox), hour, TRUE, TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(time_hbox), min,  TRUE, TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(time_hbox), sec,  TRUE, TRUE, 0) ;

  gtk_box_pack_start(GTK_BOX(content_area), calendar,   TRUE, TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(content_area), label_hbox, TRUE, TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(content_area), time_hbox,  TRUE, TRUE, 0) ;

  gtk_widget_show_all(dialog) ;

  gint ret = gtk_dialog_run(GTK_DIALOG(dialog)) ;

  if (ret == GTK_RESPONSE_ACCEPT) {

      guint year  ;
      guint month ;
      guint day   ;

      File_Editor *file_editor = g_object_get_data(G_OBJECT(current_editor.current_textview), "file_editor") ;

      gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day) ;

      gint64 timestamp = g_date_time_to_unix(g_date_time_new_local(year, month+1, day,
                            gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(hour)),
                            gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(min)),
                            gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sec)) ) ) ;

      if ( g_strcmp0(gtk_widget_get_name(widget), "atime") == 0 ) {

        datetime->st_atime = timestamp ;

        file_editor->file_info.st_atime = timestamp ;

        GDateTime *tmp = g_date_time_new_from_unix_local(timestamp) ;

        gtk_entry_set_text(GTK_ENTRY(atime_entry), g_date_time_format(tmp, "%F %T")) ;

        g_date_time_unref(tmp) ;

      }
      else if ( g_strcmp0(gtk_widget_get_name(widget), "mtime") == 0 ) {

        datetime->st_mtime = timestamp ;

        file_editor->file_info.st_mtime = timestamp ;

        GDateTime *tmp = g_date_time_new_from_unix_local(timestamp) ;

        gtk_entry_set_text(GTK_ENTRY(mtime_entry), g_date_time_format(tmp, "%F %T")) ;

        g_date_time_unref(tmp) ;

      }


      struct utimbuf *utime_buf = g_malloc(sizeof(struct utimbuf)) ;

      utime_buf->actime   = file_editor->file_info.st_atime ;
      utime_buf->modtime  = file_editor->file_info.st_mtime ;

      errno = 0 ;

      if (g_utime(g_object_get_data(G_OBJECT(file_editor->buffer), "filepath"), utime_buf) != 0) {

        display_message_dialog( _("Error changing timestamp !"), g_strerror(errno), GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

      }

      g_free(utime_buf) ;

  }

  gtk_widget_destroy(dialog) ;

  return ;

}

static void rename_file(GtkWidget *widget, File_Editor *file_editor) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *file_chooser_dialog = gtk_file_chooser_dialog_new( _("Rename file as:"), GTK_WINDOW(gui->main_window), GTK_FILE_CHOOSER_ACTION_SAVE,
                                                               _("Cancel") , GTK_RESPONSE_CANCEL,
                                                               _("Rename"),  GTK_RESPONSE_ACCEPT,
                                                               NULL);

  gchar *filepath = g_object_get_data(G_OBJECT(file_editor->buffer), "filepath") ;

  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(file_chooser_dialog), TRUE) ;

  gchar *basename = g_path_get_basename(filepath) ;

  gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(file_chooser_dialog), basename) ;

  g_free(basename) ;

  gchar *dirpath = g_path_get_dirname(filepath) ;

  if (g_strcmp0(dirpath, g_get_tmp_dir()) == 0) {

    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser_dialog), g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP)) ;

  }
  else {

    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser_dialog), dirpath) ;

  }

  g_free(dirpath) ;

  gtk_file_chooser_set_use_preview_label(GTK_FILE_CHOOSER(file_chooser_dialog), TRUE) ;

  gint ret = gtk_dialog_run(GTK_DIALOG(file_chooser_dialog)) ;

  if (ret == GTK_RESPONSE_ACCEPT) {

    gchar *new_filepath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser_dialog)) ;

    errno = 0 ;

    if ( g_rename(filepath, new_filepath) == -1) {

      display_message_dialog( _("Error renaming file !"), g_strerror(errno), GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    }
    else {

      g_object_set_data(G_OBJECT(file_editor->buffer), "filepath", new_filepath) ;

      gchar *new_file_basename = g_path_get_basename(new_filepath) ;


      gtk_entry_set_text(GTK_ENTRY(file_name_entry),  new_file_basename) ;

      gtk_widget_set_tooltip_text(gui->bottom_bar->filename_label, new_file_basename) ;




      /** Getting the notebook page tab label. **/
      GtkWidget *notebook_tab = gtk_notebook_get_tab_label(GTK_NOTEBOOK(gui->editor_notebook), current_editor.current_notebook_page);

      gtk_notebook_set_menu_label_text(GTK_NOTEBOOK(gui->editor_notebook), current_editor.current_notebook_page, new_file_basename) ;

      GList *tab_box_list = gtk_container_get_children(GTK_CONTAINER(notebook_tab)) ;

      GtkWidget *tab_label = g_list_nth_data(tab_box_list, 1) ;

      /** Update editor's component to the new file name and path. **/
      gtk_label_set_text(GTK_LABEL(tab_label), new_file_basename) ;

      gtk_widget_set_tooltip_text(tab_label, new_filepath) ;

      gtk_widget_set_tooltip_text(gui->bottom_bar->filename_label, new_filepath) ;

      gtk_label_set_text(GTK_LABEL(gui->bottom_bar->filename_label), new_file_basename) ;



      g_free(new_file_basename) ;

      g_free(filepath) ;

      GtkWidget *tab_icon  = g_list_nth_data(tab_box_list, 0) ;

      gtk_image_clear(GTK_IMAGE(tab_icon)) ;

      gchar *mimetype_image_filepath = get_mimetype_image_filepath(file_editor->source_file, PATH_TO_MIMETYPE_ICON) ;

      gtk_image_set_from_file(GTK_IMAGE(tab_icon), mimetype_image_filepath) ;

      g_free(mimetype_image_filepath) ;

      GFile *g_file = g_file_new_for_path(new_filepath) ;

      gtk_source_file_set_location(file_editor->source_file, g_file) ;

      g_list_free(tab_box_list) ;

      g_object_unref(g_file) ;

      bool can_load_content_easy = true ;

      gchar *file_content = NULL ;

      gsize file_size = 0 ;

      if (! g_file_get_contents(new_filepath, &file_content, &file_size, NULL) ) {
      /** Cannot get file content **/

        can_load_content_easy = false ;

      }


      gboolean result_uncertain = TRUE ;

      /** Try to guess the file content type. (Per filename) **/
      gchar *content_type = g_content_type_guess(new_filepath, NULL, file_size, &result_uncertain);

      if (result_uncertain && can_load_content_easy) {

        g_free(content_type);
        content_type = NULL;

        /** Try to guess the file content type. (Per content analyze) **/
        content_type = g_content_type_guess(NULL, (const guchar *) file_content, file_size, &result_uncertain);

        if (result_uncertain) {

          g_free(content_type);
          content_type = NULL;

        }

      }


      if (source_language_manager != NULL) {

        /** Try to guess the loaded source file language: **/
        GtkSourceLanguage *source_language = gtk_source_language_manager_guess_language(source_language_manager, new_filepath, content_type);

        if (source_language != NULL) {


          const gchar *lang_id = (char *) gtk_source_language_get_id(source_language);

          if (lang_id == NULL) {

            goto no_style ;
          }

          /** Settting the menu item from the differents supported language on the right value. **/
          set_syntax_highlight_radio(lang_id) ;


          /** Update buffer syntax highlight **/
          if (! gtk_source_buffer_get_highlight_syntax(file_editor->buffer)) {

            gtk_source_buffer_set_highlight_syntax(file_editor->buffer,    TRUE) ;
          }

          gtk_source_buffer_set_language(file_editor->buffer, source_language) ;

          GtkTextIter start, end ;

          gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(file_editor->buffer), &start) ;

          gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(file_editor->buffer), &end) ;

          gtk_source_buffer_ensure_highlight(file_editor->buffer, &start, &end);

        }
        else if (source_language == NULL) {

          /** Language guessing failed. **/

          no_style :

          /** Setting the menu item from the different supported language on the default value.  **/
          set_syntax_highlight_radio("text") ;

        }

      }

    g_free(content_type) ;

    g_free(file_content) ;

    #if 0
    /** We don't allow to free it because we set it as data. **/
    g_free(new_filepath) ;
    #endif

    }

  }

  gtk_widget_destroy(file_chooser_dialog) ;

  return ;

}

static void copy_file(GtkWidget *widget, File_Editor *file_editor) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *file_chooser_dialog = gtk_file_chooser_dialog_new( _("Copy file:"), GTK_WINDOW(gui->main_window), GTK_FILE_CHOOSER_ACTION_SAVE,
                                                               _("Cancel") , GTK_RESPONSE_CANCEL,
                                                               _("Copy"),  GTK_RESPONSE_ACCEPT,
                                                               NULL);

  gchar *filepath = g_object_get_data(G_OBJECT(file_editor->buffer), "filepath") ;

  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(file_chooser_dialog), TRUE) ;

  gchar *dirpath = g_path_get_dirname(filepath) ;

  if (g_strcmp0(dirpath, g_get_tmp_dir()) == 0) {

    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser_dialog), g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP)) ;

  }
  else {

    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser_dialog), dirpath) ;

  }

  g_free(dirpath) ;

  gtk_file_chooser_set_use_preview_label(GTK_FILE_CHOOSER(file_chooser_dialog), TRUE) ;

  gint ret = gtk_dialog_run(GTK_DIALOG(file_chooser_dialog)) ;

  if (ret == GTK_RESPONSE_ACCEPT) {

    GError *error = NULL ;

    GFile *g_file_dst = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(file_chooser_dialog)) ;

    GFile *g_file_src = g_file_new_for_path(filepath) ;

    if (! g_file_copy(g_file_src, g_file_dst, G_FILE_COPY_OVERWRITE | G_FILE_COPY_ALL_METADATA | G_FILE_COPY_NO_FALLBACK_FOR_MOVE | G_FILE_COPY_TARGET_DEFAULT_PERMS, NULL, NULL, NULL, &error) ) {


      display_message_dialog( _("Error copying file !"), error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

      g_error_free(error) ;

    }

    g_object_unref(g_file_src) ;
    g_object_unref(g_file_dst) ;

  }

  gtk_widget_destroy(file_chooser_dialog) ;

  return ;

}

static void update_preview(GtkWidget *widget) {

  gchar *filename = gtk_file_chooser_get_current_name(GTK_FILE_CHOOSER(widget));


  if (! g_str_has_suffix(filename, ".gz")) {

    gchar *new_filename = g_strdup_printf("%s.gz", filename) ;

    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(widget), new_filename) ;

    g_free(new_filename) ;

  }

  g_free(filename) ;

  return ;

}            


static void compress_file(GtkWidget *widget, File_Editor *file_editor) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *file_chooser_dialog = gtk_file_chooser_dialog_new( _("Compress file:"), GTK_WINDOW(gui->main_window), GTK_FILE_CHOOSER_ACTION_SAVE,
                                                               _("Cancel") , GTK_RESPONSE_CANCEL,
                                                               _("Compress"),  GTK_RESPONSE_ACCEPT,
                                                               NULL);

  gchar *filepath = g_object_get_data(G_OBJECT(file_editor->buffer), "filepath") ;

  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(file_chooser_dialog), TRUE) ;

  gchar *basename = g_path_get_basename(filepath) ;

  gchar *compressed_name = g_strdup_printf("%s.gz", basename) ;

  g_free(basename) ;

  gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(file_chooser_dialog), compressed_name) ;

  g_free(compressed_name) ;

  gchar *dirpath = g_path_get_dirname(filepath) ;

  if (g_strcmp0(dirpath, g_get_tmp_dir()) == 0) {

    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser_dialog), g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP)) ;

  }
  else {

    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser_dialog), dirpath) ;

  }

  g_free(dirpath) ;

  gtk_file_chooser_set_use_preview_label(GTK_FILE_CHOOSER(file_chooser_dialog), TRUE) ;

  g_signal_connect(G_OBJECT(file_chooser_dialog), "update-preview", G_CALLBACK(update_preview), NULL) ;

  gint ret = gtk_dialog_run(GTK_DIALOG(file_chooser_dialog)) ;

  if (ret == GTK_RESPONSE_ACCEPT) {

    GError *error = NULL ;

    GFile *g_file_dst = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(file_chooser_dialog)) ;

    gchar *filename = g_file_get_basename(g_file_dst) ;

    gchar *filepath_dst = g_file_get_path(g_file_dst) ;

    if (! g_str_has_suffix(filename, ".gz")) {

      g_object_unref(g_file_dst) ;

      gchar *new_filepath = g_strdup_printf("%s/%s.gz", g_path_get_dirname(filepath_dst),filename) ;

      g_free(filepath_dst) ;

      g_file_dst = g_file_new_for_path(new_filepath) ;

      g_free(new_filepath) ;

    }

    GFile *g_file_src = g_file_new_for_path(filepath) ;

    GtkSourceFile *source_file = gtk_source_file_new() ;

    gtk_source_file_set_location(source_file,  g_file_src) ;

    errno = 0 ;

    if (! source_file_saver_with_target(file_editor->buffer, source_file, g_file_dst, TRUE)) {

      display_message_dialog( _("Error compressing file !"), g_strerror(errno), GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

      g_error_free(error) ;

    }

    g_object_unref(g_file_src) ;
    g_object_unref(g_file_dst) ;

  }

  gtk_widget_destroy(file_chooser_dialog) ;

  return ;

}

static void move_file(GtkWidget *widget, File_Editor *file_editor) {

  GtkWidget *file_chooser_dialog = gtk_file_chooser_dialog_new( _("Move file:"), GTK_WINDOW(gui->main_window), GTK_FILE_CHOOSER_ACTION_SAVE,
                                                               _("Cancel") , GTK_RESPONSE_CANCEL,
                                                               _("Move"),  GTK_RESPONSE_ACCEPT,
                                                               NULL);

  gchar *filepath = g_object_get_data(G_OBJECT(file_editor->buffer), "filepath") ;

  gchar *dirpath = g_path_get_dirname(filepath) ;

  if (g_strcmp0(dirpath, g_get_tmp_dir()) == 0) {

    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser_dialog), g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP)) ;

  }
  else {

    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser_dialog), dirpath) ;

  }

  g_free(dirpath) ;

  GFile *g_file_source_file = g_file_new_for_path(filepath) ;

  gtk_file_chooser_set_file(GTK_FILE_CHOOSER(file_chooser_dialog),g_file_source_file, NULL);

  g_object_unref(g_file_source_file) ;

  gtk_file_chooser_set_use_preview_label(GTK_FILE_CHOOSER(file_chooser_dialog), TRUE) ;

  gint ret = gtk_dialog_run(GTK_DIALOG(file_chooser_dialog)) ;

  if (ret == GTK_RESPONSE_ACCEPT) {

    GError *error = NULL ;

    GFile *g_file_dst = gtk_file_chooser_get_current_folder_file(GTK_FILE_CHOOSER(file_chooser_dialog)) ;

    gchar *folderpath = g_file_get_path(g_file_dst) ;

    g_object_unref(g_file_dst) ;

    gchar *basename = g_path_get_basename(filepath) ;

    gchar *tmp_filepath = g_strdup_printf("%s/%s", folderpath, basename) ;

    g_free(folderpath) ;

    g_free(basename) ;

    g_file_dst = g_file_new_for_path(tmp_filepath) ;

    g_free(tmp_filepath) ;

    GFile *g_file_src = g_file_new_for_path(filepath) ;

    if (! g_file_move(g_file_src, g_file_dst, G_FILE_COPY_OVERWRITE | G_FILE_COPY_ALL_METADATA | G_FILE_COPY_NO_FALLBACK_FOR_MOVE | G_FILE_COPY_TARGET_DEFAULT_PERMS, NULL, NULL, NULL, &error) ) {


      display_message_dialog( _("Error moving file !"), error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

      g_error_free(error) ;

    }

    g_free(g_object_get_data(G_OBJECT(file_editor->buffer), "filepath")) ;

    gchar *new_filepath = g_file_get_path(g_file_dst) ;

    g_object_set_data(G_OBJECT(file_editor->buffer), "filepath", new_filepath) ;




    /** Getting the notebook page tab label. **/
    GtkWidget *notebook_tab = gtk_notebook_get_tab_label(GTK_NOTEBOOK(gui->editor_notebook), current_editor.current_notebook_page);

    GList *tab_box_list = gtk_container_get_children(GTK_CONTAINER(notebook_tab)) ;

    GtkWidget *tab_label = g_list_nth_data(tab_box_list, 1) ;

    /** Update editor's component to the new file name and path. **/

    gtk_widget_set_tooltip_text(tab_label, new_filepath) ;

    gtk_widget_set_tooltip_text(gui->bottom_bar->filename_label, new_filepath) ;

    g_object_unref(g_file_src) ;
    g_object_unref(g_file_dst) ;


  }

  gtk_widget_destroy(file_chooser_dialog) ;

  return ;

}


static void clipboard_recv_func(GtkClipboard *clipboard, GtkSelectionData *selection_data, gpointer data) {

  /** Callback function called from the copy file-path to clipbaord function: cp_filepath_to_clipboard().
    * I should free the @selection_data argument of the clipboard but
    * after excluding the case where the given @GtkSelectionData data structure equal to @NULL,
    * calling the gtk_selection_data_free() function make the program crash !?!
    * So we simply return from this function.
    ****************************************************************************************************/

  #if 0
  if (selection_data == NULL) {

    return ;
  }

  /** Does not work as expected. **/

  gtk_selection_data_free(selection_data) ;
  #endif

  return ;

}

void copy_filepath_to_clipboard(GtkWidget *widget, gchar *filepath) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  /** Function to copy the current edited file @filepath to the clipboard.
    *
    * @WARNING: This function can fail without preventing the user from the error.
    *           I have try to deal it with the @errno value but nothing works properly.
    *           @errno simply not equal to 0 in case of success ?
    *
    * @NOTE: You take a look at the specification of the Clipboard at:
    *        http://www.freedesktop.org/Standards/clipboards-spec
    *        to sea that the clipboard handle is bad, but it works.
    *
    ************************************************************************************/

  GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD) ;


  g_application_hold(G_APPLICATION(app));

  gtk_clipboard_request_contents(clipboard, GDK_SELECTION_CLIPBOARD, &clipboard_recv_func, NULL) ;

  g_application_release(G_APPLICATION(app)) ;


  while (g_main_context_pending(NULL)) {

    g_main_context_iteration(NULL, FALSE) ;

  }


  gsize bytes_written ;

  gchar *filepath_utf8 = g_filename_to_utf8(filepath, -1, NULL, &bytes_written, NULL) ;

  #if 0
  /** errno = 0 ; This error code detection don't work as it should: errno != 0 by success ! Surely because the clipboard handling is poor, see the specification for more informations.    **/
  #endif

  gtk_clipboard_set_text(clipboard, filepath_utf8, bytes_written) ;

  #if 0
  /** This doesn't work as espect. **/
  if (errno != 0) {

    display_message_dialog(_("Error storing to clipboard !"), g_strerror(errno), GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_free(filepath_utf8) ;

    return ;

  }
  #endif

  g_free(filepath_utf8) ;

  return ;

}

static void toggle_mode_bit(GtkWidget *widget, File_Editor *file_editor) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if ( gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) ) {

    if ( g_strcmp0(gtk_widget_get_name(widget), "ur") == 0) {

      file_editor->file_info.st_mode |= S_IRUSR ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "uw") == 0) {

      file_editor->file_info.st_mode |= S_IWUSR ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "ux") == 0) {

      file_editor->file_info.st_mode |= S_IXUSR ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "gr") == 0) {

      file_editor->file_info.st_mode |= S_IRGRP ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "gw") == 0) {

      file_editor->file_info.st_mode |= S_IWGRP ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "gx") == 0) {

      file_editor->file_info.st_mode |= S_IXGRP ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "or") == 0) {

      file_editor->file_info.st_mode |= S_IROTH ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "ow") == 0) {

      file_editor->file_info.st_mode |= S_IWOTH ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "ox") == 0) {

      file_editor->file_info.st_mode |= S_IXOTH ;
    }

  }
  else {

    if ( g_strcmp0(gtk_widget_get_name(widget), "ur") == 0) {

      file_editor->file_info.st_mode ^= S_IRUSR ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "uw") == 0) {

      file_editor->file_info.st_mode ^= S_IWUSR ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "ux") == 0) {

      file_editor->file_info.st_mode ^= S_IXUSR ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "gr") == 0) {

      file_editor->file_info.st_mode ^= S_IRGRP ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "gw") == 0) {

      file_editor->file_info.st_mode ^= S_IWGRP ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "gx") == 0) {

      file_editor->file_info.st_mode ^= S_IXGRP ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "or") == 0) {

      file_editor->file_info.st_mode ^= S_IROTH ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "ow") == 0) {

      file_editor->file_info.st_mode ^= S_IWOTH ;
    }
    else if ( g_strcmp0(gtk_widget_get_name(widget), "ox") == 0) {

      file_editor->file_info.st_mode ^= S_IXOTH ;
    }
  }

  errno = 0 ;

  int ret = g_chmod(g_object_get_data(G_OBJECT(file_editor->buffer), "filepath"), file_editor->file_info.st_mode);


  static bool echo = false ;

  echo = ! echo ;

  if (ret == -1 && echo) {

    display_message_dialog( _("Error changing file permissions !"), g_strerror(errno), GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    if (echo) {

      if (! gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))  {

        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE) ;

      }
      else {

        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), FALSE) ;
      }

    }

  }

  return ;

}

static char *g_get_wc(char *filepath, char *option) {

  #ifdef DEBUG
    DEBUG_FUNC_MARK
  #endif

  if (! g_file_test(filepath, G_FILE_TEST_EXISTS)) {

    return NULL ;

  }

  gint standard_output ;

  GPid child_pid ;

  GError *error = NULL ;

  char *buffer = calloc(PATH_MAX+32, sizeof(char)) ;

  char *args[] =  {"wc", option, filepath, NULL} ;

  g_spawn_async_with_pipes(NULL,
                          args,
                          NULL,
                          G_SPAWN_SEARCH_PATH,
                          NULL,
                          NULL,
                          &child_pid,
                          NULL,
                          &standard_output,
                          NULL,
                          &error);

  if (error != NULL) {

    fprintf(stderr,"%s\n", error->message) ;

    g_error_free(error) ;

    return NULL ;
  }


  if  ( read(standard_output, buffer, sizeof(char) * (PATH_MAX+32) ) <= 0 ) {

    return NULL ;
  }

  close(standard_output) ;

  g_spawn_close_pid(child_pid) ;

  int c=0 ;

  while (g_ascii_isdigit(buffer[c])) {

    ++c ;
  }

  buffer[c]='\0' ;

  return buffer ;

}

void display_file_informations_dialog(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  const gint pages_number = gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) ;

  if (pages_number <= 0) {

    return ;

  }

  GtkWidget *file_informations_dialog  = gtk_dialog_new_with_buttons( _("File informations"), GTK_WINDOW(gui->main_window),
                                                                     GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_USE_HEADER_BAR ,
                                                                     _("Close"),
                                                                     GTK_RESPONSE_CLOSE,
                                                                     NULL);



  gtk_window_set_resizable(GTK_WINDOW(file_informations_dialog), FALSE) ;

  gtk_window_set_position(GTK_WINDOW(file_informations_dialog), GTK_WIN_POS_CENTER_ON_PARENT) ;

  gchar *filepath = g_object_get_data(G_OBJECT(current_editor.current_buffer), "filepath") ;

  File_Editor *file_editor = g_object_get_data(G_OBJECT(current_editor.current_textview), "file_editor") ;

  g_stat(filepath, &file_editor->file_info) ;


  gchar *source_language_name     = NULL ;
  gchar *source_language_mimetype = NULL ;
  gchar *source_language_globs    = NULL ;

  gchar *content_type = NULL ;

  gboolean result_uncertain = true ;

  gsize  file_size = 0 ;

  GtkSourceLanguage *source_language = NULL ;

  GtkSourceLanguageManager *source_language_manager = gtk_source_language_manager_get_default();

  gchar *file_basename = g_path_get_basename(filepath) ;

  /** Try to guess the file content type. **/
  content_type = g_content_type_guess(file_basename, NULL, file_size, &result_uncertain);

  if (content_type && source_language_manager) {

    /** Try to guess the loaded source file language: **/
    source_language = gtk_source_language_manager_guess_language(source_language_manager, file_basename, content_type);

    if (source_language != NULL) {

      GFile *g_file_source_file = g_file_new_for_path(filepath) ;



      GFileInfo *g_file_info_source_file = g_file_query_info(g_file_source_file, G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE, G_FILE_QUERY_INFO_NONE, NULL, NULL) ;

      if (g_file_source_file != NULL) {
 
        g_object_unref(g_file_source_file) ;

      }

      source_language_mimetype = g_strdup(g_file_info_get_attribute_string(g_file_info_source_file, G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE)) ;

      if (g_file_info_source_file != NULL) {

        g_object_unref(g_file_info_source_file) ;

      }

      source_language_globs = g_strdup(g_strrstr(filepath, ".")) ;

      if (gtk_source_language_get_name(source_language) != NULL) {
        source_language_name      = (gchar *) gtk_source_language_get_name(source_language) ;
      }
      else {
        source_language_name=NULL ;
      }
      if (source_language_mimetype == NULL) {

        if (gtk_source_language_get_mime_types(source_language) != NULL) {
   
          source_language_mimetype  = g_strdup(gtk_source_language_get_mime_types(source_language)[0]) ;
 
        }
   
      }
      if (source_language_globs == NULL) {

        if (gtk_source_language_get_globs(source_language) != NULL) {
 
          source_language_globs     = g_strdup(gtk_source_language_get_globs(source_language)[0]) ;
   
        }
   
      }

    }
    else if (source_language == NULL) {
      /** Language guessing failed. **/
      source_language_name      = NULL ;
      source_language_mimetype  = NULL ;
      source_language_globs     = NULL ;
    }

    g_free(content_type) ;

  }

  gchar *mimetype_filepath = get_mimetype_image_filepath(file_editor->source_file, PATH_TO_MIMETYPE_BIG) ;

  GtkWidget *main_info_image = gtk_image_new_from_file(mimetype_filepath) ;

  g_free(mimetype_filepath) ;


  GtkWidget *content_vbox = gtk_dialog_get_content_area(GTK_DIALOG(file_informations_dialog));

  GtkWidget *main_info_frame = gtk_frame_new( _("Main infos")) ;

  GtkWidget *main_info_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  GtkWidget *main_infos_and_file_actions_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  gtk_container_set_border_width(GTK_CONTAINER(main_info_hbox), 4) ;



  GtkWidget *main_info_grid = gtk_grid_new() ;

  gtk_grid_set_row_homogeneous(GTK_GRID(main_info_grid),    TRUE) ;
  gtk_grid_set_column_homogeneous(GTK_GRID(main_info_grid), TRUE) ;
  gtk_grid_set_row_spacing(GTK_GRID(main_info_grid),           2) ;
  gtk_grid_set_column_spacing(GTK_GRID(main_info_grid),        2) ;


  GtkWidget *main_info_language_name_label       = gtk_label_new( _("Language:  ") ) ;
  GtkWidget *main_info_language_mimetype_label   = gtk_label_new( _("Mimetype:  ") ) ;
  GtkWidget *main_info_language_extension_label  = gtk_label_new( _("Extension: ") ) ;

  GtkWidget *main_info_language_value            = gtk_label_new( (source_language_name == NULL) ? _("Unknow") : source_language_name) ;
  GtkWidget *main_info_mimetype_value            = gtk_label_new( (source_language_mimetype == NULL) ? _("Unknow") : source_language_mimetype) ;
  GtkWidget *main_info_extension_value           = gtk_label_new( (source_language_globs == NULL) ? _("Unknow") : source_language_globs) ;



  g_free(source_language_mimetype) ;

  g_free(source_language_globs) ;



  gtk_label_set_justify(GTK_LABEL(main_info_language_name_label),      GTK_JUSTIFY_RIGHT);
  gtk_label_set_justify(GTK_LABEL(main_info_language_mimetype_label),  GTK_JUSTIFY_RIGHT);
  gtk_label_set_justify(GTK_LABEL(main_info_language_extension_label), GTK_JUSTIFY_RIGHT);

  gtk_label_set_justify(GTK_LABEL(main_info_language_value),  GTK_JUSTIFY_LEFT);
  gtk_label_set_justify(GTK_LABEL(main_info_mimetype_value),  GTK_JUSTIFY_LEFT);
  gtk_label_set_justify(GTK_LABEL(main_info_extension_value), GTK_JUSTIFY_LEFT);

  gtk_grid_attach(GTK_GRID(main_info_grid), main_info_language_name_label,      0, 0, 1, 1) ;
  gtk_grid_attach(GTK_GRID(main_info_grid), main_info_language_value,           1, 0, 1, 1) ;
  gtk_grid_attach(GTK_GRID(main_info_grid), main_info_language_mimetype_label,  0, 1, 1, 1) ;
  gtk_grid_attach(GTK_GRID(main_info_grid), main_info_mimetype_value,           1, 1, 1, 1) ;
  gtk_grid_attach(GTK_GRID(main_info_grid), main_info_language_extension_label, 0, 2, 1, 1) ;
  gtk_grid_attach(GTK_GRID(main_info_grid), main_info_extension_value,          1, 2, 1, 1) ;


  gtk_box_pack_start(GTK_BOX(main_info_hbox), main_info_image,    FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(main_info_hbox), gtk_label_new(""),  TRUE,  TRUE,  0) ;
  gtk_box_pack_start(GTK_BOX(main_info_hbox), main_info_grid,     FALSE, FALSE, 0) ;

  gtk_container_add(GTK_CONTAINER(main_info_frame), main_info_hbox) ;



  GtkWidget *file_frame = gtk_frame_new( _(" File actions ") ) ;

  GtkWidget *file_action_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  GtkWidget *file_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  GtkWidget *file_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;


  GtkWidget *file_name_label = gtk_label_new( _("Filename") )   ;

  file_name_entry = gtk_entry_new() ;

  gtk_entry_set_text(GTK_ENTRY(file_name_entry),  file_basename) ;

  g_object_set(G_OBJECT(file_name_entry), "editable", FALSE, NULL) ;

  g_object_set(G_OBJECT(file_name_entry), "is-focus", FALSE, NULL) ;

  g_free(file_basename) ;

  gtk_box_pack_start(GTK_BOX(file_hbox), file_name_label, FALSE,  FALSE,  4) ;

  gtk_box_pack_start(GTK_BOX(file_hbox), file_name_entry, TRUE,   TRUE,   4) ;





  GtkWidget *file_button_rename   = gtk_button_new_with_label( _("Rename file") ) ;

  gtk_widget_set_tooltip_text(file_button_rename, _("Rename the file on disk to the selected new name.\nAttention: by renaming a file the original file is lost.\nAnd update the editor informations:\nthe content still the same only the name has changed.")) ;

  GtkWidget *file_button_copy     = gtk_button_new_with_label( _("Copy file") ) ;

  gtk_widget_set_tooltip_text(file_button_copy, _("Copy the file on disk to the selected new location.")) ;

  GtkWidget *file_button_compress = gtk_button_new_with_label( _("Compress file") ) ;

  gtk_widget_set_tooltip_text(file_button_compress, _("Compress the file on disk to the selected new location.\nUsing the gzip compressing.\nThis can be useful by finish writing man pages per example...")) ;

  GtkWidget *file_button_move_file = gtk_button_new_with_label( _("Move file") ) ;

  gtk_widget_set_tooltip_text(file_button_move_file, _("Move file to another location.")) ;



  gtk_box_pack_start(GTK_BOX(file_action_hbox), file_button_rename,                 TRUE, TRUE, 4) ;
  gtk_box_pack_start(GTK_BOX(file_action_hbox), file_button_copy,                   TRUE, TRUE, 4) ;
  gtk_box_pack_start(GTK_BOX(file_action_hbox), file_button_compress,               TRUE, TRUE, 4) ;
  gtk_box_pack_start(GTK_BOX(file_action_hbox), file_button_move_file,  TRUE, TRUE, 4) ;

  g_signal_connect(G_OBJECT(file_button_rename),    "clicked", G_CALLBACK(rename_file),               file_editor) ;
  g_signal_connect(G_OBJECT(file_button_copy),      "clicked", G_CALLBACK(copy_file),                 file_editor) ;
  g_signal_connect(G_OBJECT(file_button_compress),  "clicked", G_CALLBACK(compress_file),             file_editor) ;
  g_signal_connect(G_OBJECT(file_button_move_file), "clicked", G_CALLBACK(move_file),                 file_editor) ;





  gtk_box_pack_start(GTK_BOX(file_vbox), file_hbox,           TRUE,   TRUE, 4) ;
  gtk_box_pack_start(GTK_BOX(file_vbox), file_action_hbox,    TRUE,   TRUE, 4) ;


  GtkWidget *file_padding_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  gtk_container_set_border_width(GTK_CONTAINER(file_vbox), 4) ;

  gtk_container_add(GTK_CONTAINER(file_frame), file_vbox) ;

  gtk_box_pack_start(GTK_BOX(file_padding_box), file_frame, TRUE, TRUE, 4) ;



  gtk_box_pack_start(GTK_BOX(main_infos_and_file_actions_hbox), main_info_frame,     FALSE,  FALSE,  8) ;
  gtk_box_pack_start(GTK_BOX(main_infos_and_file_actions_hbox), file_padding_box,   TRUE,   TRUE,   8) ;

  gtk_box_pack_start(GTK_BOX(content_vbox), main_infos_and_file_actions_hbox, TRUE, TRUE, 8) ;




  GtkWidget *mode_count_time_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;


  GtkWidget *mode_frame = gtk_frame_new( _(" Mode ") ) ;

  GtkWidget *mode_grid  = gtk_grid_new() ;

  gtk_widget_set_tooltip_text(mode_grid, _("File permissions:\nThis table permit you to change the file permissions from the file,\ndirectly from inside it-edit.\nThe changes are immediately effective.")) ;

  gtk_grid_set_row_homogeneous(GTK_GRID(mode_grid),    TRUE) ;
  gtk_grid_set_column_homogeneous(GTK_GRID(mode_grid), TRUE) ;
  gtk_grid_set_row_spacing(GTK_GRID(mode_grid),           2) ;
  gtk_grid_set_column_spacing(GTK_GRID(mode_grid),        2) ;

  gtk_container_set_border_width(GTK_CONTAINER(mode_grid), 2) ;


  GtkWidget *r = gtk_label_new("R") ;
  GtkWidget *w = gtk_label_new("W") ;
  GtkWidget *x = gtk_label_new("X") ;

  GtkWidget *u = gtk_label_new("U") ;
  GtkWidget *g = gtk_label_new("G") ;
  GtkWidget *o = gtk_label_new("O") ;

  GtkWidget *ur = gtk_check_button_new() ;
  GtkWidget *uw = gtk_check_button_new() ;
  GtkWidget *ux = gtk_check_button_new() ;


  GtkWidget *gr = gtk_check_button_new() ;
  GtkWidget *gw = gtk_check_button_new() ;
  GtkWidget *gx = gtk_check_button_new() ;


  GtkWidget *or = gtk_check_button_new() ;
  GtkWidget *ow = gtk_check_button_new() ;
  GtkWidget *ox = gtk_check_button_new() ;

  gtk_widget_set_margin_start(ur, 4);
  gtk_widget_set_margin_start(uw, 4);
  gtk_widget_set_margin_start(ux, 4);

  gtk_widget_set_margin_start(gr, 4);
  gtk_widget_set_margin_start(gw, 4);
  gtk_widget_set_margin_start(gx, 4);

  gtk_widget_set_margin_start(or, 4);
  gtk_widget_set_margin_start(ow, 4);
  gtk_widget_set_margin_start(ox, 4);


  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ur), USR_R(file_editor->file_info.st_mode)) ;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(uw), USR_W(file_editor->file_info.st_mode)) ;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ux), USR_X(file_editor->file_info.st_mode)) ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gr), GRP_R(file_editor->file_info.st_mode)) ;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gw), GRP_W(file_editor->file_info.st_mode)) ;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gx), GRP_X(file_editor->file_info.st_mode)) ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(or), OTH_R(file_editor->file_info.st_mode)) ;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ow), OTH_W(file_editor->file_info.st_mode)) ;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ox), OTH_X(file_editor->file_info.st_mode)) ;


  gtk_grid_attach(GTK_GRID(mode_grid), r, 1, 0, 1, 1) ;
  gtk_grid_attach(GTK_GRID(mode_grid), w, 2, 0, 1, 1) ;
  gtk_grid_attach(GTK_GRID(mode_grid), x, 3, 0, 1, 1) ;

  gtk_grid_attach(GTK_GRID(mode_grid), u, 0, 1, 1, 1) ;
  gtk_grid_attach(GTK_GRID(mode_grid), g, 0, 2, 1, 1) ;
  gtk_grid_attach(GTK_GRID(mode_grid), o, 0, 3, 1, 1) ;

  gtk_grid_attach(GTK_GRID(mode_grid), ur, 1, 1, 1, 1) ;
  gtk_grid_attach(GTK_GRID(mode_grid), uw, 2, 1, 1, 1) ;
  gtk_grid_attach(GTK_GRID(mode_grid), ux, 3, 1, 1, 1) ;

  gtk_grid_attach(GTK_GRID(mode_grid), gr, 1, 2, 1, 1) ;
  gtk_grid_attach(GTK_GRID(mode_grid), gw, 2, 2, 1, 1) ;
  gtk_grid_attach(GTK_GRID(mode_grid), gx, 3, 2, 1, 1) ;

  gtk_grid_attach(GTK_GRID(mode_grid), or, 1, 3, 1, 1) ;
  gtk_grid_attach(GTK_GRID(mode_grid), ow, 2, 3, 1, 1) ;
  gtk_grid_attach(GTK_GRID(mode_grid), ox, 3, 3, 1, 1) ;


  gtk_widget_set_name(ur, "ur") ;
  gtk_widget_set_name(uw, "uw") ;
  gtk_widget_set_name(ux, "ux") ;
  gtk_widget_set_name(gr, "gr") ;
  gtk_widget_set_name(gw, "gw") ;
  gtk_widget_set_name(gx, "gx") ;
  gtk_widget_set_name(or, "or") ;
  gtk_widget_set_name(ow, "ow") ;
  gtk_widget_set_name(ox, "ox") ;


  g_signal_connect(G_OBJECT(ur), "toggled", G_CALLBACK(toggle_mode_bit), file_editor) ;
  g_signal_connect(G_OBJECT(uw), "toggled", G_CALLBACK(toggle_mode_bit), file_editor) ;
  g_signal_connect(G_OBJECT(ux), "toggled", G_CALLBACK(toggle_mode_bit), file_editor) ;

  g_signal_connect(G_OBJECT(gr), "toggled", G_CALLBACK(toggle_mode_bit), file_editor) ;
  g_signal_connect(G_OBJECT(gw), "toggled", G_CALLBACK(toggle_mode_bit), file_editor) ;
  g_signal_connect(G_OBJECT(gx), "toggled", G_CALLBACK(toggle_mode_bit), file_editor) ;

  g_signal_connect(G_OBJECT(or), "toggled", G_CALLBACK(toggle_mode_bit), file_editor) ;
  g_signal_connect(G_OBJECT(ow), "toggled", G_CALLBACK(toggle_mode_bit), file_editor) ;
  g_signal_connect(G_OBJECT(ox), "toggled", G_CALLBACK(toggle_mode_bit), file_editor) ;


  gtk_container_set_border_width(GTK_CONTAINER(mode_grid), 4) ;

  gtk_container_add(GTK_CONTAINER(mode_frame), mode_grid) ;


  GtkWidget *count_frame = gtk_frame_new( _(" File counts ") ) ;

  GtkWidget *count_grid  = gtk_grid_new() ;

  gtk_grid_set_row_homogeneous(GTK_GRID(count_grid),    TRUE) ;
  gtk_grid_set_column_homogeneous(GTK_GRID(count_grid), TRUE) ;
  gtk_grid_set_row_spacing(GTK_GRID(count_grid),           2) ;
  gtk_grid_set_column_spacing(GTK_GRID(count_grid),        2) ;


  GtkWidget *line_count_label = gtk_label_new( _("Lines count") ) ;
  const gchar *line_count_tooltip_text = _("Lines count of the file on disk. Not from the current buffer.\nTo get the line count of the current buffer, look at the bottom right of the editor.") ;
  gtk_widget_set_tooltip_text(line_count_label, line_count_tooltip_text) ;

  GtkWidget *char_count_label = gtk_label_new( _("Chars count") ) ;
  const gchar *char_count_tooltip_text = _("Character count of the file on disk. Not from the current buffer.\nTo get the character count of the current buffer, look at the bottom right of the editor.") ;
  gtk_widget_set_tooltip_text(char_count_label, char_count_tooltip_text) ;

  GtkWidget *word_count_label = gtk_label_new( _("Words count") ) ;
  const gchar *word_count_tooltip_text = _("Words count of the file on disk. Not from the current buffer.") ;
  gtk_widget_set_tooltip_text(word_count_label, word_count_tooltip_text) ;







  GtkWidget *line_count_entry = gtk_entry_new() ;
  gtk_widget_set_tooltip_text(line_count_entry, line_count_tooltip_text) ;

  char *line_count = g_get_wc(filepath, "-l") ;

  if (line_count == NULL) {

    display_message_dialog( _("Warning error file !"), _("You surely ask for informations about a not existing file on disk:\n"
                                                         "A \"New\" buffer.\n"
                                                         "Which file-path is delete after creation, into your $TEMP folder.\n"
                                                         "\n"
                                                         "Take a look at the notice for further informations about \"New\" files.\n"
                                                         "\n"
                                                         "Try to save it, then retry to ask informations."),
                                                          GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE) ;

  }

  gtk_entry_set_text(GTK_ENTRY(line_count_entry), (line_count != NULL) ? line_count : "0") ;

  free(line_count) ;

  GtkWidget *char_count_entry = gtk_entry_new() ;
  gtk_widget_set_tooltip_text(char_count_entry, char_count_tooltip_text) ;
  char *char_count = g_get_wc(filepath, "-m") ;
  gtk_entry_set_text(GTK_ENTRY(char_count_entry), (char_count != NULL) ? char_count : "0") ;

  free(char_count) ;

  GtkWidget *word_count_entry = gtk_entry_new() ;
  gtk_widget_set_tooltip_text(word_count_entry, word_count_tooltip_text) ;
  char *word_count = g_get_wc(filepath, "-w") ;
  gtk_entry_set_text(GTK_ENTRY(word_count_entry), (word_count != NULL) ? word_count : "0") ;

  free(word_count) ;

  g_object_set(G_OBJECT(line_count_entry), "editable", FALSE, NULL) ;
  g_object_set(G_OBJECT(char_count_entry), "editable", FALSE, NULL) ;
  g_object_set(G_OBJECT(word_count_entry), "editable", FALSE, NULL) ;



  gtk_grid_attach(GTK_GRID(count_grid), line_count_label, 0, 0, 1, 1) ;
  gtk_grid_attach(GTK_GRID(count_grid), line_count_entry, 1, 0, 1, 1) ;

  gtk_grid_attach(GTK_GRID(count_grid), char_count_label, 0, 1, 1, 1) ;
  gtk_grid_attach(GTK_GRID(count_grid), char_count_entry, 1, 1, 1, 1) ;

  gtk_grid_attach(GTK_GRID(count_grid), word_count_label, 0, 2, 1, 1) ;
  gtk_grid_attach(GTK_GRID(count_grid), word_count_entry, 1, 2, 1, 1) ;


  gtk_container_set_border_width(GTK_CONTAINER(count_grid), 4) ;

  gtk_container_add(GTK_CONTAINER(count_frame), count_grid) ;


  GtkWidget *time_frame = gtk_frame_new( _(" Timestamps ") ) ;

  GtkWidget *time_label_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0)   ;
  GtkWidget *time_entry_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0)   ;
  GtkWidget *time_button_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0)   ;

  GtkWidget *time_hbox        = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  gtk_widget_set_tooltip_text(time_hbox, _("Display time stamp informations about the file on disk.\n\nYou can change the time stamp from the:\n* \"Last access time\".\n* \"Last modification time\".\nThe changes are immediately effective !\n\nThe \"Modify\" button from the \"Last status change time\" is\nonly for consistence of the G.U.I and does nothing.")) ;

  GDateTime *datetime_st_ctime = g_date_time_new_from_unix_local(file_editor->file_info.st_ctime) ;
  GDateTime *datetime_st_atime = g_date_time_new_from_unix_local(file_editor->file_info.st_atime) ;
  GDateTime *datetime_st_mtime = g_date_time_new_from_unix_local(file_editor->file_info.st_mtime) ;


  GtkWidget *ctime_label = gtk_label_new( _("Last status change time") ) ;
  GtkWidget *atime_label = gtk_label_new( _("Last access time") ) ;
  GtkWidget *mtime_label = gtk_label_new( _("Last modification time") ) ;

  g_object_set(G_OBJECT(ctime_label),    "margin-start", 4, NULL) ;
  g_object_set(G_OBJECT(ctime_label),    "margin-end",   4, NULL) ;

  g_object_set(G_OBJECT(atime_label),    "margin-start", 4, NULL) ;
  g_object_set(G_OBJECT(atime_label),    "margin-end",   4, NULL) ;

  g_object_set(G_OBJECT(mtime_label),    "margin-start", 4, NULL) ;
  g_object_set(G_OBJECT(mtime_label),    "margin-end",   4, NULL) ;


  ctime_entry = gtk_entry_new() ;
  atime_entry = gtk_entry_new() ;
  mtime_entry = gtk_entry_new() ;


  GtkWidget *ctime_change_button = gtk_button_new_with_label( _("  Modify ") ) ;
  GtkWidget *atime_change_button = gtk_button_new_with_label( _("  Modify ") ) ;
  GtkWidget *mtime_change_button = gtk_button_new_with_label( _("  Modify ") ) ;

  gtk_widget_set_name(ctime_change_button, "ctime") ;
  gtk_widget_set_name(atime_change_button, "atime") ;
  gtk_widget_set_name(mtime_change_button, "mtime") ;


  g_signal_connect(G_OBJECT(ctime_change_button), "clicked", G_CALLBACK(generate_calendar_and_timer), &file_editor->file_info) ;
  g_signal_connect(G_OBJECT(atime_change_button), "clicked", G_CALLBACK(generate_calendar_and_timer), &file_editor->file_info) ;
  g_signal_connect(G_OBJECT(mtime_change_button), "clicked", G_CALLBACK(generate_calendar_and_timer), &file_editor->file_info) ;




  gtk_entry_set_text(GTK_ENTRY(ctime_entry), g_date_time_format(datetime_st_ctime, "%F %T")) ;
  gtk_entry_set_text(GTK_ENTRY(atime_entry), g_date_time_format(datetime_st_atime, "%F %T")) ;
  gtk_entry_set_text(GTK_ENTRY(mtime_entry), g_date_time_format(datetime_st_mtime, "%F %T")) ;

  g_date_time_unref(datetime_st_ctime) ;
  g_date_time_unref(datetime_st_atime) ;
  g_date_time_unref(datetime_st_mtime) ;



  g_object_set(G_OBJECT(ctime_entry), "editable", FALSE, NULL) ;
  g_object_set(G_OBJECT(atime_entry), "editable", FALSE, NULL) ;
  g_object_set(G_OBJECT(mtime_entry), "editable", FALSE, NULL) ;

  gtk_box_pack_start(GTK_BOX(time_label_vbox), ctime_label, TRUE, TRUE, 2) ;
  gtk_box_pack_start(GTK_BOX(time_label_vbox), atime_label, TRUE, TRUE, 2) ;
  gtk_box_pack_start(GTK_BOX(time_label_vbox), mtime_label, TRUE, TRUE, 2) ;

  gtk_box_pack_start(GTK_BOX(time_entry_vbox), ctime_entry, TRUE, TRUE, 2) ;
  gtk_box_pack_start(GTK_BOX(time_entry_vbox), atime_entry, TRUE, TRUE, 2) ;
  gtk_box_pack_start(GTK_BOX(time_entry_vbox), mtime_entry, TRUE, TRUE, 2) ;

  gtk_box_pack_start(GTK_BOX(time_button_vbox), ctime_change_button, TRUE, TRUE, 2) ;
  gtk_box_pack_start(GTK_BOX(time_button_vbox), atime_change_button, TRUE, TRUE, 2) ;
  gtk_box_pack_start(GTK_BOX(time_button_vbox), mtime_change_button, TRUE, TRUE, 2) ;


  gtk_box_pack_start(GTK_BOX(time_hbox), time_label_vbox,  FALSE,  FALSE, 4) ;
  gtk_box_pack_start(GTK_BOX(time_hbox), time_entry_vbox,  TRUE,   TRUE,  4) ;
  gtk_box_pack_start(GTK_BOX(time_hbox), time_button_vbox, FALSE,  FALSE, 4) ;



  gtk_container_add(GTK_CONTAINER(time_frame), time_hbox) ;

  GtkWidget *mode_count_time_padding_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  gtk_box_pack_start(GTK_BOX(mode_count_time_hbox), mode_frame,   TRUE, TRUE, 4) ;
  gtk_box_pack_start(GTK_BOX(mode_count_time_hbox), count_frame,  TRUE, TRUE, 4) ;
  gtk_box_pack_start(GTK_BOX(mode_count_time_hbox), time_frame,   TRUE, TRUE, 4) ;

  gtk_box_pack_start(GTK_BOX(mode_count_time_padding_hbox), mode_count_time_hbox, TRUE, TRUE, 4) ;

  gtk_box_pack_start(GTK_BOX(content_vbox), mode_count_time_padding_hbox, TRUE, TRUE, 4) ;





  GtkWidget *chksum_frame = gtk_frame_new( _(" Checksum ") ) ;

  GtkWidget *chksum_label_vbox    = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;


  GtkWidget *chksum_entry_vbox    = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;


  GtkWidget *chksum_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,   0) ;

  GtkWidget *chksum_md5_label    = gtk_label_new("MD5    ")    ;
  GtkWidget *chksum_sha1_label   = gtk_label_new("SHA1   ")   ;
  GtkWidget *chksum_sha256_label = gtk_label_new("SHA256 ") ;
  GtkWidget *chksum_sha512_label = gtk_label_new("SHA512 ") ;


  g_object_set(G_OBJECT(chksum_md5_label),    "margin-start", 4, NULL) ;
  g_object_set(G_OBJECT(chksum_md5_label),    "margin-end",   4, NULL) ;
  g_object_set(G_OBJECT(chksum_sha1_label),   "margin-start", 4, NULL) ;
  g_object_set(G_OBJECT(chksum_sha1_label),   "margin-end",   4, NULL) ;
  g_object_set(G_OBJECT(chksum_sha256_label), "margin-start", 4, NULL) ;
  g_object_set(G_OBJECT(chksum_sha256_label), "margin-end",   4, NULL) ;
  g_object_set(G_OBJECT(chksum_sha512_label), "margin-start", 4, NULL) ;
  g_object_set(G_OBJECT(chksum_sha512_label), "margin-end",   4, NULL) ;

  GtkWidget *chksum_md5_entry     = gtk_entry_new() ;
  GtkWidget *chksum_sha1_entry    = gtk_entry_new() ;
  GtkWidget *chksum_sha256_entry  = gtk_entry_new() ;
  GtkWidget *chksum_sha512_entry  = gtk_entry_new() ;


  file_size = 0 ;

  gchar *file_content = NULL ;

  g_file_get_contents(filepath, &file_content, &file_size, NULL) ;


  gtk_entry_set_text(GTK_ENTRY(chksum_md5_entry),    g_compute_checksum_for_string(G_CHECKSUM_MD5,    file_content, (gssize) file_size)) ;
  gtk_entry_set_text(GTK_ENTRY(chksum_sha1_entry),   g_compute_checksum_for_string(G_CHECKSUM_SHA1,   file_content, (gssize) file_size)) ;
  gtk_entry_set_text(GTK_ENTRY(chksum_sha256_entry), g_compute_checksum_for_string(G_CHECKSUM_SHA256, file_content, (gssize) file_size)) ;
  gtk_entry_set_text(GTK_ENTRY(chksum_sha512_entry), g_compute_checksum_for_string(G_CHECKSUM_SHA512, file_content, (gssize) file_size)) ;

  g_object_set(G_OBJECT(chksum_md5_entry),    "editable", FALSE, NULL) ;
  g_object_set(G_OBJECT(chksum_sha1_entry),   "editable", FALSE, NULL) ;
  g_object_set(G_OBJECT(chksum_sha256_entry), "editable", FALSE, NULL) ;
  g_object_set(G_OBJECT(chksum_sha512_entry), "editable", FALSE, NULL) ;

  g_free(file_content) ;

  gtk_box_pack_start(GTK_BOX(chksum_label_vbox), chksum_md5_label,    TRUE, TRUE,   0) ;
  gtk_box_pack_start(GTK_BOX(chksum_label_vbox), chksum_sha1_label,   TRUE, TRUE,   0) ;
  gtk_box_pack_start(GTK_BOX(chksum_label_vbox), chksum_sha256_label, TRUE, TRUE,   0) ;
  gtk_box_pack_start(GTK_BOX(chksum_label_vbox), chksum_sha512_label, TRUE, TRUE,   0) ;


  gtk_box_pack_start(GTK_BOX(chksum_entry_vbox), chksum_md5_entry,    TRUE, TRUE,   0) ;
  gtk_box_pack_start(GTK_BOX(chksum_entry_vbox), chksum_sha1_entry,   TRUE, TRUE,   0) ;
  gtk_box_pack_start(GTK_BOX(chksum_entry_vbox), chksum_sha256_entry, TRUE, TRUE,   0) ;
  gtk_box_pack_start(GTK_BOX(chksum_entry_vbox), chksum_sha512_entry, TRUE, TRUE,   0) ;


  gtk_box_pack_start(GTK_BOX(chksum_hbox), chksum_label_vbox,         FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(chksum_hbox), chksum_entry_vbox,         TRUE, TRUE,   0) ;


  gtk_container_set_border_width(GTK_CONTAINER(chksum_hbox), 4) ;

  gtk_container_add(GTK_CONTAINER(chksum_frame), chksum_hbox) ;

  GtkWidget *chksum_padding_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  gtk_box_pack_start(GTK_BOX(chksum_padding_box), chksum_frame, TRUE, TRUE, 4) ;

  gtk_box_pack_start(GTK_BOX(content_vbox), chksum_padding_box, TRUE, TRUE, 4) ;



  gtk_widget_show_all(file_informations_dialog) ;

  gtk_dialog_run( GTK_DIALOG(file_informations_dialog) ) ;

  gtk_widget_destroy(file_informations_dialog) ;


  return ;

}



#ifdef GSPELL_SUPPORT
void select_spellcheck_language(GtkWidget *widget) {

  GtkWidget *spellcheck_language_dialog = gspell_language_chooser_dialog_new(GTK_WINDOW(gui->main_window), gspell_language_lookup(settings.language_code), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT) ;

  gtk_dialog_run(GTK_DIALOG(spellcheck_language_dialog)) ;

  if (gspell_language_chooser_get_language_code(GSPELL_LANGUAGE_CHOOSER(spellcheck_language_dialog)) != NULL) {

      g_free(settings.language_code) ;

      /** It cannot be NULL as say in the documentation ! **/
      settings.language_code = g_strdup(gspell_language_chooser_get_language_code(GSPELL_LANGUAGE_CHOOSER(spellcheck_language_dialog))) ;

      GKeyFile *conf_file = g_key_file_new() ;

      GError *error = NULL ;

      g_key_file_load_from_file(conf_file, PATH_TO_CONF_FILE, G_KEY_FILE_NONE, &error) ;

      if (error != NULL) {

        display_message_dialog( _("Error open configuration file !"), error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

        g_error_free(error) ;

        return ;

      }

      g_key_file_set_string(conf_file,   "GUI",      "language_code",         settings.language_code) ;

      g_key_file_save_to_file(conf_file, PATH_TO_CONF_FILE, &error);

      if (error != NULL) {

        display_message_dialog( _("Error saving configuration file !"), error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

        g_error_free(error) ;

      }

      g_key_file_unref(conf_file) ;

  }


  gtk_widget_destroy(spellcheck_language_dialog) ;

  return ;

}
#endif


void activate_replace_all_in_all_files(GtkWidget *widget) {

  /** TODO: Checking the selection if any !!! **/

  recheck :

  gui->replacing_in_all_files->window = gtk_dialog_new_with_buttons( _("Replace in all files"), GTK_WINDOW(gui->main_window), GTK_DIALOG_USE_HEADER_BAR, NULL, NULL) ;

  GtkWidget *cancel_button = gtk_dialog_add_button(GTK_DIALOG(gui->replacing_in_all_files->window), _("Cancel"), GTK_RESPONSE_CANCEL) ;

  GtkWidget *replace_button = gtk_dialog_add_button(GTK_DIALOG(gui->replacing_in_all_files->window), _("Replace"), GTK_RESPONSE_APPLY) ;

  /** Apply the Return key shortcut to the Apply button: **/
  GtkAccelGroup *accel_group = gtk_accel_group_new();

  gtk_widget_add_accelerator(replace_button,
                            "activate",
                            accel_group,
                            GDK_KEY_Return,
                            0,
                            GTK_ACCEL_VISIBLE);

  gtk_widget_add_accelerator(cancel_button,
                            "activate",
                            accel_group,
                            GDK_KEY_Escape,
                            0,
                            GTK_ACCEL_VISIBLE);


  gtk_window_add_accel_group(GTK_WINDOW(gui->replacing_in_all_files->window), accel_group) ;

  GtkWidget *pattern_label_button = gtk_button_new_with_label(_(" Pattern ")) ;

  GtkWidget *pattern_image = gtk_image_new_from_file( PATH_TO_BUTTON_ICON "system-search.png") ;

  gtk_button_set_image( GTK_BUTTON(pattern_label_button), pattern_image) ;

  g_object_set(G_OBJECT(pattern_label_button),"always-show-image",TRUE,NULL) ;


  GtkWidget *replace_label_button = gtk_button_new_with_label(_(" Replace")) ;

  GtkWidget *replace_image = gtk_image_new_from_file( PATH_TO_BUTTON_ICON "edit-find-replace.png") ;

  gtk_button_set_image( GTK_BUTTON(replace_label_button), replace_image) ;

  g_object_set(G_OBJECT(replace_label_button),"always-show-image",TRUE,NULL) ;


  GtkWidget *mode_label_button = gtk_button_new_with_label(_(" Mode")) ;

  GtkWidget *mode_image = gtk_image_new_from_file( PATH_TO_BUTTON_ICON "preferences-system.png") ;

  gtk_button_set_image( GTK_BUTTON(mode_label_button), mode_image) ;

  g_object_set(G_OBJECT(mode_label_button),"always-show-image",TRUE,NULL) ;


  GtkWidget *label_buttons_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_box_pack_start(GTK_BOX(label_buttons_vbox), pattern_label_button, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(label_buttons_vbox), replace_label_button, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(label_buttons_vbox), mode_label_button,    FALSE, FALSE, 0) ;




  gui->replacing_in_all_files->pattern_entry = gtk_search_entry_new() ;

  gboolean has_selection = FALSE ;
  g_object_get(G_OBJECT(current_editor.current_buffer), "has-selection", &has_selection, NULL) ;

  if (has_selection) {

    GtkTextIter start_selection  ;
    GtkTextIter end_selection    ;

    /** Getting selection iterators **/
    gtk_text_buffer_get_selection_bounds(GTK_TEXT_BUFFER(current_editor.current_buffer), &start_selection, &end_selection);

    /** Getting selected text **/
    gchar *selection_text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(current_editor.current_buffer), &start_selection, &end_selection, TRUE);

    gtk_entry_set_text(GTK_ENTRY(gui->replacing_in_all_files->pattern_entry), selection_text) ;

    g_free(selection_text) ;

  }
  else {

    gtk_window_set_focus(GTK_WINDOW(gui->replacing_in_all_files->window), gui->replacing_in_all_files->pattern_entry) ;

  }


  gui->replacing_in_all_files->replace_entry = gtk_entry_new() ;


  gui->replacing_in_all_files->mode_combobox = gtk_combo_box_text_new_with_entry() ;

  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gui->replacing_in_all_files->mode_combobox), _("Raw text")) ;
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gui->replacing_in_all_files->mode_combobox), _("Word boundary")) ;
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gui->replacing_in_all_files->mode_combobox), _("Regular expression")) ;

  gtk_combo_box_set_active(GTK_COMBO_BOX(gui->replacing_in_all_files->mode_combobox), 0) ;

  gui->replacing_in_all_files->case_sensitiv = gtk_check_button_new_with_label( _("Case-sensitive")) ;

  gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(gui->replacing_in_all_files->case_sensitiv), FALSE) ;


  GtkWidget *settings_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  gtk_box_pack_start(GTK_BOX(settings_hbox), gui->replacing_in_all_files->mode_combobox, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(settings_hbox), gui->replacing_in_all_files->case_sensitiv, FALSE, FALSE, 0) ;


  GtkWidget *data_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_box_pack_start(GTK_BOX(data_vbox), gui->replacing_in_all_files->pattern_entry, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(data_vbox), gui->replacing_in_all_files->replace_entry, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(data_vbox), settings_hbox,                              FALSE, FALSE, 0) ;

  GtkWidget *main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  gtk_box_pack_start(GTK_BOX(main_hbox), label_buttons_vbox, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(main_hbox), data_vbox, FALSE, FALSE, 0) ;



  GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(gui->replacing_in_all_files->window)) ;

  gtk_box_pack_start(GTK_BOX(content_area), main_hbox,           FALSE, FALSE, 0) ;


  gtk_widget_show_all(gui->replacing_in_all_files->window) ;

  gint result = gtk_dialog_run(GTK_DIALOG(gui->replacing_in_all_files->window)) ;

  switch (result) {

    case GTK_RESPONSE_APPLY : {

      guint counter = replace_all_occurrence_in_all_files() ;

      if (counter > 0) {

        display_message_dialog( _("Replacement"), g_strdup_printf( _("%u occurrences successful replaced\nin all open file(s)."), counter), GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE) ;

      }

      gtk_widget_destroy(gui->replacing_in_all_files->window) ;


      goto recheck ;

      break;

    }

    case GTK_RESPONSE_CANCEL :

       gtk_widget_destroy(gui->replacing_in_all_files->window) ;
       break;
  }


  return ;

}

gboolean test(GtkWidget *window) {

  static bool destroying = false ;

  if (destroying) {

    g_usleep(10000000) ;

    gtk_widget_destroy(window) ;

    destroying = false ;

    return G_SOURCE_REMOVE ;

  }

  gtk_widget_set_name(window, "window_tooltip") ;

  GtkCssProvider *provider = gtk_css_provider_new();

  GdkDisplay *display = gdk_display_get_default() ;
  GdkScreen *screen = gdk_display_get_default_screen(display);

  gtk_style_context_add_provider_for_screen(screen,
                                           GTK_STYLE_PROVIDER (provider),
                                           GTK_STYLE_PROVIDER_PRIORITY_USER) ; //GTK_STYLE_PROVIDER_PRIORITY_APPLICATION

  gtk_css_provider_load_from_data(provider,
                                   "#window_tooltip {\n"
                                   "color : rgba(84%,84%,84%,0.96);\n"
                                   "background-color: rgba(32%,32%,32%,0.64);\n"
                                   "border: 8px groove black ;\n"
                                   "}\n"
                                    , -1, NULL);
  g_object_unref(provider);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS) ;

  gtk_window_set_decorated(GTK_WINDOW(window), FALSE) ;

  GDateTime *now = g_date_time_new_now_local();

  GTimeSpan it_edit_uptime_ms = g_date_time_difference(now, program_time);

  GTimeVal *tv = g_malloc(sizeof(GTimeVal)) ;

  tv->tv_sec = 0 ;
  tv->tv_usec = 0 ;

  GDateTime *tmp = g_date_time_new_from_timeval_local(tv);

  g_free(tv) ;

  GDateTime *it_edit_uptime = g_date_time_add(tmp, it_edit_uptime_ms);

  g_date_time_unref(tmp) ;

  it_edit_uptime = g_date_time_add_hours(it_edit_uptime, -1) ;

  gchar *now_str_fmt = g_date_time_format(now, "%F %T") ;




  g_date_time_unref(now) ;

  const gchar *user_name = g_get_real_name() ;

  gchar *user_salutation = g_strdup_printf( _("Hi %s (<b>it-edit</b> user).\n\nCurrent date-time: <b>%s</b>  \n\nYou are working since: <b>%02dh-%02dm-%02ds</b>"), user_name, now_str_fmt, g_date_time_get_hour(it_edit_uptime), g_date_time_get_minute(it_edit_uptime), g_date_time_get_second(it_edit_uptime) ) ;

  GNotification *notification = g_notification_new( g_get_application_name() ) ;

  g_notification_set_title(notification, "it-edit uptime !") ;

  g_notification_set_default_action(notification, "app.actions") ;

  gchar *it_edit_uptime_str = g_strdup_printf(_("You are working since: %02dh-%02dm-%02ds"),g_date_time_get_hour(it_edit_uptime), g_date_time_get_minute(it_edit_uptime), g_date_time_get_second(it_edit_uptime) ) ;

  g_notification_set_body(notification, it_edit_uptime_str) ;

  g_notification_set_priority(notification, G_NOTIFICATION_PRIORITY_NORMAL) ;

  g_application_send_notification(G_APPLICATION(app), "at_it_edit_startup", notification);

  g_free(it_edit_uptime_str) ;

  g_date_time_unref(it_edit_uptime) ;

  g_free(now_str_fmt) ;

  GtkWidget *label_salutation = gtk_label_new(NULL) ;

  gtk_label_set_markup(GTK_LABEL(label_salutation), user_salutation);

  g_free(user_salutation) ;

  GtkWidget *image = gtk_image_new_from_file(PATH_TO_UPTIME_ICON) ;

  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8) ;

  gtk_box_pack_start(GTK_BOX(hbox), image, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(hbox), label_salutation, FALSE, FALSE, 0) ;

  gtk_container_add(GTK_CONTAINER(window), hbox) ;

  gtk_container_set_border_width(GTK_CONTAINER(window), 8) ;

  gtk_widget_show_all(window) ;

  destroying = true ;

  return G_SOURCE_CONTINUE ;

}

void user_salutation(GtkWidget *widget) {

  GtkWidget *window = gtk_window_new(GTK_WINDOW_POPUP) ;

  g_timeout_add(100, (GSourceFunc) test, window) ;

  return ;

}





