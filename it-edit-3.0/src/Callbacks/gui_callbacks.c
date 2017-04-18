/** ***********************************************************************************
  * it-edit the Integrated Terminal Editor: a text editor with severals               *
  * integrated functionalities.                                                       *
  *                                                                                   *
  * Copyright (C) 2015-2017 Brüggemann Eddie.                                         *
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

#include "./gui_callbacks.h"

static void set_new_terminals(GtkWidget *terminals_notebook) ;

static void switch_big_term_fterms(void) ;

static void update_toggle_button_state(GtkWidget *widget) ;

static void switch_big_term_on_off(void) ;

static void show_hide_sidebar_terminals(void) ;

/** The *_echo variables are used for inhibit unwanted toggles from both the menu item and the (@toggle) button. **/
static bool toggle_big_terminals_echo = true ;

static bool toggle_sidebar_terminals_echo = true ;

/** Keep track of the last big terminal mode (Simple or divided into 4 terminals). **/
static guint8 active_big_terminal = 1 ;

static void show_hide_sidebar_terminals(void) {
 
  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif
 
  if (! gtk_widget_get_visible(gui->terminal_notebook)) {

    /** Make the terminals side bar visible and give the focus to the terminal. **/

    gtk_widget_show(gui->terminal_notebook) ;

    Terminals *terminals  = g_object_get_data(G_OBJECT(gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->terminal_notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(gui->terminal_notebook)))), "terminals") ;

    gtk_widget_grab_focus(terminals->terminal_1) ;

  }
  else {

    /** Hide terminals sidebar and give the editor the focus. **/

    gtk_widget_hide(gui->terminal_notebook) ;

    if ( (gui->editor_notebook != NULL) && (gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) > 0) ) {

      if (gtk_widget_get_visible(gui->editor_notebook)) {

        GtkWidget *current_textview = gtk_bin_get_child(GTK_BIN(gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->editor_notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(gui->editor_notebook))))) ;

        gtk_widget_grab_focus(current_textview) ;
      }

    }

  }

  return ;

}

void toggle_display_sidebar_terminals_menuitem(GtkWidget *widget) {
  /** Toggle displaying terminals **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (!  toggle_sidebar_terminals_echo) {

    toggle_sidebar_terminals_echo = true ;

    return ;

  }



  show_hide_sidebar_terminals() ;

  toggle_sidebar_terminals_echo = false ;

  update_toggle_button_state(gui->buttons->terminals_show_button) ;

  toggle_sidebar_terminals_echo = true ;

  return ;


}

void toggle_display_sidebar_terminals_toggle_button(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (!  toggle_sidebar_terminals_echo) {

    toggle_sidebar_terminals_echo = true ;

    return ;

  }

  show_hide_sidebar_terminals() ;

  toggle_sidebar_terminals_echo = false ;

  gtk_menu_item_activate(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(gui->menuitems->show_term_menuitem))) ;

  toggle_sidebar_terminals_echo = true ;

  return ;

}



static void switch_big_term_on_off(void) {

  /** Keep track of the current Big term feature: **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif


  if (gtk_notebook_get_current_page(GTK_NOTEBOOK(gui->main_notebook)) == 0) {

    /** Switch "Big term" on. **/

    gtk_notebook_set_current_page(GTK_NOTEBOOK(gui->main_notebook), active_big_terminal) ;


    switch (active_big_terminal) {

      case 1 :
        gtk_widget_grab_focus(gui->big_terminal) ;
        break ;

      case 2 :
        gtk_widget_grab_focus(gui->big_four_terminals->terminal_1) ;
        break ;
    }

  }
  else {

    /** Switch "Big term" off and set focus to editor. **/

    gtk_notebook_set_current_page(GTK_NOTEBOOK(gui->main_notebook), 0) ;

    /** Set focus to current editor text-view. **/
    if ( (gui->editor_notebook != NULL) && (gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) > 0) ) {

      GtkWidget *current_textview = gtk_bin_get_child(GTK_BIN(gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->editor_notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(gui->editor_notebook))))) ;

      gtk_widget_grab_focus(current_textview) ;

    }

  }

  return ;

}



static void update_toggle_button_state(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (! gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) ) {

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), TRUE) ;

  }
  else {

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), FALSE) ;

  }

  return ;

}


void toggle_big_terminal_main_interface_menuitem(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (! toggle_big_terminals_echo) {

    toggle_big_terminals_echo = true ;

    return ;

  }

  switch_big_term_on_off() ;

  toggle_big_terminals_echo = false ;

  update_toggle_button_state(gui->buttons->big_terminal_button) ;

  toggle_big_terminals_echo = true ;

  return ;

}

void toggle_big_terminal_main_interface_toggle_button(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (! toggle_big_terminals_echo) {

    toggle_big_terminals_echo = true ;

    return ;

  }


  switch_big_term_on_off() ;

  toggle_big_terminals_echo = false ;

  /** Simply call the menu item connected handler to do the work. **/
  gtk_menu_item_activate(GTK_MENU_ITEM( gtk_smart_menu_item_get_menuitem(gui->menuitems->big_term_menuitem))) ;

  toggle_big_terminals_echo = true ;

  return ;

}

static void switch_big_term_fterms(void) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  switch (gtk_notebook_get_current_page(GTK_NOTEBOOK(gui->main_notebook))) {

    case 1 :

      active_big_terminal = 2 ;

      gtk_notebook_set_current_page(GTK_NOTEBOOK(gui->main_notebook), active_big_terminal) ;

      gtk_widget_grab_focus(gui->big_four_terminals->terminal_1) ;

      break ;


    case 2 :

      active_big_terminal = 1 ;

      gtk_notebook_set_current_page(GTK_NOTEBOOK(gui->main_notebook), active_big_terminal) ;

      gtk_widget_grab_focus(gui->big_terminal) ;

      break ;

  }

  return ;
}


void toogle_between_big_terminal_features(GtkWidget *widget) {

  /** Toggle between

      1. a big terminal occupying the full interface.

      2. a big terminal divided into  4 terminals.

  **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (gtk_notebook_get_current_page(GTK_NOTEBOOK(gui->main_notebook)) == 0)  {

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui->buttons->big_terminal_button), TRUE) ;

  }
  else {

    switch_big_term_fterms() ;

  }

  return ;

}




void toggle_display_search_and_replace_bar(GtkToggleButton *togglebutton) {

  /** Toggle displaying search and replace bar. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif



  if (gtk_toggle_button_get_active(togglebutton)) {

    gtk_widget_show(gui->search_and_replace->search_and_replace_main_vbox) ;

    gtk_widget_grab_focus(gui->search_and_replace->search_entry) ;

  }
  else {


    gtk_widget_hide(gui->search_and_replace->search_and_replace_main_vbox) ;

    reset_search() ;

    if ( (gui->editor_notebook != NULL) && (gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) > 0) ) {
      GtkWidget *current_textview = gtk_bin_get_child(GTK_BIN(gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->editor_notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(gui->editor_notebook))))) ;

      gtk_widget_grab_focus(current_textview) ;
    }
  }

  return ;

}

void close_search_and_replace_bar(GtkWidget *button) {

  /** Close search and replace bar. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif



  if ( gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) <= 0)  {

    return ;
  }

  destroy_highlight_selection() ;

  if ( searching.source_search_highlight_context != NULL) {

    gtk_source_search_context_set_highlight(searching.source_search_highlight_context, FALSE) ;

  }



  gtk_widget_hide(gui->search_and_replace->search_and_replace_main_vbox) ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui->buttons->search_and_replace_button), FALSE);

  reset_search() ;



  if ( (gui->editor_notebook != NULL) && (gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) > 0) ) {

    GtkWidget *current_textview = gtk_bin_get_child(GTK_BIN(gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->editor_notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(gui->editor_notebook))))) ;

    gtk_widget_grab_focus(current_textview) ;
  }

  return ;

}

void toggle_use_space_instead_of_tabs(GtkWidget *widget) {

  /** Toggle between use tabs or spaces callback. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  settings.use_spaces_as_tabs = ! settings.use_spaces_as_tabs ;

  GtkWidget *tabs_image = gtk_smart_icon_button_get_image(gui->buttons->tabs_button) ;

  GtkWidget *menuitem_image = gtk_smart_menu_item_get_image(gui->menuitems->switch_tabs_menuitem) ;

  gtk_image_clear(GTK_IMAGE(menuitem_image)) ;

  if (settings.use_spaces_as_tabs) {

      gtk_image_set_from_file(GTK_IMAGE(tabs_image), PATH_TO_BUTTON_ICON "tabs-off.png") ;

      gtk_image_set_from_file(GTK_IMAGE(menuitem_image), PATH_TO_MENU_ICON "tabs-off.png") ;

  }
  else {

     gtk_image_set_from_file(GTK_IMAGE(tabs_image), PATH_TO_BUTTON_ICON "tabs-on.png") ;

     gtk_image_set_from_file(GTK_IMAGE(menuitem_image), PATH_TO_MENU_ICON "tabs-on.png") ;

  }

  switch_spaces_tabs(gui->editor_notebook, &settings.use_spaces_as_tabs) ;

  return ;

}

static void set_new_terminals(GtkWidget *terminals_notebook) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  /** Hard GUI coded terminals: **/

  Terminals *terminals  = g_malloc(sizeof(Terminals)) ;

  terminals->terminal_1 = gtk_itterm_new(TRUE, TRUE) ;
  terminals->terminal_2 = gtk_itterm_new(TRUE, TRUE) ;

  GtkWidget *vteterminal = gtk_itterm_get_vte_terminal(terminals->terminal_1) ;

  /** Set the paned to vertical, so that we can resize the terminals vertically. **/
  terminals->terminals_vertical_paned = gtk_paned_new(GTK_ORIENTATION_VERTICAL) ;



  gtk_paned_pack1(GTK_PANED(terminals->terminals_vertical_paned), terminals->terminal_1, TRUE, TRUE);
  gtk_paned_pack2(GTK_PANED(terminals->terminals_vertical_paned), terminals->terminal_2, TRUE, TRUE);


  /** Terminals notebook container tab. **/
  GtkWidget *terminal_header_hbox          = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;
  GtkWidget *terminal_header_button        = gtk_button_new() ;
  GtkWidget *terminal_header_image         = gtk_image_new_from_file(PATH_TO_MENU_ICON "utilities-terminal.png") ;
  GtkWidget *terminal_header_label         = gtk_label_new(" Terminals ") ;
  GtkWidget *terminal_header_close_button  = gtk_button_new() ;
  GtkWidget *terminal_header_close_image   = gtk_image_new_from_file(PATH_TO_MENU_ICON "window-close.png") ;


  gtk_button_set_image(GTK_BUTTON(terminal_header_button), terminal_header_image) ;
  gtk_button_set_image(GTK_BUTTON(terminal_header_close_button), terminal_header_close_image) ;

  gtk_button_set_relief(GTK_BUTTON(terminal_header_button),       GTK_RELIEF_NONE) ;
  gtk_button_set_relief(GTK_BUTTON(terminal_header_close_button), GTK_RELIEF_NONE) ;

  gtk_box_pack_start(GTK_BOX(terminal_header_hbox),terminal_header_button,        FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(terminal_header_hbox),terminal_header_label,         FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(terminal_header_hbox),terminal_header_close_button,  FALSE, FALSE, 0) ;

  gtk_widget_show_all(terminal_header_hbox) ;



  /** Not deletable first page of the terminal notebook: we can add and remove new pages but not the last. **/
  gint *ret=malloc(sizeof(gint)) ;
  gint ret_tmp = gtk_notebook_append_page(GTK_NOTEBOOK(terminals_notebook), terminals->terminals_vertical_paned, terminal_header_hbox) ;

  memcpy(ret, &ret_tmp, sizeof(gint)) ;

  if (*ret > 0) {
    gtk_widget_show_all(terminals->terminals_vertical_paned) ;
  }

  g_object_set_data(G_OBJECT(gtk_notebook_get_nth_page(GTK_NOTEBOOK(terminals_notebook), *ret)), "term_page_number", ret) ;

  /** Store the notebook page. **/
  g_object_set_data(G_OBJECT(gtk_notebook_get_nth_page(GTK_NOTEBOOK(terminals_notebook), *ret)), "terminals", terminals) ;

  g_signal_connect(G_OBJECT(terminal_header_close_button), "clicked", G_CALLBACK(remove_terminal_tabs), gtk_notebook_get_nth_page(GTK_NOTEBOOK(terminals_notebook), *ret) ) ;

  g_signal_connect(G_OBJECT(gui->terminal_notebook), "page-reordered", G_CALLBACK(terminals_page_reorder), NULL) ;

  gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK(terminals_notebook), terminals->terminals_vertical_paned, TRUE) ;

  gchar *tab_text = g_strdup_printf("Terminals %d",(*ret)+1) ;

  gtk_notebook_set_menu_label_text(GTK_NOTEBOOK(terminals_notebook), terminals->terminals_vertical_paned,  tab_text) ;

  free(tab_text) ;

  gtk_notebook_set_current_page(GTK_NOTEBOOK(terminals_notebook), *ret) ;

  gtk_widget_grab_focus(vteterminal) ;

  return ;

}

void add_new_terminals(GtkWidget *widget) {

  /** Add new terminals to sidebar callback. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  set_new_terminals(gui->terminal_notebook) ;

  return ;
}

void set_syntax_highlight_radio(const gchar *id) {
  /** Called to set the syntax highlight menu radio-button according to the current editor content mime-type. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (id == NULL) {

    return ;
  }

  GList *menu_syntax_items = gtk_container_get_children(GTK_CONTAINER(gui->menus->menu_syntax)) ;


  while (menu_syntax_items != NULL) {

    const gchar *lang_id = g_object_get_data(G_OBJECT(menu_syntax_items->data), "lang_id") ;

    if (lang_id) {

      if (g_strcmp0(id, lang_id) == 0 ) {

        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_syntax_items->data), TRUE);
        break ;

      }

    }

    if (menu_syntax_items->next == NULL) {

      break ;

    }

    menu_syntax_items = menu_syntax_items->next ;

  }

  return ;

}

gboolean set_syntax_highlight(GtkMenuItem *menuitem, GdkEvent  *event, const char *lang_id) {
  /** Radio-buttons syntax highlight setting callback. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif



  if ( g_strcmp0(lang_id, "text") != 0 && lang_id != NULL && gui->editor_notebook ) {

    GtkWidget *notebook_child                = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->editor_notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(gui->editor_notebook))) ;

    if (notebook_child) {

      GtkWidget *current_textview            = gtk_bin_get_child(GTK_BIN(notebook_child)) ;
      GtkTextBuffer *current_textview_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(current_textview)) ;

      GtkSourceLanguageManager *source_language_manager = gtk_source_language_manager_get_default();

      GtkSourceLanguage *source_language = gtk_source_language_manager_get_language(source_language_manager, lang_id) ;

      gtk_source_buffer_set_language(GTK_SOURCE_BUFFER(current_textview_buffer), source_language) ;

    }
  }

  return FALSE ;

}

void set_scheme_highlight(GtkMenuItem *menuitem, char *scheme_id) {
  /** Save the selected scheme to configuration file. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif



  if ( scheme_id != NULL && gui->editor_notebook != NULL ) {

    if (g_strcmp0(scheme_id, settings.scheme) != 0) {

      GKeyFile *conf_file = g_key_file_new() ;

      GError *error = NULL ;

      g_key_file_load_from_file(conf_file, PATH_TO_CONF_FILE, G_KEY_FILE_NONE, &error) ;

      g_free(settings.scheme) ;

      settings.scheme = g_strdup(scheme_id) ;

      if (error) {

        g_warning( _("Error open configuration file.\n%s\nCannot store the scheme id %s"), error->message, settings.scheme) ;
        g_error_free(error)  ;
      }

      g_key_file_set_string(conf_file, "Editor", "scheme", settings.scheme);

      error = NULL ;

      g_key_file_save_to_file(conf_file, PATH_TO_CONF_FILE, &error);

      if (error != NULL) {
        g_warning( _("Error save to configuration file.\n%s\nCannot store the scheme id %s"), error->message, settings.scheme) ;
        g_error_free(error)  ;
      }

      g_key_file_unref(conf_file) ;

      guint16 c=0 ;

      for (  ; c < gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) ; ++c) {

        GtkWidget *notebook_child              = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->editor_notebook), c) ;
        GtkWidget *current_textview            = gtk_bin_get_child(GTK_BIN(notebook_child)) ;
        GtkTextBuffer *current_textview_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(current_textview)) ;

        GtkSourceStyleScheme *style_scheme = gtk_source_style_scheme_manager_get_scheme(source_style_language_manager, scheme_id)  ;

        gtk_source_buffer_set_style_scheme(GTK_SOURCE_BUFFER(current_textview_buffer), style_scheme) ;

      }

    }

  }

  return ;

}

void launch_application(GtkWidget *widget, const char *cmdline) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GError *error = NULL ;

  GAppInfo *app = g_app_info_create_from_commandline(cmdline, cmdline, G_APP_INFO_CREATE_NONE, NULL) ;

  g_app_info_launch(app, NULL, NULL, &error) ;

  if (error != NULL) {

    display_message_dialog(_("Error application launching !"), error->message , GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_error_free(error) ;

    if (app != NULL) {

      g_object_unref(app) ;
    }

    return ;
  }

  g_object_unref(app) ;

  if (! settings.session_mode) {

    return ;
  }

  guint16 length = 0 ;

  if (session->session_app != NULL) {

    length = g_strv_length(session->session_app);
  }

  gchar **strv_app = g_strdupv(session->session_app) ;

  g_strfreev(session->session_app) ;

  session->session_app = calloc(length+2, sizeof(gchar *)) ;

  guint16 i = 0 ;

  for ( ; i < length ; ++i) {

    g_free(session->session_app[i]) ;

    session->session_app[i] = g_strdup(strv_app[i]) ;

  }


  g_strfreev(strv_app) ;

  session->session_app[i] = g_strdup(cmdline) ;

  session->session_app[++i] = NULL ;

  session->session_app_nb = i ;

  return ;
}

void launch_readme_html(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  char *readme_realpath = realpath(PATH_TO_README, NULL) ;

  gchar *readme_uri = g_filename_to_uri(readme_realpath, NULL, NULL);

  gtk_show_uri(NULL, readme_uri, GDK_CURRENT_TIME, NULL);

  g_free(readme_uri) ;

  free(readme_realpath) ;

  return ;

}

void launch_license_html(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  char *html_license_realpath = realpath(PATH_TO_GPL_HTML, NULL) ;

  gchar *html_license_uri = g_filename_to_uri(html_license_realpath, NULL, NULL);

  gtk_show_uri(NULL, html_license_uri, GDK_CURRENT_TIME, NULL);

  g_free(html_license_uri) ;

  free(html_license_realpath) ;

  return ;

}

void quit_application(GtkWidget *widget) {

  /** Quit application callback. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *really_quit = gtk_message_dialog_new(GTK_WINDOW(gui->main_window), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO, _("Do you really want to quit ?")) ;

  gint res = gtk_dialog_run(GTK_DIALOG(really_quit)) ;

  switch (res) {

    case GTK_RESPONSE_YES :

      gtk_widget_destroy(really_quit) ;
      g_application_quit(G_APPLICATION(app)) ;

      return ;

  }

  gtk_widget_destroy(really_quit) ;

  return ;

}
