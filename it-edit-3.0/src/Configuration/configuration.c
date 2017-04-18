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

#include "configuration.h"

char *have_app_application(const char *default_list[]) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  gchar *path_env_variable = (gchar *) g_environ_getenv(g_get_environ(), "PATH")  ;

  int c = 0 ;

  while ( default_list[c] != NULL) {

    char *dir = NULL ;

    while ((dir=strtok(path_env_variable, ":")) != NULL) {

      gchar *filepath = g_strdup_printf("%s/%s",dir,default_list[c]) ;

      if (g_file_test(filepath, G_FILE_TEST_EXISTS)) {

        return filepath ;
      }

      g_free(filepath) ;

      path_env_variable = NULL ;

    }

    ++c ;

  }

  return g_strdup("") ;

}


void set_default_settings_app_conf(void) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  const char *default_app_diff[] = {"kompare", "meld", "fldiff", "diffuse", NULL } ;

  const char *default_app_debugger[] = { "ddd", "xxdbg", "valkyrie", NULL } ;

  const char *default_app_python[] = {"pycrust",  "dreampie", "idle-python2.7", "idle-python3.5", "idle-python3.6", "idle-python3.7", NULL } ;

  const char *default_app_gui_generator[] = {"glade", "wxglade", "kapptemplate", NULL } ;

  const char *default_app_devhelp[] = {"devhelp", NULL} ;

  const char *default_app_dict[] = {"goldendict", "stardict", "opendict", "babiloo", NULL } ;

  const char *default_app_color_picker[] = {"gcolor2", "kcolorchooser", "gpick", NULL} ;

  const char *default_app_calculator[] = { "galculator", "kcalc", "speedcrunch", "gnome-calculator", NULL } ;

  const char *default_app_file_manager[] = { "nautilus", "dolphin", "dolphin4", "gnome-commander", "thunar", "xfe", "pcmanfm", NULL } ;

  const char *default_app_notes[] = { "keepnote", "bijiben", "gnotes" , "xpad", "kjots" , "tuxcards", "rhinote", NULL } ;  // pkg-search note

  const char *default_app_browser[] = {"firefox", "chromium-browser", "konqueror", "epiphany-browser", NULL } ;

  gui_app.diff = have_app_application(default_app_diff) ;

  gui_app.debugger = have_app_application(default_app_debugger) ;

  gui_app.python = have_app_application(default_app_python) ;

  gui_app.gui_designer = have_app_application(default_app_gui_generator) ;

  gui_app.devhelp = have_app_application(default_app_devhelp) ;

  gui_app.calculator = have_app_application(default_app_calculator) ;

  gui_app.color_picker = have_app_application(default_app_color_picker) ;

  gui_app.dictionary = have_app_application(default_app_dict) ;

  gui_app.file_manager = have_app_application(default_app_file_manager) ;

  gui_app.notes = have_app_application(default_app_notes) ;

  gui_app.browser = have_app_application(default_app_browser) ;

  gui_app.others = NULL ;

  gui_app.nb_of_others = 0 ;

  return ;

}

gboolean configuration_file_exists(const gchar *filepath) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (! g_file_test(filepath, G_FILE_TEST_EXISTS)) {

    /** create the configuration folder if doesn't exist. **/

    gchar *config_dirpath = g_path_get_dirname(filepath) ;

    g_mkdir_with_parents(config_dirpath, 0766) ;

    g_free(config_dirpath) ;

    int fd = g_creat(filepath, 0777) ;

    g_close(fd, NULL) ;

    return TRUE ;

  }

  return FALSE ;

}

void set_default_settings_main_conf(void) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GKeyFile *conf_file = g_key_file_new() ;

  /** Editor settings **/
  settings.scheme               = g_strdup("kate") ;
  settings.editor_font          = g_strdup("Monospace 10") ;
  settings.backup_file          = TRUE ;
  settings.display_line_numbers = TRUE ;
  settings.display_tabs_chars   = TRUE ;
  settings.display_all_spaces   = FALSE;
  settings.use_spaces_as_tabs   = TRUE ;
  settings.tabs_width           = 2    ;
  settings.use_auto_indent      = TRUE ;
  settings.indent_width         = 2    ;
  settings.rm_trailing_spaces   = TRUE ;



  settings.overwrite_anyway     = TRUE ;
  settings.newline_type         = 0 ;

  settings.display_all_spaces   = FALSE ;
  settings.warn_read_only       = TRUE ;
  settings.warn_file_open       = TRUE ;
  settings.warn_file_save       = TRUE ;

  settings.save_file_mode       = 0644 ;

  settings.session_mode         = -1 ;

  settings.use_monospace_font   = TRUE ;

  settings.grid_background      = TRUE ;

  settings.use_right_margin     = FALSE ;

  settings.right_margin_value   = 80 ;


  /** @No proposal into the window for this stupid option, but you can set it to TRUE before compiling ! **/
  settings.notifications        = FALSE;


  /** Terminal settings **/
  settings.term_font            = g_strdup("Monospace 10") ;
  settings.font_scale           = 1.0   ;
  settings.term_bg              = g_strdup("#000000") ;
  settings.term_fg              = g_strdup("#FFFFFF") ;

  settings.user_shell           = g_strdup(vte_get_user_shell()) ;
  settings.start_dir            = g_strdup(g_get_home_dir()) ;
  settings.cursor_shape         =  0 ;
  settings.scrollback_lines     = -1 ;
  settings.scroll_on_output     = TRUE ;


  settings.scroll_on_keystroke  = FALSE ;
  settings.backspace_binding    = 0     ;
  settings.delete_binding       = 0     ;
  settings.command              = g_strdup("set -o history ; ls") ;

  settings.cursor_color         = g_strdup("#FFFFFF") ;
  settings.cursor_blink         = 0 ;

  settings.bold_allow           = TRUE ;
  settings.bold_color           = g_strdup("#FFFFFF") ;

  settings.audible_bell         = TRUE  ;
  settings.pointer_autohide     = FALSE ;

  /** Application settings **/
  settings.side_terms_factor    = 1.0     ;
  settings.side_terms_on        = TRUE    ;
  settings.big_term_on          = FALSE   ;
  settings.big_term_div         = FALSE   ;
  settings.buttonbar_on         = TRUE    ;
  settings.fullscreen           = FALSE   ;

  #ifdef GSPELL_SUPPORT
  settings.language_code        =  g_strdup(gspell_language_get_code((gspell_language_lookup("en") == NULL) ? gspell_language_get_default() : gspell_language_lookup("en"))) ;
  #endif

  settings.charset              =  g_strdup(gtk_source_encoding_get_charset(gtk_source_encoding_get_utf8())) ;

  GError *error = NULL ;

  apply_configuration_change(conf_file) ;

  g_key_file_save_to_file(conf_file, PATH_TO_CONF_FILE, &error);

  if (error != NULL) {

    display_message_dialog( _("Error saving configuration file !"), error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_error_free(error) ;

    return ;

  }

  g_key_file_unref(conf_file) ;

  configuration_file_exists(PATH_TO_LINKS_FILE) ;

  return ;

}

void get_main_configuration(void) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif



  if (configuration_file_exists(PATH_TO_CONF_FILE)) {

    set_default_settings_main_conf() ;

  }

  /** Program configuration file. **/
  GKeyFile *conf_file = g_key_file_new() ;

  GError *error = NULL ;

  g_key_file_load_from_file(conf_file, PATH_TO_CONF_FILE, G_KEY_FILE_NONE, &error) ;

  if (error != NULL) {

    g_error_free(error)  ;

    return ;
  }


  /** Read the configuration file:  getting strings values: **/
  settings.scheme                 = g_key_file_get_string(conf_file,  "Editor",     "scheme",               NULL)  ;
  settings.editor_font            = g_key_file_get_string(conf_file,  "Editor",     "font",                 NULL)  ;
  settings.term_font              = g_key_file_get_string(conf_file,  "Terminal",   "font",                 NULL)  ;
  settings.term_bg                = g_key_file_get_string(conf_file,  "Terminal",   "background",           NULL)  ;
  settings.term_fg                = g_key_file_get_string(conf_file,  "Terminal",   "foreground",           NULL)  ;

  settings.user_shell             = g_key_file_get_string(conf_file,  "Terminal",   "user_shell",           NULL)  ;
  settings.start_dir              = g_key_file_get_string(conf_file,  "Terminal",   "start_dir",            NULL)  ;
  settings.command                = g_key_file_get_string(conf_file,  "Terminal",   "command",              NULL)  ;


  /** Read the configuration file: getting boolean or integer values. **/
  settings.backup_file            = g_key_file_get_boolean(conf_file,  "Editor",    "backup",               NULL) ;
  settings.display_line_numbers   = g_key_file_get_boolean(conf_file,  "Editor",    "display_line_numbers", NULL) ;
  settings.display_tabs_chars     = g_key_file_get_boolean(conf_file,  "Editor",    "display_tabs_chars",   NULL) ;
  settings.use_spaces_as_tabs     = g_key_file_get_boolean(conf_file,  "Editor",    "use_spaces_as_tabs",   NULL) ;
  settings.tabs_width             = g_key_file_get_integer(conf_file,  "Editor",    "tabs_width",           NULL) ;
  settings.use_auto_indent        = g_key_file_get_boolean(conf_file,  "Editor",    "use_auto_indent",      NULL) ;
  settings.rm_trailing_spaces     = g_key_file_get_boolean(conf_file,  "Editor",    "rm_trailing_spaces",   NULL) ;
  settings.indent_width           = g_key_file_get_integer(conf_file,  "Editor",    "indent_width",         NULL) ;

  settings.save_file_mode         = g_key_file_get_integer(conf_file,  "Editor",    "save_file_mode",       NULL) ;
  settings.session_mode           = g_key_file_get_integer(conf_file,  "Editor",    "session_mode",         NULL) ;

  settings.newline_type           = g_key_file_get_integer(conf_file,  "Editor",    "newline_type",         NULL) ;
  settings.display_all_spaces     = g_key_file_get_boolean(conf_file,  "Editor",    "display_all_spaces",   NULL) ;
  settings.overwrite_anyway       = g_key_file_get_boolean(conf_file,  "Editor",    "overwrite_anyway",     NULL) ;
  settings.warn_read_only         = g_key_file_get_boolean(conf_file,  "Editor",    "warn_read_only",       NULL) ;
  settings.warn_file_open         = g_key_file_get_boolean(conf_file,  "Editor",    "warn_file_open",       NULL) ;
  settings.warn_file_save         = g_key_file_get_boolean(conf_file,  "Editor",    "warn_file_save",       NULL) ;
  settings.notifications          = g_key_file_get_boolean(conf_file,  "Editor",    "notifications",        NULL) ;
  settings.use_monospace_font     = g_key_file_get_boolean(conf_file,  "Editor",    "use_monospace_font",   NULL) ;

  settings.right_margin_value     = g_key_file_get_integer(conf_file,   "Editor",    "right_margin_value",   NULL)  ;

  #if 0
  settings.smart_backspace        = g_key_file_get_boolean(conf_file,  "Editor",   "smart_backspace",      NULL) ;
  #endif

  settings.grid_background        = g_key_file_get_boolean(conf_file,  "Editor",    "grid_background",      NULL) ;

  settings.use_right_margin       = g_key_file_get_boolean(conf_file,  "Editor",    "use_right_margin",     NULL) ;

  settings.cursor_shape           = g_key_file_get_integer(conf_file,  "Terminal",  "cursor_shape",         NULL) ;
  settings.scrollback_lines       = g_key_file_get_integer(conf_file,  "Terminal",  "scrollback_lines",     NULL) ;
  settings.backspace_binding      = g_key_file_get_integer(conf_file,  "Terminal",  "backspace_binding",    NULL) ;
  settings.delete_binding         = g_key_file_get_integer(conf_file,  "Terminal",  "delete_binding",       NULL) ;
  settings.scroll_on_output       = g_key_file_get_boolean(conf_file,  "Terminal",  "scroll_on_output",     NULL) ;
  settings.scroll_on_keystroke    = g_key_file_get_boolean(conf_file,  "Terminal",  "scroll_on_keystroke",  NULL) ;

  settings.audible_bell           = g_key_file_get_boolean(conf_file,  "Terminal",  "audible_bell",         NULL) ;
  settings.pointer_autohide       = g_key_file_get_boolean(conf_file,  "Terminal",  "pointer_autohide",     NULL) ;

  settings.side_terms_factor      = g_key_file_get_double(conf_file,   "GUI",       "side_terms_factor",    NULL) ;
  settings.side_terms_on          = g_key_file_get_boolean(conf_file,  "GUI",       "side_terms_on",        NULL) ;
  settings.buttonbar_on           = g_key_file_get_boolean(conf_file,  "GUI",       "buttonbar_on",         NULL) ;
  settings.big_term_on            = g_key_file_get_boolean(conf_file,  "GUI",       "big_term_on",          NULL) ;
  settings.big_term_div           = g_key_file_get_boolean(conf_file,  "GUI",       "big_term_div",         NULL) ;
  settings.fullscreen             = g_key_file_get_boolean(conf_file,  "GUI",       "fullscreen",           NULL) ;
  #ifdef GSPELL_SUPPORT
  settings.language_code          = g_key_file_get_string(conf_file,   "GUI",       "language_code",        NULL) ;
  #endif
  settings.charset                = g_key_file_get_string(conf_file,   "GUI",       "charset",              NULL) ;

  #if 0
  settings.spellcheck_inline      = g_key_file_get_boolean(conf_file,  "GUI",        "spellcheck_inline",  NULL) ;
  #endif

  settings.font_scale             = g_key_file_get_double(conf_file,    "Terminal",   "font_scale",         NULL)  ;
  settings.cursor_color           = g_key_file_get_string(conf_file,    "Terminal",   "cursor_color",       NULL)  ;
  settings.cursor_blink           = g_key_file_get_integer(conf_file,   "Terminal",   "cursor_blink",       NULL)  ;
  settings.bold_allow             = g_key_file_get_boolean(conf_file,   "Terminal",   "bold_allow",         NULL)  ;
  settings.bold_color             = g_key_file_get_string(conf_file,    "Terminal",   "bold_color",         NULL)  ;


  g_key_file_unref(conf_file) ;





  return ;
}

void get_app_configuration(void) {

  #ifdef DEBUG
    DEBUG_FUNC_MARK
  #endif


  if (! g_file_test(PATH_TO_APP_FILE, G_FILE_TEST_EXISTS)) {

    /** Create the configuration folder if it does not exists. **/

    char *config_dirpath = g_path_get_dirname(PATH_TO_CONF_FILE) ;

    g_mkdir_with_parents(config_dirpath, 0766) ;

    free(config_dirpath) ;

  }

  GKeyFile *conf_file = g_key_file_new() ;

  GError *error = NULL ;


  if (! g_file_test(PATH_TO_APP_FILE, G_FILE_TEST_EXISTS) ) {

    int fd = g_creat(PATH_TO_APP_FILE, 00777) ;

    g_close(fd, NULL) ;

    set_default_settings_app_conf() ;

    apply_applications_change(conf_file) ;

    g_key_file_set_string_list(conf_file, "Others",  "others",  (const gchar **) gui_app.others, gui_app.nb_of_others)  ;

    g_key_file_save_to_file(conf_file, PATH_TO_APP_FILE, &error);


    if (error != NULL) {

      display_message_dialog("Error generating file !", error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

      g_error_free(error) ;

      error = NULL ;

    }

    g_key_file_unref(conf_file) ;

    return ;

  }

  g_key_file_load_from_file(conf_file, PATH_TO_APP_FILE, G_KEY_FILE_NONE, &error) ;

  if (error != NULL) {

    display_message_dialog("Error loading file !", error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_error_free(error) ;

    error = NULL ;

    return ;

  }



  /** Read the configuration file:  getting strings values: **/
  gui_app.diff              = g_key_file_get_string(conf_file, "Programming",  "diff",            NULL)  ;
  gui_app.debugger          = g_key_file_get_string(conf_file, "Programming",  "debugger",        NULL)  ;
  gui_app.python            = g_key_file_get_string(conf_file, "Programming",  "python",          NULL)  ;
  gui_app.gui_designer      = g_key_file_get_string(conf_file, "Programming",  "gui_designer",    NULL)  ;
  gui_app.devhelp           = g_key_file_get_string(conf_file, "Programming",  "devhelp",         NULL)  ;

  gui_app.calculator        = g_key_file_get_string(conf_file, "Utils",        "calculator",      NULL)  ;
  gui_app.color_picker      = g_key_file_get_string(conf_file, "Utils",        "color_picker",    NULL)  ;
  gui_app.dictionary        = g_key_file_get_string(conf_file, "Utils",        "dictionary",      NULL)  ;
  gui_app.file_manager      = g_key_file_get_string(conf_file, "Utils",        "file_manager",    NULL)  ;
  gui_app.notes             = g_key_file_get_string(conf_file, "Utils",        "notes",           NULL)  ;
  gui_app.browser           = g_key_file_get_string(conf_file, "Utils",        "browser",         NULL)  ;

  gui_app.others            = g_key_file_get_string_list(conf_file, "Others",  "others",  &gui_app.nb_of_others, NULL)  ;

  g_key_file_unref(conf_file) ;

  return ;

}
