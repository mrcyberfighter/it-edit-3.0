/** ***********************************************************************************
  * it-edit the Integrated Terminal Editor: a text editor with severals               *
  * integrated functionalities.                                                      *
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

#include "configuration_callbacks.h"


void view_buttonbar(GtkWidget *widget, GtkWidget *to_show) {
  /** Show | hide the button-bar. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if ( gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget)) ) {

    gtk_widget_show_all(to_show) ;

  }
  else {

    gtk_widget_hide(to_show) ;
  }

  return ;
}

void toggle_fullscreen(GtkWidget *widget, GtkWidget *window) {
  /** Toggle into full-screen mode. **/

  #ifdef DEBUG
    DEBUG_FUNC_MARK
  #endif

  if ( gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget)) )  {

    gtk_window_fullscreen(GTK_WINDOW(window)) ;
  }
  else {

    gtk_window_unfullscreen(GTK_WINDOW(window)) ;
  }

  return ;

}

void apply_configuration_change(GKeyFile *conf_file) {

  g_key_file_set_string(conf_file,   "Editor", "scheme",                  settings.scheme);

  #if 0 /** I can't set another font. **/
  g_key_file_set_string(conf_file,   "Editor", "font",                    settings.editor_font);
  #endif

  g_key_file_set_boolean(conf_file, "Editor", "backup",                  settings.backup_file);

  g_key_file_set_boolean(conf_file, "Editor", "display_line_numbers",    settings.display_line_numbers);

  g_key_file_set_boolean(conf_file, "Editor", "display_tabs_chars",      settings.display_tabs_chars);

  g_key_file_set_boolean(conf_file, "Editor", "use_auto_indent",         settings.use_auto_indent);

  g_key_file_set_integer(conf_file, "Editor", "indent_width",            settings.indent_width);

  g_key_file_set_boolean(conf_file, "Editor", "use_spaces_as_tabs",      settings.use_spaces_as_tabs);

  g_key_file_set_boolean(conf_file, "Editor", "rm_trailing_spaces",      settings.rm_trailing_spaces);

  g_key_file_set_integer(conf_file, "Editor", "tabs_width",              settings.tabs_width);

  g_key_file_set_integer(conf_file,  "Editor", "newline_type",           settings.newline_type) ;

  g_key_file_set_boolean(conf_file,  "Editor", "display_all_spaces",     settings.display_all_spaces) ;

  g_key_file_set_integer(conf_file,  "Editor", "save_file_mode",         settings.save_file_mode) ;

  g_key_file_set_integer(conf_file,  "Editor", "session_mode",           settings.session_mode) ;

  g_key_file_set_boolean(conf_file,  "Editor", "overwrite_anyway",       settings.overwrite_anyway) ;

  g_key_file_set_boolean(conf_file,  "Editor", "warn_read_only",         settings.warn_read_only) ;

  g_key_file_set_boolean(conf_file,  "Editor", "warn_file_open",         settings.warn_file_open) ;

  g_key_file_set_boolean(conf_file,  "Editor", "warn_file_save",         settings.warn_file_save) ;

  g_key_file_set_boolean(conf_file,  "Editor", "notifications",          settings.notifications) ;


  g_key_file_set_string(conf_file,   "Terminal", "font",                  settings.term_font);

  g_key_file_set_double(conf_file,   "Terminal", "font_scale",            settings.font_scale) ;

  g_key_file_set_integer(conf_file,  "Terminal", "cursor_shape",          settings.cursor_shape);

  g_key_file_set_string(conf_file,   "Terminal", "background",            settings.term_bg) ;

  g_key_file_set_string(conf_file,   "Terminal", "foreground",            settings.term_fg) ;

  g_key_file_set_string(conf_file,   "Terminal", "user_shell",            settings.user_shell)  ;
  g_key_file_set_string(conf_file,   "Terminal", "start_dir",             settings.start_dir)  ;

  g_key_file_set_integer(conf_file, "Terminal", "scrollback_lines",      settings.scrollback_lines) ;

  g_key_file_set_boolean(conf_file, "Terminal", "scroll_on_output",      settings.scroll_on_output) ;
  g_key_file_set_boolean(conf_file, "Terminal", "scroll_on_keystroke",   settings.scroll_on_keystroke) ;

  g_key_file_set_integer(conf_file, "Terminal", "backspace_binding",     settings.backspace_binding) ;
  g_key_file_set_integer(conf_file, "Terminal", "delete_binding",        settings.delete_binding) ;

  g_key_file_set_string(conf_file,   "Terminal", "command",               settings.command) ;

  g_key_file_set_integer(conf_file, "Terminal", "cursor_blink",          settings.cursor_blink) ;

  g_key_file_set_boolean(conf_file, "Terminal", "bold_allow",            settings.bold_allow) ;

  g_key_file_set_string(conf_file,   "Terminal", "cursor_color",          settings.cursor_color)  ;
  g_key_file_set_string(conf_file,   "Terminal", "bold_color",            settings.bold_color)  ;

  g_key_file_set_boolean(conf_file, "Terminal", "audible_bell",          settings.audible_bell)  ;
  g_key_file_set_boolean(conf_file, "Terminal", "pointer_autohide",      settings.pointer_autohide)  ;

  g_key_file_set_double(conf_file,  "GUI",      "side_terms_factor",     settings.side_terms_factor) ;

  g_key_file_set_boolean(conf_file, "GUI",      "side_terms_on",         settings.side_terms_on) ;
  g_key_file_set_boolean(conf_file, "GUI",      "buttonbar_on",          settings.buttonbar_on) ;
  g_key_file_set_boolean(conf_file, "GUI",      "big_term_on",           settings.big_term_on) ;
  g_key_file_set_boolean(conf_file, "GUI",      "big_term_div",          settings.big_term_div) ;
  g_key_file_set_boolean(conf_file, "GUI",      "fullscreen",            settings.fullscreen) ;
  #ifdef GSPELL_SUPPORT
  g_key_file_set_string(conf_file,   "GUI",      "language_code",         settings.language_code) ;
  #endif
  g_key_file_set_string(conf_file,   "GUI",      "charset",               settings.charset) ;

  #if 0
  g_key_file_set_boolean(conf_file, "GUI",      "spellcheck_inline",     settings.spellcheck_inline) ;
  #endif

  #ifdef MINI_MULTIPURPOSE_EDITOR_SUPPORT  /** This feature is too much unstable. **/
  g_key_file_set_boolean(conf_file, "Todo", "backup",                   todo_settings->backup_file);
  g_key_file_set_string(conf_file,  "Todo", "font",                     todo_settings->editor_font);
  g_key_file_set_string(conf_file,  "Todo", "lang_id",                  todo_settings->lang_id);
  g_key_file_set_string(conf_file,  "Todo", "scheme",                   todo_settings->scheme);
  g_key_file_set_boolean(conf_file, "Todo", "display_line_numbers",     todo_settings->display_line_numbers);
  g_key_file_set_boolean(conf_file, "Todo", "display_tabs_chars",       todo_settings->display_tabs_chars);
  g_key_file_set_boolean(conf_file, "Todo", "use_auto_indent",          todo_settings->use_auto_indent);
  g_key_file_set_integer(conf_file, "Todo", "indent_width",             todo_settings->indent_width);
  g_key_file_set_boolean(conf_file, "Todo", "use_spaces_as_tabs",       todo_settings->use_spaces_as_tabs);
  g_key_file_set_boolean(conf_file, "Todo", "rm_trailing_spaces",       todo_settings->rm_trailing_spaces);
  g_key_file_set_integer(conf_file, "Todo", "tabs_width",               todo_settings->tabs_width);
  #ifdef GSPELL_SUPPORT
  g_key_file_set_string(conf_file,   "Todo", "language_code",            todo_settings->language_code) ;
  #endif
  g_key_file_set_string(conf_file,   "Todo", "charset",                  todo_settings->charset) ;
  #endif

  return ;

}


void write_changes_to_conf_file(void) {
  /** Write all settings to configuration file. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif



  GKeyFile *conf_file = g_key_file_new() ;

  GError *error = NULL ;

  g_key_file_load_from_file(conf_file, PATH_TO_CONF_FILE, G_KEY_FILE_NONE, &error) ;

  if (error != NULL) {

    gchar *msg = g_strdup_printf( _("Open configuration file error:\n%s\n%s"), g_path_get_basename(PATH_TO_CONF_FILE), error->message) ;

    display_message_dialog( _("Error configuration !"), msg, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    free(msg) ;

    g_error_free(error) ;

    return ;

  }




  apply_configuration_change(conf_file) ;

  g_key_file_save_to_file(conf_file, PATH_TO_CONF_FILE, &error);

  if (error != NULL) {

    display_message_dialog( _("Error saving configuration file !"), error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_error_free(error) ;

  }

  g_key_file_unref(conf_file) ;

  return ;

}

void apply_applications_change(GKeyFile *conf_file) {

  /** Applying changes to the applications key file. **/
 
  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  g_key_file_set_string(conf_file, "Programming",  "diff",            gui_app.diff)  ;
  g_key_file_set_string(conf_file, "Programming",  "debugger",        gui_app.debugger)  ;
  g_key_file_set_string(conf_file, "Programming",  "python",          gui_app.python)  ;
  g_key_file_set_string(conf_file, "Programming",  "gui_designer",    gui_app.gui_designer)  ;
  g_key_file_set_string(conf_file, "Programming",  "devhelp",         gui_app.devhelp)  ;

  g_key_file_set_string(conf_file, "Utils",        "calculator",      gui_app.calculator)  ;
  g_key_file_set_string(conf_file, "Utils",        "color_picker",    gui_app.color_picker)  ;
  g_key_file_set_string(conf_file, "Utils",        "dictionary",      gui_app.dictionary)  ;
  g_key_file_set_string(conf_file, "Utils",        "file_manager",    gui_app.file_manager)  ;
  g_key_file_set_string(conf_file, "Utils",        "notes",           gui_app.notes)  ;
  g_key_file_set_string(conf_file, "Utils",        "browser",         gui_app.browser)  ;

}

void write_changes_to_app_file(void) {

  /** Write all settings to the applications configuration file. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif



  GKeyFile *conf_file = g_key_file_new() ;

  GError *error = NULL ;

  g_key_file_load_from_file(conf_file, PATH_TO_APP_FILE, G_KEY_FILE_NONE, &error) ;

  if (error != NULL) {

    char *msg = g_strdup_printf( _("Configuration file error:\n%s\n%s") , g_path_get_basename(PATH_TO_APP_FILE), error->message) ;

    display_message_dialog( _("Error configuration !"), msg, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    free(msg) ;

    g_error_free(error) ;

    return ;

  }


  apply_applications_change(conf_file) ;

  g_key_file_save_to_file(conf_file, PATH_TO_APP_FILE, &error);

  if (error != NULL) {

    fprintf(stderr, _("Cannot save configuration file:\n%s\n"), error->message) ;

    g_error_free(error) ;
  }

  g_key_file_unref(conf_file) ;

  return ;



}


