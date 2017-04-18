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


#include "./files_callbacks.h"

static GError *errors = NULL ;

static gboolean is_close_all_files = FALSE ;

static void reload_last_documentation_thread(gpointer data) ;

static void reload_last_applications_thread(gpointer *user_data) ;


static void reset_file_selector_curdirpath(gchar *new_value) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (g_strcmp0(file_selector_curdirpath, new_value) == 0) {

    g_free(new_value) ;

    return ;

  }


  g_free(file_selector_curdirpath) ;

  if (! g_file_test(new_value, G_FILE_TEST_IS_DIR)) {

    gchar *dirname = g_path_get_dirname(new_value) ;

    file_selector_curdirpath = dirname ;

    g_free(new_value) ;

  }
  else {

    file_selector_curdirpath = new_value ;

  }

  return ;

}


static gboolean check_if_file_into_editor(GtkWidget *notebook, const char *filepath) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  /** Update the page number data from the tab_button. **/

  const gint nb_of_pages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ;

  gint16 c=0 ;

  for ( ; c < nb_of_pages ; ++c) {

    GtkWidget *notebook_page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), c) ;
    GtkWidget *textview      = gtk_bin_get_child(GTK_BIN(notebook_page)) ;
    GtkTextBuffer *buffer    = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)) ;

    const gchar *filepath_stored = g_object_get_data(G_OBJECT(buffer), "filepath") ;

    if (g_strcmp0(filepath_stored, filepath) == 0) {

      return TRUE ;
    }

  }

  return FALSE ;

}




void register_files(void) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (! settings.session_mode) {

    return ;
  }

  const gint nb_page = gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) ;

  if (nb_page == 0) {

    return ;
  }

  g_strfreev(session->session_files) ;

  session->session_files = calloc(nb_page+1, sizeof(gchar *)) ;

  gint16 i=0  ;

  gint16 c=0 ;

  for ( ; c < nb_page ; ++c) {

    GtkWidget *page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->editor_notebook), c) ;

    GtkWidget *current_textview = gtk_bin_get_child(GTK_BIN(page)) ;

    GtkTextBuffer *current_buffer   = gtk_text_view_get_buffer(GTK_TEXT_VIEW(current_textview)) ;

    const gchar *filepath = g_object_get_data(G_OBJECT(current_buffer), "filepath") ;

    gchar *dirpath = g_path_get_dirname(filepath) ;

    if (g_strcmp0(dirpath, g_get_tmp_dir()) == 0) {

        g_free(dirpath) ;

        continue ;
    }

    g_free(dirpath) ;

    g_free(session->session_files[i]) ;

    session->session_files[i] = g_strdup(filepath) ;

    ++i ;

  }

  session->session_files[i] = NULL ;

  session->session_files_nb = ++i ;

  return ;

}

void process_selected_file(GFile *g_file) {

  /** Callback from the open file selector for every selected file. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  /** FIXME: Open a distant files !!! */

  gchar *filepath = g_file_get_path(g_file) ;

  if (filepath != NULL) {

    if (settings.warn_file_open) {

      if (check_if_file_into_editor(gui->editor_notebook, filepath)) {

        if (! get_answers_message_dialog( _("File already into editor !"), _("The file you attempt to load is already into the editor.\nLoading file into editor anyway ?"), ("Load file"), _("Don't load file"), GTK_MESSAGE_WARNING) ) {

          g_free(filepath) ;

          return ;
        }

      }

    }


    /** Every file has an associated File_Editor structure which hold the Scrolled Window, TextView and GtkSourceBuffer. **/
    File_Editor *file_editor = g_malloc(sizeof(File_Editor)) ;

    if (file_editor != NULL) {

      /** Add file content to the editor has an new notebook page. @NOTE: this function does more than the initialization work, take a look at it. **/
      add_file_to_editor(gui->editor_notebook, file_editor, g_file, false) ;

    }

    while (gtk_events_pending()) { //

      gtk_main_iteration() ;

    }

  }

  return ;

}


void reload_last_files(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GKeyFile *conf_file = g_key_file_new() ;

  g_key_file_load_from_file(conf_file, PATH_TO_SESSION_FILE, G_KEY_FILE_NONE, &errors) ;

  if (errors != NULL) {

    g_clear_error(&errors) ;

    if (conf_file != NULL) {

      g_key_file_unref(conf_file) ;
    }

    return ;

  }

  gsize nb_of_files = 0 ;

  gchar **files_list = NULL ;

  files_list = g_key_file_get_string_list(conf_file, "Session", "session_files", &nb_of_files, NULL) ;

  if (nb_of_files == 0) {

    return ;
  }

  guint16 c=0 ;

  for ( ; c < nb_of_files ; ++c) {

    if (g_file_test(files_list[c], G_FILE_TEST_EXISTS)) {

      GFile *g_file = g_file_new_for_path(files_list[c]) ;

      File_Editor *file_editor = g_malloc(sizeof(File_Editor)) ;

      add_file_to_editor(gui->editor_notebook,  file_editor, g_file, false) ;

    }

  }

  g_strfreev(files_list) ;

  g_key_file_unref(conf_file) ;

  register_files() ;

  return ;

}



static void reload_last_documentation_thread(gpointer data) {


  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  /** Function to reload last session opened documentation.
    *
    *   NOTE:
    *
    *     It was a hard task to success to load all the documentation,
    *
    *     because most of it was *.HTML documentation launched with firefox
    *     and firefox doesn't properly open every file in the same application.
    *
    *     Firefox say that that it's currently running and refused to open more files...
    *
    *     FIXME: Load every stored file in the same instance of your @BROWSER.
    *
    *     @Advice: You can open your browser before reload last documentation.
    *
    *  @Sorry for the dysfunction.
    *
  **/

  GKeyFile *conf_file = g_key_file_new() ;

  g_key_file_load_from_file(conf_file, PATH_TO_SESSION_FILE, G_KEY_FILE_NONE, &errors) ;

  if (errors != NULL) {

    g_clear_error(&errors) ;

    if (conf_file != NULL) {

      g_key_file_unref(conf_file) ;
    }

    return ;

  }

  gsize nb_of_doc = 0 ;

  gchar **doc_list = NULL ;

  doc_list = g_key_file_get_string_list(conf_file, "Session", "session_doc", &nb_of_doc, NULL) ;

  if (nb_of_doc <= 0) {

    g_key_file_unref(conf_file) ;

    return ;
  }

  g_key_file_unref(conf_file) ;






  gboolean doc_have_html = FALSE ;

  gchar **cmdline_strv = calloc(nb_of_doc+2, sizeof(gchar *)) ;

  gint16 cc = 0 ;

  g_free(cmdline_strv[cc]) ;

  GAppInfo *appinfo_html = g_app_info_get_default_for_type("text/html", TRUE);

  cmdline_strv[cc++] = g_strdup(g_app_info_get_executable(appinfo_html)) ;

  guint16 c = 0 ;

  for ( ; c < nb_of_doc ; ++c) {

    GFile *g_file = g_file_new_for_uri(doc_list[c]);

    GFileInfo *g_file_info = g_file_query_info(g_file, G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE, G_FILE_QUERY_INFO_NONE, NULL, NULL);

    const gchar *attribute_str_value = g_file_info_get_attribute_string(g_file_info, G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE) ;

    gchar *mimetype = g_content_type_get_mime_type(attribute_str_value) ;

    if (g_strcmp0(mimetype, "text/html") == 0) {
    
      g_free(cmdline_strv[cc]) ;
    
      cmdline_strv[cc++] = g_strdup(doc_list[c]) ;
    
      if (! doc_have_html) {

        doc_have_html = TRUE ;
      }

    }
    else {

      g_app_info_launch_default_for_uri(doc_list[c], NULL, NULL) ;

    }

    g_free(mimetype) ;

    g_object_unref(g_file_info) ;

    g_object_unref(g_file) ;


  }



  GMainContext *this_thread_main_context = g_main_context_get_thread_default() ;

  if (doc_have_html) {
 
    while (g_main_context_pending(this_thread_main_context)) {

      g_main_context_iteration(this_thread_main_context, FALSE) ;

    }
     
    gchar *cmdline_str = g_strjoinv(" ", cmdline_strv) ;
  
    g_spawn_command_line_sync(cmdline_str, NULL, NULL, NULL, &errors) ;
  
    while (g_main_context_pending(this_thread_main_context)) {

      g_main_context_iteration(this_thread_main_context, FALSE) ;

    }

    if (errors != NULL) {

      display_message_dialog( _("Error launching HTML files !"), errors->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

      g_clear_error(&errors) ;

      return ;

    }
   
    g_free(cmdline_str) ;
   
  }

  g_strfreev(cmdline_strv) ;

  if (! settings.session_mode) {

    return ;
  }

  /** Copy the reload list to the current list, so that it get not lost for further session. **/
  g_strfreev(session->session_doc) ;

  session->session_doc = calloc(nb_of_doc+2, sizeof(gchar *)) ;

  guint i = 0 ;

  for ( ; i < nb_of_doc ; ++i) {

    g_free(session->session_doc[i]) ;

    session->session_doc[i] = g_strdup(doc_list[i]) ;

  }

  session->session_doc[++i] = NULL ;

  session->session_doc_nb = i ;


  g_strfreev(doc_list) ;

  return ;

}

void reload_last_documentation(GtkWidget *widget) {

  GThread *thread = g_thread_try_new(NULL, (GThreadFunc) reload_last_documentation_thread, NULL, &errors) ;

  if (errors != NULL) {

    display_message_dialog( _("Error launching documentation !"), errors->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_clear_error(&errors) ;

  }

  if (thread != NULL) {

    g_thread_unref(thread) ;

  }

  return ;

}

void reload_last_applications(GtkWidget *widget) {

  GThread *thread = g_thread_try_new(NULL, (GThreadFunc) reload_last_applications_thread, NULL, &errors) ;

  if (errors != NULL) {

    display_message_dialog( _("Error launching application(s) !"), errors->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_clear_error(&errors) ;

  }

  if (thread != NULL) {

    g_thread_unref(thread) ;

  }

  return ;

}

static void reload_last_applications_thread(gpointer *user_data) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GKeyFile *conf_file = g_key_file_new() ;

  g_key_file_load_from_file(conf_file, PATH_TO_SESSION_FILE, G_KEY_FILE_NONE, &errors) ;

  if (errors != NULL) {

    g_error_free(errors) ;

    errors = NULL ;

    if (conf_file != NULL) {

      g_key_file_unref(conf_file) ;
    }

    return ;

  }


  gsize nb_of_app = 0 ;

  gchar **app_list = NULL ;

  app_list = g_key_file_get_string_list(conf_file, "Session", "session_app", &nb_of_app, NULL) ;


  if (nb_of_app == 0) {

    return ;
  }

  guint16 c=0 ;

  for ( ; c < nb_of_app ; ++c) {

    GAppInfo *app = g_app_info_create_from_commandline(app_list[c], app_list[c], G_APP_INFO_CREATE_NONE, NULL) ;

    if (app != NULL) {

      g_app_info_launch(app, NULL, NULL, NULL) ;

    }

  }

  /** Copy the reload list to the current list. **/
  g_strfreev(session->session_app) ;

  if (! settings.session_mode) {

    return ;
  }

  session->session_app = calloc(nb_of_app+2, sizeof(gchar *)) ;

  guint16 i = 0 ;

  for ( ; i < nb_of_app ; ++i) {

    g_free(session->session_app[i]) ;

    session->session_app[i] = g_strdup(app_list[i]) ;

  }

  session->session_app[++i] = NULL ;

  session->session_app_nb = i ;

  g_strfreev(app_list) ;

  g_key_file_unref(conf_file) ;

  return ;

}




void reload_last_session(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  reload_last_files(NULL) ;

  reload_last_documentation(NULL) ;

  reload_last_applications(NULL) ;

  return ;
}

void clear_session(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GKeyFile *conf_file = g_key_file_new() ;

  g_strfreev(session->session_files) ;

  session->session_files = NULL ;

  session->session_files_nb = 0 ;

  g_strfreev(session->session_doc) ;

  session->session_doc = NULL ;

  session->session_doc_nb = 0 ;

  g_strfreev(session->session_app) ;

  session->session_app = NULL ;

  session->session_app_nb = 0 ;

  g_key_file_set_string_list(conf_file, "Session", "session_files", (const gchar **) session->session_files, session->session_files_nb) ;

  g_key_file_set_string_list(conf_file, "Session", "session_doc", (const gchar **)  session->session_doc, session->session_doc_nb) ;

  g_key_file_set_string_list(conf_file, "Session", "session_app", (const gchar **) session->session_app, session->session_app_nb) ;

  g_key_file_save_to_file(conf_file, PATH_TO_SESSION_FILE, &errors);

  if (errors != NULL) {

    display_message_dialog("Error clearing session !", errors->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_error_free(errors) ;

    errors = NULL ;

    return ;

  }

  g_key_file_unref(conf_file) ;

  return ;

}

void open_file(GtkWidget *widget) {

  /** File open callback. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif


  GtkWidget *file_chooser = gtk_file_chooser_dialog_new( _("Open file"),
                                                        GTK_WINDOW(gui->main_window),
                                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                                        _("Cancel"), GTK_RESPONSE_CANCEL,
                                                        _("Open"),  GTK_RESPONSE_ACCEPT,
                                                        NULL);

  const gint page_number = gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) ;

  if (page_number <= 0) {

    goto editor_empty ;

  }


  if (g_strcmp0(file_selector_curdirpath, g_get_tmp_dir()) != 0) { /** To enable according user settings. **/

    const gchar *filepath = g_object_get_data(G_OBJECT(current_editor.current_buffer), "filepath") ;

    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(file_chooser), filepath);


  }
  else {

    editor_empty :

    if (g_strcmp0(file_selector_curdirpath, g_get_tmp_dir()) == 0) {

      reset_file_selector_curdirpath(g_strdup(g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP))) ;

      /** @NOTE: Would use g_path_get_home_dir(..) but it would make bug the file selector, so we set the file selector to desktop. **/

    }

    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser), file_selector_curdirpath) ;

  }

  gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(file_chooser), TRUE) ;

  gint res = gtk_dialog_run (GTK_DIALOG (file_chooser));

  if (res == GTK_RESPONSE_ACCEPT) {
    /** The user has selected file(s) **/

    GtkFileChooser *chooser = GTK_FILE_CHOOSER (file_chooser);

    GSList *selected_files = gtk_file_chooser_get_filenames(chooser) ;

    selected_files = g_slist_nth(selected_files, 0) ;

    while (selected_files != NULL) {

      if (selected_files->data != NULL) {

        GFile *tmp = g_file_new_for_path((const gchar *) selected_files->data) ;

        if (tmp != NULL) {

          process_selected_file(tmp) ;

        }

      }

      if (selected_files->next != NULL) {

        selected_files = selected_files->next ;
      }
      else {

        break ;
      }

    }

    g_slist_free(selected_files) ;

    register_files() ;

  }

  gtk_widget_destroy(file_chooser);

  return ;

}

void open_recent(GtkRecentChooser *chooser) {
  /** The user select an item from the recent file list. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  /** retrieve the file URI from the selected item. **/
  gchar *uri = gtk_recent_chooser_get_current_uri(GTK_RECENT_CHOOSER(chooser)) ;

  if (uri != NULL)  {

    gchar *filepath = g_filename_from_uri(uri, NULL, NULL) ;

    if (settings.warn_file_open) {

      if (check_if_file_into_editor(gui->editor_notebook, filepath)) {

        if (! get_answers_message_dialog( _("File already into editor !"), _("The file you attempt to load is already into the editor.\nLoading file into editor anyway ?"), ("Load file"), _("Don't load file"), GTK_MESSAGE_WARNING) ) {

          g_free(filepath) ;

          return ;
        }

      }

    }

    /** Every file has an associated File_Editor structure which hold the Scrolled Window, TextView and GtkSourceBuffer. **/
    File_Editor *file_editor = g_malloc(sizeof(File_Editor)) ;

    /** Add file content to the editor has an new notebook page. @NOTE: this function does more than the initialization work, take a look at it. **/
    GFile *g_file = g_file_new_for_path(filepath) ;


    if (g_file != NULL) {

      add_file_to_editor(gui->editor_notebook, file_editor , g_file, false) ;

    }

    g_free(uri) ;

    g_free(filepath) ;

  }

  register_files() ;

  return ;

}



void save_as_file(GtkWidget *widget) {
  /** Save the current editor content as the choosen file. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *file_chooser = gtk_file_chooser_dialog_new( _("Save as file"),
                                                        GTK_WINDOW(gui->main_window),
                                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                                        _("Cancel"),  GTK_RESPONSE_CANCEL,
                                                        _("Save as"), GTK_RESPONSE_ACCEPT,
                                                        NULL) ;

  if (g_strcmp0(file_selector_curdirpath, g_get_tmp_dir()) == 0) {

    reset_file_selector_curdirpath(g_strdup(g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP))) ;

    /** @NOTE: Would use g_path_get_home_dir(..) but it would make bug the file selector, so we set the file selector to desktop. **/

  }

  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser), file_selector_curdirpath );

  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(file_chooser), TRUE);


  gint res = gtk_dialog_run(GTK_DIALOG(file_chooser)) ;

  if (res == GTK_RESPONSE_ACCEPT) {

    gchar *filepath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));

    File_Editor *file_editor = (File_Editor *) g_object_get_data(G_OBJECT(current_editor.current_textview), "file_editor") ;

    GFile *g_file_current = g_file_new_for_path(filepath) ;

    if (! source_file_saver_with_target(GTK_SOURCE_BUFFER(current_editor.current_buffer), file_editor->source_file, g_file_current, FALSE) ) {

      goto end ;

    }

    /** Getting current editor content:  **/
    GtkTextIter iter_start, iter_end  ;

    gtk_text_buffer_get_start_iter(current_editor.current_buffer, &iter_start);
    gtk_text_buffer_get_end_iter(current_editor.current_buffer,   &iter_end);

    gchar *file_content = gtk_text_buffer_get_text(current_editor.current_buffer, &iter_start, &iter_end, FALSE) ;

    /** Only useful if the content type has changed like a new file saved as a *.c file. **/
    GtkSourceLanguage        *source_language         = NULL ;

    gboolean result_uncertain ;
    gchar *content_type ;

    content_type = g_content_type_guess(filepath, (const guchar *) file_content, strlen(file_content), &result_uncertain) ;

    g_free(file_content) ;

    if (content_type && source_language_manager) {

      source_language = gtk_source_language_manager_guess_language(source_language_manager, filepath, content_type);

      if (source_language) {

        set_syntax_highlight_radio(gtk_source_language_get_id(source_language)) ;

        gtk_source_buffer_set_language(GTK_SOURCE_BUFFER(current_editor.current_buffer), source_language) ;

        g_object_set_data(G_OBJECT(current_editor.current_textview), "lang_id", (char *) gtk_source_language_get_id(source_language)) ;
      }

      g_free(content_type) ;

    }

    /** Update the notebook label tab. **/
    GtkWidget *notebook_tab = gtk_notebook_get_tab_label(GTK_NOTEBOOK(gui->editor_notebook), current_editor.current_notebook_page);

    /** The tab contains an mime-type icon, the filename and the page closing button. **/
    GList *tab_box_list = gtk_container_get_children(GTK_CONTAINER(notebook_tab)) ;

    GtkWidget *tab_icon  = g_list_nth_data(tab_box_list, 0) ;

    GtkWidget *tab_label = g_list_nth_data(tab_box_list, 1) ;

    gtk_widget_set_tooltip_text(tab_label, filepath) ;

    gtk_widget_set_tooltip_text(gui->bottom_bar->filename_label, filepath) ;

    if (tab_label != NULL) {

      gchar *basename = g_path_get_basename(filepath) ;

      gtk_label_set_text(GTK_LABEL(tab_label), basename) ;

      g_free(basename) ;

    }



    if  (source_language != NULL) {

      gchar **mimetypes = NULL ;

      gint16 c=0 ;

      for ( ; ; ++c) {

          if ((mimetypes = gtk_source_language_get_mime_types(source_language)) == NULL) {

            break ;

          }




          gchar *mimetype = mimetypes[c] ;

          if (mimetype == NULL) {

              /** don't find an specific mime-type for this new file extension use default icon. **/
              g_object_set(G_OBJECT(tab_icon),"file", PATH_TO_MIMETYPE_ICON "unknown.png", NULL) ;
              break ;
          }

          /** We search for an image filename ending with the corresponding mime-type: **/
          char *ptr = strchr(mimetype, '/') ;

          if (ptr != NULL) {

            /** Simply pointer arithmetic to exchange the '/' (used in mime-types) and the '-' (used in the images names) character. **/
            mimetype[ptr - mimetype] = '-' ;


            gchar *filepath = g_strdup_printf("%s%s.png", PATH_TO_MIMETYPE_ICON, mimetype) ;

            if ( g_file_test(filepath, G_FILE_TEST_EXISTS) && tab_icon != NULL ) {
              /** We found a corresponding image for this mime-type. **/
              g_object_set(G_OBJECT(tab_icon),"file", filepath, NULL) ;
              free(filepath) ;
              break ;
            }

            free(filepath) ;

          }

      }

      g_strfreev(mimetypes) ;

    }

    g_list_free(tab_box_list) ;

    /** Storing file-path for further saving operations. **/
    g_object_set_data(G_OBJECT(current_editor.current_buffer), "filepath", filepath) ;

    /** setting the base filename in the bottom bar. **/
    gchar *basename = g_path_get_basename(filepath) ;

    gtk_label_set_text(GTK_LABEL(gui->bottom_bar->filename_label), basename) ;


    if (settings.rm_trailing_spaces) {

      /** Deleting trailing spaces. **/
      gchar *trailing_spaces_deleting = g_strdup_printf("sed -i 's/[[:space:]]$//' '%s'", filepath) ;

      int ret ;

      if ((ret = system(trailing_spaces_deleting)) == -1) {

        g_warning( _("Removing trailing space failure:\n%s\n"), trailing_spaces_deleting) ;

      }

      free(trailing_spaces_deleting) ;
    }

    gtk_notebook_set_menu_label_text(GTK_NOTEBOOK(gui->editor_notebook), file_editor->scrolled_window,  basename) ;

    g_free(basename) ;

    g_chmod(filepath, settings.save_file_mode);

  }

  end :

  gtk_widget_destroy(file_chooser);

  register_files() ;

}

void save_file(GtkWidget *widget) {
  /** Save editor content as the stored filename. **/

  #ifdef DEBUG
    DEBUG_FUNC_MARK
  #endif

  /** Retrieve the stored file-path: **/
  const gchar *filepath =  g_object_get_data(G_OBJECT(current_editor.current_buffer), "filepath") ;

  char *cmp_filepath = g_path_get_dirname(filepath) ;

  if ( g_strcmp0(g_get_tmp_dir(), cmp_filepath) == 0) {

    /** File is a start file. **/

    free(cmp_filepath) ;

    save_as_file(NULL) ;

    return ;

  }


  free(cmp_filepath) ;

  File_Editor *file_editor = g_object_get_data(G_OBJECT(current_editor.current_textview), "file_editor") ;

  GFile *g_file_current = g_file_new_for_path(filepath) ;

  gtk_source_file_set_location(file_editor->source_file, g_file_current) ;

  g_object_unref(g_file_current) ;

  source_file_saver(GTK_SOURCE_BUFFER(current_editor.current_buffer), file_editor->source_file) ;


  /** Update the notebook label tab. **/
  GtkWidget *notebook_tab = gtk_notebook_get_tab_label(GTK_NOTEBOOK(gui->editor_notebook), current_editor.current_notebook_page);

  /** The tab contains an mime-type icon, the filename and the page closing button. **/
  GList *tab_box_list = gtk_container_get_children(GTK_CONTAINER(notebook_tab)) ;

  GtkWidget *tab_label =  g_list_nth_data(tab_box_list, 1) ;

  gchar *basename = g_path_get_basename(filepath) ;

  /** We reset the filename without the asterisk ('*'). **/
  gtk_label_set_text(GTK_LABEL(tab_label), basename) ;

  g_list_free(tab_box_list) ;


  /** setting the base filename in the bottom bar. **/
  gtk_label_set_text(GTK_LABEL(gui->bottom_bar->filename_label), basename) ;

  gtk_widget_set_tooltip_text(gui->bottom_bar->filename_label, filepath) ;

  gtk_notebook_set_menu_label_text(GTK_NOTEBOOK(gui->editor_notebook), file_editor->scrolled_window, basename) ;

  g_free(basename) ;

  if (settings.rm_trailing_spaces) {

    /** Deleting trailing spaces. **/
    gchar *trailing_spaces_deleting = g_strdup_printf("sed -i 's/[[:space:]]$//' '%s'", filepath) ;

    int ret ;

    if ((ret = system(trailing_spaces_deleting)) == -1) {
      g_warning( _("Removing trailing space failure:\n%s\n"), trailing_spaces_deleting) ;
    }

    g_free(trailing_spaces_deleting) ;
  }

  g_chmod(filepath, settings.save_file_mode);

  return ;

}

void save_all_file(GtkWidget *button) {

  /** All files saving callback. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  gint16 c=0 ;

  const gint pages_number = gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) ;

  for ( ; c < pages_number ; ++c) {

    /** We iterate over every notebook page... **/

    GtkWidget *notebook_page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->editor_notebook), c) ;
    GtkWidget *notebook_tab  = gtk_notebook_get_tab_label(GTK_NOTEBOOK(gui->editor_notebook), notebook_page);

    /** The tab contains an mime-type icon, the filename and the page closing button. **/
    GList *tab_box_list = gtk_container_get_children(GTK_CONTAINER(notebook_tab)) ;


    GtkWidget *tab_label_widget = g_list_nth_data(tab_box_list, 1) ;

    const gchar *tab_label = gtk_label_get_text(GTK_LABEL(tab_label_widget)) ;

    /** Check if the file is modified (marked with an'*') and is not the default "@New" named file. **/
    if (tab_label[0] == '*' && g_strcmp0(tab_label,"*New") != 0) {

      GtkWidget     *current_textview        = gtk_bin_get_child(GTK_BIN(notebook_page)) ;
      GtkTextBuffer *current_buffer          = gtk_text_view_get_buffer(GTK_TEXT_VIEW(current_textview)) ;

      const gchar *filepath = g_object_get_data(G_OBJECT(current_buffer), "filepath") ;

      if (filepath != NULL) {

        File_Editor *file_editor = g_object_get_data(G_OBJECT(current_textview), "file_editor") ;

        GFile *g_file_current = g_file_new_for_path(filepath) ;

        gtk_source_file_set_location(file_editor->source_file, g_file_current) ;

        g_object_unref(g_file_current) ;


        if (! source_file_saver(GTK_SOURCE_BUFFER(current_buffer), file_editor->source_file)) {

          continue ;

        }

        gtk_widget_set_tooltip_text(tab_label_widget, filepath) ;

        gtk_widget_set_tooltip_text(gui->bottom_bar->filename_label, filepath) ;

        gchar *basename = g_path_get_basename(filepath) ;

        /** setting the base filename in the bottom bar. **/
        gtk_label_set_text(GTK_LABEL(tab_label_widget), basename) ;

        g_free(basename) ;

        if (settings.rm_trailing_spaces) {
          /** Deleting trailing spaces. **/

          gchar *trailing_spaces_deleting = g_strdup_printf("sed -i 's/[[:space:]]$//' '%s'", (char *) filepath) ;

          int ret ;

          if ((ret = system(trailing_spaces_deleting)) == -1) {
            g_warning(_("Removing trailing space failure:\n%s\n"), trailing_spaces_deleting) ;
          }

          free(trailing_spaces_deleting) ;
        }

        g_chmod(filepath, settings.save_file_mode);

      }
    }

    g_list_free(tab_box_list) ;

  }

  return ;
}



void close_all_files(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  gint nb_of_pages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) ;

  if (nb_of_pages <= 0) {

    return ;

  }

  is_close_all_files = TRUE ;

  while (nb_of_pages > 0) {

    gtk_notebook_get_current_page(GTK_NOTEBOOK(gui->editor_notebook)) ;

    GtkWidget *notebook_page      = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->editor_notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(gui->editor_notebook))-1) ;

    /** Get the notebook label tab **/
    GtkWidget *notebook_tab = gtk_notebook_get_tab_label(GTK_NOTEBOOK(gui->editor_notebook), notebook_page);

    /** The tab contains an mime-type icon, the filename and the page closing button. **/
    GList *tab_box_list = gtk_container_get_children(GTK_CONTAINER(notebook_tab)) ;

    GtkWidget *tab_button =  g_list_nth_data(tab_box_list, 3) ;

    gint *page_number = (gint *) g_object_get_data(G_OBJECT(tab_button), "page_number") ;

    g_list_free(tab_box_list) ;

    close_file(NULL, page_number) ;


    --nb_of_pages ;

  }

  is_close_all_files = FALSE ;

  reset_file_selector_curdirpath(g_strdup(g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP))) ;

  /** @NOTE: Would use g_path_get_home_dir(...) but it would make bug the file selector, so we set the file selector to desktop. **/

  return ;

}




void close_file(GtkWidget *widget, gint *page_nb) {

    /** Close a file by removing it from the notebook and reorder the page numbers.
      *****************************************************************************/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif


  gint *page_number = NULL ;

  if (widget != NULL) {

    page_number = g_object_get_data(G_OBJECT(widget), "page_number") ;
  }
  else {

    page_number = page_nb ;
  }


  /**  Getting the notebook page, text-view and buffer **/
  GtkWidget *notebook_page      = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->editor_notebook), *page_number) ;
  GtkWidget *current_textview   = gtk_bin_get_child(GTK_BIN(notebook_page)) ;



  if (! is_close_all_files) {

    GtkWidget *notebook_tab  = gtk_notebook_get_tab_label(GTK_NOTEBOOK(gui->editor_notebook), notebook_page);

    /** The tab contains an mime-type icon, the filename and the page closing button. **/
    GList *tab_box_list = gtk_container_get_children(GTK_CONTAINER(notebook_tab)) ;

    GtkWidget *tab_label =  g_list_nth_data(tab_box_list, 1) ;



    /** We get the filename tab label. **/
    const gchar *tab_label_txt = gtk_label_get_text(GTK_LABEL(tab_label)) ;



    /** Check if the file is modified (marked with an '*') and is not the default "@New" named file. **/
    if (tab_label_txt[0] == '*') {

      if (settings.warn_file_save) {

        if (get_answers_message_dialog( _("File not saved !"), _("You attempt to close an unsaved file.\nReally close file without saving ?"), _("Don't close file"), _("Close file"), GTK_MESSAGE_WARNING)) {

          return ;
        }

      }

    }


    g_list_free(tab_box_list) ;

  }




  gchar *filepath = g_object_get_data(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(current_textview))), "filepath") ;

  g_free(filepath) ;

  File_Editor *file_editor = g_object_get_data(G_OBJECT(current_textview), "file_editor") ;

  if (g_object_get_data(G_OBJECT(file_editor->textview), "highlight_textview_selection")) {

    g_free(g_object_get_data(G_OBJECT(file_editor->textview), "highlight_textview_selection")) ;



  }

  g_object_unref(file_editor->source_file) ;

  free(file_editor) ;

  gtk_notebook_remove_page(GTK_NOTEBOOK(gui->editor_notebook), *page_number );

  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) == 0) {

    gtk_label_set_text(GTK_LABEL(gui->bottom_bar->current_row_col), _("Line: 0 ; Column: 0 ;") ) ;
    gtk_label_set_text(GTK_LABEL(gui->bottom_bar->total_rows_cols), _("Total lines: 0 ; Total chars: 0 ;") ) ;
    gtk_label_set_text(GTK_LABEL(gui->bottom_bar->filename_label), "") ;


    reset_file_selector_curdirpath(g_strdup(g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP))) ;

    /** @NOTE: Would use g_path_get_home_dir(..) but it would make bug the file selector, so we set the file selector to desktop. **/

    free(page_number) ;

    return  ;

  }


  free(page_number) ;

  update_page_number(gui->editor_notebook) ;

  register_files() ;

  return  ;

}

void new_file(GtkWidget *widget) {

  /** "/tempdir/New" file callback. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif



  /** Every file has an associated File_Editor structure which hold the Scrolled Window, Text-view and @GtkSourceBuffer. **/
  File_Editor *file_editor = g_malloc(sizeof(File_Editor)) ;

  gchar *tmp_name = NULL ;

  gint ret = g_file_open_tmp("New_XXXXXX", &tmp_name, NULL) ;

  g_close(ret, NULL);

  GFile *new_g_file = g_file_new_for_path(tmp_name) ;

  g_free(tmp_name) ;

  /** Add file content to the editor has an new notebook page. @NOTE: this function does more than the initialization work, take a look at it. **/
  add_file_to_editor(gui->editor_notebook, file_editor, new_g_file, true) ;

  return ;

}

void reloading_file(GtkWidget *widget) {

  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) <= 0) {

    return ;

  }

  File_Editor *file_editor = g_object_get_data(G_OBJECT(current_editor.current_textview), "file_editor") ;

  const gchar *filepath = g_object_get_data(G_OBJECT(current_editor.current_buffer), "filepath") ;

  reload_file(GTK_SOURCE_BUFFER(current_editor.current_buffer), filepath, file_editor) ;

  /** We mark the TextBuffer as not modified. **/
  gtk_text_buffer_set_modified(current_editor.current_buffer, FALSE) ;

}



guint16 files_not_saved_check(void) {

  /** This function is called by application exit to check is some files still unsaved and reference it at the same time. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  guint16 counter = 0 ;

  gint16 c=0 ;

  const gint page_number = gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) ;

  for ( ; c < page_number ; ++c) {
    GtkWidget *notebook_page        = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->editor_notebook), c) ;
    GtkWidget *current_textview     = gtk_bin_get_child(GTK_BIN(notebook_page)) ;
    GtkTextBuffer *current_buffer   = gtk_text_view_get_buffer(GTK_TEXT_VIEW(current_textview)) ;

    if (gtk_text_buffer_get_modified(current_buffer)) {

      /** The editor content has not being saved **/

      struct Unsaved_File *unsaved_file = g_malloc(sizeof(struct Unsaved_File)) ;

      unsaved_file->filepath = g_strdup((char *) g_object_get_data(G_OBJECT(current_buffer), "filepath")) ;
      unsaved_file->filepath = g_strdup((char *) g_object_get_data(G_OBJECT(current_buffer), "filepath")) ;
      unsaved_file->page_number = c ;

      unsaved_files = g_slist_insert(unsaved_files, (gpointer) unsaved_file, -1) ;

      counter++ ;
    }

  }


  return counter ;

}
