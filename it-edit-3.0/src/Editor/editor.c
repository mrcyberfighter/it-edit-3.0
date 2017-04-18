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



#include "./editor.h"

#ifdef DEBUG
static void file_loader_progress_callback(goffset current_num_bytes, goffset total_num_bytes, gpointer user_data) ;
#endif

static void file_loader_result_callback(GObject *source_object, GAsyncResult *res, gpointer loader) ;

static void destoy_notify_asio_load(gpointer data) ;

static gboolean source_view_loader_success = FALSE ;

static gboolean file_loading_completed = FALSE ;

static GError *end_loading_error = NULL ;

#ifdef DEBUG
static void file_loader_progress_callback(goffset current_num_bytes, goffset total_num_bytes, gpointer user_data) {


  fprintf(stdout,"N° of bytes offset: %li loaded from total: %li\n", current_num_bytes, total_num_bytes) ;


  return ;

}
#endif

static void file_loader_result_callback(GObject *source_object, GAsyncResult *res, gpointer loader) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  source_view_loader_success = gtk_source_file_loader_load_finish(loader, res, &end_loading_error) ;

  if (end_loading_error != NULL) {

    display_message_dialog( _("Error open file !!!"), end_loading_error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_error_free(end_loading_error) ;

    end_loading_error = NULL ;

  }

  g_object_unref(source_object) ;

  file_loading_completed = TRUE ;

  return ;

}


static void destoy_notify_asio_load(gpointer data) {

  free(data) ;

  return ;

}

gchar* get_mimetype_image_filepath(GtkSourceFile *source_file, const char *filepath_prefix) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  gchar *mimetype_image_filepath = NULL ;

  GFile *g_file_source_file = gtk_source_file_get_location(source_file) ;

  GFileInfo *g_file_info_source_file = g_file_query_info(g_file_source_file, G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE, G_FILE_QUERY_INFO_NONE, NULL, NULL) ;

  if (g_file_info_source_file == NULL) {

    goto set_default_mimetype_image ;

  }

  const char *content_type = g_file_info_get_attribute_string(g_file_info_source_file, G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE) ;

  if (content_type == NULL) {

    goto set_default_mimetype_image ;

  }

  gchar *mimetype = g_content_type_get_mime_type(content_type) ;

  if (mimetype == NULL) {

    goto set_default_mimetype_image ;

  }

  char *mimetype_ptr = strchr(mimetype, '/') ;


  /** Simple pointer arithmetic '/' replacement to '-'. So that the mime-type correspond to the filename of a mime-type image, without his extension. **/
  mimetype[mimetype_ptr - mimetype] = '-' ;

  mimetype_image_filepath = g_strdup_printf("%s%s.png", filepath_prefix, mimetype) ;

  g_free(mimetype) ;



  /** Creating GtkImage widget from computed file-path. **/
  if (! g_file_test(mimetype_image_filepath, G_FILE_TEST_EXISTS) ) {

    /** Mime-type image found **/

    set_default_mimetype_image :

    g_free(mimetype_image_filepath) ;

    mimetype_image_filepath = g_strdup_printf("%sunknown.png", filepath_prefix) ;

  }

  g_object_unref(g_file_info_source_file) ;

  return mimetype_image_filepath ;

}



void add_file_to_editor(GtkWidget *editor_notebook, File_Editor  *file_editor, GFile *g_file, bool start) {

  /** This function does all the file loading work into the editor and so construct the notebook window among aothers things like settings properties .
    *
    * We can divide this function into  parts:
    *
    * 1. File loading.
    * ----------------
    *   Running a @GtkSourceFileLoader is an undoable action for the @GtkSourceBuffer.
    *   That is, gtk_source_buffer_begin_not_undoable_action() and gtk_source_buffer_end_not_undoable_action() are called, which delete the undo/redo history.
    *   After a file loading, the buffer is reset to the contents provided by the GFile or GInputStream, so the buffer is set as “unmodified”,
    *   that is, gtk_text_buffer_set_modified() is called with FALSE.
    *
    * 2. Setting up the notebook page.
    * --------------------------------
    *
    * +) Test the file mime-type as for the syntax highlight as for displaying the corresponding icon in the notebook tab.
    * +) Store some informations used in the future like the file-path for saving per example.
    *
   **/

  /** This function does all the file loading into the editor work and some other like.
    * +) Test the file mime-type as for the syntax highlight as for displaying the corresponding icon in the notebook tab.
    * +) Store some informations used in the future like the file-path for saving per example.
    *
    * @NEWS using the @GtkSourceFileLoader for loading files.
    *
    */

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif



  file_loading_completed = FALSE ;


  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  gsize  file_size = 0 ;

  GtkSourceLanguage *source_language = NULL ;

  GtkSourceUndoManager *source_undo_manager = NULL ;

  gchar *file_content = NULL ;

  gchar *filepath = g_file_get_path(g_file) ;

  gchar *file_basename = g_path_get_basename(filepath) ;

  gchar *content_type = NULL ;

  gchar *lang_id      = NULL ;

  gboolean result_uncertain ;

  bool support_source_language = true ;

  bool can_load_content_easy = true ;


  file_loading_completed = FALSE ;



  GtkSourceFile *source_file = gtk_source_file_new() ;

  gtk_source_file_set_location(source_file, g_file) ;

  file_editor->source_file = source_file ;

  if (settings.warn_read_only) {

    if (gtk_source_file_is_local(file_editor->source_file)) {

      gtk_source_file_check_file_on_disk(file_editor->source_file) ;

      if (gtk_source_file_is_readonly(file_editor->source_file)) {

        display_message_dialog( _("Warning !"), _("This file is read-only !\nYou won't be able to save it at it's current location.\nAlternatively you can save it elsewhere."), GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE) ;

      }
    }

  }

  if ( ! start ) {
    /** Load file from argument vector or file selector. **/

    if (! g_file_get_contents(filepath, &file_content, &file_size, NULL) ) {
      /** Cannot get file content **/

      can_load_content_easy = false ;

    }

    gchar *filename_dup = g_strdup(filepath) ;

    while (filename_dup[(int) strlen(filename_dup)-1] == '~') {
      /** Remove backup suffix for file-type auto-detection. **/
      filename_dup[(int) strlen(filename_dup)-1] = '\0' ;
    }

  
    gchar *filename_dup_basename = g_path_get_basename(filename_dup) ;
  
    /** Try to guess the file content type. (Per filename) **/
    content_type = g_content_type_guess(filename_dup_basename, NULL, file_size, &result_uncertain);
    
    g_free(filename_dup_basename) ;
    
    if (result_uncertain && can_load_content_easy) {

      g_free(content_type);
      content_type = NULL;

      /** Try to guess the file content type. (Per content analyze) **/
      content_type = g_content_type_guess(NULL, (const guchar *) file_content, file_size, &result_uncertain);

      if (result_uncertain) {

        g_free(content_type);
    
        content_type = NULL;
    
        GFile *g_file_source_file = g_file_new_for_path(filepath) ;

        GFileInfo *g_file_info_source_file = g_file_query_info(g_file_source_file, G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE, G_FILE_QUERY_INFO_NONE, NULL, NULL) ;
    
        if (g_file_source_file != NULL) {
      
          g_object_unref(g_file_source_file) ;
      
        }
    
        if (g_file_info_source_file == NULL) {
      
          goto no_style ;
      
        }
        else {
      
          content_type = g_strdup(g_file_info_get_attribute_string(g_file_info_source_file, G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE)) ;
      
          if (g_file_info_source_file != NULL) {
      
            g_object_unref(g_file_info_source_file) ;
          }
      
        }

      }

    }


    if (source_language_manager != NULL) {
   
      /** Try to guess the loaded source file language: **/
  
      gchar *filename = g_path_get_basename(filename_dup) ;
  
      source_language = gtk_source_language_manager_guess_language(source_language_manager, filename, content_type);
   
      g_free(filename) ;
   
      if (source_language != NULL) {


        lang_id = (char *) gtk_source_language_get_id(source_language) ;

        if (lang_id == NULL) {

          goto no_style ;
        }

        /** Setting the menu item from the different supported language on the right value. **/
        set_syntax_highlight_radio(lang_id) ;

      }
      else if (source_language == NULL) {
        /** Language guessing failed. **/

        no_style :
    
        support_source_language=false ;

        /** Setting the menu item from the different supported language on the default value.  **/
        set_syntax_highlight_radio("text") ;
      }


      g_free(content_type) ;

      g_free(filename_dup) ;

      g_free(file_content) ;

    }

  }







  file_editor->scrolled_window=gtk_scrolled_window_new(NULL, NULL);

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(file_editor->scrolled_window),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC) ;



  if ( ! start && support_source_language ) {
    /** file open and source language detected. **/
    file_editor->buffer = gtk_source_buffer_new_with_language(gtk_source_language_manager_get_language(source_language_manager, lang_id )) ;
  }
  else {
    /** Default New named file or source language not detected. **/
    GtkTextTagTable *text_tag_table = gtk_text_tag_table_new() ;
    file_editor->buffer = gtk_source_buffer_new(text_tag_table) ;
  }



  if (! start) {

    /** We use the the source file loader. **/

    GtkSourceFileLoader *loader = gtk_source_file_loader_new(file_editor->buffer, file_editor->source_file) ;

    GSList *encoding_candidates = gtk_source_encoding_get_default_candidates() ;

    gtk_source_file_loader_set_candidate_encodings(loader, encoding_candidates) ;

    g_slist_free(encoding_candidates) ;

    #ifdef DEBUG
    gtk_source_file_loader_load_async(loader, G_PRIORITY_DEFAULT, NULL, &file_loader_progress_callback, NULL, &destoy_notify_asio_load, &file_loader_result_callback, loader) ;
    #else
    gtk_source_file_loader_load_async(loader, G_PRIORITY_DEFAULT, NULL, NULL, NULL, &destoy_notify_asio_load, &file_loader_result_callback, loader) ;
    #endif

    while (! file_loading_completed) {

      gtk_main_iteration_do(FALSE) ;

      g_main_context_iteration(NULL, FALSE) ;

    }

    if (! source_view_loader_success) {

        return ;
    }


  }


  file_editor->textview = gtk_source_view_new_with_buffer(file_editor->buffer) ;

  source_undo_manager = gtk_source_buffer_get_undo_manager(file_editor->buffer) ;

  /** This call is important so that by undoing the user does not return to an empty editor but at the loaded file begin state.  **/
  gtk_source_undo_manager_begin_not_undoable_action(source_undo_manager);


  if (file_size == 0) {

    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(file_editor->buffer),"",0) ;

  }


  /** Mark the buffer as not modified for asterisk indication displaying. **/

  if (start) {

    gtk_text_buffer_set_modified(GTK_TEXT_BUFFER(file_editor->buffer), FALSE) ;

  }

  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(file_editor->textview), TRUE);

  GtkTextIter start_iter ;
  gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(file_editor->buffer), &start_iter);
  gtk_text_buffer_place_cursor(GTK_TEXT_BUFFER(file_editor->buffer),   &start_iter);

  /** End of undo-able action. **/
  gtk_source_undo_manager_end_not_undoable_action(source_undo_manager);

  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(file_editor->textview), 10) ;


  gtk_container_add(GTK_CONTAINER(file_editor->scrolled_window),file_editor->textview) ;


  GtkWidget *tab_hbox   ;  /** Notebook tab container. **/

  GtkWidget *tab_icon   ;  /** Add an file-type icon.  **/
  GtkWidget *tab_label  ;  /** Filename label.         **/
  GtkWidget *tab_pad    ;  /** Padding label.          **/
  GtkWidget *tab_button ;  /** Close button.           **/
  GtkWidget *tab_image  ;  /** Close button image.     **/

  tab_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0) ;

  tab_pad = gtk_label_new("  ") ;

  tab_button = gtk_button_new() ;
  gtk_button_set_always_show_image(GTK_BUTTON(tab_button), TRUE);
  gtk_button_set_relief(GTK_BUTTON(tab_button),GTK_RELIEF_NONE) ;

  tab_image = gtk_image_new_from_file(PATH_TO_MENU_ICON "window-close.png") ;
  gtk_button_set_image(GTK_BUTTON(tab_button),tab_image) ;

  if ( ! start ) {
    /** Loaded file. **/

   /** Setting filename as notebook tab content and setting the language corresponding image on tab. **/

   if (support_source_language) {

      /** Store the source file language. **/
      g_object_set_data(G_OBJECT(file_editor->textview), "lang_id", (char *) gtk_source_language_get_id(source_language)) ;


    }
    else {

      /** Store the source file language. **/
      g_object_set_data(G_OBJECT(file_editor->textview), "lang_id", "text") ;

    }

    tab_label = gtk_label_new(file_basename) ;

  }
  else {
    /** Default New named file. **/
    tab_label = gtk_label_new("New") ;
    g_object_set_data(G_OBJECT(file_editor->textview), "lang_id", "text") ;
  }

  gchar *mimetype_image_filepath = get_mimetype_image_filepath(file_editor->source_file, PATH_TO_MIMETYPE_ICON) ;

  tab_icon = gtk_image_new_from_file(mimetype_image_filepath) ;

  g_free(mimetype_image_filepath) ;

  g_object_unref(g_file) ;

  gtk_widget_set_tooltip_text(tab_label, filepath) ;

  gtk_box_pack_start(GTK_BOX(tab_hbox), tab_icon,   TRUE,  TRUE,  4) ;
  gtk_box_pack_start(GTK_BOX(tab_hbox), tab_label,  TRUE,  TRUE,  0) ;
  gtk_box_pack_start(GTK_BOX(tab_hbox), tab_pad,    TRUE,  TRUE,  0) ;
  gtk_box_pack_start(GTK_BOX(tab_hbox), tab_button, TRUE,  TRUE,  0) ;

  gtk_widget_show_all(tab_hbox) ;


  gtk_widget_set_can_focus(file_editor->textview, TRUE);

  if (! start) {
    /** Loaded file. **/

    /** Storing file-path for saving it, will be free by closing file. **/
    g_object_set_data(G_OBJECT(file_editor->buffer), "filepath", filepath) ;
    /** Set the base filename in the bottom bar. **/
    gtk_label_set_text(GTK_LABEL(gui->bottom_bar->filename_label), file_basename) ;

  }
  else {
    /** The default New named file. **/

    /** Storing file-path for saving it, will be free by closing file. **/
    g_object_set_data(G_OBJECT(file_editor->buffer), "filepath", filepath ) ;
    /** Set the base filename in the bottom bar. **/
    gtk_label_set_text(GTK_LABEL(gui->bottom_bar->filename_label), "New") ;

    if (g_remove(filepath) < 0) {

      g_warning("Error removing temp file:\n%s\n", filepath) ;
    }

  }

  gtk_widget_set_tooltip_text(gui->bottom_bar->filename_label, filepath) ;

  gtk_source_buffer_set_max_undo_levels(file_editor->buffer, -1);


  gtk_source_buffer_set_style_scheme(file_editor->buffer, gtk_source_style_scheme_manager_get_scheme(source_style_language_manager, settings.scheme)) ;




  GtkSourceCompletion *source_completion = gtk_source_view_get_completion(GTK_SOURCE_VIEW(file_editor->textview)) ;

  GtkSourceCompletionWords *source_completion_words = gtk_source_completion_words_new("completion", NULL) ;

  gtk_source_completion_words_register(source_completion_words, GTK_TEXT_BUFFER(file_editor->buffer)) ;


  /** @NOTE: This could be user defined ! **/
  g_object_set(G_OBJECT(source_completion_words), "minimum-word-size", 3, NULL) ;
  g_object_set(G_OBJECT(source_completion_words), "interactive-delay", 25, NULL) ;


  g_object_set(G_OBJECT(source_completion), "show-headers", FALSE, NULL ) ;


  gtk_source_completion_add_provider(source_completion,GTK_SOURCE_COMPLETION_PROVIDER(source_completion_words), NULL) ;

  setting_sourceview_settings(GTK_SOURCE_VIEW(file_editor->textview)) ;

  Highlight_Selection *highlight_textview_selection = g_malloc(sizeof(Highlight_Selection)) ;

  if (highlight_textview_selection != NULL) {

    /** Setting up settings highlight for selection matches. **/
    highlight_textview_selection->source_search_settings = gtk_source_search_settings_new() ;

    gtk_source_search_settings_set_at_word_boundaries(highlight_textview_selection->source_search_settings, TRUE) ;
    gtk_source_search_settings_set_case_sensitive(highlight_textview_selection->source_search_settings,     TRUE) ;

    /** Setting up source context. **/
    highlight_textview_selection->source_search_highlight_context = gtk_source_search_context_new(GTK_SOURCE_BUFFER(file_editor->buffer), highlight_textview_selection->source_search_settings);

    g_object_set_data(G_OBJECT(file_editor->textview), "highlight_textview_selection", highlight_textview_selection) ;

  }

  GtkSourceSearchContext  *source_search_steps_context      = gtk_source_search_context_new(GTK_SOURCE_BUFFER(file_editor->buffer), searching.source_search_settings );
  GtkSourceSearchContext  *source_search_highlight_context  = gtk_source_search_context_new(GTK_SOURCE_BUFFER(file_editor->buffer), searching.source_search_settings );

  #ifdef GSPELL_SUPPORT
  file_editor->gspell_view = gspell_text_view_get_from_gtk_text_view(GTK_TEXT_VIEW(file_editor->textview)) ;
  #endif

  g_object_set_data(G_OBJECT(file_editor->textview), "source_search_steps_context",     source_search_steps_context) ;
  g_object_set_data(G_OBJECT(file_editor->textview), "source_search_highlight_context", source_search_highlight_context) ;


  gint *ret=malloc(sizeof(gint)) ; /** Will be free wenn closing file **/

  gint tmp_ret = gtk_notebook_append_page(GTK_NOTEBOOK(editor_notebook), file_editor->scrolled_window, tab_hbox) ;

  memcpy(ret, &tmp_ret, sizeof(gint));

  /** Update current notebook page editor. **/
  current_editor.current_notebook_page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(editor_notebook),  *ret) ;
  current_editor.current_textview      = gtk_bin_get_child(GTK_BIN(current_editor.current_notebook_page)) ;
  current_editor.current_buffer        = gtk_text_view_get_buffer(GTK_TEXT_VIEW(current_editor.current_textview)) ;

  g_object_set_data(G_OBJECT(tab_button),            "page_number", ret) ;
  g_object_set_data(G_OBJECT(file_editor->textview), "file_editor", file_editor) ;


  /** Connecting signals: **/

  #if 0
  /** Auto-completion updating every buffer content change.
    * It seems not needed !
    * g_signal_connect(G_OBJECT(file_editor->buffer),   "changed", G_CALLBACK(update_completion), file_editor->textview) ;
  **/
  #endif

  /** Update the bottom bar displayed informations every-time an event occurs: very often. **/
  g_signal_connect(G_OBJECT(file_editor->textview), "event-after", G_CALLBACK(update_rows_cols_info), NULL) ;

  /** Update the asterisk indication in the filename tab component. **/
  g_signal_connect(G_OBJECT(file_editor->buffer),   "modified-changed", G_CALLBACK(textbuffer_changed_modified), tab_label)  ;

  /** Buttons event. **/
  g_signal_connect(G_OBJECT(file_editor->textview), "button-press-event",   G_CALLBACK(textview_button_press_event),   NULL)  ;
  g_signal_connect(G_OBJECT(file_editor->textview), "button-release-event", G_CALLBACK(textview_button_release_event), NULL)  ;

  /** Close page by click on the button included in the tab. **/
  g_signal_connect(G_OBJECT(tab_button), "clicked", G_CALLBACK(close_file), NULL) ;

  /** Page reorder callback: we update the "page_number" data from tab_button. **/
  g_signal_connect(G_OBJECT(editor_notebook), "page-reordered", G_CALLBACK(page_reorder), NULL) ;


  gtk_notebook_set_menu_label_text(GTK_NOTEBOOK(editor_notebook), file_editor->scrolled_window, file_basename) ;

  gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK(editor_notebook), file_editor->scrolled_window, TRUE) ;

  gtk_widget_show_all(file_editor->scrolled_window);

  gtk_notebook_set_current_page(GTK_NOTEBOOK(editor_notebook), *ret) ;


  g_free(file_selector_curdirpath) ;

  file_selector_curdirpath = g_path_get_dirname(filepath) ;


  if (gtk_notebook_get_current_page(GTK_NOTEBOOK(gui->main_notebook)) != 0) {

    gtk_notebook_set_current_page(GTK_NOTEBOOK(gui->main_notebook), 0) ;

  }

  gtk_widget_grab_focus(file_editor->textview) ;

  g_free(file_basename) ;

  return ;

}


void reload_file(GtkSourceBuffer *buffer, const char *filepath, File_Editor *file_editor) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  g_stat(filepath, &file_editor->file_info) ;

  gchar *file_basename = g_path_get_basename(filepath) ;

  if (file_editor->file_info.st_size == 0) {

    GtkTextIter start_iter ;
    GtkTextIter end_iter   ;

    gtk_source_buffer_begin_not_undoable_action(buffer) ;

    gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(buffer), &start_iter) ;

    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(buffer), &end_iter) ;

    gtk_text_buffer_delete(GTK_TEXT_BUFFER(buffer), &start_iter, &end_iter) ;

    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buffer), "", 0) ;

    gtk_source_buffer_end_not_undoable_action(buffer) ;

    GtkWidget *tab_label = g_object_get_data(G_OBJECT(buffer), "tab_label") ;

    gtk_label_set_text(GTK_LABEL(tab_label), file_basename) ;

  }
  else {

    GtkSourceFileLoader *loader = gtk_source_file_loader_new(buffer, file_editor->source_file) ;

    #ifdef DEBUG
    gtk_source_file_loader_load_async(loader, G_PRIORITY_DEFAULT, NULL, &file_loader_progress_callback, NULL, &destoy_notify_asio_load, &file_loader_result_callback, loader) ;
    #else
    gtk_source_file_loader_load_async(loader, G_PRIORITY_DEFAULT, NULL, NULL, NULL, &destoy_notify_asio_load, &file_loader_result_callback, loader) ;
    #endif

    while (! file_loading_completed) {

      gtk_main_iteration_do(FALSE) ;

      g_main_context_iteration(NULL, FALSE) ;

    }

    if (! source_view_loader_success) {

        return ;
    }

    GtkWidget *tab_label = g_object_get_data(G_OBJECT(buffer), "tab_label") ;

    gtk_label_set_text(GTK_LABEL(tab_label), file_basename) ;

  }

  g_free(file_basename) ;

  gtk_widget_grab_focus(file_editor->textview) ;

  return ;

}
