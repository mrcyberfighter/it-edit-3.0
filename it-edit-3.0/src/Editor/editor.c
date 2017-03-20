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


    /** Try to guess the file content type. (Per filename) **/
    content_type = g_content_type_guess(filename_dup, NULL, file_size, &result_uncertain);

    if (result_uncertain && can_load_content_easy) {

      g_free(content_type);
      content_type = NULL;

      /** Try to guess the file content type. (Per content analyze) **/
      content_type = g_content_type_guess(NULL, (const guchar *) file_content, file_size, &result_uncertain);

      if (result_uncertain) {

        g_free(content_type);
        content_type = NULL;

        goto no_style ;

      }

    }


    if (source_language_manager != NULL) {

      /** Try to guess the loaded source file language: **/
      source_language = gtk_source_language_manager_guess_language(source_language_manager, filename_dup, content_type);

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

/** *********************************************************************** **/

#ifdef MINI_MULTIPURPOSE_EDITOR_SUPPORT /** This feature is too much unstable. **/

/** The "Mini-multi-purpose" editor is unstable and all works you can do with it
  * can be done with a simple editor notebook page.
  * So we exclude it with the undefined definition above, everywhere into the source code.
  ****************************************************************************************/

static void register_todo_settings_str(const gchar *section, const gchar *settings_string) ;

static void todo_set_scheme(GtkWidget *widget, GtkSourceBuffer *buffer) ;

static void todo_set_highlight_language(GtkWidget *widget, GtkSourceBuffer *buffer) ;

static void export_todo_content(GtkWidget *widget, Todo_Content *todo_content) ;

static gboolean todo_set_syntax_highlight(GtkWidget *widget, GdkEvent  *event, const gchar *lang_id) ;

static gboolean is_lang_id_valid(const gchar *lang_id) ;

static GtkWidget* construct_language_menu_items(void) ;







static void todo_set_scheme(GtkWidget *widget, GtkSourceBuffer *buffer) {

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

    const gchar *scheme_id = gtk_source_style_scheme_get_id(scheme_style) ;

    if (g_strcmp0(todo_settings->scheme, scheme_id) != 0) {

      g_free(todo_settings->scheme) ;

      todo_settings->scheme = g_strdup(scheme_id) ;

      gtk_source_buffer_set_style_scheme(buffer, scheme_style);

      register_todo_settings_str("scheme", scheme_id) ;

      if (! gtk_source_buffer_get_highlight_syntax(buffer)) {

        gtk_source_buffer_set_highlight_syntax(buffer,    TRUE) ;
      }

    }

  }

  gtk_widget_destroy(dialog) ;

  return ;

}


static gboolean todo_set_syntax_highlight(GtkWidget *widget, GdkEvent  *event, const gchar *lang_id) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  g_free(todo_settings->lang_id) ;

  todo_settings->lang_id = g_strdup(lang_id) ;

  return FALSE ;

}

static GtkWidget* construct_language_menu_items(void) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *menu_syntax = gtk_grid_new() ;

  g_object_set(menu_syntax, "column-homogeneous", TRUE, NULL) ;
  g_object_set(menu_syntax, "row-homogeneous",    TRUE, NULL) ;
  g_object_set(menu_syntax, "column-spacing",        1, NULL) ;
  g_object_set(menu_syntax, "row-spacing",           1, NULL) ;

  gint c=0 ;

  gchar *lang_id = (gchar *) gtk_source_language_manager_get_language_ids(source_language_manager)[c] ;

  GSList *syntax_radiogroup = NULL ;

  GtkWidget *lang ;

  gint col = 0 ;
  gint row = 0 ;

  if (lang_id) {

    lang = gtk_radio_button_new_with_label(syntax_radiogroup, lang_id) ;

    syntax_radiogroup = gtk_radio_button_get_group(GTK_RADIO_BUTTON(lang));

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lang),  (g_strcmp0(todo_settings->lang_id, lang_id) == 0) ) ;

    g_signal_connect(G_OBJECT(lang), "button-press-event", G_CALLBACK(todo_set_syntax_highlight), lang_id) ;

    gtk_grid_attach(GTK_GRID(menu_syntax),lang, row, col, 1, 1);


    while (1) {
      c++ ;
      col++ ;

      if (col == 18) {
        col=0 ;
        row++ ;
      }

      lang_id = (gchar *) gtk_source_language_manager_get_language_ids(source_language_manager)[c] ;

      if (lang_id == NULL) {
        break ;
      }

      lang = gtk_radio_button_new_with_label(syntax_radiogroup, lang_id) ;
      syntax_radiogroup = gtk_radio_button_get_group(GTK_RADIO_BUTTON(lang));

      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lang),  (g_strcmp0(todo_settings->lang_id, lang_id) == 0) ) ;

      g_signal_connect(G_OBJECT(lang), "button-press-event", G_CALLBACK(todo_set_syntax_highlight), lang_id) ;

      gtk_grid_attach(GTK_GRID(menu_syntax),lang, row, col, 1, 1);

    }

  }

  lang = gtk_radio_button_new_with_label(syntax_radiogroup, "text") ;

  syntax_radiogroup = gtk_radio_button_get_group(GTK_RADIO_BUTTON(lang));

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lang),  (g_strcmp0(todo_settings->lang_id, "text") == 0) ) ;

  g_signal_connect(G_OBJECT(lang), "button-press-event", G_CALLBACK(todo_set_syntax_highlight), "text") ;

  gtk_grid_attach(GTK_GRID(menu_syntax),lang, row, col, 1, 1);

  return menu_syntax ;

}

static void set_buffer_syntax_highlight(GtkSourceBuffer *buffer, const gchar *lang_id) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (is_lang_id_valid(todo_settings->lang_id)) {

    errno = 0 ;

    GtkSourceLanguage *source_language = gtk_source_language_manager_get_language(source_language_manager, lang_id);

    int errno_bkup = errno ;

    if (source_language != NULL) {





        if (! gtk_source_buffer_get_highlight_syntax(buffer)) {

          gtk_source_buffer_set_highlight_syntax(buffer,    TRUE) ;
        }

        gtk_source_buffer_set_language(buffer, source_language) ;

        GtkTextIter start, end ;

        gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(buffer), &start) ;

        gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(buffer), &end) ;

        gtk_source_buffer_ensure_highlight(buffer, &start, &end);


        register_todo_settings_str("lang_id", todo_settings->lang_id) ;

    }
    else {

      display_message_dialog( _("Error settings !"), g_strerror(errno_bkup), GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    }

  }
  else {

    gtk_source_buffer_set_language(buffer, NULL) ;

    GtkTextIter start, end ;

    gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(buffer), &start) ;

    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(buffer), &end) ;

    gtk_source_buffer_ensure_highlight(buffer, &start, &end);

    register_todo_settings_str("lang_id", "text") ;

  }



  return ;

}

static void todo_set_highlight_language(GtkWidget *widget, GtkSourceBuffer *buffer) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *dialog = gtk_dialog_new_with_buttons( _("Select a programming language"),
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



  GtkWidget *menu_syntax = construct_language_menu_items() ;



  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL) ;

  gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled_window),  24 * 16 * 1.25)    ;
  gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrolled_window),   24 * 16 * 2)   ;

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_scrolled_window_set_overlay_scrolling(GTK_SCROLLED_WINDOW(scrolled_window), FALSE) ;

  gtk_container_add(GTK_CONTAINER(scrolled_window), menu_syntax) ;




  gtk_widget_show_all(scrolled_window) ;

  GtkWidget *dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

  gtk_container_add(GTK_CONTAINER(dialog_content), scrolled_window) ;

  gint res = gtk_dialog_run(GTK_DIALOG(dialog)) ;

  if (res == GTK_RESPONSE_APPLY) {

    if (source_language_manager != NULL) {

      set_buffer_syntax_highlight(buffer, todo_settings->lang_id) ;

    }

  }

  gtk_source_style_scheme_manager_force_rescan(source_style_language_manager) ;

  gtk_widget_destroy(dialog) ;

  return ;

}

#ifdef GSPELL_SUPPORT

static gboolean inline_spellcheck_on = FALSE ;

static void set_inline_spellcheck(Todo_Content *todo_content) ;

static void unset_inline_spellcheck(GspellTextView *gspell_view) ;

static void todo_inline_spellcheck(GtkWidget *widget, Todo_Content *todo_content) ;


static void todo_select_spellcheck_language(GtkWidget *widget, Todo_Content *todo_content) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *spellcheck_language_dialog = gspell_language_chooser_dialog_new(GTK_WINDOW(gui->main_window), gspell_language_lookup(todo_settings->language_code), GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT) ;

  gtk_dialog_run(GTK_DIALOG(spellcheck_language_dialog)) ;

  if (gspell_language_chooser_get_language_code(GSPELL_LANGUAGE_CHOOSER(spellcheck_language_dialog)) != NULL) {

      g_free(todo_settings->language_code) ;

      /** It cannot be NULL as say in the documentation ! **/
      todo_settings->language_code = g_strdup(gspell_language_chooser_get_language_code(GSPELL_LANGUAGE_CHOOSER(spellcheck_language_dialog))) ;

  }

  gboolean is_inline = gspell_text_view_get_inline_spell_checking(gspell_text_view_get_from_gtk_text_view(GTK_TEXT_VIEW(todo_content->textview))) ;

  if (is_inline) {

    set_inline_spellcheck(todo_content) ;

  }


  gtk_widget_destroy(spellcheck_language_dialog) ;

  return ;

}

static void set_inline_spellcheck(Todo_Content *todo_content) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GspellChecker *checker = gspell_checker_new(gspell_language_lookup(todo_settings->language_code))  ;

  GspellTextBuffer *gspell_buffer = gspell_text_buffer_get_from_gtk_text_buffer(GTK_TEXT_BUFFER(todo_content->buffer)) ;
  gspell_text_buffer_set_spell_checker(gspell_buffer, checker);
  g_object_unref(checker);

  gspell_text_view_set_inline_spell_checking(todo_content->gspell_view, TRUE);
  #ifdef GSPELL_SUPPORT_1_2_1
  gspell_text_view_set_enable_language_menu(todo_content->gspell_view, TRUE);
  #endif

  inline_spellcheck_on = TRUE ;

  return ;

}

static void unset_inline_spellcheck(GspellTextView *gspell_view) {

  /** Switch in-line-spell-check @OFF. **/

  gspell_text_view_set_inline_spell_checking(gspell_view, FALSE) ;

  inline_spellcheck_on = FALSE ;

  return ;
}

static void todo_inline_spellcheck(GtkWidget *widget, Todo_Content *todo_content) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {

    set_inline_spellcheck(todo_content) ;

  }
  else {

    unset_inline_spellcheck(todo_content->gspell_view) ;

  }

  return ;

}

static void todo_spellcheck_dialog(GtkWidget *widget, Todo_Content *todo_content) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  gboolean is_inline = gspell_text_view_get_inline_spell_checking(gspell_text_view_get_from_gtk_text_view(GTK_TEXT_VIEW(todo_content->textview))) ;

  GspellChecker *checker = gspell_checker_new(gspell_language_lookup(todo_settings->language_code))   ;

  GspellTextBuffer *gspell_buffer = gspell_text_buffer_get_from_gtk_text_buffer(GTK_TEXT_BUFFER(todo_content->buffer)) ;
  gspell_text_buffer_set_spell_checker(gspell_buffer, checker);
  g_object_unref(checker);

  GspellNavigator *spell_navigator  = gspell_navigator_text_view_new(GTK_TEXT_VIEW(todo_content->textview));
  GtkWidget *spell_check_dialog = gspell_checker_dialog_new(GTK_WINDOW(gui->main_window), spell_navigator);

  gtk_dialog_run(GTK_DIALOG(spell_check_dialog));

  gspell_text_buffer_set_spell_checker(gspell_buffer, NULL);

  gtk_widget_destroy(spell_check_dialog) ;

  if (is_inline) {

    set_inline_spellcheck(todo_content) ;

  }


  return ;

}

#endif

static void export_todo_content(GtkWidget *widget, Todo_Content *todo_content) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *file_chooser = gtk_file_chooser_dialog_new( _("Choose the file as location to export the content"),
                                                          GTK_WINDOW(gui->main_window),
                                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                                          _("Export as"),
                                                          GTK_RESPONSE_ACCEPT,
                                                          _("Cancel"),
                                                          GTK_RESPONSE_CLOSE,
                                                          NULL) ;
                   

  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(file_chooser), TRUE) ;

  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser), g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP)) ;



  gint res = gtk_dialog_run(GTK_DIALOG(file_chooser));

  if (res == GTK_RESPONSE_ACCEPT) {

    GFile *g_file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(file_chooser)) ;

    GtkSourceFile *source_file = gtk_source_file_new() ;

    source_file_saver_with_target(todo_content->buffer, source_file, g_file, FALSE) ;

    g_object_unref(source_file) ;

  }

  gtk_widget_destroy(file_chooser) ;

  gtk_window_present(GTK_WINDOW(todo_content->todo_window)) ;

  gtk_window_set_focus(GTK_WINDOW(todo_content->todo_window), todo_content->textview);

  return ;

}


static gboolean is_lang_id_valid(const gchar *lang_id) {

  /** Silly check if the language is valid @BUT I had some dysfunctions as the language is equal to "it-edit" ???
    *
    * So we get sure and implement this silly function.
  **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (source_language_manager == NULL) {

    return FALSE ;
  }

  guint c=0 ;

  gchar *current_lang_id = (gchar *) gtk_source_language_manager_get_language_ids(source_language_manager)[c] ;

  while (current_lang_id) {

    current_lang_id = (gchar *) gtk_source_language_manager_get_language_ids(source_language_manager)[c] ;

    if (g_strcmp0(lang_id, current_lang_id) == 0) {

      return TRUE ;
    }

    ++c ;

  }

  return FALSE ;

}

static gboolean is_scheme_id_valid(const gchar *scheme_id) {

  /** The same problem as for the language...??? **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (source_style_language_manager == NULL) {

    return FALSE ;
  }

  guint c=0 ;

  gchar *current_scheme_id = (gchar *) gtk_source_style_scheme_manager_get_scheme_ids(source_style_language_manager)[c] ;

  while (current_scheme_id) {

    current_scheme_id = (gchar *) gtk_source_style_scheme_manager_get_scheme_ids(source_style_language_manager)[c] ;

    if (g_strcmp0(scheme_id, current_scheme_id) == 0) {

      return TRUE ;
    }

    ++c ;

  }

  return FALSE ;

}

static void import_todo_content(GtkWidget *widget, Todo_Content *todo_content) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *file_chooser = gtk_file_chooser_dialog_new( _("Choose the file to import as content"),
                                                          GTK_WINDOW(gui->main_window),
                                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                                          _("Import file"),
                                                          GTK_RESPONSE_ACCEPT,
                                                          _("Cancel"),
                                                          GTK_RESPONSE_CLOSE,
                                                          NULL) ;
                   

  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_chooser), g_get_user_special_dir(G_USER_DIRECTORY_DESKTOP)) ;

  gint res = gtk_dialog_run(GTK_DIALOG(file_chooser));

  if (res == GTK_RESPONSE_ACCEPT) {

    file_loading_completed = FALSE ;

    source_view_loader_success = FALSE ;

    GFile *g_file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(file_chooser)) ;

    GtkSourceFile *source_file = gtk_source_file_new() ;

    gtk_source_file_set_location(source_file, g_file) ;

    GtkSourceFileLoader *loader = gtk_source_file_loader_new(todo_content->buffer, source_file) ;

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

      gtk_widget_destroy(file_chooser) ;

      return ;

    }


    gchar *filepath = g_file_get_path(g_file) ;

    g_object_unref(g_file) ;

    gsize file_size = 0 ;

    gchar *file_content = NULL ;

    g_file_get_contents(filepath, &file_content, &file_size, NULL) ;

    gboolean result_uncertain = TRUE ;

    /** Try to guess the file content type. (Per content analyze) **/
    gchar *content_type = g_content_type_guess(NULL, (const guchar *) file_content, file_size, &result_uncertain);

    g_free(file_content) ;

    gchar *lang_id = NULL ;

    GtkSourceLanguage *source_language = NULL ;

    if (source_language_manager != NULL) {

      /** Try to guess the loaded source file language: **/
      source_language = gtk_source_language_manager_guess_language(source_language_manager, filepath, content_type);

      if (source_language != NULL) {

        lang_id = (gchar *) gtk_source_language_get_id(source_language) ;

        if (lang_id == NULL) {

          lang_id = g_strdup("text") ;

        }

      }
      else if (source_language == NULL) {

        /** Language guessing failed. **/

        lang_id = g_strdup("text") ;

      }

    }

    g_free(content_type) ;

    if (g_strcmp0(lang_id, "text") != 0) {

      set_buffer_syntax_highlight(todo_content->buffer, lang_id) ;

    }

    #ifdef GSPELL_SUPPORT
    if (inline_spellcheck_on) {

      set_inline_spellcheck(todo_content) ;

    }
    #endif


  }

  gtk_widget_destroy(file_chooser) ;

  return ;

}


static void todo_save_content(GtkWidget *widget, GtkSourceBuffer *buffer) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GFile *g_file = g_file_new_for_path(PATH_TO_TODO_FILE) ;

  GtkSourceFile *source_file = gtk_source_file_new() ;

  gtk_source_file_set_location(source_file, g_file) ;

  source_file_saver_with_target(buffer, source_file, g_file, FALSE) ;

  g_object_unref(source_file) ;

  return ;

}



static void register_todo_settings_str(const gchar *section, const gchar *settings_string) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GKeyFile *conf_file = g_key_file_new() ;

  GError *error = NULL ;

  g_key_file_load_from_file(conf_file, PATH_TO_CONF_FILE, G_KEY_FILE_NONE, &error) ;

  if (error != NULL) {

    display_message_dialog( _("Error saving setting"), error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_error_free(error) ;

    return ;

  }

  g_key_file_set_string(conf_file, "Todo", section, settings_string) ;

  g_key_file_save_to_file(conf_file, PATH_TO_CONF_FILE,  &error) ;

  if (error != NULL) {

    display_message_dialog( _("Error saving setting"), error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_error_free(error) ;

    return ;

  }

  g_key_file_unref(conf_file) ;

  return ;

}

void display_todo_window(GtkWidget *widget, gpointer user_data) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  static gint calls = -1 ;

  ++calls ;

  Todo_Content *todo_content = g_malloc(sizeof(Todo_Content)) ;

  todo_content->todo_window = gtk_window_new(GTK_WINDOW_TOPLEVEL) ;

  gtk_window_set_decorated(GTK_WINDOW(todo_content->todo_window), TRUE) ;
  gtk_window_set_resizable(GTK_WINDOW(todo_content->todo_window), TRUE) ;
  gtk_window_set_position(GTK_WINDOW(todo_content->todo_window), GTK_WIN_POS_CENTER);
  gtk_window_set_destroy_with_parent(GTK_WINDOW(todo_content->todo_window), TRUE);
  gtk_window_set_title(GTK_WINDOW(todo_content->todo_window), _("mini multipurpose editor"));

  gtk_window_set_application(GTK_WINDOW(todo_content->todo_window), app);

  GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  /** Main button bar: **/
  GtkWidget *main_buttonbox_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  g_object_set(G_OBJECT(main_buttonbox_hbox), "margin-start",   8,    NULL) ;
  g_object_set(G_OBJECT(main_buttonbox_hbox), "margin-end",     8,    NULL) ;
  g_object_set(G_OBJECT(main_buttonbox_hbox), "margin-top",     8/2,  NULL) ;
  g_object_set(G_OBJECT(main_buttonbox_hbox), "margin-bottom",  8/2,  NULL) ;


  /** Different button-bars to contains all the buttons: **/
  GtkWidget *sourceview_buttonbox   = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;

  gtk_box_set_spacing(GTK_BOX(sourceview_buttonbox), 8) ;
  gtk_button_box_set_layout(GTK_BUTTON_BOX(sourceview_buttonbox), GTK_BUTTONBOX_START) ;


  GtkWidget *spellcheck_buttonbox  = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;

  gtk_box_set_spacing(GTK_BOX(spellcheck_buttonbox), 8) ;
  gtk_button_box_set_layout(GTK_BUTTON_BOX(spellcheck_buttonbox), GTK_BUTTONBOX_START) ;

  GtkWidget *import_export_buttonbox  = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;

  gtk_box_set_spacing(GTK_BOX(import_export_buttonbox), 8) ;
  gtk_button_box_set_layout(GTK_BUTTON_BOX(import_export_buttonbox), GTK_BUTTONBOX_START) ;

  GtkWidget *saving_buttonbox  = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;

  gtk_box_set_spacing(GTK_BOX(saving_buttonbox), 8) ;
  gtk_button_box_set_layout(GTK_BUTTON_BOX(saving_buttonbox), GTK_BUTTONBOX_START) ;


  /** To-do button-bar buttons: **/
  GtkWidget *scheme_chooser_button  = gtk_button_new() ;
  GtkWidget *scheme_chooser_image   = gtk_image_new_from_file( PATH_TO_BUTTON_ICON "select-scheme.png" ) ;

  g_object_set(G_OBJECT(scheme_chooser_button), "always-show-image", TRUE, NULL) ;
  g_object_set(G_OBJECT(scheme_chooser_button), "image", scheme_chooser_image, NULL) ;

  gtk_widget_set_tooltip_text(scheme_chooser_button, _("Select the scheme to use into the \"mini multipurpose editor\"") ) ;

  GtkWidget *lang_button = gtk_button_new() ;
  GtkWidget *lang_button_image  = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "input-keyboard.png") ;

  gtk_widget_set_tooltip_text(lang_button, _("Select a programming language if needed as per example ReST ot Mardown.") ) ;

  g_object_set(G_OBJECT(lang_button),"always-show-image",TRUE,NULL) ;
  g_object_set(G_OBJECT(lang_button), "image", lang_button_image, NULL) ;


  #ifdef GSPELL_SUPPORT
  GtkWidget *inline_spellcheck_button   = gtk_toggle_button_new() ;
  GtkWidget *inline_spellcheck_image    = gtk_image_new_from_file( PATH_TO_BUTTON_ICON "tools-check-spelling.png" ) ;

  g_object_set(G_OBJECT(inline_spellcheck_button), "always-show-image", TRUE, NULL) ;
  g_object_set(G_OBJECT(inline_spellcheck_button), "image", inline_spellcheck_image, NULL) ;

  gtk_widget_set_tooltip_text(inline_spellcheck_button, _("Enable/Disable in-line misspelled words underlining: in-line spell-check.") ) ;


  GtkWidget *dialog_spellcheck_button = gtk_button_new() ;
  GtkWidget *dialog_spellcheck_image  = gtk_image_new_from_file( PATH_TO_BUTTON_ICON "tools-check-spellcheck-dialog.png" ) ;

  g_object_set(G_OBJECT(dialog_spellcheck_button), "always-show-image", TRUE, NULL) ;
  g_object_set(G_OBJECT(dialog_spellcheck_button), "image", dialog_spellcheck_image, NULL) ;

  gtk_widget_set_tooltip_text(dialog_spellcheck_button, _("Spell-check the entire content of the \"mini multipurpose editor\" using a spell-check dialog window.") ) ;


  GtkWidget *language_spellcheck_button = gtk_button_new() ;
  GtkWidget *language_spellcheck_image  = gtk_image_new_from_file( PATH_TO_BUTTON_ICON "preferences-desktop-locale.png" ) ;

  g_object_set(G_OBJECT(language_spellcheck_button), "always-show-image", TRUE, NULL) ;
  g_object_set(G_OBJECT(language_spellcheck_button), "image", language_spellcheck_image, NULL) ;

  gtk_widget_set_tooltip_text(language_spellcheck_button, _("Choose the language used for spell-check operations.") ) ;
  #endif


  GtkWidget *import_button = gtk_button_new() ;
  GtkWidget *import_image  = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "todo-import.png") ;

  g_object_set(G_OBJECT(import_button),"always-show-image",TRUE,NULL) ;
  g_object_set(G_OBJECT(import_button), "image", import_image, NULL) ;

  gtk_widget_set_tooltip_text(import_button, _("Import and so overwrite the content of the \"mini multipurpose editor\".") ) ;


  GtkWidget *export_button = gtk_button_new() ;
  GtkWidget *export_image  = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "todo-export.png") ;

  g_object_set(G_OBJECT(export_button),"always-show-image",TRUE,NULL) ;
  g_object_set(G_OBJECT(export_button), "image", export_image, NULL) ;

  gtk_widget_set_tooltip_text(export_button, _("Export the content of the \"mini multipurpose editor\" to a file.") ) ;


  GtkWidget *saving_button = gtk_button_new_with_label( _("  Save content")) ;
  GtkWidget *saving_image  = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "document-save.png") ;

  g_object_set(G_OBJECT(saving_button),"always-show-image",TRUE,NULL) ;
  g_object_set(G_OBJECT(saving_button), "image", saving_image, NULL) ;

  gtk_widget_set_tooltip_text(saving_button, _("Save the content of the \"mini multipurpose editor\".\nNot to a file !") ) ;








  /** Pack buttons into button-bar: **/
  gtk_box_pack_start(GTK_BOX(sourceview_buttonbox), scheme_chooser_button,  FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(sourceview_buttonbox), lang_button,            FALSE, FALSE, 0) ;

  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(sourceview_buttonbox), scheme_chooser_button,  TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(sourceview_buttonbox), lang_button,       TRUE) ;

  #ifdef GSPELL_SUPPORT
  gtk_box_pack_start(GTK_BOX(spellcheck_buttonbox), inline_spellcheck_button,   FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(spellcheck_buttonbox), dialog_spellcheck_button,   FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(spellcheck_buttonbox), language_spellcheck_button, FALSE, FALSE, 0) ;


  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(spellcheck_buttonbox), inline_spellcheck_button,      TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(spellcheck_buttonbox), dialog_spellcheck_button,      TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(spellcheck_buttonbox), language_spellcheck_button,    TRUE) ;
  #endif

  gtk_box_pack_start(GTK_BOX(import_export_buttonbox), import_button,   FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(import_export_buttonbox), export_button,   FALSE, FALSE, 0) ;

  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(import_export_buttonbox), import_button,      TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(import_export_buttonbox), export_button,      TRUE) ;


  gtk_box_pack_start(GTK_BOX(saving_buttonbox), saving_button,   FALSE, FALSE, 0) ;



  gtk_box_pack_start(GTK_BOX(main_buttonbox_hbox),      sourceview_buttonbox,        FALSE, FALSE,  0)  ;
  #ifdef GSPELL_SUPPORT
  gtk_box_pack_start(GTK_BOX(main_buttonbox_hbox),      gtk_label_new(""),           TRUE,  TRUE,   0)  ;
  gtk_box_pack_start(GTK_BOX(main_buttonbox_hbox),      spellcheck_buttonbox,        FALSE, FALSE,  0)  ;
  #endif
  gtk_box_pack_start(GTK_BOX(main_buttonbox_hbox),      gtk_label_new(""),           TRUE,  TRUE,   0)  ;

  gtk_box_pack_start(GTK_BOX(main_buttonbox_hbox),      import_export_buttonbox,     FALSE, FALSE,  0)  ;

  gtk_box_pack_start(GTK_BOX(main_buttonbox_hbox),      gtk_label_new(""),           TRUE,  TRUE,   0)  ;


  gtk_box_pack_start(GTK_BOX(main_buttonbox_hbox),      saving_buttonbox,            FALSE, FALSE,  0)  ;




  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);

  gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled_window),  24 *  16 * 1.25)   ;
  gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrolled_window),   80 *  16 * 0.75)    ;

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);


  /** Get the "mini-multipurpose-editor" configuration. **/

  get_todo_configuration(calls) ;



  /** Initialize the @Todo_Content structure members. **/

  GtkTextTagTable *text_tag_table = gtk_text_tag_table_new() ;

  todo_content->buffer = gtk_source_buffer_new(text_tag_table) ;



  file_loading_completed = FALSE ;

  source_view_loader_success = FALSE ;

  GFile *g_file = g_file_new_for_path(PATH_TO_TODO_FILE) ;

  GtkSourceFile *source_file = gtk_source_file_new() ;

  gtk_source_file_set_location(source_file, g_file) ;

  GtkSourceFileLoader *loader = gtk_source_file_loader_new(todo_content->buffer, source_file) ;

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

    gtk_widget_destroy(todo_content->todo_window) ;

    return ;

  }

  g_object_unref(g_file) ;


  if (g_strcmp0(todo_settings->lang_id, "text") != 0) {

    set_buffer_syntax_highlight(todo_content->buffer, todo_settings->lang_id) ;

    gtk_source_style_scheme_manager_force_rescan(source_style_language_manager) ;

  }



  todo_content->textview = gtk_source_view_new_with_buffer(todo_content->buffer) ;

  #ifdef GSPELL_SUPPORT
  todo_content->gspell_view = gspell_text_view_get_from_gtk_text_view(GTK_TEXT_VIEW(todo_content->textview));
  #endif

  gtk_source_buffer_set_max_undo_levels(todo_content->buffer, -1);

  if (source_style_language_manager != NULL) {

    if (is_scheme_id_valid(todo_settings->scheme)) {

      GtkSourceStyleScheme *style_scheme = gtk_source_style_scheme_manager_get_scheme(source_style_language_manager, todo_settings->scheme)  ;

      gtk_source_buffer_set_style_scheme(todo_content->buffer, style_scheme) ;

    }

  }


  GtkSourceCompletion *source_completion = gtk_source_view_get_completion(GTK_SOURCE_VIEW(todo_content->textview)) ;

  GtkSourceCompletionWords *source_completion_words = gtk_source_completion_words_new("todo_completion", NULL) ;

  gtk_source_completion_words_register(source_completion_words, GTK_TEXT_BUFFER(todo_content->buffer)) ;

  /** TODO: This can be user defined ! **/
  g_object_set(G_OBJECT(source_completion_words), "minimum-word-size", 3, NULL) ;
  g_object_set(G_OBJECT(source_completion_words), "interactive-delay", 25, NULL) ;


  g_object_set(G_OBJECT(source_completion), "show-headers", FALSE, NULL ) ;


  gtk_source_completion_add_provider(source_completion,GTK_SOURCE_COMPLETION_PROVIDER(source_completion_words), NULL) ;

  setting_sourceview_settings(GTK_SOURCE_VIEW(todo_content->textview)) ;

  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(todo_content->textview), TRUE);

  GtkTextIter start_iter ;
  gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(todo_content->buffer), &start_iter);
  gtk_text_buffer_place_cursor(GTK_TEXT_BUFFER(todo_content->buffer),   &start_iter);


  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(todo_content->textview), 10) ;

  gtk_container_add(GTK_CONTAINER(scrolled_window), todo_content->textview) ;


  gtk_box_pack_start(GTK_BOX(main_vbox), main_buttonbox_hbox, FALSE,  FALSE,  0) ;
  gtk_box_pack_start(GTK_BOX(main_vbox), scrolled_window,     TRUE,   TRUE,   0) ;

  gtk_container_add(GTK_CONTAINER(todo_content->todo_window), main_vbox);



  /** Connecting button-bar buttons to signals: **/
  g_signal_connect(G_OBJECT(scheme_chooser_button),     "clicked",      G_CALLBACK(todo_set_scheme),              todo_content->buffer) ;
  g_signal_connect(G_OBJECT(lang_button),               "clicked",      G_CALLBACK(todo_set_highlight_language),  todo_content->buffer) ;

  #ifdef GSPELL_SUPPORT
  g_signal_connect(G_OBJECT(inline_spellcheck_button),  "clicked",      G_CALLBACK(todo_inline_spellcheck),       todo_content) ;
  g_signal_connect(G_OBJECT(dialog_spellcheck_button),  "clicked",      G_CALLBACK(todo_spellcheck_dialog),       todo_content) ;

  g_signal_connect(G_OBJECT(language_spellcheck_button),"clicked",      G_CALLBACK(todo_select_spellcheck_language),   todo_content) ;
  #endif

  g_signal_connect(G_OBJECT(export_button),             "clicked",      G_CALLBACK(export_todo_content), todo_content) ;
  g_signal_connect(G_OBJECT(import_button),             "clicked",      G_CALLBACK(import_todo_content), todo_content) ;

  g_signal_connect(G_OBJECT(saving_button),             "clicked",      G_CALLBACK(todo_save_content), GTK_TEXT_BUFFER(todo_content->buffer)) ;

  g_signal_connect(G_OBJECT(todo_content->todo_window), "destroy",      G_CALLBACK(gtk_widget_destroy), todo_content->todo_window) ;


  gtk_widget_show_all(todo_content->todo_window) ;

  gtk_window_present(GTK_WINDOW(todo_content->todo_window)) ;

}
#endif

