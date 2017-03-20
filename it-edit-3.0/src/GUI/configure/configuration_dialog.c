/** ***********************************************************************************
  * it-edit the Integrated Terminal Editor: a text editor with severals               *
  * integrated functionalities.                                                      *
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

static GtkWidget *generate_checkbuton(const gchar *label, gboolean is_active) ;

static void set_applications(char *app, GtkWidget *file_chooser) ;

static void set_session_mode(GtkWidget *widget, gint *value) ;

static void change_applications(char *app_name, GtkWidget *menu_item) ;

static void set_as_default(GtkWidget *widget, gpointer user_data) ;

static void update_sidebar_position(GtkSpinButton *spin_button) ;

static void reset_default(GtkWidget *widget, gpointer user_data) ;

static gboolean is_perm_value_set(const gint mask, const gint bit)  ;

static void set_perm_value(GtkWidget *widget, gint *mask,  gint value) ;

static void update_scheme_menu_items(const gchar *scheme_id) ;



static void update_scheme_menu_items(const gchar *scheme_id) {

  GList *g_list_scheme_menu = gtk_container_get_children(GTK_CONTAINER(gui->menus->menu_scheme)) ;

  g_list_scheme_menu = g_list_first(g_list_scheme_menu) ;

  while (g_list_scheme_menu) {

    if (g_list_scheme_menu->data != NULL) {

      gchar *scheme = g_object_get_data(G_OBJECT(g_list_scheme_menu->data), "scheme_id") ;

      if ( g_strcmp0(scheme, scheme_id) == 0 ) {
 
        gtk_widget_activate(g_list_scheme_menu->data) ;
 
        break ;

      }
    }

    if (g_list_scheme_menu->next != NULL) {

      g_list_scheme_menu = g_list_scheme_menu->next ;

    }
    else {

      break ;

    }

  }

  return ;

}

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

static GtkWidget *generate_checkbuton(const gchar *label, gboolean is_active) {

  GtkWidget *checkbutton = gtk_check_button_new_with_label(label) ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), is_active) ;

  gtk_container_set_border_width(GTK_CONTAINER(checkbutton), CONTAINER_PADDING) ;

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

    if (mimetypes != NULL) {

      int c = 0 ;

      while (mimetypes[c] != NULL) {

        g_app_info_set_as_default_for_type(it_edit_app, mimetypes[c], NULL) ;

        ++c ;
      }

    }

    g_strfreev(mimetypes) ;

    ++ids ;

 }


 /** I can't set text/plain as default because of too much unwanted side-effects.
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

#if 0
static gchar *scheme_id = NULL ;


static void set_editor_scheme(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *dialog = gtk_dialog_new_with_buttons( _("Select a scheme"),
                                                  GTK_WINDOW(gui->main_window),
                                                  GTK_DIALOG_USE_HEADER_BAR | GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                  NULL,
                                                  0,
                                                  NULL) ;
   
   
  gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE) ;


  GtkWidget *select_button = gtk_dialog_add_button(GTK_DIALOG(dialog),_("Choose"), GTK_RESPONSE_APPLY) ;
  GtkWidget *cancel_button = gtk_dialog_add_button(GTK_DIALOG(dialog),_("Cancel"), GTK_RESPONSE_CANCEL) ;

  GtkWidget *select_image = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "gtk-select-color.png") ;
  GtkWidget *cancel_image = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "dialog-cancel.png") ;


  g_object_set(G_OBJECT(select_button), "always-show-image", TRUE, NULL) ;
  g_object_set(G_OBJECT(cancel_button), "always-show-image", TRUE, NULL) ;

  gtk_button_set_image(GTK_BUTTON(select_button), select_image) ;
  gtk_button_set_image(GTK_BUTTON(cancel_button), cancel_image) ;



  GtkWidget *source_style_scheme_chooser_widget = gtk_source_style_scheme_chooser_widget_new() ;

  gtk_source_style_scheme_chooser_set_style_scheme(GTK_SOURCE_STYLE_SCHEME_CHOOSER(source_style_scheme_chooser_widget),
                                                   gtk_source_style_scheme_manager_get_scheme(source_style_language_manager, todo_settings->scheme)) ;

  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL) ;

  gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled_window),  24 * 16* 1.5)   ;
  gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrolled_window),   24 * 16)   ;

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_scrolled_window_set_overlay_scrolling(GTK_SCROLLED_WINDOW(scrolled_window), FALSE) ;

  gtk_container_add(GTK_CONTAINER(scrolled_window), source_style_scheme_chooser_widget) ;

  gtk_widget_show_all(scrolled_window) ;

  GtkWidget *dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

  gtk_container_add(GTK_CONTAINER(dialog_content), scrolled_window) ;

  gint res = gtk_dialog_run(GTK_DIALOG(dialog)) ;

  if (res == GTK_RESPONSE_APPLY) {

    GtkSourceStyleScheme *scheme_style = gtk_source_style_scheme_chooser_get_style_scheme(GTK_SOURCE_STYLE_SCHEME_CHOOSER(source_style_scheme_chooser_widget)) ;

    scheme_id = (gchar *) gtk_source_style_scheme_get_id(scheme_style) ;

  }

  gtk_widget_destroy(dialog) ;

  return ;

}
#endif

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

#ifdef GSPELL_SUPPORT
static gchar *language_code = NULL ;

static void get_language_spell_check(GtkWidget *widget)  {

  GtkWidget *spellcheck_language_dialog = gspell_language_chooser_dialog_new(GTK_WINDOW(gui->main_window), gspell_language_lookup(settings.language_code), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT) ;

  gtk_dialog_run(GTK_DIALOG(spellcheck_language_dialog)) ;

  if (gspell_language_chooser_get_language_code(GSPELL_LANGUAGE_CHOOSER(spellcheck_language_dialog)) != NULL) {

      g_free(language_code) ;

      /** It cannot be NULL as say in the documentation ! **/
      language_code = g_strdup(gspell_language_chooser_get_language_code(GSPELL_LANGUAGE_CHOOSER(spellcheck_language_dialog))) ;

  }

  gtk_widget_destroy(spellcheck_language_dialog) ;

  gtk_button_set_label(GTK_BUTTON(widget), language_code) ;

  return ;

}
#endif

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





  GtkSourceStyleScheme *source_style_scheme =  gtk_source_style_scheme_manager_get_scheme(source_style_language_manager, settings.scheme);

  GtkWidget *use_scheme_button = gtk_source_style_scheme_chooser_button_new() ;

  gtk_source_style_scheme_chooser_set_style_scheme(GTK_SOURCE_STYLE_SCHEME_CHOOSER(use_scheme_button),  source_style_scheme) ;
       
  GtkWidget *use_scheme_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  gtk_container_set_border_width(GTK_CONTAINER(use_scheme_hbox), CONTAINER_PADDING) ;


  gtk_container_set_border_width(GTK_CONTAINER(use_scheme_button), CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(use_scheme_hbox), use_scheme_button, TRUE, TRUE, CONTAINER_PADDING) ;


  GtkWidget *use_scheme_frame           = gtk_frame_new( _(" Use scheme ") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(use_scheme_frame),          FRAME_BORDER_SIZE) ;

  gtk_container_add(GTK_CONTAINER(use_scheme_frame), use_scheme_hbox) ;



  GtkWidget *show_lines_checkbutton = generate_checkbuton( _("Display lines numbers"), settings.display_line_numbers) ;

  GtkWidget *display_lines_frame        = gtk_frame_new( _(" Show lines ") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(display_lines_frame),       FRAME_BORDER_SIZE) ;

  gtk_container_add(GTK_CONTAINER(display_lines_frame), show_lines_checkbutton) ;






  GtkWidget *show_tabs_checkbutton = generate_checkbuton( _("Display TABS characters"),settings.display_tabs_chars) ;

  GtkWidget *display_tabs_frame         = gtk_frame_new( _(" Show tabulations ") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(display_tabs_frame),        FRAME_BORDER_SIZE) ;

  gtk_container_add(GTK_CONTAINER(display_tabs_frame), show_tabs_checkbutton) ;









  GtkWidget *show_spaces_checkbutton =   generate_checkbuton( _("Show all spaces"),  settings.display_all_spaces) ;

  GtkWidget *display_spaces_frame       = gtk_frame_new( _(" Show spaces ") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(display_spaces_frame),      FRAME_BORDER_SIZE) ;

  gtk_container_add(GTK_CONTAINER(display_spaces_frame), show_spaces_checkbutton) ;



  #ifdef GSPELL_SUPPORT
  GtkWidget *configure_spell_check_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, CONTAINER_PADDING) ;

  GtkWidget *configure_spell_check_label = gtk_label_new( _("Select spell-check language") ) ;

  GtkWidget *configure_spell_check_button = gtk_button_new_with_label(settings.language_code) ;

  g_signal_connect(configure_spell_check_button, "clicked", G_CALLBACK(get_language_spell_check), configure_spell_check_button) ;

  gtk_box_pack_start(GTK_BOX(configure_spell_check_vbox), configure_spell_check_label,    FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(configure_spell_check_vbox), configure_spell_check_button,   FALSE, FALSE, 0) ;

  g_object_set(configure_spell_check_vbox, "margin", CONTAINER_PADDING*2+4, NULL) ;

  gtk_container_set_border_width(GTK_CONTAINER(configure_spell_check_vbox), CONTAINER_PADDING) ;

  GtkWidget *configure_spell_check_frame = gtk_frame_new( _(" Spell check ") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(configure_spell_check_frame), FRAME_BORDER_SIZE) ;

  gtk_container_add(GTK_CONTAINER(configure_spell_check_frame), configure_spell_check_vbox) ;
  #endif























  GtkWidget *indent_settings_hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, CONTAINER_PADDING) ;

  GtkWidget *auto_indent_checkbutton = generate_checkbuton( _("Use auto-indent"), settings.use_auto_indent) ;

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

  gtk_container_set_border_width(GTK_CONTAINER(indent_settings_hbox), CONTAINER_PADDING) ;

  GtkWidget *indent_settings_frame      = gtk_frame_new( _(" Indentation ") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(indent_settings_frame),     FRAME_BORDER_SIZE) ;

  gtk_container_add(GTK_CONTAINER(indent_settings_frame), indent_settings_hbox) ;




  GtkWidget *tabs_settings_hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, CONTAINER_PADDING) ;

  GtkWidget *use_spaces_instead_of_tabs = generate_checkbuton( _("Use spaces instead of TABS"), settings.use_spaces_as_tabs) ;





  GtkWidget *tabs_width_hbox       = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0) ;

  GtkWidget *tabs_width_label      = gtk_label_new( _("TABS width") ) ;

  GtkWidget *tabs_width_spinbutton = gtk_spin_button_new_with_range(2, 32, 2) ;

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(tabs_width_spinbutton), settings.tabs_width) ;
  gtk_spin_button_set_digits(GTK_SPIN_BUTTON(tabs_width_spinbutton),                  0) ;
  gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(tabs_width_spinbutton),              TRUE) ;


  gtk_box_pack_start(GTK_BOX(tabs_width_hbox),tabs_width_label,      TRUE,   TRUE, 0) ;
  gtk_box_pack_start(GTK_BOX(tabs_width_hbox),tabs_width_spinbutton, FALSE, FALSE, 0) ;


  gtk_box_pack_start(GTK_BOX(tabs_settings_hbox), use_spaces_instead_of_tabs, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(tabs_settings_hbox), tabs_width_hbox,            FALSE, FALSE, 0) ;

  gtk_container_set_border_width(GTK_CONTAINER(tabs_settings_hbox), CONTAINER_PADDING) ;


  GtkWidget *tabs_settings_frame        = gtk_frame_new( _(" Tabulations ") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(tabs_settings_frame),       FRAME_BORDER_SIZE) ;

  gtk_container_add(GTK_CONTAINER(tabs_settings_frame), tabs_settings_hbox) ;







  GtkWidget *editor_configuration_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_box_set_spacing(GTK_BOX(editor_configuration_vbox), CONTAINER_PADDING) ;


  gtk_box_pack_start(GTK_BOX(editor_configuration_vbox), use_scheme_frame,              FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(editor_configuration_vbox), display_lines_frame,           FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(editor_configuration_vbox), display_tabs_frame,            FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(editor_configuration_vbox), display_spaces_frame,          FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(editor_configuration_vbox), indent_settings_frame,         FALSE, FALSE, 0) ;

  #ifdef GSPELL_SUPPORT
  gtk_box_pack_start(GTK_BOX(editor_configuration_vbox), tabs_settings_frame,           FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(editor_configuration_vbox), configure_spell_check_frame,   TRUE,  TRUE,  0) ;
  #else
  gtk_box_pack_start(GTK_BOX(editor_configuration_vbox), tabs_settings_frame,           TRUE,  TRUE, 0) ;
  #endif



  gtk_notebook_append_page(GTK_NOTEBOOK(notebook_configuration), editor_configuration_vbox,  gtk_label_new( _("Editor Settings") )) ;








  GtkWidget *open_file_warn_already_in_editor_chkbox = generate_checkbuton( _("Warning by loaded file already into editor"), settings.warn_file_open) ;

  gtk_widget_set_tooltip_text(open_file_warn_already_in_editor_chkbox, _("The program emit a warning by opening a file already contains into the editor.\nIt will let you the choice to open it or not.") ) ;


  GtkWidget *open_file_warn_readonly_chkbox = generate_checkbuton( _("Warning by loaded file is read-only file"),  settings.warn_read_only) ;

  gtk_widget_set_tooltip_text(open_file_warn_readonly_chkbox, _("The program emit a warning by opening read-only files") ) ;



  GtkWidget *open_file_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_box_set_spacing(GTK_BOX(open_file_vbox), CONTAINER_PADDING) ;


  gtk_box_pack_start(GTK_BOX(open_file_vbox), open_file_warn_already_in_editor_chkbox,  FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(open_file_vbox), open_file_warn_readonly_chkbox,           FALSE, FALSE, 0) ;



  GtkWidget *open_file_frame = gtk_frame_new(" Open file ") ;

  gtk_container_add(GTK_CONTAINER(open_file_frame), open_file_vbox) ;

  gtk_container_set_border_width(GTK_CONTAINER(open_file_frame), FRAME_BORDER_SIZE) ;








  GtkWidget *save_file_despite_modification_timestamp = generate_checkbuton( _("Save despite modification time stamp"), settings.overwrite_anyway) ;

  gtk_widget_set_tooltip_text(save_file_despite_modification_timestamp, _("Allow the program to save a file despite a later time stamp.\nAnother process have change the file maybe.") ) ;





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

  GtkWidget *save_file_mask_ur_label = gtk_label_new( "r" ) ;
  GtkWidget *save_file_mask_ur_checkbutton = gtk_check_button_new() ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_ur_checkbutton), is_perm_value_set(mask, S_IRUSR)) ;




  GtkWidget *save_file_mask_uw_label = gtk_label_new( "w" ) ;
  GtkWidget *save_file_mask_uw_checkbutton = gtk_check_button_new() ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_uw_checkbutton), is_perm_value_set(mask, S_IWUSR)) ;

  GtkWidget *save_file_mask_ux_label = gtk_label_new( "x" ) ;
  GtkWidget *save_file_mask_ux_checkbutton = gtk_check_button_new() ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_ux_checkbutton), is_perm_value_set(mask, S_IXUSR)) ;



  GtkWidget *save_file_mask_gr_label = gtk_label_new( "r" ) ;
  GtkWidget *save_file_mask_gr_checkbutton = gtk_check_button_new() ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_gr_checkbutton), is_perm_value_set(mask, S_IRGRP)) ;


  GtkWidget *save_file_mask_gw_label = gtk_label_new( "w" ) ;
  GtkWidget *save_file_mask_gw_checkbutton = gtk_check_button_new() ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_gw_checkbutton), is_perm_value_set(mask, S_IWGRP)) ;


  GtkWidget *save_file_mask_gx_label = gtk_label_new( "x" ) ;
  GtkWidget *save_file_mask_gx_checkbutton = gtk_check_button_new() ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_gx_checkbutton), is_perm_value_set(mask, S_IXGRP)) ;



  GtkWidget *save_file_mask_or_label = gtk_label_new( "r" ) ;
  GtkWidget *save_file_mask_or_checkbutton = gtk_check_button_new() ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_or_checkbutton), is_perm_value_set(mask, S_IROTH)) ;

  gint value_or = 0004 ;
  g_object_set_data( G_OBJECT(save_file_mask_or_checkbutton), "perm_value", &value_or) ;

  GtkWidget *save_file_mask_ow_label = gtk_label_new( "w" ) ;
  GtkWidget *save_file_mask_ow_checkbutton = gtk_check_button_new() ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_ow_checkbutton), is_perm_value_set(mask, S_IWOTH)) ;

  gint value_ow = 0002 ;
  g_object_set_data( G_OBJECT(save_file_mask_ow_checkbutton), "perm_value", &value_ow) ;


  GtkWidget *save_file_mask_ox_label = gtk_label_new( "x" ) ;
  GtkWidget *save_file_mask_ox_checkbutton = gtk_check_button_new() ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(save_file_mask_ox_checkbutton), is_perm_value_set(mask, S_IXOTH)) ;

  gint value_ox = 0001 ;
  g_object_set_data(G_OBJECT(save_file_mask_ox_checkbutton), "perm_value", &value_ox) ;



  GtkWidget *save_file_user_mask_grid   = gtk_grid_new() ;
  GtkWidget *save_file_group_mask_grid  = gtk_grid_new() ;
  GtkWidget *save_file_other_mask_grid  = gtk_grid_new() ;

  gtk_grid_attach(GTK_GRID(save_file_user_mask_grid),  save_file_mask_ur_label,       0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(save_file_user_mask_grid),  save_file_mask_ur_checkbutton, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(save_file_user_mask_grid),  save_file_mask_uw_label,       1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(save_file_user_mask_grid),  save_file_mask_uw_checkbutton, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(save_file_user_mask_grid),  save_file_mask_ux_label,       2, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(save_file_user_mask_grid),  save_file_mask_ux_checkbutton, 2, 1, 1, 1);

  g_object_set(save_file_user_mask_grid, "column-homogeneous", TRUE, "column-spacing", 8,  NULL) ;

  gtk_grid_attach(GTK_GRID(save_file_group_mask_grid),  save_file_mask_gr_label,       3, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(save_file_group_mask_grid),  save_file_mask_gr_checkbutton, 3, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(save_file_group_mask_grid),  save_file_mask_gw_label,       4, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(save_file_group_mask_grid),  save_file_mask_gw_checkbutton, 4, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(save_file_group_mask_grid),  save_file_mask_gx_label,       5, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(save_file_group_mask_grid),  save_file_mask_gx_checkbutton, 5, 1, 1, 1);

  g_object_set(save_file_group_mask_grid, "column-homogeneous", TRUE, "column-spacing", 8,  NULL) ;

  gtk_grid_attach(GTK_GRID(save_file_other_mask_grid),  save_file_mask_or_label,       6, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(save_file_other_mask_grid),  save_file_mask_or_checkbutton, 6, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(save_file_other_mask_grid),  save_file_mask_ow_label,       7, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(save_file_other_mask_grid),  save_file_mask_ow_checkbutton, 7, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(save_file_other_mask_grid),  save_file_mask_ox_label,       8, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(save_file_other_mask_grid),  save_file_mask_ox_checkbutton, 8, 1, 1, 1);

  g_object_set(save_file_other_mask_grid, "column-homogeneous", TRUE, "column-spacing", 8,  NULL) ;


  GtkWidget *save_file_mask_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  gtk_container_set_border_width(GTK_CONTAINER(save_file_user_mask_grid),  CONTAINER_PADDING) ;
  gtk_container_set_border_width(GTK_CONTAINER(save_file_group_mask_grid), CONTAINER_PADDING) ;
  gtk_container_set_border_width(GTK_CONTAINER(save_file_other_mask_grid), CONTAINER_PADDING) ;

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

  GtkWidget *file_saving_rm_trailing_spaces_frame = gtk_frame_new(" Remove trailing spaces ") ;

  gtk_container_set_border_width(GTK_CONTAINER(file_saving_rm_trailing_spaces_frame), FRAME_BORDER_SIZE) ;

  gtk_container_add(GTK_CONTAINER(file_saving_rm_trailing_spaces_frame), file_saving_rm_trailing_spaces_hbox) ;





  GtkWidget *file_saving_backup_file_frame = gtk_frame_new(" Create backup file ") ;

  gtk_container_set_border_width(GTK_CONTAINER(file_saving_backup_file_frame), FRAME_BORDER_SIZE) ;

  gtk_container_add(GTK_CONTAINER(file_saving_backup_file_frame), file_saving_backup_file_vbox) ;



  GtkWidget *save_file_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_box_set_spacing(GTK_BOX(save_file_vbox), CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(save_file_vbox), save_file_despite_modification_timestamp,   FALSE, FALSE, 0) ;
  //gtk_box_pack_start(GTK_BOX(save_file_vbox), close_file_unsaved_checkbutton,        FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(save_file_vbox), save_file_encoding_hbox,                    FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(save_file_vbox), save_file_newline_hbox,                     FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(save_file_vbox), save_file_mask_hbox,                       FALSE, FALSE, 0) ;



  GtkWidget *save_file_frame = gtk_frame_new(" Save file ") ;

  gtk_container_set_border_width(GTK_CONTAINER(save_file_frame), FRAME_BORDER_SIZE) ;

  gtk_container_add(GTK_CONTAINER(save_file_frame), save_file_vbox) ;




  GtkWidget *close_file_unsaved_checkbutton = generate_checkbuton( _("Warning by closing an unsaved file"), settings.warn_file_save) ;

  gtk_widget_set_tooltip_text(close_file_unsaved_checkbutton, _("Emit a warning if you attempt to close an unsaved file" )) ;



  GtkWidget *close_file_frame = gtk_frame_new(" Close file ") ;

  gtk_container_set_border_width(GTK_CONTAINER(close_file_frame), FRAME_BORDER_SIZE) ;

  gtk_container_add(GTK_CONTAINER(close_file_frame), close_file_unsaved_checkbutton) ;





  GtkWidget *files_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_box_set_spacing(GTK_BOX(files_vbox), CONTAINER_PADDING) ;


  gtk_box_pack_start(GTK_BOX(files_vbox), open_file_frame,                        FALSE,  FALSE,  0) ;
  gtk_box_pack_start(GTK_BOX(files_vbox), save_file_frame,                        FALSE,  FALSE,  0) ;
  gtk_box_pack_start(GTK_BOX(files_vbox), close_file_frame,                       FALSE,  FALSE,  0) ;
  gtk_box_pack_start(GTK_BOX(files_vbox), file_saving_backup_file_frame,          FALSE,  FALSE,  0) ;
  gtk_box_pack_start(GTK_BOX(files_vbox), file_saving_rm_trailing_spaces_frame,   FALSE,  FALSE,  0) ;




  gtk_notebook_append_page(GTK_NOTEBOOK(notebook_configuration), files_vbox, gtk_label_new( _("Files")) ) ;


  /** TODO:
    *
    * %1. align the encoding, even translate it in clearer.
    *
    * %2. Think at switch buttons for their warnings.
    *
    * %3. Add the close section in this notebook page (for the warning).
    *
    * %4. Add (u)mask in Save file.
    *
    * %5. Set the spellcheck language_code into the "Editor" page.
    *
    * FIXME: The sidebar terminal(s) scrollbar is weak after using configure program.
    *
    *********************************************************************************/




  GtkWidget *terminal_settings_notebook         = gtk_notebook_new() ;


  GtkWidget *terminal_use_scheme_frame         = gtk_frame_new( _("Font settings") ) ;

  GtkWidget *terminal_font_scale_frame          = gtk_frame_new( _("Font scale") ) ;

  GtkWidget *terminal_cursor_frame              = gtk_frame_new( _("Cursor settings") ) ;

  GtkWidget *terminal_colors_frame              = gtk_frame_new( _("Colors settings") ) ;

  GtkWidget *terminal_bold_frame                = gtk_frame_new( _("Bold settings") ) ;

  GtkWidget *terminal_pointer_autohide          = gtk_frame_new( _("Pointer autohide") ) ;


  gtk_container_set_border_width(GTK_CONTAINER(terminal_use_scheme_frame),       FRAME_BORDER_SIZE) ;
  gtk_container_set_border_width(GTK_CONTAINER(terminal_font_scale_frame),        FRAME_BORDER_SIZE) ;
  gtk_container_set_border_width(GTK_CONTAINER(terminal_cursor_frame),            FRAME_BORDER_SIZE) ;
  gtk_container_set_border_width(GTK_CONTAINER(terminal_colors_frame),            FRAME_BORDER_SIZE) ;
  gtk_container_set_border_width(GTK_CONTAINER(terminal_bold_frame),              FRAME_BORDER_SIZE) ;
  gtk_container_set_border_width(GTK_CONTAINER(terminal_pointer_autohide),        FRAME_BORDER_SIZE) ;

  GtkWidget *terminal_use_font_button = gtk_font_button_new_with_font(settings.term_font) ;


  g_object_set(G_OBJECT(terminal_use_font_button), "margin", CONTAINER_PADDING, NULL) ;

  gtk_font_button_set_show_style(GTK_FONT_BUTTON(terminal_use_font_button),  TRUE) ;
  gtk_font_button_set_show_size(GTK_FONT_BUTTON(terminal_use_font_button),   TRUE) ;
  gtk_font_button_set_use_font(GTK_FONT_BUTTON(terminal_use_font_button),    TRUE) ;
  gtk_font_button_set_use_size(GTK_FONT_BUTTON(terminal_use_font_button),    TRUE) ;


  gtk_container_add(GTK_CONTAINER(terminal_use_scheme_frame), terminal_use_font_button) ;

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

  gtk_container_add(GTK_CONTAINER(terminal_font_scale_frame), terminal_font_scale_vbox) ;



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



  gtk_container_add(GTK_CONTAINER(terminal_cursor_frame), terminal_cursor_vbox) ;




  GtkWidget *terminal_bold_vbox        = gtk_box_new(GTK_ORIENTATION_VERTICAL,    5) ;
  GtkWidget *terminal_bold_color_hbox  = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,  5) ;

  GtkWidget *terminal_bold_allowed_checkbutton = generate_checkbuton( _("  Allow bold  "), settings.bold_allow ) ;

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

  gtk_container_add(GTK_CONTAINER(terminal_bold_frame), terminal_bold_vbox) ;




  GtkWidget *terminal_pointer_autohide_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, CONTAINER_PADDING) ;

  GtkWidget *terminal_pointer_autohide_chkbox = generate_checkbuton( _("Pointer autohide"), settings.pointer_autohide) ;

  gtk_box_pack_start(GTK_BOX(terminal_pointer_autohide_hbox), terminal_pointer_autohide_chkbox, TRUE, TRUE, 0) ;


  gtk_container_set_border_width(GTK_CONTAINER(terminal_pointer_autohide_chkbox),        CONTAINER_PADDING) ;

  gtk_container_add(GTK_CONTAINER(terminal_pointer_autohide), terminal_pointer_autohide_hbox) ;



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


  gtk_container_add(GTK_CONTAINER(terminal_colors_frame), terminal_colors_hbox) ;

  gtk_container_set_border_width(GTK_CONTAINER(terminal_colors_hbox), CONTAINER_PADDING) ;


  GtkWidget *terminal_user_shell_frame          = gtk_frame_new( _("User shell") ) ;

  GtkWidget *terminal_start_dir_frame           = gtk_frame_new( _("Start directory") ) ;

  GtkWidget *terminal_audible_bell_frame        = gtk_frame_new( _("Audible bell") ) ;

  GtkWidget *terminal_scrollback_frame          = gtk_frame_new( _("Scrollback lines") ) ;

  GtkWidget *terminal_scrolling_frame           = gtk_frame_new( _("Scrolling") ) ;

  GtkWidget *terminal_erase_binding_frame       = gtk_frame_new( _("Erasing binding") ) ;


  gtk_container_set_border_width(GTK_CONTAINER(terminal_user_shell_frame),         FRAME_BORDER_SIZE) ;
  gtk_container_set_border_width(GTK_CONTAINER(terminal_start_dir_frame),          FRAME_BORDER_SIZE) ;
  gtk_container_set_border_width(GTK_CONTAINER(terminal_audible_bell_frame),       FRAME_BORDER_SIZE) ;
  gtk_container_set_border_width(GTK_CONTAINER(terminal_scrollback_frame),         FRAME_BORDER_SIZE) ;
  gtk_container_set_border_width(GTK_CONTAINER(terminal_scrolling_frame),          FRAME_BORDER_SIZE) ;
  gtk_container_set_border_width(GTK_CONTAINER(terminal_erase_binding_frame),      FRAME_BORDER_SIZE) ;



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

  gtk_container_add(GTK_CONTAINER(terminal_user_shell_frame), user_shell_hbox) ;





  GtkWidget *start_directory_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  GtkWidget *start_directory_label = gtk_label_new( _("Start directory          ") ) ;

  GtkWidget *start_directory_file_chooser_button = gtk_file_chooser_button_new( _("Select the terminal starting directory"), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER) ;

  gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(start_directory_file_chooser_button), settings.start_dir) ;

  gtk_box_pack_start(GTK_BOX(start_directory_hbox), start_directory_label,               FALSE, FALSE,   0) ;
  gtk_box_pack_start(GTK_BOX(start_directory_hbox), start_directory_file_chooser_button, TRUE,  TRUE,    0) ;

  gtk_container_set_border_width(GTK_CONTAINER(start_directory_file_chooser_button),  CONTAINER_PADDING) ;

  gtk_container_set_border_width(GTK_CONTAINER(start_directory_hbox),  CONTAINER_PADDING) ;

  gtk_box_set_spacing(GTK_BOX(start_directory_hbox), CONTAINER_PADDING) ;

  gtk_container_add(GTK_CONTAINER(terminal_start_dir_frame), start_directory_hbox) ;



  GtkWidget *audible_bell_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  GtkWidget *audible_bell_chkbox = generate_checkbuton( _("Audible bell"), settings.audible_bell) ;

  gtk_box_pack_start(GTK_BOX(audible_bell_vbox), audible_bell_chkbox,  TRUE, TRUE,   CONTAINER_PADDING) ;

  gtk_container_add(GTK_CONTAINER(terminal_audible_bell_frame), audible_bell_vbox) ;



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

  GtkWidget *scroll_on_output_checkbutton = generate_checkbuton( _("Scroll on output"),    settings.scroll_on_output) ;

  GtkWidget *scroll_on_keystroke_checkbutton = generate_checkbuton( _("Scroll on keystroke"), settings.scroll_on_keystroke) ;


  gtk_box_pack_start(GTK_BOX(scrolling_vbox), scroll_on_output_checkbutton,    FALSE, FALSE, CONTAINER_PADDING) ;
  gtk_box_pack_start(GTK_BOX(scrolling_vbox), scroll_on_keystroke_checkbutton, FALSE, FALSE, CONTAINER_PADDING) ;

  gtk_container_set_border_width(GTK_CONTAINER(scrolling_vbox), CONTAINER_PADDING) ;

  gtk_container_add(GTK_CONTAINER(terminal_scrolling_frame), scrolling_vbox) ;


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

  gtk_container_add(GTK_CONTAINER(terminal_erase_binding_frame), erasing_vbox) ;


  GtkWidget *terminal_appearance_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_box_set_spacing(GTK_BOX(terminal_appearance_vbox), CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(terminal_appearance_vbox), terminal_use_scheme_frame ,    FALSE, FALSE, 0) ;

  gtk_box_pack_start(GTK_BOX(terminal_appearance_vbox), terminal_font_scale_frame,      FALSE, FALSE, 0) ;

  gtk_box_pack_start(GTK_BOX(terminal_appearance_vbox), terminal_cursor_frame ,         FALSE, FALSE, 0) ;

  gtk_box_pack_start(GTK_BOX(terminal_appearance_vbox), terminal_colors_frame ,         FALSE, FALSE, 0) ;



  gtk_box_pack_start(GTK_BOX(terminal_appearance_vbox), terminal_bold_frame,            FALSE, FALSE, 0) ;



  gtk_box_pack_start(GTK_BOX(terminal_appearance_vbox), terminal_pointer_autohide,      TRUE,   TRUE, 0) ;

  gtk_container_add(GTK_CONTAINER(terminal_scrollback_frame), scrollback_lines_vbox) ;


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


  GtkWidget *files_manager_list_frame         = gtk_frame_new( _("Files manager list") ) ;

  GtkWidget *files_manager_action_frame       = gtk_frame_new( _("Actions") ) ;


  gtk_container_set_border_width(GTK_CONTAINER(files_manager_list_frame),   FRAME_BORDER_SIZE) ;

  gtk_container_set_border_width(GTK_CONTAINER(files_manager_action_frame), FRAME_BORDER_SIZE) ;


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

  gtk_container_add(GTK_CONTAINER(files_manager_list_frame),  link_scrolled_window ) ;

  gtk_container_add(GTK_CONTAINER(files_manager_action_frame), files_manager_action_buttonbox) ;



  gtk_box_pack_start(GTK_BOX(files_manager_main_vbox), files_manager_list_frame,       TRUE,  TRUE,  0) ;
  gtk_box_pack_start(GTK_BOX(files_manager_main_vbox), files_manager_action_frame,     FALSE, FALSE, 0) ;






  gtk_notebook_append_page(GTK_NOTEBOOK(notebook_configuration), terminal_settings_notebook, gtk_label_new( _("Terminals Settings") )) ;

  gtk_notebook_append_page(GTK_NOTEBOOK(notebook_configuration), files_manager_main_vbox,    gtk_label_new( _("Files handler manager") )) ;



  gtk_container_add(GTK_CONTAINER(configuration_dialog_content), notebook_configuration) ;




  GtkWidget *gui_application_notebook           = gtk_notebook_new() ;


  GtkWidget *gui_application_main_vbox          = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;


  GtkWidget *gui_application_programming_frame  = gtk_frame_new( _("Programing") ) ;

  GtkWidget *gui_application_utils_frame        = gtk_frame_new( _("Utils") ) ;

  GtkWidget *gui_application_others_frame       = gtk_frame_new( _("Others") ) ;



  GtkWidget *gui_application_action_frame       = gtk_frame_new( _("Actions") ) ;


  gtk_container_set_border_width(GTK_CONTAINER(gui_application_programming_frame),  FRAME_BORDER_SIZE) ;

  gtk_container_set_border_width(GTK_CONTAINER(gui_application_utils_frame),        FRAME_BORDER_SIZE) ;

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


  gtk_container_add(GTK_CONTAINER(gui_application_programming_frame), gui_application_programming_vbox) ;




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


  gtk_container_add(GTK_CONTAINER(gui_application_utils_frame), gui_application_utils_vbox) ;






  GtkWidget *gui_application_others_main_vbox          = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;


  GtkWidget *gui_application_others_list_frame         = gtk_frame_new( _("User defined applications list") ) ;

  GtkWidget *gui_application_others_action_frame       = gtk_frame_new( _("Actions") ) ;


  gtk_container_set_border_width(GTK_CONTAINER(gui_application_others_list_frame),    FRAME_BORDER_SIZE) ;

  gtk_container_set_border_width(GTK_CONTAINER(gui_application_others_action_frame),  FRAME_BORDER_SIZE) ;


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

  gtk_container_add(GTK_CONTAINER(gui_application_others_list_frame),  gui_application_others_scrolled_window ) ;

  gtk_container_add(GTK_CONTAINER(gui_application_others_action_frame), gui_application_others_action_buttonbox) ;



  gtk_box_pack_start(GTK_BOX(gui_application_others_main_vbox), gui_application_others_list_frame,       TRUE,  TRUE,  0) ;
  gtk_box_pack_start(GTK_BOX(gui_application_others_main_vbox), gui_application_others_action_frame,     FALSE, FALSE, 0) ;



  gtk_box_pack_start(GTK_BOX(gui_application_main_vbox), gui_application_programming_frame, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui_application_main_vbox), gui_application_utils_frame,       FALSE, FALSE, 0) ;

  gtk_notebook_append_page(GTK_NOTEBOOK(gui_application_notebook), gui_application_main_vbox, gtk_label_new( _("Predefined applications") )) ;

  gtk_notebook_append_page(GTK_NOTEBOOK(gui_application_notebook), gui_application_others_main_vbox, gtk_label_new( _("User defined applications")  )) ;

  gtk_notebook_append_page(GTK_NOTEBOOK(notebook_configuration), gui_application_notebook, gtk_label_new( _("Applications")  )) ;







  GtkWidget *gui_configuration_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gtk_box_set_spacing(GTK_BOX(gui_configuration_vbox), CONTAINER_PADDING) ;


  GtkWidget *gui_sidebar_factor_frame       = gtk_frame_new( _("Editor / Sidebar space factor") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(gui_sidebar_factor_frame),       FRAME_BORDER_SIZE) ;


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


  gtk_container_add(GTK_CONTAINER(gui_sidebar_factor_frame), gui_sidebar_factor_vbox) ;




  GtkWidget *startup_settings_frame = gtk_frame_new( _("Settings at start") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(startup_settings_frame),     FRAME_BORDER_SIZE) ;






  GtkWidget *show_terminals_sidebar_checkbutton = generate_checkbuton( _("Show Terminals-sidebar at start ?"), settings.side_terms_on) ;

  GtkWidget *show_big_term_checkbutton = generate_checkbuton( _("Display Big-terminal at start ?"), settings.big_term_on) ;

  GtkWidget *show_big_term_divided_checkbutton = generate_checkbuton( _("Divide Big-terminal in 4 at start ?"), settings.big_term_div) ;

  GtkWidget *show_buttonbar_checkbutton = generate_checkbuton( _("Show buttonbar at start ?"), settings.buttonbar_on) ;

  GtkWidget *show_fullscreen_checkbutton = generate_checkbuton( _("Toggle to fullscreen at start ?"), settings.fullscreen) ;


  GtkWidget *startup_settings_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL, CONTAINER_PADDING) ;

  gtk_box_pack_start(GTK_BOX(startup_settings_vbox), show_terminals_sidebar_checkbutton,          TRUE, TRUE, CONTAINER_PADDING) ;
  gtk_box_pack_start(GTK_BOX(startup_settings_vbox), show_big_term_checkbutton,       TRUE, TRUE, CONTAINER_PADDING) ;
  gtk_box_pack_start(GTK_BOX(startup_settings_vbox), show_buttonbar_checkbutton,         TRUE, TRUE, CONTAINER_PADDING) ;
  gtk_box_pack_start(GTK_BOX(startup_settings_vbox), show_fullscreen_checkbutton,      TRUE, TRUE, CONTAINER_PADDING) ;


  gtk_container_add(GTK_CONTAINER(startup_settings_frame), startup_settings_vbox) ;


  GtkWidget *gui_set_as_default_frame       = gtk_frame_new( _("Set as default editor") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(gui_set_as_default_frame),       FRAME_BORDER_SIZE) ;

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

  gtk_container_add(GTK_CONTAINER(gui_set_as_default_frame), gui_set_as_default_vbox) ;




  GtkWidget *gui_launch_cmd_frame       = gtk_frame_new( _("Launch command at start") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(gui_launch_cmd_frame),       FRAME_BORDER_SIZE) ;

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

  gtk_container_add(GTK_CONTAINER(gui_launch_cmd_frame), gui_launch_cmd_vbox) ;





  GtkWidget *session_configuration_frame       = gtk_frame_new( _("Session management") ) ;

  gtk_container_set_border_width(GTK_CONTAINER(session_configuration_frame),       FRAME_BORDER_SIZE) ;



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

  gtk_container_add(GTK_CONTAINER(session_configuration_frame), session_configuration_vbox) ;

  gtk_container_set_border_width(GTK_CONTAINER(session_configuration_frame), CONTAINER_PADDING) ;



  gtk_box_pack_start(GTK_BOX(gui_configuration_vbox), gui_sidebar_factor_frame,         FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui_configuration_vbox), startup_settings_frame,           FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui_configuration_vbox), gui_launch_cmd_frame,             FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui_configuration_vbox), gui_set_as_default_frame,         FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui_configuration_vbox), session_configuration_frame,      TRUE,  TRUE,  0) ;


  gtk_notebook_append_page(GTK_NOTEBOOK(notebook_configuration), gui_configuration_vbox, gtk_label_new( _("Settings") )) ;



  gtk_widget_show_all(configuration_dialog) ;

  gint result = gtk_dialog_run(GTK_DIALOG(configuration_dialog));

  if (result ==  GTK_RESPONSE_APPLY) {


    g_free(settings.scheme) ;
    settings.scheme = g_strdup( gtk_source_style_scheme_get_id(gtk_source_style_scheme_chooser_get_style_scheme(GTK_SOURCE_STYLE_SCHEME_CHOOSER(use_scheme_button))) ) ;

    update_scheme_menu_items(settings.scheme) ;


    settings.display_line_numbers = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_lines_checkbutton))     ;
    settings.display_tabs_chars   = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(show_tabs_checkbutton))      ;
    settings.use_auto_indent      = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(auto_indent_checkbutton))    ;
    settings.indent_width         = (gint) gtk_spin_button_get_value(GTK_SPIN_BUTTON(indent_width_spinbutton))  ;
    settings.use_spaces_as_tabs   = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(use_spaces_instead_of_tabs)) ;
    settings.tabs_width           = (gint) gtk_spin_button_get_value(GTK_SPIN_BUTTON(tabs_width_spinbutton))    ;
    settings.backup_file          = gtk_switch_get_active(GTK_SWITCH(save_file_backup_file_switch))    ;


    g_free(settings.term_font) ;

    settings.term_font = g_strdup(gtk_font_button_get_font_name(GTK_FONT_BUTTON(terminal_use_font_button))) ;


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


    free(bold_color_activated) ;
    free(cursor_color_activated) ;

    #ifdef GSPELL_SUPPORT

    if (language_code != NULL) {

      g_free(settings.language_code) ;

      settings.language_code        =  language_code ;
    }
    else {
 
      settings.language_code        = g_strdup(gspell_language_get_code((gspell_language_lookup("en") == NULL) ? gspell_language_get_default() : gspell_language_lookup("en"))) ;
    }
 

    #endif

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
