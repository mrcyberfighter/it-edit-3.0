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

#include "./configuration_dialog.h"

static void add_a_new_item_to_files_manager(GtkWidget *widget, GtkWidget *files_manager_vbox) ;

static void is_a_new_color_choosen(GtkColorButton *widget, bool *confirm) ;

static void remove_selected_items_from_files_manager(GtkWidget *widget, GtkWidget *files_manager_vbox) ;

static void add_a_new_item_to_applications(GtkWidget *widget, gpointer user_data) ;

static void remove_selected_items_from_applications(GtkWidget *widget, GtkWidget *files_manager_vbox) ;

static void check_user_shell(GtkFileChooserButton *chooser) ;

static GtkWidget *generate_applications_chooser(GtkWidget *hbox, const char *icon_filepath, const char *label_text, char *app_filepath) ;

static GtkWidget *generate_checkbutton(const gchar *label, gboolean is_active, const gchar *tooltip_text) ;

static GtkWidget *generate_perm_grid(GtkWidget *grid, const gchar *text, const guint8 row) ;

static GtkWidget *generate_frame_with_content(const gchar *text, GtkWidget *widget) ;

static void set_applications(char *app, GtkWidget *file_chooser) ;

static void set_session_mode(GtkWidget *widget, gint *value) ;

static void change_applications(char *app_name, GtkWidget *menu_item) ;

static void set_as_default(GtkWidget *widget, gpointer user_data) ;

static void update_sidebar_position(GtkSpinButton *spin_button) ;

static void reset_default(GtkWidget *widget, gpointer user_data) ;

static gboolean is_perm_value_set(const gint mask, const gint bit)  ;

static void set_perm_value(GtkWidget *widget, gint *mask,  gint value) ;

static void editor_font_toggled_use_set_state(GtkWidget *toggle_button, GtkWidget *widget) ;

static void use_right_margin_toggled_state(GtkWidget *toggle_button, GtkWidget *widget) ;

static void add_a_new_item_to_files_manager(GtkWidget *widget, GtkWidget *files_manager_vbox) {
  /** Display a dialog to add a new entry to the "File Handler" fubnctionnality. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *add_a_file_to_files_manager_dialog  = gtk_dialog_new_with_buttons( _("Add a new file to files manager"),
                                                                                GTK_WINDOW(gui->main_window),
                                                                                GTK_DIALOG_USE_HEADER_BAR | GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                                                _("Add file"),
                                                                                GTK_RESPONSE_OK,
                                                                                _("Cancel"),
                                                                                GTK_RESPONSE_CANCEL,
                                                                                NULL);

  gtk_window_set_transient_for(GTK_WINDOW(add_a_file_to_files_manager_dialog), GTK_WINDOW(gui->main_window)) ;

  gtk_window_set_title(GTK_WINDOW(add_a_file_to_files_manager_dialog), _("Adding a file") );

  gtk_window_set_resizable(GTK_WINDOW(add_a_file_to_files_manager_dialog), FALSE) ;

  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(add_a_file_to_files_manager_dialog), PATH_TO_DOC_FOLDER) ;

  gtk_container_set_border_width(GTK_CONTAINER(add_a_file_to_files_manager_dialog), 15) ;

  GtkWidget *new_entry_title_label = gtk_label_new( _("Title") ) ;

  GtkWidget *new_entry_file_label  = gtk_label_new( _("Files") )  ;

  GtkWidget *new_entry_title_entry = gtk_entry_new() ;

  gtk_entry_set_width_chars(GTK_ENTRY(new_entry_title_entry), 35) ;
  gtk_entry_set_max_length(GTK_ENTRY(new_entry_title_entry),  35) ;

  GtkWidget *new_entry_file_button = gtk_file_chooser_button_new(NULL, GTK_FILE_CHOOSER_ACTION_OPEN);

  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(new_entry_file_button), g_get_home_dir() );

  GtkWidget *main_add_file_hbox    = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5) ;

  GtkWidget *new_entry_title_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL,   5) ;

  GtkWidget *new_entry_data_vbox   = gtk_box_new(GTK_ORIENTATION_VERTICAL,   5) ;

  gtk_box_set_homogeneous(GTK_BOX(new_entry_title_vbox), TRUE) ;
  gtk_box_set_homogeneous(GTK_BOX(new_entry_data_vbox),  TRUE) ;

  gtk_box_pack_start(GTK_BOX(new_entry_title_vbox), new_entry_title_label,  FALSE, FALSE,0) ;
  gtk_box_pack_start(GTK_BOX(new_entry_title_vbox), new_entry_file_label,   FALSE, FALSE,0) ;

  gtk_box_pack_start(GTK_BOX(new_entry_data_vbox), new_entry_title_entry, FALSE, FALSE,0) ;
  gtk_box_pack_start(GTK_BOX(new_entry_data_vbox), new_entry_file_button, FALSE, FALSE,0) ;

  gtk_box_pack_start(GTK_BOX(main_add_file_hbox), new_entry_title_vbox, FALSE, FALSE,0) ;
  gtk_box_pack_start(GTK_BOX(main_add_file_hbox), new_entry_data_vbox,  FALSE, FALSE,0) ;

  gtk_box_set_spacing(GTK_BOX(new_entry_title_vbox),  5) ;
  gtk_box_set_spacing(GTK_BOX(new_entry_data_vbox),   5) ;
  gtk_box_set_spacing(GTK_BOX(main_add_file_hbox),    5) ;

  gtk_container_set_border_width(GTK_CONTAINER(new_entry_title_vbox), 5) ;
  gtk_container_set_border_width(GTK_CONTAINER(new_entry_data_vbox), 5) ;
  gtk_container_set_border_width(GTK_CONTAINER(main_add_file_hbox), 5) ;

  GtkWidget *dialog_content_area = gtk_dialog_get_content_area(GTK_DIALOG(add_a_file_to_files_manager_dialog)) ;

  GtkWidget *add_file_button = gtk_dialog_get_widget_for_response (GTK_DIALOG(add_a_file_to_files_manager_dialog), GTK_RESPONSE_OK) ;

  GtkWidget *add_file_image  = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "list-add.png");

  g_object_set(G_OBJECT(add_file_button), "always-show-image", TRUE, NULL ) ;

  gtk_button_set_image(GTK_BUTTON(add_file_button), add_file_image);

  GtkWidget *cancel_button   = gtk_dialog_get_widget_for_response (GTK_DIALOG(add_a_file_to_files_manager_dialog), GTK_RESPONSE_CANCEL) ;

  GtkWidget *cancel_image  = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "dialog-cancel.png");

  g_object_set(G_OBJECT(cancel_button), "always-show-image", TRUE, NULL ) ;

  gtk_button_set_image(GTK_BUTTON(cancel_button), cancel_image);

  gtk_container_add(GTK_CONTAINER(dialog_content_area), main_add_file_hbox) ;

  gtk_widget_show_all(add_a_file_to_files_manager_dialog) ;

  gint result = gtk_dialog_run (GTK_DIALOG(add_a_file_to_files_manager_dialog));
  switch (result) {

      case GTK_RESPONSE_OK :

        if (g_strcmp0(gtk_entry_get_text(GTK_ENTRY(new_entry_title_entry)), "") == 0 || gtk_entry_get_text(GTK_ENTRY(new_entry_title_entry)) == NULL || gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(new_entry_file_button)) == NULL ) {
          gtk_widget_destroy(add_a_file_to_files_manager_dialog) ;
          return ;
        }

        Files_Manager *file_item = g_malloc(sizeof(Files_Manager)) ;
        file_item->label = g_strdup(gtk_entry_get_text(GTK_ENTRY(new_entry_title_entry)) ) ;
        file_item->uri   = g_strdup(gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(new_entry_file_button)) )   ;

        FILE *fp = fopen(PATH_TO_LINKS_FILE, "a") ;

        fputs(file_item->label, fp) ;
        fputc('\n', fp) ;
        fputs(file_item->uri, fp) ;
        fputc('\n', fp) ;

        fclose(fp) ;


        GtkWidget *files_checkbutton = gtk_check_button_new_with_label(file_item->label) ;

        g_object_set_data(G_OBJECT(files_checkbutton), "item", file_item) ;

        gtk_box_pack_start(GTK_BOX(files_manager_vbox), files_checkbutton, FALSE, FALSE, 0) ;

        gtk_widget_show_all(GTK_WIDGET(files_manager_vbox)) ;

        gtk_widget_destroy(add_a_file_to_files_manager_dialog) ;
        return ;

      case GTK_RESPONSE_CANCEL :

        gtk_widget_destroy(add_a_file_to_files_manager_dialog) ;
        return ;


     default :

        gtk_widget_destroy(add_a_file_to_files_manager_dialog) ;
        return ;

  }


}



static void remove_selected_items_from_files_manager(GtkWidget *widget, GtkWidget *files_manager_vbox) {

  /** Remove all selected items from the "File Handler" functionality configuration file,
    * We store all not selected (to not remove) items in an GSlist using as a file content buffer
    * to overwrite the file content.
    *********************************************************************************************/

  #ifdef DEBUG
    DEBUG_FUNC_MARK
  #endif



  /** Get the check-buttons which contains the data. **/
  GList *check_buttons_list = gtk_container_get_children(GTK_CONTAINER(files_manager_vbox)) ;

  FILE *fp = fopen(PATH_TO_LINKS_FILE, "w");

  while (check_buttons_list != NULL) {

    if ( gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_buttons_list->data)) ) {
      /** The check-button is selected so we remove it from the GSList and freeing the data. **/

      files_manager_list=g_slist_remove(files_manager_list, g_object_get_data(G_OBJECT(check_buttons_list->data), "item") ) ;

      Files_Manager *file_item = (Files_Manager *) g_object_get_data(G_OBJECT(check_buttons_list->data), "item") ;

      /** Freeing the file_item members. **/
      free(file_item->label) ;
      free(file_item->uri)   ;

      /** Freeing the structure. **/
      free(file_item) ;

      /** Update the GUI. **/
      gtk_widget_destroy(GTK_WIDGET(check_buttons_list->data));
    }
    else {

      /** Append data to GSList and write it to file. **/
      files_manager_list = g_slist_append(files_manager_list, g_object_get_data(G_OBJECT(check_buttons_list->data), "item") ) ;

      Files_Manager *file_item = (Files_Manager *) g_object_get_data(G_OBJECT(check_buttons_list->data), "item") ;

      /** Write the data to file. **/
      fputs(file_item->label, fp) ;
      fputc('\n', fp) ;
      fputs(file_item->uri, fp) ;
      fputc('\n', fp) ;

    }

    if (check_buttons_list->next == NULL) {
      break ;
    }
    else {
      check_buttons_list = check_buttons_list->next ;
    }

  }

  fclose(fp) ;

  return ;

}



static void add_a_new_item_to_applications(GtkWidget *widget, gpointer user_data) {

  /** Add a new application as shortcut for starting it. **/

  #ifdef DEBUG
    DEBUG_FUNC_MARK
  #endif





  GtkWidget *file_dialog = gtk_file_chooser_dialog_new( _("Add a new aplication."), GTK_WINDOW(gui->main_window), GTK_FILE_CHOOSER_ACTION_OPEN, _("Add"), GTK_RESPONSE_OK, _("Cancel"), GTK_RESPONSE_CANCEL, NULL) ;

  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_dialog), PATH_TO_BIN_FOLDER) ;

  gtk_widget_show_all(file_dialog) ;

  int ret = gtk_dialog_run(GTK_DIALOG(file_dialog)) ;

  if (ret == GTK_RESPONSE_OK) {

      GKeyFile *conf_file = g_key_file_new() ;

      GError *error = NULL ;

      g_key_file_load_from_file(conf_file, PATH_TO_APP_FILE, G_KEY_FILE_NONE, &error) ;

      if (error != NULL) {

        gchar *msg = g_strdup_printf( _("Configuration file error:\n%s\n%s"), g_path_get_basename(PATH_TO_APP_FILE), error->message) ;

        display_message_dialog( _("Cannot register new application !"), msg, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

        g_free(msg) ;

        g_error_free(error) ;

        return ;

      }


      MenuItem *menu_item = g_malloc(sizeof(MenuItem)) ;

      gchar *gui_new_app =  gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_dialog)) ;

      gchar *text = g_strdup_printf("  %s  ", g_path_get_basename(gui_new_app)) ;

      setup_menu_item(menu_item, 'N', text, "applications-system.png", NULL, 0, 0) ;

      g_free(text) ;

      gtk_menu_shell_append(GTK_MENU_SHELL(gui->menus->app_others_menu), menu_item->menuitem) ;
      g_signal_connect(G_OBJECT(menu_item->menuitem), "activate", G_CALLBACK(launch_application), gui_new_app) ;

      gtk_widget_show_all(menu_item->menuitem) ;


      gui_app.others = g_key_file_get_string_list(conf_file, "Others",  "others",  &gui_app.nb_of_others, &error)  ;

      if (error != NULL) {

        gchar *msg = g_strdup_printf(_("Cannot register new application: configuration file error:\n%s\n%s"), PATH_TO_APP_FILE, error->message) ;

        display_message_dialog( _("Cannot register new application !"), msg, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

        g_free(msg) ;

        g_error_free(error) ;

        return ;

      }

      guint16 c  ;

      gchar *app_others_cp[gui_app.nb_of_others+2];

      for (c=0 ; c < gui_app.nb_of_others ; ++c) {
        app_others_cp[c] = gui_app.others[c] ;
      }

      app_others_cp[c] = gui_new_app ;

      app_others_cp[++c] = NULL ;


      g_key_file_set_string_list(conf_file, "Others",  "others",  (const gchar **) app_others_cp, ++gui_app.nb_of_others)  ;

      g_key_file_save_to_file(conf_file, PATH_TO_APP_FILE, &error) ;


      if (error != NULL) {

        gchar *msg = g_strdup_printf( _("Cannot register new application: configuration file error:\n%s\n%s"), PATH_TO_APP_FILE, error->message) ;

        display_message_dialog( _("Error !!!"), msg, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

        g_free(msg) ;

        g_error_free(error) ;

        return ;

      }


      gchar *gui_app_label = g_strdup_printf("%s", g_path_get_basename(gui_new_app)) ;

      GtkWidget *files_checkbutton = gtk_check_button_new_with_label(gui_app_label) ;

      g_free(gui_app_label) ;

      gtk_box_pack_start(GTK_BOX(user_data), files_checkbutton, FALSE, FALSE, 0) ;

      gtk_widget_show_all(files_checkbutton) ;

      g_key_file_unref(conf_file) ;


      g_strfreev(gui_app.others) ;

      gtk_widget_show_all(gtk_smart_menu_item_get_menuitem(gui->menuitems->app_others_menuitem))  ;

      gtk_widget_show_all(gui->menus->app_others_menu) ;


  }

  gtk_widget_destroy(file_dialog) ;

  display_message_dialog( _("Success registering !"), _("Success to register new application !\nYou can access every time you need it through the menu item\n\tApplications -> Others- > \"Your application\"."), GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE) ;

  return ;

}

static void remove_selected_items_from_applications(GtkWidget *widget, GtkWidget *files_manager_vbox) {

  /** Remove an application from the applications list . **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  /** Get the check buttons which contains the data. **/
  GList *check_buttons_list = gtk_container_get_children(GTK_CONTAINER(files_manager_vbox)) ;

  GList *menu_items_list    = gtk_container_get_children(GTK_CONTAINER(gui->menus->app_others_menu)) ;

  int c = 0 ;


  GKeyFile *conf_file = g_key_file_new() ;

  GError *error = NULL ;

  g_key_file_load_from_file(conf_file, PATH_TO_APP_FILE, G_KEY_FILE_NONE, &error) ;

  if (error != NULL) {

    gchar *msg = g_strdup_printf( _("Configuration file error:\n%s\n%s"), g_path_get_basename(PATH_TO_APP_FILE), error->message) ;

    display_message_dialog( _("Error configuration !"), msg, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_free(msg) ;

    g_error_free(error) ;

    return ;

  }


  gui_app.others = g_key_file_get_string_list(conf_file, "Others",  "others",  &gui_app.nb_of_others, &error)  ;

  gchar *gui_app_cp[gui_app.nb_of_others] ;

  int cc = 0 ;

  int rm_counter = 0 ;

  while (check_buttons_list != NULL) {

    if ( gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(check_buttons_list->data)) ) {
      /** The check button is selected so we.. */

      g_signal_handlers_disconnect_by_func(check_buttons_list->data, launch_application, gui_app.others[c]) ;

      gtk_widget_destroy(GTK_WIDGET(menu_items_list->data)) ;

      /** Update the GUI. **/
      gtk_widget_destroy(GTK_WIDGET(check_buttons_list->data));

      ++rm_counter ;

    }
    else {

      gui_app_cp[cc] = gui_app.others[c] ;

      ++cc ;

    }

    if (check_buttons_list->next == NULL || menu_items_list->next == NULL) {
      break ;
    }
    else {
      check_buttons_list = check_buttons_list->next ;
      menu_items_list = menu_items_list->next ;
      ++c ;
    }

  }

  gui_app_cp[++cc] = NULL ;

  g_key_file_set_string_list(conf_file, "Others",  "others",  (const gchar **) gui_app_cp, gui_app.nb_of_others-rm_counter)  ;

  g_key_file_save_to_file(conf_file, PATH_TO_APP_FILE, &error) ;


  if (error != NULL) {

    gchar *msg = g_strdup_printf( _("Cannot remove application(s) from list: configuration file error:\n%s\n%s"), g_path_get_basename(PATH_TO_APP_FILE), error->message) ;

    display_message_dialog( _("Error !!!"), msg, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_free(msg) ;

    g_error_free(error) ;

    return ;

  }



  g_strfreev(gui_app.others) ;

  g_key_file_unref(conf_file) ;

  if ( gui_app.nb_of_others-rm_counter <= 0) {

    gtk_widget_hide(gtk_smart_menu_item_get_menuitem(gui->menuitems->app_others_menuitem))  ;

  }

  return ;

}

static void check_user_shell(GtkFileChooserButton *chooser) {

  /** Check if the user has select a valid shell. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (! g_file_test(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser)), G_FILE_TEST_IS_EXECUTABLE) ) {

    display_message_dialog( _("The file is not an executable !"), _("The shell you have chosen isn't an executable !!!") , GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(chooser), settings.user_shell) ;

    return ;
  }

  gchar *user_shell_test = g_strdup_printf("%s -c ls $HOME", gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser)) ) ;

  if (system(user_shell_test) != 0) {

    display_message_dialog( _("Wrong shell !"), _("The file you have chosen seems not to be a valid shell."), GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(chooser), settings.user_shell) ;

    g_free(user_shell_test) ;

    return ;

  }

  g_free(user_shell_test) ;

  return ;
}

static GtkWidget *generate_applications_chooser(GtkWidget *hbox, const char *icon_filepath, const char *label_text, char *app_filepath) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif


  GtkWidget *icon_image = gtk_image_new_from_file(icon_filepath) ;

  GtkWidget *label = gtk_label_new(label_text) ;

  GtkWidget *file_chooser_button = gtk_file_chooser_button_new(label_text, GTK_FILE_CHOOSER_ACTION_OPEN) ;

  gtk_widget_set_size_request(file_chooser_button, (128+64), -1 ) ;

  if (g_strcmp0(app_filepath, "") != 0) {
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser_button),  g_path_get_dirname(app_filepath)) ;
    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(file_chooser_button), app_filepath) ;
  }
  else {
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser_button),  PATH_TO_BIN_FOLDER) ;
  }


  gtk_box_pack_start(GTK_BOX(hbox), icon_image,          FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(hbox), label,               TRUE,  TRUE,  0) ;
  gtk_box_pack_start(GTK_BOX(hbox), file_chooser_button, FALSE, FALSE, 0) ;

  gtk_box_set_spacing(GTK_BOX(hbox), 5) ;
  gtk_container_set_border_width(GTK_CONTAINER(hbox), 5) ;

  return file_chooser_button ;

}

static GtkWidget *generate_checkbutton(const gchar *label, gboolean is_active, const gchar *tooltip_text) {

  GtkWidget *checkbutton = gtk_check_button_new_with_label(label) ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), is_active) ;

  gtk_container_set_border_width(GTK_CONTAINER(checkbutton), CONTAINER_PADDING) ;

  if (tooltip_text != NULL) {

    gtk_widget_set_tooltip_text(checkbutton, tooltip_text) ;
  }

  return checkbutton ;

}



static void set_applications(char *app, GtkWidget *file_chooser) {

  if (app != NULL) {

    free(app) ;
  }
  else {

    app = g_strdup("") ;

    return ;

  }

  app = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser)) ;

  if (app == NULL) {
    app = g_strdup("") ;
  }

}

static void change_applications(char *app_name, GtkWidget *menu_item) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (g_strcmp0(app_name, "") != 0) {

    gchar *app_label = g_strdup_printf("  %s  ", g_path_get_basename(app_name)) ;

    gtk_label_set_text(GTK_LABEL(gtk_smart_menu_item_get_label(menu_item)), app_label) ;

    g_free(app_label) ;

    if (! gtk_widget_get_visible(gtk_smart_menu_item_get_menuitem(menu_item)) ) {

      gtk_widget_show(gtk_smart_menu_item_get_menuitem(menu_item)) ;

      g_signal_connect(gtk_smart_menu_item_get_menuitem(menu_item), "activate", G_CALLBACK(launch_application), app_name) ;

    }

  }

  return ;
}

static void set_as_default(GtkWidget *widget, gpointer user_data) {

  const gchar * const *ids = gtk_source_language_manager_get_language_ids(source_language_manager) ;


  GError *error = NULL ;

  GAppInfo *it_edit_app = g_app_info_create_from_commandline(PATH_TO_IT_EDIT, "it-edit", G_APP_INFO_CREATE_SUPPORTS_URIS, &error);

  if (error != NULL || it_edit_app == NULL) {

    char *msg = g_strdup(error->message) ;

    display_message_dialog( _("Cannot find it-edit executable."), msg, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE) ;

    free(msg) ;

    g_error_free(error) ;

    return ;
  }


  while (*ids != NULL) {

    GtkSourceLanguage *source_language = gtk_source_language_manager_get_language(source_language_manager, *ids) ;

    gchar **mimetypes = gtk_source_language_get_mime_types(source_language) ;

    if ((g_strcmp0("HTML", gtk_source_language_get_name(source_language)) == 0) || (g_strcmp0("XML", gtk_source_language_get_name(source_language)) == 0)) {

      ++ids ;

      continue ;


    }

    gboolean is_set_as_default = FALSE ;

    if (mimetypes != NULL) {

      int c = 0 ;

      while (mimetypes[c] != NULL) {

        is_set_as_default = g_app_info_set_as_default_for_type(it_edit_app, mimetypes[c], NULL) ;

        g_app_info_add_supports_type(it_edit_app, mimetypes[c], NULL) ;

        ++c ;
      }

    }

    g_strfreev(mimetypes) ;

    if (! is_set_as_default) {

      /** Case the file-type has no associated mimetype.
        * So we use the file extension to set it-edit as default application.
        *********************************************************************/

      gchar **globs = gtk_source_language_get_globs(source_language) ;

      if (globs != NULL) {

        int c = 0 ;

        while (globs[c] != NULL) {

          gchar *extension = g_strrstr(globs[c], ".");

          if (extension != NULL) {
 
            g_app_info_set_as_default_for_extension(it_edit_app, ++extension, NULL) ;
          }

          --extension ;

          ++c ;

        }

      }

      g_strfreev(globs) ;

    }

    ++ids ;

 }


 /** I can't set "text/plain" as default because of too much unwanted side-effects.
   * g_app_info_set_as_default_for_type(it_edit_app,"text/plain", NULL) ;
   *********************************************************************************/

 display_message_dialog( _("it-edit set as default editor."), _("The right choice !!!"), GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE) ;

 return ;

}

static void reset_default(GtkWidget *widget, gpointer user_data) {

  const gchar * const *ids = gtk_source_language_manager_get_language_ids(source_language_manager) ;

  while (*ids != NULL) {

    GtkSourceLanguage *source_language = gtk_source_language_manager_get_language(source_language_manager, *ids) ;

    gchar **mimetypes = gtk_source_language_get_mime_types(source_language) ;

    if (mimetypes != NULL) {

      int c = 0 ;

      while (mimetypes[c] != NULL) {

        g_app_info_reset_type_associations(mimetypes[c]) ;

        ++c ;
      }

    }

    g_strfreev(mimetypes) ;

    ++ids ;

 }

 #if 0
 g_app_info_reset_type_associations("text/plain") ;
 #endif

 display_message_dialog( _("Application associations reset to default."), _("The wrong choice !!!"), GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE) ;

 return ;

}

static void use_right_margin_toggled_state(GtkWidget *toggle_button, GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle_button)) ) {

    gtk_widget_set_state_flags(widget, GTK_STATE_FLAG_NORMAL, TRUE);

  }
  else {

    gtk_widget_set_state_flags(widget, GTK_STATE_FLAG_INSENSITIVE, TRUE);

  }

  return ;

}

static void editor_font_toggled_use_set_state(GtkWidget *toggle_button, GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggle_button)) ) {

    gtk_widget_set_state_flags(widget, GTK_STATE_FLAG_INSENSITIVE, TRUE);

  }
  else {

    gtk_widget_set_state_flags(widget, GTK_STATE_FLAG_NORMAL, TRUE);

  }

  return ;

}

static gboolean editor_font_chooser_filter(PangoFontFamily *family, const PangoFontFace *face, gpointer data) {

  return pango_font_family_is_monospace(family) ;

}




static void update_sidebar_position(GtkSpinButton *spin_button) {

  gdouble value = gtk_spin_button_get_value(spin_button) ;

  gtk_widget_set_size_request(gui->terminal_notebook, value * ( 80 * 8 + 24), -1) ;

  return ;

}

static gint session_mode = 0 ;

static void set_session_mode(GtkWidget *widget, gint *value) {

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) ) {

    session_mode = *value ;

  }



}

static void is_a_new_color_choosen(GtkColorButton *widget, bool *confirm) {

  *confirm = true ;

  return ;

}


static gboolean is_perm_value_set(const gint mask, const gint bit)   {

  return mask & bit ;

}

static void set_perm_value(GtkWidget *widget, gint *mask, gint value) {

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) ) {

    *mask += value ;

  }

  return ;

}

static GtkWidget *generate_perm_grid(GtkWidget *grid, const gchar *text, const guint8 row) {

  GtkWidget *label = gtk_label_new(text) ;

  GtkWidget *checkbutton = gtk_check_button_new() ;

  gtk_grid_attach(GTK_GRID(grid),  label,       row, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid),  checkbutton, row, 1, 1, 1);

  return checkbutton ;

}

static GtkWidget *generate_frame_with_content(const gchar *text, GtkWidget *widget) {

  GtkWidget *frame = gtk_frame_new(text) ;

  gtk_container_set_border_width(GTK_CONTAINER(frame), FRAME_BORDER_SIZE) ;

  gtk_container_add(GTK_CONTAINER(frame), widget) ;

  return frame ;
}

void display_configuration_dialog(GtkWidget *widget, gpointer user_data) {
  /** Generate the application configuration dialog window and register changes if some. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif


  GtkWidget *configuration_dialog = gtk_dialog_new_with_buttons( _("Configure program"),
                                                                GTK_WINDOW(gui->main_window),
                                                                GTK_DIALOG_USE_HEADER_BAR | GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT ,
                                                                _("Apply"), GTK_RESPONSE_APPLY,
                                                                _("Cancel"), GTK_RESPONSE_CANCEL,
                                                                NULL);




  gtk_window_set_transient_for(GTK_WINDOW(configuration_dialog), GTK_WINDOW(gui->main_window)) ;

  gtk_window_set_resizable(GTK_WINDOW(configuration_dialog), FALSE) ;

  GtkWidget *configuration_dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(configuration_dialog));

  GtkWidget *notebook_configuration = gtk_notebook_new() ;

  gtk_notebook_set_scrollable(GTK_NOTEBOOK(notebook_configuration), TRUE);







  GtkWidget *show_lines_checkbutton   = generate_checkbutton( _("Display lines numbers"), settings.display_line_numbers, NULL) ;

  GtkWidget *show_tabs_checkbutton    = generate_checkbutton( _("Display TABS characters"),settings.display_tabs_chars, _("By pressing the TAB key an arrow will be draw "
                                                                                                                         "at the cursor location indicating a tabulation.\n"
                                                                                                                         "Else if your document contains other tabulation(s)\n"
                                                                                                                          "they will be drawn too.") ) ;

  GtkWidget *show_spaces_checkbutton  = generate_checkbutton( _("Display all spaces"),  settings.display_all_spaces, _("All spaces will be draw:\n\n"
                                                                                                                      "* Line end as a returned arrow.\n"
                                                                                                                      "* Normal spaces as an point.\n"
                                                                                                                      "* Tabulations as an forward arrow.\n"
                                                                                                                      "This feature can be use to visualize your document\n"
                                                                                                                      "for leading and trailing spaces per example...")) ;

  GtkWidget *grid_background_checkbutton =   generate_checkbutton( _("Display background grid"),  settings.grid_background, _("Display a grid on the background\n"
                                                                                                                             "for a better visualization of your typing work.") );

  GtkWidget *display_options_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_container_set_border_width(GTK_CONTAINER(display_options_vbox), CONTAINER_PADDING) ;

  gtk_box_pack_start( GTK_BOX(display_options_vbox), show_lines_checkbutton,      FALSE, FALSE, 0) ;
  gtk_box_pack_start( GTK_BOX(display_options_vbox), show_tabs_checkbutton,       FALSE, FALSE, 0) ;
  gtk_box_pack_start( GTK_BOX(display_options_vbox), show_spaces_checkbutton,     FALSE, FALSE, 0) ;
  gtk_box_pack_start( GTK_BOX(display_options_vbox), grid_background_checkbutton, FALSE, FALSE, 0) ;


  GtkWidget *displaying_options_frame = generate_frame_with_content(_(" Editor display options "), display_options_vbox) ;












  GtkWidget *editor_font_button = gtk_font_button_new_with_font(settings.editor_font) ;

  g_object_set(G_OBJECT(editor_font_button), "margin", CONTAINER_PADDING*2,  NULL) ;

  gtk_font_button_set_show_style(GTK_FONT_BUTTON(editor_font_button),  TRUE) ;
  gtk_font_button_set_show_size(GTK_FONT_BUTTON(editor_font_button),   TRUE) ;
  gtk_font_button_set_use_font(GTK_FONT_BUTTON(editor_font_button),    TRUE) ;
  gtk_font_button_set_use_size(GTK_FONT_BUTTON(editor_font_button),    TRUE) ;

  gtk_font_chooser_set_filter_func(GTK_FONT_CHOOSER(editor_font_button), (GtkFontFilterFunc) editor_font_chooser_filter, NULL,  NULL) ;

  GtkWidget *use_monospace_font_checkbutton = generate_checkbutton( _(" Use default Monospace font "), settings.use_monospace_font, _("Here you can choose to use the default mono-space font.\n"
                                                                                                                                     "Or to set the wanted font and font-size.")) ;

  g_signal_connect(G_OBJECT(use_monospace_font_checkbutton), "toggled", G_CALLBACK(editor_font_toggled_use_set_state), editor_font_button) ;

  if (settings.use_monospace_font) {

    gtk_widget_set_state_flags(editor_font_button, GTK_STATE_FLAG_INSENSITIVE, TRUE);

  }
  else {

    gtk_widget_set_state_flags(editor_font_button, GTK_STATE_FLAG_NORMAL, TRUE);

  }


  GtkWidget *use_font_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_container_set_border_width(GTK_CONTAINER(use_font_vbox), CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(use_font_vbox), use_monospace_font_checkbutton, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(use_font_vbox), editor_font_button,             TRUE,  TRUE, 0) ;


  GtkWidget *editor_font_frame = generate_frame_with_content( _(" Use font "), use_font_vbox) ;


  GtkWidget *right_margin_spinbutton = gtk_spin_button_new_with_range(0, 1000, 1) ;

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(right_margin_spinbutton), settings.right_margin_value) ;
  gtk_spin_button_set_digits(GTK_SPIN_BUTTON(right_margin_spinbutton),                          0) ;
  gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(right_margin_spinbutton),                      TRUE) ;

  GtkWidget *right_margin_hbox  = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  GtkWidget *right_margin_label = gtk_label_new( _("Right-margin width (in characters)") ) ;

  gtk_box_pack_start(GTK_BOX(right_margin_hbox), right_margin_label,      TRUE,   TRUE,   0) ;
  gtk_box_pack_start(GTK_BOX(right_margin_hbox), right_margin_spinbutton, FALSE,  FALSE,  0) ;

  GtkWidget *right_margin_checkbutton = generate_checkbutton( _("Use right margin"), settings.use_right_margin, _("The right margin can be useful if you want to limit your document\n"
                                                                                                                 "to a configured width per example...")) ;

  g_signal_connect(G_OBJECT(right_margin_checkbutton), "toggled", G_CALLBACK(use_right_margin_toggled_state), right_margin_spinbutton) ;

  if (! settings.use_right_margin) {

    gtk_widget_set_state_flags(right_margin_spinbutton, GTK_STATE_FLAG_INSENSITIVE, TRUE);

  }
  else {

    gtk_widget_set_state_flags(right_margin_spinbutton, GTK_STATE_FLAG_NORMAL, TRUE);

  }

  GtkWidget *right_margin_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(right_margin_vbox),right_margin_checkbutton,      FALSE, FALSE,  0) ;
  gtk_box_pack_start(GTK_BOX(right_margin_vbox),right_margin_hbox,             FALSE, FALSE,  0) ;
  gtk_container_set_border_width(GTK_CONTAINER(right_margin_vbox), CONTAINER_PADDING) ;

  GtkWidget *right_margin_frame = generate_frame_with_content( _(" Right margin "), right_margin_vbox) ;


  GtkWidget *indent_settings_hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  GtkWidget *auto_indent_checkbutton = generate_checkbutton( _("Use auto-indent"), settings.use_auto_indent, NULL) ;

  GtkWidget *indent_width_hbox       = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  GtkWidget *indent_width_label      = gtk_label_new( _("Indent width") ) ;

  GtkWidget *indent_width_spinbutton = gtk_spin_button_new_with_range(1, 32, 1) ;

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(indent_width_spinbutton),   (gdouble) settings.indent_width ) ;
  gtk_spin_button_set_digits(GTK_SPIN_BUTTON(indent_width_spinbutton),                                 0) ;
  gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(indent_width_spinbutton),                             TRUE) ;


  gtk_box_pack_start(GTK_BOX(indent_width_hbox),indent_width_label,       TRUE, TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(indent_width_hbox),indent_width_spinbutton,  FALSE, FALSE, 0) ;


  gtk_box_pack_start(GTK_BOX(indent_settings_hbox), auto_indent_checkbutton, FALSE, FALSE, 0) ;

  gtk_box_pack_start(GTK_BOX(indent_settings_hbox), indent_width_hbox, FALSE, FALSE, 0) ;

  gtk_container_set_border_width(GTK_CONTAINER(indent_settings_hbox), CONTAINER_PADDING*2) ;



  GtkWidget *indent_settings_frame = generate_frame_with_content( _(" Indentation "), indent_settings_hbox) ;



  GtkWidget *use_spaces_instead_of_tabs = generate_checkbutton( _("Use spaces instead of TABS"), settings.use_spaces_as_tabs, _("By typing a tabulation no tabulation character\n"
                                                                                                                                "will be write into your document.\n"
                                                                                                                                "Instead the number of configured spaces will be written.")  ) ;





  GtkWidget *tabs_width_hbox       = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0) ;

  GtkWidget *tabs_width_label      = gtk_label_new( _("TABS width") ) ;

  GtkWidget *tabs_width_spinbutton = gtk_spin_button_new_with_range(2, 32, 2) ;

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(tabs_width_spinbutton), settings.tabs_width) ;
  gtk_spin_button_set_digits(GTK_SPIN_BUTTON(tabs_width_spinbutton),                  0) ;
  gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(tabs_width_spinbutton),              TRUE) ;


  gtk_box_pack_start(GTK_BOX(tabs_width_hbox),tabs_width_label,      TRUE,   TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(tabs_width_hbox),tabs_width_spinbutton, FALSE, FALSE, 0) ;


  GtkWidget *tabs_settings_hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(tabs_settings_hbox), use_spaces_instead_of_tabs, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(tabs_settings_hbox), tabs_width_hbox,            FALSE, FALSE, 0) ;

  gtk_container_set_border_width(GTK_CONTAINER(tabs_settings_hbox), CONTAINER_PADDING*2) ;



  GtkWidget *tabs_settings_frame = generate_frame_with_content( _(" Tabulations "), tabs_settings_hbox) ;





  GtkWidget *editor_configuration_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_box_set_spacing(GTK_BOX(editor_configuration_vbox), CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(editor_configuration_vbox), displaying_options_frame,      FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(editor_configuration_vbox), editor_font_frame,             FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(editor_configuration_vbox), right_margin_frame,            FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(editor_configuration_vbox), indent_settings_frame,         TRUE, TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(editor_configuration_vbox), tabs_settings_frame,           TRUE,  TRUE, 0) ;




  gtk_notebook_append_page(GTK_NOTEBOOK(notebook_configuration), editor_configuration_vbox,  gtk_label_new( _("Editor Settings") )) ;








  GtkWidget *open_file_warn_already_in_editor_chkbox = generate_checkbutton( _("Warning by loaded file already into editor"), settings.warn_file_open, _("The program emit a warning by opening a file already contains into the editor.\nIt will let you the choice to open it or not.") ) ;



  GtkWidget *open_file_warn_readonly_chkbox = generate_checkbutton( _("Warning by loaded file is read-only file"),  settings.warn_read_only, _("The program emit a warning by opening read-only files")) ;




  GtkWidget *open_file_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_box_set_spacing(GTK_BOX(open_file_vbox), CONTAINER_PADDING) ;


  gtk_box_pack_start(GTK_BOX(open_file_vbox), open_file_warn_already_in_editor_chkbox,  FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(open_file_vbox), open_file_warn_readonly_chkbox,           FALSE, FALSE, 0) ;



  GtkWidget *open_file_frame = generate_frame_with_content( _(" Open file "), open_file_vbox) ;






  GtkWidget *save_file_despite_modification_timestamp = generate_checkbutton( _("Save despite modification time stamp"), settings.overwrite_anyway, _("Allow the program to save a file despite a later time stamp.\nAnother process have change the file.")) ;






  GtkWidget *save_file_encoding_label = gtk_label_new( _(" Charset: ") ) ;

  GtkWidget *save_file_encoding_comboboxtext = gtk_combo_box_text_new() ;



  guint16 encoding_set = 0 ;

  guint16 encoding_counter = 0 ;

  const GtkSourceEncoding *user_encoding = gtk_source_encoding_get_from_charset(settings.charset) ;

  GSList *g_slist_encoding = gtk_source_encoding_get_all() ;

  g_slist_encoding = g_slist_nth(g_slist_encoding, 0) ;

  while (g_slist_encoding != NULL) {

    if (g_slist_encoding->data != NULL) {

      if (g_strcmp0(gtk_source_encoding_get_charset( (const GtkSourceEncoding *) g_slist_encoding->data), gtk_source_encoding_get_charset(user_encoding)) ==  0) {

        encoding_set = encoding_counter ;

      }

      gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(save_file_encoding_comboboxtext), gtk_source_encoding_get_charset(g_slist_encoding->data)) ;

      ++encoding_counter ;

    }

    if (g_slist_encoding->next != NULL) {

      g_slist_encoding = g_slist_encoding->next ;
    }
    else {

      break ;
    }

  }

  gtk_combo_box_set_column_span_column(GTK_COMBO_BOX(save_file_encoding_comboboxtext), -1) ;

  gtk_combo_box_set_active(GTK_COMBO_BOX(save_file_encoding_comboboxtext),  encoding_set) ;

  gtk_widget_set_size_request(save_file_encoding_comboboxtext, 32*7+16+4, -1) ;


  GtkWidget *save_file_encoding_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  gtk_box_pack_start(GTK_BOX(save_file_encoding_hbox), save_file_encoding_label,        TRUE,   TRUE,   0) ;
  gtk_box_pack_start(GTK_BOX(save_file_encoding_hbox), save_file_encoding_comboboxtext, FALSE,  FALSE,  0) ;

  gtk_container_set_border_width(GTK_CONTAINER(save_file_encoding_hbox), CONTAINER_PADDING) ;







  GtkWidget *save_file_newline_label = gtk_label_new( _(" Newline type: ") ) ;

  GtkWidget *save_file_newline_comboboxtext = gtk_combo_box_text_new() ;

  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(save_file_newline_comboboxtext), "Linefeed (\\n)")    ;
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(save_file_newline_comboboxtext), "Carriage return (\\r)")    ;
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(save_file_newline_comboboxtext), "Carriage return & Linefeed (\\r\\n)")    ;

  if (settings.newline_type == 0) {

    gtk_combo_box_set_active(GTK_COMBO_BOX(save_file_newline_comboboxtext), 0) ;
  }
  else if (settings.newline_type == 1) {

    gtk_combo_box_set_active(GTK_COMBO_BOX(save_file_newline_comboboxtext), 1) ;

  }
  else if (settings.newline_type == 2) {

    gtk_combo_box_set_active(GTK_COMBO_BOX(save_file_newline_comboboxtext), 2) ;

  }



  GtkWidget *save_file_newline_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  gtk_box_pack_start(GTK_BOX(save_file_newline_hbox), save_file_newline_label,        TRUE,   TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(save_file_newline_hbox), save_file_newline_comboboxtext, FALSE,  FALSE, 0) ;

  gtk_container_set_border_width(GTK_CONTAINER(save_file_newline_hbox), CONTAINER_PADDING) ;


  GtkWidget *save_file_mask_label = gtk_label_new( _("Save file permissions") ) ;




  gint mask = settings.save_file_mode ;

  GtkWidget *save_file_user_mask_grid   = gtk_grid_new() ;

  g_object_set(save_file_user_mask_grid, "column-homogeneous", TRUE, "column-spacing", 8,  NULL) ;

  gtk_container_set_border_width(GTK_CONTAINER(save_file_user_mask_grid),  CONTAINER_PADDING) ;

  GtkWidget *save_file_mask_ur_checkbutton = generate_perm_grid(save_file_user_mask_grid, "r", 0) ;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_ur_checkbutton), is_perm_value_set(mask, S_IRUSR)) ;

  GtkWidget *save_file_mask_uw_checkbutton = generate_perm_grid(save_file_user_mask_grid, "w", 1) ;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_uw_checkbutton), is_perm_value_set(mask, S_IWUSR)) ;

  GtkWidget *save_file_mask_ux_checkbutton = generate_perm_grid(save_file_user_mask_grid, "x", 2) ;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_uw_checkbutton), is_perm_value_set(mask, S_IWUSR)) ;


  GtkWidget *save_file_group_mask_grid  = gtk_grid_new() ;

  g_object_set(save_file_group_mask_grid, "column-homogeneous", TRUE, "column-spacing", 8,  NULL) ;

  gtk_container_set_border_width(GTK_CONTAINER(save_file_group_mask_grid), CONTAINER_PADDING) ;

  GtkWidget *save_file_mask_gr_checkbutton = generate_perm_grid(save_file_group_mask_grid, "r", 3) ;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_gr_checkbutton), is_perm_value_set(mask, S_IRGRP)) ;

  GtkWidget *save_file_mask_gw_checkbutton = generate_perm_grid(save_file_group_mask_grid, "w", 4) ;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_gw_checkbutton), is_perm_value_set(mask, S_IWGRP)) ;

  GtkWidget *save_file_mask_gx_checkbutton = generate_perm_grid(save_file_group_mask_grid, "x", 5) ;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_gx_checkbutton), is_perm_value_set(mask, S_IXGRP)) ;




  GtkWidget *save_file_other_mask_grid  = gtk_grid_new() ;

  g_object_set(save_file_other_mask_grid, "column-homogeneous", TRUE, "column-spacing", 8,  NULL) ;

  gtk_container_set_border_width(GTK_CONTAINER(save_file_other_mask_grid), CONTAINER_PADDING) ;

  GtkWidget *save_file_mask_or_checkbutton = generate_perm_grid(save_file_other_mask_grid, "r", 6) ;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_or_checkbutton), is_perm_value_set(mask, S_IROTH)) ;

  GtkWidget *save_file_mask_ow_checkbutton = generate_perm_grid(save_file_other_mask_grid, "w", 7) ;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_ow_checkbutton), is_perm_value_set(mask, S_IWOTH)) ;

  GtkWidget *save_file_mask_ox_checkbutton = generate_perm_grid(save_file_other_mask_grid, "x", 8) ;
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_ox_checkbutton), is_perm_value_set(mask, S_IXOTH)) ;




  GtkWidget *save_file_mask_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  gtk_box_pack_start(GTK_BOX(save_file_mask_hbox), save_file_mask_label,         TRUE,   TRUE,   0) ;
  gtk_box_pack_start(GTK_BOX(save_file_mask_hbox), save_file_user_mask_grid,     FALSE,  FALSE,  4) ;
  gtk_box_pack_start(GTK_BOX(save_file_mask_hbox), save_file_group_mask_grid,    FALSE,  FALSE,  4) ;
  gtk_box_pack_start(GTK_BOX(save_file_mask_hbox), save_file_other_mask_grid,    FALSE,  FALSE,  4) ;

  gtk_container_set_border_width(GTK_CONTAINER(save_file_mask_hbox), CONTAINER_PADDING) ;












  GtkWidget *file_saving_save_file_hbox  = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  GtkWidget *file_saving_save_file_label = gtk_label_new( _("Create a back up file on saving:") ) ;

  GtkWidget *save_file_backup_file_switch = gtk_switch_new() ;

  gtk_switch_set_active(GTK_SWITCH(save_file_backup_file_switch), settings.backup_file) ;

  gtk_box_pack_start(GTK_BOX(file_saving_save_file_hbox), file_saving_save_file_label,  TRUE,   TRUE,   0) ;
  gtk_box_pack_start(GTK_BOX(file_saving_save_file_hbox), save_file_backup_file_switch, FALSE,  FALSE,  0) ;



  GtkWidget *backup_suffix_hbox      = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *backup_suffix_label     = gtk_label_new( _(" Backup file suffix character: ") ) ;

  GtkWidget *backup_suffix_entry     = gtk_entry_new() ;

  gtk_entry_set_text(GTK_ENTRY(backup_suffix_entry), "~") ;

  gtk_entry_set_width_chars(GTK_ENTRY(backup_suffix_entry), CONTAINER_PADDING) ;

  gtk_entry_set_alignment(GTK_ENTRY(backup_suffix_entry), 0.5) ;


  g_object_set(G_OBJECT(backup_suffix_entry), "editable", FALSE, NULL) ;

  g_object_set(backup_suffix_entry, "margin", 4*3+2, NULL) ;


  gtk_box_pack_start(GTK_BOX(backup_suffix_hbox), backup_suffix_label,   TRUE,  TRUE,  0) ;
  gtk_box_pack_start(GTK_BOX(backup_suffix_hbox), backup_suffix_entry,   FALSE, FALSE, 0) ;


  GtkWidget *file_saving_backup_file_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_container_set_border_width(GTK_CONTAINER(file_saving_backup_file_vbox), CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(file_saving_backup_file_vbox), file_saving_save_file_hbox, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(file_saving_backup_file_vbox), backup_suffix_hbox,         FALSE, FALSE, 0) ;







  GtkWidget *file_saving_rm_trailing_spaces_label = gtk_label_new( _("Remove trailing space on saving file:") ) ;

  GtkWidget *file_saving_rm_trailing_spaces_switch = gtk_switch_new() ;

  gtk_switch_set_active(GTK_SWITCH(file_saving_rm_trailing_spaces_switch), settings.rm_trailing_spaces) ;

  GtkWidget *file_saving_rm_trailing_spaces_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  gtk_box_pack_start(GTK_BOX(file_saving_rm_trailing_spaces_hbox), file_saving_rm_trailing_spaces_label,  TRUE, TRUE,   0) ;
  gtk_box_pack_start(GTK_BOX(file_saving_rm_trailing_spaces_hbox), file_saving_rm_trailing_spaces_switch, FALSE, FALSE, 0) ;

  gtk_container_set_border_width(GTK_CONTAINER(file_saving_rm_trailing_spaces_hbox), 24) ;


  GtkWidget *file_saving_rm_trailing_spaces_frame = generate_frame_with_content( _(" Remove trailing spaces "),  file_saving_rm_trailing_spaces_hbox) ;


  GtkWidget *file_saving_backup_file_frame = generate_frame_with_content( _(" Create backup file "), file_saving_backup_file_vbox) ;


  GtkWidget *save_file_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_box_set_spacing(GTK_BOX(save_file_vbox), CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(save_file_vbox), save_file_despite_modification_timestamp,   FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(save_file_vbox), save_file_encoding_hbox,                    FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(save_file_vbox), save_file_newline_hbox,                     FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(save_file_vbox), save_file_mask_hbox,                        FALSE, FALSE, 0) ;

  GtkWidget *save_file_frame = generate_frame_with_content( _(" Save file "), save_file_vbox) ;


  GtkWidget *close_file_unsaved_checkbutton = generate_checkbutton( _("Warning by closing an unsaved file"), settings.warn_file_save, _("Emit a warning if you attempt to close an unsaved file" )) ;


  GtkWidget *close_file_frame = generate_frame_with_content( _(" Close file "), close_file_unsaved_checkbutton) ;



  GtkWidget *files_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_box_set_spacing(GTK_BOX(files_vbox), CONTAINER_PADDING) ;


  gtk_box_pack_start(GTK_BOX(files_vbox), open_file_frame,                        FALSE,  FALSE,  0) ;
  gtk_box_pack_start(GTK_BOX(files_vbox), save_file_frame,                        FALSE,  FALSE,  0) ;
  gtk_box_pack_start(GTK_BOX(files_vbox), close_file_frame,                       FALSE,  FALSE,  0) ;
  gtk_box_pack_start(GTK_BOX(files_vbox), file_saving_backup_file_frame,          FALSE,  FALSE,  0) ;
  gtk_box_pack_start(GTK_BOX(files_vbox), file_saving_rm_trailing_spaces_frame,   FALSE,  FALSE,  0) ;


  gtk_notebook_append_page(GTK_NOTEBOOK(notebook_configuration), files_vbox, gtk_label_new( _("File(s) settings")) ) ;



  GtkWidget *terminal_settings_notebook         = gtk_notebook_new() ;

  GtkWidget *terminal_use_font_button = gtk_font_button_new_with_font(settings.term_font) ;


  g_object_set(G_OBJECT(terminal_use_font_button), "margin", CONTAINER_PADDING, NULL) ;

  gtk_font_button_set_show_style(GTK_FONT_BUTTON(terminal_use_font_button),  TRUE) ;
  gtk_font_button_set_show_size(GTK_FONT_BUTTON(terminal_use_font_button),   TRUE) ;
  gtk_font_button_set_use_font(GTK_FONT_BUTTON(terminal_use_font_button),    TRUE) ;
  gtk_font_button_set_use_size(GTK_FONT_BUTTON(terminal_use_font_button),    TRUE) ;


  GtkWidget *terminal_use_scheme_frame         = generate_frame_with_content( _("Font settings"),  terminal_use_font_button) ;

  gtk_container_set_border_width(GTK_CONTAINER(terminal_use_font_button), CONTAINER_PADDING) ;




  GtkWidget *terminal_font_scale_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,   CONTAINER_PADDING) ;
  GtkWidget *terminal_font_scale_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *terminal_font_scale_label = gtk_label_new( _("  Font scale  ") ) ;

  GtkWidget *terminal_font_scale_spinbutton = gtk_spin_button_new_with_range(0.25, 4.0, 0.01) ;

  gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(terminal_font_scale_spinbutton), TRUE) ;

  gtk_spin_button_set_digits(GTK_SPIN_BUTTON(terminal_font_scale_spinbutton), 2) ;

  gtk_spin_button_set_update_policy(GTK_SPIN_BUTTON(terminal_font_scale_spinbutton), GTK_UPDATE_IF_VALID) ;

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(terminal_font_scale_spinbutton), settings.font_scale) ;


  gtk_container_set_border_width(GTK_CONTAINER(terminal_font_scale_hbox), CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(terminal_font_scale_hbox), terminal_font_scale_label,        FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(terminal_font_scale_hbox), gtk_label_new(""),                TRUE,  TRUE,  0) ;
  gtk_box_pack_start(GTK_BOX(terminal_font_scale_hbox), terminal_font_scale_spinbutton,   FALSE, FALSE, 0) ;

  gtk_box_pack_start(GTK_BOX(terminal_font_scale_vbox), terminal_font_scale_hbox, FALSE, FALSE, 0) ;

  GtkWidget *terminal_font_scale_frame = generate_frame_with_content( _("Font scale"), terminal_font_scale_vbox) ;


  GtkWidget *terminal_cursor_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, CONTAINER_PADDING) ;


  GtkWidget *terminal_cursor_shape_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;


  GtkWidget *cursor_shape_block_radiobutton     = gtk_radio_button_new_with_label(NULL, _("BLOCK") );

  GtkWidget *cursor_shape_ibeam_radiobutton     = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(cursor_shape_block_radiobutton), _("IBEAM") ) ;

  GtkWidget *cursor_shape_underline_radiobutton = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(cursor_shape_block_radiobutton), _("UNDERLINE") ) ;

  switch (settings.cursor_shape) {

    case 0 :
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cursor_shape_block_radiobutton), TRUE );
      break ;

    case 1 :

      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cursor_shape_ibeam_radiobutton), TRUE );
      break ;

    case 2 :

      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cursor_shape_underline_radiobutton), TRUE );
      break ;
  }

  gtk_box_set_homogeneous(GTK_BOX(terminal_cursor_shape_hbox), TRUE) ;

  gtk_container_set_border_width(GTK_CONTAINER(terminal_cursor_shape_hbox), CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(terminal_cursor_shape_hbox), cursor_shape_block_radiobutton,          FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(terminal_cursor_shape_hbox), cursor_shape_ibeam_radiobutton,          FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(terminal_cursor_shape_hbox), cursor_shape_underline_radiobutton,      FALSE, FALSE, 0) ;


  gtk_box_pack_start(GTK_BOX(terminal_cursor_vbox), terminal_cursor_shape_hbox, FALSE, FALSE, 0) ;







  GtkWidget *terminal_cursor_color_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *terminal_cursor_color_label  = gtk_label_new( _("  Cursor color  ") ) ;

  GdkRGBA cursor_color ;
  if (! gdk_rgba_parse(&cursor_color, settings.cursor_color)) {

    g_warning("Error getting cursor color !\n") ;
  }

  GtkWidget *terminal_cursor_color_button = gtk_color_button_new_with_rgba(&cursor_color) ;

  bool *cursor_color_activated = malloc(sizeof(bool)) ;

  *cursor_color_activated = false ;

  g_signal_connect(G_OBJECT(terminal_cursor_color_button), "color-set", G_CALLBACK(is_a_new_color_choosen), cursor_color_activated) ;

  gtk_container_set_border_width(GTK_CONTAINER(terminal_cursor_color_hbox), CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(terminal_cursor_color_hbox), terminal_cursor_color_label,  FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(terminal_cursor_color_hbox), gtk_label_new(""),  TRUE, TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(terminal_cursor_color_hbox), terminal_cursor_color_button, FALSE, FALSE, 0) ;

  gtk_box_pack_start(GTK_BOX(terminal_cursor_vbox), terminal_cursor_color_hbox, FALSE, FALSE, 0) ;





  GtkWidget *terminal_cursor_blinking_hbox  = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *terminal_cursor_blinking_label = gtk_label_new( _("  Cursor blink mode  ") ) ;

  GtkWidget *terminal_cursor_blinking_combo = gtk_combo_box_text_new() ;

  gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(terminal_cursor_blinking_combo), 0, "BLINK SYSTEM" ) ;
  gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(terminal_cursor_blinking_combo), 1, "BLINK ON"     ) ;
  gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(terminal_cursor_blinking_combo), 2, "BLINK OFF"    ) ;

  gtk_combo_box_set_active(GTK_COMBO_BOX(terminal_cursor_blinking_combo), settings.cursor_blink) ;

  gtk_container_set_border_width(GTK_CONTAINER(terminal_cursor_blinking_hbox), CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(terminal_cursor_blinking_hbox), terminal_cursor_blinking_label,  FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(terminal_cursor_blinking_hbox), gtk_label_new(""),  TRUE, TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(terminal_cursor_blinking_hbox), terminal_cursor_blinking_combo, FALSE, FALSE, 0) ;

  gtk_box_pack_start(GTK_BOX(terminal_cursor_vbox), terminal_cursor_blinking_hbox, FALSE, FALSE, 0) ;

  GtkWidget *terminal_cursor_frame = generate_frame_with_content( _("Cursor settings"), terminal_cursor_vbox) ;



  GtkWidget *terminal_bold_vbox        = gtk_box_new(GTK_ORIENTATION_VERTICAL,    5) ;
  GtkWidget *terminal_bold_color_hbox  = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,  5) ;

  GtkWidget *terminal_bold_allowed_checkbutton = generate_checkbutton( _("  Allow bold  "), settings.bold_allow, NULL) ;

  GtkWidget *terminal_bold_color_label  = gtk_label_new( _("  Bold color  ") ) ;

  GdkRGBA bold_color ;
  if (! gdk_rgba_parse(&bold_color, settings.bold_color)) {

    g_warning("Error getting bold color !\n") ;

  }

  GtkWidget *terminal_bold_color_button = gtk_color_button_new_with_rgba(&bold_color) ;

  bool *bold_color_activated = malloc(sizeof(bool)) ;

  *bold_color_activated = false ;

  g_signal_connect(G_OBJECT(terminal_bold_color_button), "color-set", G_CALLBACK(is_a_new_color_choosen), bold_color_activated) ;

  gtk_box_pack_start(GTK_BOX(terminal_bold_color_hbox), terminal_bold_allowed_checkbutton,          FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(terminal_bold_color_hbox), gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0), TRUE, TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(terminal_bold_color_hbox), terminal_bold_color_label,                  FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(terminal_bold_color_hbox), gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0), TRUE, TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(terminal_bold_color_hbox), terminal_bold_color_button,                 FALSE, FALSE, 0) ;


  gtk_box_pack_start(GTK_BOX(terminal_bold_vbox), terminal_bold_color_hbox,          FALSE, FALSE, 0) ;


  gtk_container_set_border_width(GTK_CONTAINER(terminal_bold_color_hbox), CONTAINER_PADDING) ;


  GtkWidget *terminal_bold_frame = generate_frame_with_content( _("Bold settings"), terminal_bold_vbox) ;



  GtkWidget *terminal_pointer_autohide_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *terminal_pointer_autohide_chkbox = generate_checkbutton( _("Pointer autohide"), settings.pointer_autohide, NULL) ;

  gtk_box_pack_start(GTK_BOX(terminal_pointer_autohide_hbox), terminal_pointer_autohide_chkbox, TRUE, TRUE, 0) ;


  gtk_container_set_border_width(GTK_CONTAINER(terminal_pointer_autohide_chkbox),        CONTAINER_PADDING) ;

  GtkWidget *terminal_pointer_autohide = generate_frame_with_content( _("Pointer autohide"), terminal_pointer_autohide_hbox) ;

  GdkRGBA bg_color ;

  if (! gdk_rgba_parse(&bg_color, settings.term_bg)) {

    g_warning("Error getting terminal background color !\n") ;
  }

  GtkWidget *terminal_bg_color_button = gtk_color_button_new_with_rgba(&bg_color) ;

  GdkRGBA fg_color ;

  if (! gdk_rgba_parse(&fg_color, settings.term_fg)) {

    g_warning("Error getting terminal foreground color !\n") ;

  }

  GtkWidget *terminal_fg_color_button = gtk_color_button_new_with_rgba(&fg_color) ;


  GtkWidget *terminal_colors_hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, CONTAINER_PADDING) ;

  GtkWidget *terminal_bg_color_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;
  GtkWidget *terminal_fg_color_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  GtkWidget *terminal_bg_color_label = gtk_label_new( _("  Background color  ") ) ;
  GtkWidget *terminal_fg_color_label = gtk_label_new( _("  Foreground color  ") ) ;

  gtk_box_pack_start(GTK_BOX(terminal_bg_color_hbox), terminal_bg_color_label,  TRUE,  TRUE,  0) ;
  gtk_box_pack_start(GTK_BOX(terminal_bg_color_hbox), terminal_bg_color_button, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(terminal_bg_color_hbox), terminal_fg_color_label,  TRUE,  TRUE,  0) ;
  gtk_box_pack_start(GTK_BOX(terminal_bg_color_hbox), terminal_fg_color_button, FALSE, FALSE, 0) ;

  gtk_box_pack_start(GTK_BOX(terminal_colors_hbox), terminal_bg_color_hbox, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(terminal_colors_hbox), terminal_fg_color_hbox, FALSE, FALSE, 0) ;


  GtkWidget *terminal_colors_frame = generate_frame_with_content( _("Colors settings"), terminal_colors_hbox) ;

  gtk_container_set_border_width(GTK_CONTAINER(terminal_colors_hbox), CONTAINER_PADDING) ;


  GtkWidget *user_shell_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  GtkWidget *user_shell_label = gtk_label_new(     _("User shell                 ") ) ;

  GtkWidget *user_shell_file_chooser_button = gtk_file_chooser_button_new( _("Select the shell you want to use"), GTK_FILE_CHOOSER_ACTION_OPEN) ;

  g_signal_connect(G_OBJECT(user_shell_file_chooser_button), "file-set", G_CALLBACK(check_user_shell), NULL) ;

  gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(user_shell_file_chooser_button), settings.user_shell) ;

  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(user_shell_file_chooser_button), PATH_TO_BIN_FOLDER) ;

  gtk_box_pack_start(GTK_BOX(user_shell_hbox), user_shell_label,               FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(user_shell_hbox), user_shell_file_chooser_button, TRUE,  TRUE,  0) ;

  gtk_container_set_border_width(GTK_CONTAINER(user_shell_file_chooser_button),  CONTAINER_PADDING) ;

  gtk_container_set_border_width(GTK_CONTAINER(user_shell_hbox),  CONTAINER_PADDING) ;

  gtk_box_set_spacing(GTK_BOX(user_shell_hbox), CONTAINER_PADDING) ;

  GtkWidget *terminal_user_shell_frame = generate_frame_with_content( _("User shell"), user_shell_hbox) ;




  GtkWidget *start_directory_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  GtkWidget *start_directory_label = gtk_label_new( _("Start directory          ") ) ;

  GtkWidget *start_directory_file_chooser_button = gtk_file_chooser_button_new( _("Select the terminal starting directory"), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER) ;

  gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(start_directory_file_chooser_button), settings.start_dir) ;

  gtk_box_pack_start(GTK_BOX(start_directory_hbox), start_directory_label,               FALSE, FALSE,   0) ;
  gtk_box_pack_start(GTK_BOX(start_directory_hbox), start_directory_file_chooser_button, TRUE,  TRUE,    0) ;

  gtk_container_set_border_width(GTK_CONTAINER(start_directory_file_chooser_button),  CONTAINER_PADDING) ;

  gtk_container_set_border_width(GTK_CONTAINER(start_directory_hbox),  CONTAINER_PADDING) ;

  gtk_box_set_spacing(GTK_BOX(start_directory_hbox), CONTAINER_PADDING) ;


  GtkWidget *terminal_start_dir_frame = generate_frame_with_content( _("Start directory"), start_directory_hbox) ;


  GtkWidget *audible_bell_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  GtkWidget *audible_bell_chkbox = generate_checkbutton( _("Audible bell"), settings.audible_bell, NULL) ;

  gtk_box_pack_start(GTK_BOX(audible_bell_vbox), audible_bell_chkbox,  TRUE, TRUE,   CONTAINER_PADDING) ;

  GtkWidget *terminal_audible_bell_frame = generate_frame_with_content( _("Audible bell"), audible_bell_vbox) ;


  GtkWidget *scrollback_lines_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  GtkWidget *scrollback_lines_unlimited = gtk_radio_button_new_with_label(NULL, _("Unlimited") ) ;

  GtkWidget *scrollback_lines_nb_hbox   = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  GtkWidget *scrollback_lines_other     = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(scrollback_lines_unlimited), _("Number of lines:") ) ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(scrollback_lines_unlimited), (settings.scrollback_lines == -1) ? TRUE : FALSE) ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(scrollback_lines_other), (settings.scrollback_lines != -1) ? TRUE : FALSE) ;




  GtkWidget *scrollback_lines_nb_spinner    = gtk_spin_button_new_with_range(0, 65535, 1) ;

  gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(scrollback_lines_nb_spinner), TRUE) ;
  gtk_spin_button_set_update_policy(GTK_SPIN_BUTTON(scrollback_lines_nb_spinner), GTK_UPDATE_IF_VALID) ;

  if (settings.scrollback_lines > -1) {
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(scrollback_lines_nb_spinner), settings.scrollback_lines) ;
  }

  gtk_container_set_border_width(GTK_CONTAINER(scrollback_lines_vbox), CONTAINER_PADDING) ;

  gtk_container_set_border_width(GTK_CONTAINER(scrollback_lines_nb_hbox), 0) ;

  gtk_box_set_spacing(GTK_BOX(scrollback_lines_nb_hbox), CONTAINER_PADDING) ;


  gtk_box_pack_start(GTK_BOX(scrollback_lines_nb_hbox), scrollback_lines_other,      FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(scrollback_lines_nb_hbox), scrollback_lines_nb_spinner, FALSE, FALSE, 0) ;


  gtk_box_pack_start(GTK_BOX(scrollback_lines_vbox), scrollback_lines_unlimited,  TRUE, TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(scrollback_lines_vbox), scrollback_lines_nb_hbox,    TRUE, TRUE, 0) ;



  GtkWidget *scrolling_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  GtkWidget *scroll_on_output_checkbutton = generate_checkbutton( _("Scroll on output"),    settings.scroll_on_output, NULL) ;

  GtkWidget *scroll_on_keystroke_checkbutton = generate_checkbutton( _("Scroll on keystroke"), settings.scroll_on_keystroke, NULL) ;


  gtk_box_pack_start(GTK_BOX(scrolling_vbox), scroll_on_output_checkbutton,    FALSE, FALSE, CONTAINER_PADDING) ;
  gtk_box_pack_start(GTK_BOX(scrolling_vbox), scroll_on_keystroke_checkbutton, FALSE, FALSE, CONTAINER_PADDING) ;

  gtk_container_set_border_width(GTK_CONTAINER(scrolling_vbox), CONTAINER_PADDING) ;

  GtkWidget *terminal_scrolling_frame = generate_frame_with_content( _("Scrolling"), scrolling_vbox) ;


  GtkWidget *erasing_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, CONTAINER_PADDING) ;

  GtkWidget *erasing_backspace_binding_hbox  = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *erasing_delete_binding_hbox     = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *erasing_backspace_binding_label = gtk_label_new( _("Backspace key generate: ") ) ;

  GtkWidget *erasing_delete_binding_label    = gtk_label_new( _("Delete key generate:       ") ) ;

  GtkWidget *erasing_backspace_binding_combo = gtk_combo_box_text_new() ;

  GtkWidget *erasing_delete_binding_combo    = gtk_combo_box_text_new() ;

  gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(erasing_backspace_binding_combo), 0, "Automatic" ) ;
  gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(erasing_backspace_binding_combo), 1, "ASCII backspace character (0x08)" ) ;
  gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(erasing_backspace_binding_combo), 2, "ASCII delete character (0x7F)" ) ;
  gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(erasing_backspace_binding_combo), 3, "@7 control sequence" ) ;
  gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(erasing_backspace_binding_combo), 4, "Terminal's erase setting" ) ;

  gtk_combo_box_set_active(GTK_COMBO_BOX(erasing_backspace_binding_combo), settings.backspace_binding) ;



  gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(erasing_delete_binding_combo), 0, "Automatic" ) ;
  gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(erasing_delete_binding_combo), 1, "ASCII backspace character (0x08)" ) ;
  gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(erasing_delete_binding_combo), 2, "ASCII delete character (0x7F)"  );
  gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(erasing_delete_binding_combo), 3, "@7 control sequence" ) ;
  gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(erasing_delete_binding_combo), 4, "Terminal's erase setting" ) ;

  gtk_combo_box_set_active(GTK_COMBO_BOX(erasing_delete_binding_combo), settings.delete_binding) ;

  gtk_box_pack_start(GTK_BOX(erasing_backspace_binding_hbox), erasing_backspace_binding_label, TRUE,  TRUE,   0) ;
  gtk_box_pack_start(GTK_BOX(erasing_backspace_binding_hbox), erasing_backspace_binding_combo, FALSE, FALSE,  0) ;

  gtk_box_pack_start(GTK_BOX(erasing_delete_binding_hbox), erasing_delete_binding_label, TRUE,  TRUE,   0) ;
  gtk_box_pack_start(GTK_BOX(erasing_delete_binding_hbox), erasing_delete_binding_combo, FALSE, FALSE,  0) ;

  gtk_box_set_spacing(GTK_BOX(erasing_backspace_binding_hbox), CONTAINER_PADDING) ;
  gtk_box_set_spacing(GTK_BOX(erasing_delete_binding_hbox),    CONTAINER_PADDING) ;


  gtk_box_pack_start(GTK_BOX(erasing_vbox), erasing_backspace_binding_hbox, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(erasing_vbox), erasing_delete_binding_hbox,    FALSE, FALSE, 0) ;

  gtk_container_set_border_width(GTK_CONTAINER(erasing_vbox), CONTAINER_PADDING) ;

  GtkWidget *terminal_erase_binding_frame = generate_frame_with_content( _("Erasing binding"), erasing_vbox) ;

  GtkWidget *terminal_appearance_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_box_set_spacing(GTK_BOX(terminal_appearance_vbox), CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(terminal_appearance_vbox), terminal_use_scheme_frame ,    FALSE, FALSE, 0) ;

  gtk_box_pack_start(GTK_BOX(terminal_appearance_vbox), terminal_font_scale_frame,      FALSE, FALSE, 0) ;

  gtk_box_pack_start(GTK_BOX(terminal_appearance_vbox), terminal_cursor_frame ,         FALSE, FALSE, 0) ;

  gtk_box_pack_start(GTK_BOX(terminal_appearance_vbox), terminal_colors_frame ,         FALSE, FALSE, 0) ;



  gtk_box_pack_start(GTK_BOX(terminal_appearance_vbox), terminal_bold_frame,            FALSE, FALSE, 0) ;



  gtk_box_pack_start(GTK_BOX(terminal_appearance_vbox), terminal_pointer_autohide,      TRUE,   TRUE, 0) ;

  GtkWidget *terminal_scrollback_frame = generate_frame_with_content( _("Scrollback lines"), scrollback_lines_vbox) ;

  gtk_notebook_append_page(GTK_NOTEBOOK(terminal_settings_notebook), terminal_appearance_vbox, gtk_label_new( _("Appearance") )) ;


  GtkWidget *terminal_settings_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;


  gtk_box_pack_start(GTK_BOX(terminal_settings_vbox), terminal_user_shell_frame,           FALSE, FALSE,  0) ;

  gtk_box_pack_start(GTK_BOX(terminal_settings_vbox), terminal_start_dir_frame,            FALSE, FALSE,  0) ;

  gtk_box_pack_start(GTK_BOX(terminal_settings_vbox), terminal_audible_bell_frame,         FALSE, FALSE,  0) ;

  gtk_box_pack_start(GTK_BOX(terminal_settings_vbox), terminal_scrollback_frame,           FALSE, FALSE,  0) ;

  gtk_box_pack_start(GTK_BOX(terminal_settings_vbox), terminal_scrolling_frame,            FALSE, FALSE,  0) ;

  gtk_box_pack_start(GTK_BOX(terminal_settings_vbox), terminal_erase_binding_frame,        TRUE,  TRUE,   0) ;


  gtk_notebook_append_page(GTK_NOTEBOOK(terminal_settings_notebook), terminal_settings_vbox, gtk_label_new( _("Settings") )) ;



  GtkWidget *files_manager_main_vbox          = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;


  GtkWidget *files_manager_action_buttonbox  = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;



  gtk_button_box_set_layout(GTK_BUTTON_BOX(files_manager_action_buttonbox), GTK_BUTTONBOX_SPREAD);

  gtk_container_set_border_width(GTK_CONTAINER(files_manager_action_buttonbox), CONTAINER_PADDING) ;


  GtkWidget *files_manager_delete_button    = gtk_button_new_with_label( _("Delete selected files"))  ;

  GtkWidget *files_manager_adding_button    = gtk_button_new_with_label( _("Add a file to manager") ) ;

  gtk_box_pack_start(GTK_BOX(files_manager_action_buttonbox), files_manager_delete_button, FALSE, FALSE, 0) ;

  gtk_box_pack_start(GTK_BOX(files_manager_action_buttonbox), files_manager_adding_button, FALSE, FALSE, 0) ;


  GtkWidget *link_scrolled_window           = gtk_scrolled_window_new(NULL, NULL) ;

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(link_scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );

  GtkWidget *files_manager_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_container_set_border_width(GTK_CONTAINER(files_manager_vbox), CONTAINER_PADDING) ;

  GError *error = NULL ;

  GIOChannel *gio_channel = g_io_channel_new_file(PATH_TO_LINKS_FILE, "r", &error) ;

  if (error != NULL) {

    display_message_dialog("Error open file !", error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_error_free(error) ;

    return ;

  }


  int c = 0 ;

  while (TRUE) {

    gchar *label = NULL ;

    // Getting the title from the entry (button label text).
    if (g_io_channel_read_line(gio_channel, &label, NULL, NULL, NULL) != G_IO_STATUS_NORMAL) {

      break ;

    }

    char *label_linefeed = strrchr(label,'\n') ;

    if ( label_linefeed != NULL  ) {
      label[label_linefeed - label] = '\0' ;
    }

    gchar *uri = NULL ;
    // Getting the URI corresponding to the file to be launched.
    if (g_io_channel_read_line(gio_channel, &uri, NULL, NULL, NULL) != G_IO_STATUS_NORMAL) {

      break ;

    }

    label_linefeed = strrchr(uri,'\n') ;

    if ( label_linefeed != NULL  ) {
      uri[label_linefeed - uri] = '\0' ;
    }


    Files_Manager *file_manager_item = g_malloc(sizeof(Files_Manager)) ;
    file_manager_item->label = g_strdup(label) ;
    file_manager_item->uri   = g_strdup(uri)   ;


    files_manager_list = g_slist_insert(files_manager_list, file_manager_item, c);

    GtkWidget *files_checkbutton = gtk_check_button_new_with_label(label) ;

    g_object_set_data(G_OBJECT(files_checkbutton), "item", file_manager_item) ;

    gtk_box_pack_start(GTK_BOX(files_manager_vbox), files_checkbutton, FALSE, FALSE, 0) ;

    free(label) ;
    free(uri)   ;

    c++ ;

  }

  g_io_channel_shutdown(gio_channel, FALSE, NULL) ;

  g_signal_connect(G_OBJECT(files_manager_delete_button), "clicked", G_CALLBACK(remove_selected_items_from_files_manager), files_manager_vbox) ;
  g_signal_connect(G_OBJECT(files_manager_adding_button), "clicked", G_CALLBACK(add_a_new_item_to_files_manager), files_manager_vbox) ;

  GtkWidget *files_manager_delete_image  = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "list-remove.png");

  g_object_set(G_OBJECT(files_manager_delete_button), "always-show-image", TRUE, NULL ) ;

  gtk_button_set_image(GTK_BUTTON(files_manager_delete_button), files_manager_delete_image);


  GtkWidget *files_manager_adding_image  = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "list-add.png");

  g_object_set(G_OBJECT(files_manager_adding_button), "always-show-image", TRUE, NULL ) ;

  gtk_button_set_image(GTK_BUTTON(files_manager_adding_button), files_manager_adding_image);




  gtk_container_add(GTK_CONTAINER(link_scrolled_window), files_manager_vbox) ;

  GtkWidget *files_manager_list_frame = generate_frame_with_content( _("Files manager list"), link_scrolled_window) ;

  GtkWidget *files_manager_action_frame = generate_frame_with_content( _("Actions"), files_manager_action_buttonbox) ;

  gtk_box_pack_start(GTK_BOX(files_manager_main_vbox), files_manager_list_frame,       TRUE,  TRUE,  0) ;
  gtk_box_pack_start(GTK_BOX(files_manager_main_vbox), files_manager_action_frame,     FALSE, FALSE, 0) ;






  gtk_notebook_append_page(GTK_NOTEBOOK(notebook_configuration), terminal_settings_notebook, gtk_label_new( _("Terminals Settings") )) ;

  gtk_notebook_append_page(GTK_NOTEBOOK(notebook_configuration), files_manager_main_vbox,    gtk_label_new( _("Files handler manager") )) ;



  gtk_container_add(GTK_CONTAINER(configuration_dialog_content), notebook_configuration) ;




  GtkWidget *gui_application_notebook           = gtk_notebook_new() ;


  GtkWidget *gui_application_main_vbox          = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;



  GtkWidget *gui_application_others_frame       = gtk_frame_new( _("Others") ) ;

  GtkWidget *gui_application_action_frame       = gtk_frame_new( _("Actions") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(gui_application_others_frame),       FRAME_BORDER_SIZE) ;

  gtk_container_set_border_width(GTK_CONTAINER(gui_application_action_frame),       FRAME_BORDER_SIZE) ;



  GKeyFile *conf_file = g_key_file_new() ;

  g_key_file_load_from_file(conf_file, PATH_TO_APP_FILE, G_KEY_FILE_NONE, NULL) ;

  gui_app.others = g_key_file_get_string_list(conf_file, "Others",  "others",  &gui_app.nb_of_others, NULL)  ;

  GtkWidget *gui_application_programming_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;


  GtkWidget *gui_application_programming_diff_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *gui_application_programming_diff_chooser = generate_applications_chooser(gui_application_programming_diff_hbox, PATH_TO_BUTTON_ICON "text-x-ldif.png", _("Diff GUI tool"), gui_app.diff) ;

  gtk_box_pack_start(GTK_BOX(gui_application_programming_vbox), gui_application_programming_diff_hbox, FALSE, FALSE, 0) ;


  GtkWidget *gui_application_programming_debugger_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *gui_application_programming_debugger_chooser = generate_applications_chooser(gui_application_programming_debugger_hbox, PATH_TO_BUTTON_ICON "debug-run.png", _("Debugger GUI tool"), gui_app.debugger) ;

  gtk_box_pack_start(GTK_BOX(gui_application_programming_vbox), gui_application_programming_debugger_hbox, FALSE, FALSE, 0) ;


  GtkWidget *gui_application_programming_python_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *gui_application_programming_python_chooser = generate_applications_chooser(gui_application_programming_python_hbox, PATH_TO_BUTTON_ICON "python.png", _("Python smart interpreter"), gui_app.python) ;

  gtk_box_pack_start(GTK_BOX(gui_application_programming_vbox), gui_application_programming_python_hbox, FALSE, FALSE, 0) ;




  GtkWidget *gui_application_programming_gui_designer_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *gui_application_programming_gui_designer_chooser = generate_applications_chooser(gui_application_programming_gui_designer_hbox, PATH_TO_BUTTON_ICON "kde-windows.png", _("GUI designer tool"), gui_app.gui_designer) ;

  gtk_box_pack_start(GTK_BOX(gui_application_programming_vbox), gui_application_programming_gui_designer_hbox, FALSE, FALSE, 0) ;


  GtkWidget *gui_application_programming_devhelp_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *gui_application_programming_devhelp_chooser = generate_applications_chooser(gui_application_programming_devhelp_hbox, PATH_TO_BUTTON_ICON "devhelp.png", "Devhelp", gui_app.devhelp) ;

  gtk_box_pack_start(GTK_BOX(gui_application_programming_vbox), gui_application_programming_devhelp_hbox, FALSE, FALSE, 0) ;



  GtkWidget *gui_application_programming_frame = generate_frame_with_content( _("Programing"), gui_application_programming_vbox) ;



  GtkWidget *gui_application_utils_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  GtkWidget *gui_application_utils_calculator_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *gui_application_utils_calculator_chooser = generate_applications_chooser(gui_application_utils_calculator_hbox, PATH_TO_BUTTON_ICON "accessories-calculator.png", _("Calculator GUI tool"), gui_app.calculator) ;

  gtk_box_pack_start(GTK_BOX(gui_application_utils_vbox), gui_application_utils_calculator_hbox, FALSE, FALSE, 0) ;



  GtkWidget *gui_application_utils_color_picker_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *gui_application_utils_color_picker_chooser = generate_applications_chooser(gui_application_utils_color_picker_hbox, PATH_TO_BUTTON_ICON "kcolorchooser.png", _("Color picker GUI tool"), gui_app.color_picker) ;

  gtk_box_pack_start(GTK_BOX(gui_application_utils_vbox), gui_application_utils_color_picker_hbox, FALSE, FALSE, 0) ;



  GtkWidget *gui_application_utils_dictionary_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *gui_application_utils_dictionary_chooser = generate_applications_chooser(gui_application_utils_dictionary_hbox, PATH_TO_BUTTON_ICON "accessories-dictionary.png", _("Dictionary GUI tool"), gui_app.dictionary) ;

  gtk_box_pack_start(GTK_BOX(gui_application_utils_vbox), gui_application_utils_dictionary_hbox, FALSE, FALSE, 0) ;




  GtkWidget *gui_application_utils_file_manager_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *gui_application_utils_file_manager_chooser = generate_applications_chooser(gui_application_utils_file_manager_hbox, PATH_TO_BUTTON_ICON "system-file-manager.png", _("File manager"), gui_app.file_manager) ;

  gtk_box_pack_start(GTK_BOX(gui_application_utils_vbox), gui_application_utils_file_manager_hbox, FALSE, FALSE, 0) ;


  GtkWidget *gui_application_utils_notes_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *gui_application_utils_notes_chooser = generate_applications_chooser(gui_application_utils_notes_hbox, PATH_TO_BUTTON_ICON "knotes.png", _("Notes taker GUI tool"), gui_app.notes) ;

  gtk_box_pack_start(GTK_BOX(gui_application_utils_vbox), gui_application_utils_notes_hbox, FALSE, FALSE, 0) ;



  GtkWidget *gui_application_utils_browser_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *gui_application_utils_browser_chooser = generate_applications_chooser(gui_application_utils_browser_hbox, PATH_TO_BUTTON_ICON "internet-web-browser.png", _("Browser"), gui_app.browser) ;

  gtk_box_pack_start(GTK_BOX(gui_application_utils_vbox), gui_application_utils_browser_hbox, FALSE, FALSE, 0) ;

  GtkWidget *gui_application_utils_frame = generate_frame_with_content( _("Utils"), gui_application_utils_vbox) ;




  GtkWidget *gui_application_others_main_vbox          = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  GtkWidget *gui_application_others_action_buttonbox  = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;



  gtk_button_box_set_layout(GTK_BUTTON_BOX(gui_application_others_action_buttonbox), GTK_BUTTONBOX_SPREAD);

  gtk_container_set_border_width(GTK_CONTAINER(gui_application_others_action_buttonbox), CONTAINER_PADDING) ;


  GtkWidget *gui_application_others_delete_button    = gtk_button_new_with_label( _("Delete applications") ) ;

  GtkWidget *gui_application_others_adding_button    = gtk_button_new_with_label( _("Add an application") ) ;

  gtk_box_pack_start(GTK_BOX(gui_application_others_action_buttonbox), gui_application_others_delete_button, FALSE, FALSE, 0) ;

  gtk_box_pack_start(GTK_BOX(gui_application_others_action_buttonbox), gui_application_others_adding_button, FALSE, FALSE, 0) ;


  GtkWidget *gui_application_others_scrolled_window  = gtk_scrolled_window_new(NULL, NULL) ;

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(gui_application_others_scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );

  GtkWidget *gui_application_others_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_container_set_border_width(GTK_CONTAINER(gui_application_others_vbox), CONTAINER_PADDING) ;




  if (gui_app.nb_of_others == 0) {

    goto list_empty ;

  }

  c = 0 ;

  while (gui_app.others[c] != NULL) {

    GtkWidget *files_checkbutton = gtk_check_button_new_with_label( g_path_get_basename(gui_app.others[c]) ) ;

    gtk_box_pack_start(GTK_BOX(gui_application_others_vbox), files_checkbutton, FALSE, FALSE, 0) ;

    ++c ;

  }

  list_empty :


  g_strfreev(gui_app.others) ;


  g_signal_connect(G_OBJECT(gui_application_others_delete_button), "clicked", G_CALLBACK(remove_selected_items_from_applications), gui_application_others_vbox) ;
  g_signal_connect(G_OBJECT(gui_application_others_adding_button), "clicked", G_CALLBACK(add_a_new_item_to_applications), gui_application_others_vbox) ;


  GtkWidget *gui_application_others_delete_image  = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "list-remove.png");

  g_object_set(G_OBJECT(gui_application_others_delete_button), "always-show-image", TRUE, NULL ) ;

  gtk_button_set_image(GTK_BUTTON(gui_application_others_delete_button), gui_application_others_delete_image);


  GtkWidget *gui_application_others_adding_image  = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "list-add.png");

  g_object_set(G_OBJECT(gui_application_others_adding_button), "always-show-image", TRUE, NULL ) ;

  gtk_button_set_image(GTK_BUTTON(gui_application_others_adding_button), gui_application_others_adding_image);




  gtk_container_add(GTK_CONTAINER(gui_application_others_scrolled_window), gui_application_others_vbox) ;

  GtkWidget *gui_application_others_list_frame = generate_frame_with_content( _("User defined applications list"), gui_application_others_scrolled_window ) ;

  GtkWidget *gui_application_others_action_frame = generate_frame_with_content( _("Actions"), gui_application_others_action_buttonbox) ;



  gtk_box_pack_start(GTK_BOX(gui_application_others_main_vbox), gui_application_others_list_frame,       TRUE,  TRUE,  0) ;
  gtk_box_pack_start(GTK_BOX(gui_application_others_main_vbox), gui_application_others_action_frame,     FALSE, FALSE, 0) ;



  gtk_box_pack_start(GTK_BOX(gui_application_main_vbox), gui_application_programming_frame, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui_application_main_vbox), gui_application_utils_frame,       FALSE, FALSE, 0) ;

  gtk_notebook_append_page(GTK_NOTEBOOK(gui_application_notebook), gui_application_main_vbox, gtk_label_new( _("Predefined applications") )) ;

  gtk_notebook_append_page(GTK_NOTEBOOK(gui_application_notebook), gui_application_others_main_vbox, gtk_label_new( _("User defined applications")  )) ;

  gtk_notebook_append_page(GTK_NOTEBOOK(notebook_configuration), gui_application_notebook, gtk_label_new( _("Applications")  )) ;







  GtkWidget *gui_configuration_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_box_set_spacing(GTK_BOX(gui_configuration_vbox), CONTAINER_PADDING) ;


  GtkWidget *gui_sidebar_factor_vbox  = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  gtk_container_set_border_width(GTK_CONTAINER(gui_sidebar_factor_vbox),  CONTAINER_PADDING) ;

  gtk_box_set_spacing(GTK_BOX(gui_sidebar_factor_vbox), CONTAINER_PADDING) ;

  GtkWidget *gui_sidebar_factor_label  = gtk_label_new( _("Ratio of the size of sidebar left-right") ) ;

  gtk_label_set_justify(GTK_LABEL(gui_sidebar_factor_label), GTK_JUSTIFY_LEFT);

  GtkWidget *gui_sidebar_factor_spinbutton  = gtk_spin_button_new_with_range(0.0, 4.0, 0.01) ;

  g_signal_connect(gui_sidebar_factor_spinbutton, "value-changed", G_CALLBACK(update_sidebar_position), NULL) ;

  gtk_spin_button_set_digits(GTK_SPIN_BUTTON(gui_sidebar_factor_spinbutton), 3) ;

  gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(gui_sidebar_factor_spinbutton), TRUE) ;

  gtk_spin_button_set_update_policy(GTK_SPIN_BUTTON(gui_sidebar_factor_spinbutton), GTK_UPDATE_IF_VALID) ;

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(gui_sidebar_factor_spinbutton), settings.side_terms_factor) ;


  gtk_box_pack_start(GTK_BOX(gui_sidebar_factor_vbox), gui_sidebar_factor_label     , FALSE, FALSE,  CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(gui_sidebar_factor_vbox), gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0), TRUE, TRUE, 0) ;

  gtk_box_pack_start(GTK_BOX(gui_sidebar_factor_vbox), gui_sidebar_factor_spinbutton, FALSE, FALSE, CONTAINER_PADDING) ;


  GtkWidget *gui_sidebar_factor_frame = generate_frame_with_content( _("Editor / Sidebar space factor"), gui_sidebar_factor_vbox) ;



  GtkWidget *show_terminals_sidebar_checkbutton = generate_checkbutton( _("Show Terminals-sidebar at start ?"), settings.side_terms_on, NULL) ;

  GtkWidget *show_big_term_checkbutton = generate_checkbutton( _("Display Big-terminal at start ?"), settings.big_term_on, NULL) ;

  GtkWidget *show_big_term_divided_checkbutton = generate_checkbutton( _("Divide Big-terminal in 4 at start ?"), settings.big_term_div, NULL) ;

  GtkWidget *show_buttonbar_checkbutton = generate_checkbutton( _("Show buttonbar at start ?"), settings.buttonbar_on, NULL) ;

  GtkWidget *show_fullscreen_checkbutton = generate_checkbutton( _("Toggle to fullscreen at start ?"), settings.fullscreen, NULL) ;


  GtkWidget *startup_settings_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(startup_settings_vbox), show_terminals_sidebar_checkbutton,          TRUE, TRUE, CONTAINER_PADDING) ;
  gtk_box_pack_start(GTK_BOX(startup_settings_vbox), show_big_term_checkbutton,       TRUE, TRUE, CONTAINER_PADDING) ;
  gtk_box_pack_start(GTK_BOX(startup_settings_vbox), show_buttonbar_checkbutton,         TRUE, TRUE, CONTAINER_PADDING) ;
  gtk_box_pack_start(GTK_BOX(startup_settings_vbox), show_fullscreen_checkbutton,      TRUE, TRUE, CONTAINER_PADDING) ;


  GtkWidget *startup_settings_frame = generate_frame_with_content( _("Settings at start"), startup_settings_vbox) ;

  GtkWidget *gui_set_as_default_vbox  = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;

  gtk_container_set_border_width(GTK_CONTAINER(gui_set_as_default_vbox),  CONTAINER_PADDING) ;

  gtk_box_set_spacing(GTK_BOX(gui_set_as_default_vbox), CONTAINER_PADDING) ;

  GtkWidget *gui_set_as_default_button = gtk_button_new_with_label( _("Set it-edit as default editor") ) ;

  GtkWidget *gui_set_as_default_reset_button = gtk_button_new_with_label( _("Reset to default") ) ;

  gtk_box_pack_start(GTK_BOX(gui_set_as_default_vbox), gui_set_as_default_button,       TRUE, TRUE, CONTAINER_PADDING) ;
  gtk_box_pack_start(GTK_BOX(gui_set_as_default_vbox), gui_set_as_default_reset_button, TRUE, TRUE, CONTAINER_PADDING) ;

  gtk_container_set_border_width(GTK_CONTAINER(gui_set_as_default_button),       CONTAINER_PADDING) ;
  gtk_container_set_border_width(GTK_CONTAINER(gui_set_as_default_reset_button), CONTAINER_PADDING) ;

  g_signal_connect(G_OBJECT(gui_set_as_default_button),       "clicked", G_CALLBACK(set_as_default), NULL) ;
  g_signal_connect(G_OBJECT(gui_set_as_default_reset_button), "clicked", G_CALLBACK(reset_default), NULL) ;

  GtkWidget *gui_set_as_default_frame = generate_frame_with_content( _("Set as default editor"), gui_set_as_default_vbox) ;


  GtkWidget *gui_launch_cmd_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, CONTAINER_PADDING) ;

  gtk_container_set_border_width(GTK_CONTAINER(gui_launch_cmd_vbox),  CONTAINER_PADDING) ;

  gtk_box_set_spacing(GTK_BOX(gui_launch_cmd_vbox), CONTAINER_PADDING) ;

  GtkWidget *gui_launch_cmd_label = gtk_label_new( _("Launch a command in all terminals at start:") ) ;

  GtkWidget *gui_launch_cmd_entry = gtk_entry_new() ;

  gtk_widget_set_tooltip_text(gui_launch_cmd_entry, _("Per example: set -o history ; ls") ) ;

  if (settings.command != NULL) {

    gtk_entry_set_text(GTK_ENTRY(gui_launch_cmd_entry), settings.command) ;
  }

  GtkWidget *gui_launch_cmd_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(gui_launch_cmd_hbox), gui_launch_cmd_entry, TRUE, TRUE, CONTAINER_PADDING) ;

  gtk_container_set_border_width(GTK_CONTAINER(gui_launch_cmd_hbox), CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(gui_launch_cmd_vbox), gui_launch_cmd_label, FALSE, FALSE, CONTAINER_PADDING) ;
  gtk_box_pack_start(GTK_BOX(gui_launch_cmd_vbox), gui_launch_cmd_hbox,  FALSE, FALSE, CONTAINER_PADDING) ;


  GtkWidget *gui_launch_cmd_frame = generate_frame_with_content( _("Launch command at start"), gui_launch_cmd_vbox) ;


  GtkWidget *session_configuration_asked      = gtk_radio_button_new_with_label(NULL, _("Ask at exit for saving session.") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(session_configuration_asked), CONTAINER_PADDING) ;

  gint value_asked = 1 ;

  g_signal_connect(session_configuration_asked, "toggled", G_CALLBACK(set_session_mode), &value_asked) ;

  GtkWidget *session_configuration_automatic  = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(session_configuration_asked), _("Automatic session registering mechanic.") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(session_configuration_automatic), CONTAINER_PADDING) ;

  gint value_automatic = -1 ;

  g_signal_connect(session_configuration_automatic, "toggled", G_CALLBACK(set_session_mode), &value_automatic) ;

  GtkWidget *session_configuration_disabled   = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(session_configuration_asked), _("Disable session registering.") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(session_configuration_disabled), CONTAINER_PADDING) ;

  gint value_disabled = 0 ;

  g_signal_connect(session_configuration_disabled, "toggled", G_CALLBACK(set_session_mode), &value_disabled) ;

  switch (settings.session_mode) {

    case -1 :

      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(session_configuration_automatic), TRUE) ;
      break ;

    case 0 :

      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(session_configuration_disabled), TRUE) ;
      break ;

    case 1 :

      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(session_configuration_asked), TRUE) ;
      break ;

  }


  GtkWidget *session_configuration_vbox       = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_box_pack_start(GTK_BOX(session_configuration_vbox), session_configuration_asked,      FALSE, FALSE, 2) ;
  gtk_box_pack_start(GTK_BOX(session_configuration_vbox), session_configuration_automatic,  FALSE, FALSE, 2) ;
  gtk_box_pack_start(GTK_BOX(session_configuration_vbox), session_configuration_disabled,   FALSE, FALSE, 2) ;

  GtkWidget *session_configuration_frame = generate_frame_with_content( _("Session management"), session_configuration_vbox) ;



  gtk_box_pack_start(GTK_BOX(gui_configuration_vbox), gui_sidebar_factor_frame,         FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui_configuration_vbox), startup_settings_frame,           FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui_configuration_vbox), gui_launch_cmd_frame,             FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui_configuration_vbox), gui_set_as_default_frame,         FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui_configuration_vbox), session_configuration_frame,      TRUE,  TRUE,  0) ;


  gtk_notebook_append_page(GTK_NOTEBOOK(notebook_configuration), gui_configuration_vbox, gtk_label_new( _("Main settings") )) ;



  gtk_widget_show_all(configuration_dialog) ;

  gint result = gtk_dialog_run(GTK_DIALOG(configuration_dialog));

  if (result ==  GTK_RESPONSE_APPLY) {



    settings.grid_background      = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(grid_background_checkbutton))    ;
    settings.display_line_numbers = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_lines_checkbutton))         ;
    settings.display_tabs_chars   = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_tabs_checkbutton))          ;
    settings.use_auto_indent      = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(auto_indent_checkbutton))        ;
    settings.use_right_margin     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(right_margin_checkbutton))       ;
    settings.use_monospace_font   = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(use_monospace_font_checkbutton)) ;

    settings.indent_width         = (gint) gtk_spin_button_get_value(GTK_SPIN_BUTTON(indent_width_spinbutton))    ;

    if (settings.use_right_margin) {

      settings.right_margin_value = (gint) gtk_spin_button_get_value(GTK_SPIN_BUTTON(right_margin_spinbutton))    ;

    }

    settings.use_spaces_as_tabs   = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(use_spaces_instead_of_tabs))   ;

    settings.tabs_width           = (gint) gtk_spin_button_get_value(GTK_SPIN_BUTTON(tabs_width_spinbutton))      ;


    settings.backup_file          = gtk_switch_get_active(GTK_SWITCH(save_file_backup_file_switch))               ;


    g_free(settings.term_font) ;

    settings.term_font = g_strdup(gtk_font_button_get_font_name(GTK_FONT_BUTTON(terminal_use_font_button))) ;

    if (gtk_font_button_get_font_name(GTK_FONT_BUTTON(editor_font_button)) != NULL) {

      g_free(settings.editor_font) ;

      settings.editor_font = g_strdup(gtk_font_button_get_font_name(GTK_FONT_BUTTON(editor_font_button))) ;

    }


    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cursor_shape_block_radiobutton))  ) {
      settings.cursor_shape = 0 ;
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cursor_shape_ibeam_radiobutton))  ) {
      settings.cursor_shape = 1 ;
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cursor_shape_underline_radiobutton))  ) {
      settings.cursor_shape = 2 ;
    }



    GdkRGBA term_bg ;
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(terminal_bg_color_button), &term_bg);

    g_free(settings.term_bg) ;

    settings.term_bg = g_strdup_printf("#%02x%02x%02x", (int) (term_bg.red * 255.0) , (int) (term_bg.green * 255.0), (int) (term_bg.blue * 255.0) ) ;


    GdkRGBA term_fg ;
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(terminal_fg_color_button), &term_fg);

    g_free(settings.term_fg) ;

    settings.term_fg = g_strdup_printf("#%02x%02x%02x", (int) (term_fg.red * 255.0) , (int) (term_fg.green * 255.0) , (int) (term_fg.blue * 255.0) ) ;


    g_free(settings.user_shell) ;

    settings.user_shell        = g_strdup(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(user_shell_file_chooser_button))) ;



    g_free(settings.start_dir)  ;

    settings.start_dir         = g_strdup(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(start_directory_file_chooser_button))) ;



    settings.scrollback_lines  = ((gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(scrollback_lines_unlimited)) == TRUE) ? -1 : gtk_spin_button_get_value(GTK_SPIN_BUTTON(scrollback_lines_nb_spinner))) ;

    settings.scroll_on_output     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(scroll_on_output_checkbutton)) ;
    settings.scroll_on_keystroke  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(scroll_on_keystroke_checkbutton)) ;


    settings.backspace_binding    = gtk_combo_box_get_active(GTK_COMBO_BOX(erasing_backspace_binding_combo)) ;
    settings.delete_binding       = gtk_combo_box_get_active(GTK_COMBO_BOX(erasing_delete_binding_combo)) ;


    g_free(settings.command) ;


    settings.command              = g_strdup(gtk_entry_get_text(GTK_ENTRY(gui_launch_cmd_entry))) ;

    settings.font_scale           = gtk_spin_button_get_value(GTK_SPIN_BUTTON(terminal_font_scale_spinbutton))   ;

    settings.cursor_blink         = gtk_combo_box_get_active(GTK_COMBO_BOX(terminal_cursor_blinking_combo)) ;

    settings.bold_allow           = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(terminal_bold_allowed_checkbutton)) ;


    if (*bold_color_activated) {



      g_free(settings.bold_color) ;

      GdkRGBA bold_color ;
      gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(terminal_bold_color_button), &bold_color) ;

      settings.bold_color= g_strdup_printf("#%02x%02x%02x", (int) (bold_color.red * 255.0) , (int) (bold_color.green * 255.0), (int) (bold_color.blue * 255.0) ) ;

    }



    if (*cursor_color_activated) {

      g_free(settings.cursor_color) ;

      GdkRGBA cursor_color ;
      gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(terminal_cursor_color_button), &cursor_color) ;

      settings.cursor_color = g_strdup_printf("#%02x%02x%02x", (int) (cursor_color.red * 255.0) , (int) (cursor_color.green * 255.0), (int) (cursor_color.blue * 255.0) ) ;

    }


    settings.side_terms_factor = gtk_spin_button_get_value(GTK_SPIN_BUTTON(gui_sidebar_factor_spinbutton)) ;


    g_free(bold_color_activated) ;
    g_free(cursor_color_activated) ;


    g_free(settings.charset) ;

    settings.charset              =  gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(save_file_encoding_comboboxtext))    ;

    settings.overwrite_anyway     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(save_file_despite_modification_timestamp)) ;

    if (gtk_combo_box_get_active(GTK_COMBO_BOX(save_file_newline_comboboxtext)) ==  0) {

      settings.newline_type         = GTK_SOURCE_NEWLINE_TYPE_LF ;

    }
    else if (gtk_combo_box_get_active(GTK_COMBO_BOX(save_file_newline_comboboxtext)) ==  1) {

      settings.newline_type         = GTK_SOURCE_NEWLINE_TYPE_CR ;

    }
    else if (gtk_combo_box_get_active(GTK_COMBO_BOX(save_file_newline_comboboxtext)) ==  2) {

      settings.newline_type         = GTK_SOURCE_NEWLINE_TYPE_CR_LF ;

    }

    /** TODO: update scheme into menubar. **/

    settings.display_all_spaces   = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_spaces_checkbutton)) ;

    settings.warn_read_only       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(open_file_warn_readonly_chkbox)) ;
    settings.warn_file_open       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(open_file_warn_already_in_editor_chkbox)) ;
    settings.warn_file_save       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(close_file_unsaved_checkbutton)) ;

    #if 0
    /** @No proposal into the window fore this stupid option ! **/
    settings.notifications        = FALSE;
    #endif


    mask = 0 ;

    set_perm_value(save_file_mask_ur_checkbutton, &mask, S_IRUSR) ;
    set_perm_value(save_file_mask_uw_checkbutton, &mask, S_IWUSR) ;
    set_perm_value(save_file_mask_ux_checkbutton, &mask, S_IXUSR) ;

    set_perm_value(save_file_mask_gr_checkbutton, &mask, S_IRGRP) ;
    set_perm_value(save_file_mask_gw_checkbutton, &mask, S_IWGRP) ;
    set_perm_value(save_file_mask_gx_checkbutton, &mask, S_IXGRP) ;

    set_perm_value(save_file_mask_or_checkbutton, &mask, S_IROTH) ;
    set_perm_value(save_file_mask_ow_checkbutton, &mask, S_IWOTH) ;
    set_perm_value(save_file_mask_ox_checkbutton, &mask, S_IXOTH) ;

    settings.save_file_mode = mask ;

    settings.side_terms_on = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_terminals_sidebar_checkbutton)) ;

    settings.big_term_on   = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_big_term_checkbutton)) ;

    settings.buttonbar_on  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_buttonbar_checkbutton)) ;

    settings.fullscreen    = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_fullscreen_checkbutton)) ;

    settings.big_term_div  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_big_term_divided_checkbutton)) ;


    settings.spellcheck_inline = FALSE ;


    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(session_configuration_automatic)) ) {

      settings.session_mode = -1 ;
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(session_configuration_disabled)) )  {

      settings.session_mode = 0 ;
    }
    else {

      settings.session_mode = 1 ;
    }


    set_applications(gui_app.diff,         gui_application_programming_diff_chooser) ;
    set_applications(gui_app.debugger,     gui_application_programming_debugger_chooser) ;
    set_applications(gui_app.python,       gui_application_programming_python_chooser) ;
    set_applications(gui_app.gui_designer, gui_application_programming_gui_designer_chooser) ;
    set_applications(gui_app.devhelp,      gui_application_programming_devhelp_chooser) ;


    set_applications(gui_app.calculator,   gui_application_utils_calculator_chooser) ;
    set_applications(gui_app.color_picker, gui_application_utils_color_picker_chooser) ;
    set_applications(gui_app.dictionary,   gui_application_utils_dictionary_chooser) ;
    set_applications(gui_app.file_manager, gui_application_utils_file_manager_chooser) ;
    set_applications(gui_app.notes,        gui_application_utils_notes_chooser) ;
    set_applications(gui_app.browser,      gui_application_utils_browser_chooser) ;

    /** Write settings to configuration file. **/
    write_changes_to_conf_file() ;
    write_changes_to_app_file() ;



    change_applications(gui_app.diff,         gui->menuitems->app_programming_diff_menuitem) ;
    change_applications(gui_app.debugger,     gui->menuitems->app_programming_debugger_menuitem) ;
    change_applications(gui_app.python,       gui->menuitems->app_programming_python_menuitem) ;
    change_applications(gui_app.gui_designer, gui->menuitems->app_programming_gui_designer_menuitem) ;
    change_applications(gui_app.devhelp,      gui->menuitems->app_programming_devhelp_menuitem) ;
    change_applications(gui_app.calculator,   gui->menuitems->app_utils_calculator_menuitem) ;
    change_applications(gui_app.color_picker, gui->menuitems->app_utils_color_picker_menuitem) ;
    change_applications(gui_app.dictionary,   gui->menuitems->app_utils_dictionary_menuitem) ;
    change_applications(gui_app.file_manager, gui->menuitems->app_utils_file_manager_menuitem) ;
    change_applications(gui_app.notes,        gui->menuitems->app_utils_notes_menuitem) ;
    change_applications(gui_app.browser,      gui->menuitems->app_utils_browser_menuitem) ;


    /** Editor changes application. **/
    apply_editor_change() ;

    /** Terminals changes application. **/
    apply_terminal_change() ;


    while (gtk_events_pending()) {
       /** Waiting for configuration changes completion. **/
       gtk_main_iteration() ;
    }

  }

  gtk_widget_destroy(configuration_dialog) ;

  return ;

}
