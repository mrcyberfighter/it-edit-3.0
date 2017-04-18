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

#include "editor_callbacks.h"

#ifdef GSPELL_SUPPORT

static gboolean inline_spellcheck_on = FALSE ;

static void set_inline_spellcheck(GspellTextView *gspell_view) ;

static void unset_inline_spellcheck(GspellTextView *gspell_view) ;

static void set_inline_spellcheck(GspellTextView *gspell_view) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  /** Switch in-line-spell-check @ON. **/

  GspellChecker *checker = gspell_checker_new(gspell_language_lookup(settings.language_code))  ;

  GspellTextBuffer *gspell_buffer = gspell_text_buffer_get_from_gtk_text_buffer(GTK_TEXT_BUFFER(current_editor.current_buffer)) ;
  gspell_text_buffer_set_spell_checker(gspell_buffer, checker);
  g_object_unref(checker);

  gspell_text_view_set_inline_spell_checking(gspell_view, TRUE);
  #ifdef GSPELL_SUPPORT_1_2_1
  /** This is never defined by a normal compilation but you can define it !
   *  But the language is not stored.
   *  For further informations read the notice.
  **/
  gspell_text_view_set_enable_language_menu(todo_content->gspell_view, TRUE);
  #endif


  inline_spellcheck_on = TRUE ;

  return ;

}

static void unset_inline_spellcheck(GspellTextView *gspell_view) {

  /** Switch in-line-spell-check @OFF. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  gspell_text_view_set_inline_spell_checking(gspell_view, FALSE) ;

  inline_spellcheck_on = FALSE ;

  return ;
}

/** Variable for inhibiting the toggled effect according the menu-item. **/
static bool inline_spellcheck_echo = true ;

void inline_spellcheck_button(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (! inline_spellcheck_echo) {

    return ;

  }

  File_Editor *file_editor = NULL ;

  file_editor = (File_Editor *) g_object_get_data(G_OBJECT(current_editor.current_textview), "file_editor") ;

  if ( gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)) ) {

    set_inline_spellcheck(file_editor->gspell_view) ;

  }
  else {

    unset_inline_spellcheck(file_editor->gspell_view) ;

  }

  inline_spellcheck_echo = false ;

  /** Simply call the menu item connected handler to do the work. **/
  gtk_menu_item_activate(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(gui->menuitems->inline_spellcheck_menuitem))) ;

  inline_spellcheck_echo = true ;

  return ;

}

void inline_spellcheck_menuitem(GtkWidget *widget) {

  if (! inline_spellcheck_echo) {

    return ;

  }


  File_Editor *file_editor = NULL ;

  file_editor = (File_Editor *) g_object_get_data(G_OBJECT(current_editor.current_textview), "file_editor") ;

  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {

    set_inline_spellcheck(file_editor->gspell_view) ;

  }
  else {

    unset_inline_spellcheck(file_editor->gspell_view) ;

  }

  inline_spellcheck_echo = false ;

  if (! gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui->buttons->inline_spellcheck_button)) ) {

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui->buttons->inline_spellcheck_button), TRUE) ;

  }
  else {

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui->buttons->inline_spellcheck_button), FALSE) ;

  }

  inline_spellcheck_echo = true ;

  return ;

}


void spellcheck_dialog(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  gboolean is_inline = gspell_text_view_get_inline_spell_checking(gspell_text_view_get_from_gtk_text_view(GTK_TEXT_VIEW(current_editor.current_textview))) ;

  GspellChecker *checker = gspell_checker_new(gspell_language_lookup(settings.language_code))   ;

  GspellTextBuffer *gspell_buffer = gspell_text_buffer_get_from_gtk_text_buffer(GTK_TEXT_BUFFER(current_editor.current_buffer)) ;
  gspell_text_buffer_set_spell_checker (gspell_buffer, checker);
  g_object_unref(checker);

  GspellNavigator *spell_navigator  = gspell_navigator_text_view_new(GTK_TEXT_VIEW(current_editor.current_textview));
  GtkWidget *spell_check_dialog = gspell_checker_dialog_new(GTK_WINDOW(gui->main_window), spell_navigator);

  gtk_dialog_run(GTK_DIALOG(spell_check_dialog));

  gspell_text_buffer_set_spell_checker(gspell_buffer, NULL);

  gtk_widget_destroy(spell_check_dialog) ;

  if (is_inline) {

    File_Editor *file_editor = g_object_get_data(G_OBJECT(current_editor.current_textview), "file_editor") ;

    set_inline_spellcheck(file_editor->gspell_view) ;

  }


  return ;

}
#endif

void update_page_number(GtkWidget *notebook) {

  /** Update the page number data from the tab_button. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  gint c=0 ;

  const gint page_number = gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ;

  for (; c < page_number  ; ++c) {

    GtkWidget *notebook_page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), c) ;

    GtkWidget *tab_hbox = gtk_notebook_get_tab_label(GTK_NOTEBOOK(notebook), notebook_page) ;


    GList *tab_child = gtk_container_get_children(GTK_CONTAINER(tab_hbox)) ;

    GList *node = g_list_nth(tab_child, 3) ;


    GtkWidget *tab_button = node->data ;


    gint *page_number = g_object_get_data(G_OBJECT(tab_button), "page_number") ;

    *page_number = c ;

    g_object_set_data(G_OBJECT(tab_button), "page_number", page_number) ;

  }

  return ;

}




void change_current_page(GtkNotebook *notebook, GtkWidget *page, guint page_num) {

  /** Change notebook page callback. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  /** Getting current notebook page editor. **/
  current_editor.current_notebook_page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), page_num) ;
  current_editor.current_textview      = gtk_bin_get_child(GTK_BIN(current_editor.current_notebook_page)) ;
  current_editor.current_buffer        = gtk_text_view_get_buffer(GTK_TEXT_VIEW(current_editor.current_textview)) ;


  gchar *filepath =  g_object_get_data(G_OBJECT(current_editor.current_buffer), "filepath") ;


  #if 0 /** This cause too many problems ! And I think this doesn't work properly. **/

  File_Editor *file_editor = NULL ;

  file_editor = (File_Editor *) g_object_get_data(G_OBJECT(current_editor.current_textview), "file_editor") ;

  if (file_editor != NULL) {

    if (gtk_source_file_is_local(file_editor->source_file)) {

      gtk_source_file_check_file_on_disk(file_editor->source_file) ;

      if (gtk_source_file_is_externally_modified(file_editor->source_file)) {

        if (ask_for_reload_file(filepath)) {

          reload_file(GTK_SOURCE_BUFFER(current_editor.current_buffer), filepath, file_editor) ;
        }
      }
    }
  }
  #endif


  reset_search() ;

  /** Getting the search contexts **/
  searching.source_search_steps_context     = (GtkSourceSearchContext  *) g_object_get_data(G_OBJECT(current_editor.current_textview), "source_search_steps_context") ;
  searching.source_search_highlight_context = (GtkSourceSearchContext  *) g_object_get_data(G_OBJECT(current_editor.current_textview), "source_search_highlight_context") ;

  gtk_source_search_context_set_highlight(searching.source_search_steps_context, FALSE) ;


  gchar *basename = g_path_get_basename(filepath) ;

  if (g_str_has_prefix(basename, "New_") && strlen(basename) == strlen("New_XXXXXX")) {

    gtk_label_set_text(GTK_LABEL(gui->bottom_bar->filename_label), "New") ;

  }
  else {

    gtk_label_set_text(GTK_LABEL(gui->bottom_bar->filename_label), basename) ;

  }

  g_free(basename) ;


  set_syntax_highlight_radio( (const gchar *) g_object_get_data(G_OBJECT(current_editor.current_textview), "lang_id") ) ;

  g_free(file_selector_curdirpath) ;

  file_selector_curdirpath = g_path_get_dirname(filepath) ;

  gtk_widget_set_tooltip_text(gui->bottom_bar->filename_label, filepath) ;

  #ifdef GSPELL_SUPPORT

  File_Editor *file_editor = NULL ;

  file_editor = g_object_get_data(G_OBJECT(current_editor.current_textview), "file_editor") ;

  if (file_editor != NULL) {

    if (inline_spellcheck_on) {

      set_inline_spellcheck(file_editor->gspell_view) ;

    }
    else {

      unset_inline_spellcheck(file_editor->gspell_view) ;

    }

  }
  #endif


  gtk_widget_grab_focus(current_editor.current_textview) ;

  return ;

}

void page_reorder(GtkNotebook *notebook, GtkWidget   *child, guint page_num) {

  /** Page-reordering callback. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  update_page_number(GTK_WIDGET(notebook)) ;

  register_files() ;

  return ;

}

void update_rows_cols_info(GtkWidget *widget, GdkEvent  *event) {

  /** Update the file editor bottom bar by updating:
    *
    * -) The cursor position line and column.
    *
    * -) The total number of lines and characters.
    *
    * This function is called at the end from every event: very often.
    *
    ******************************************************************/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  const gint page_nb = gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) ;

  if (page_nb <= 0) {

    return ;

  }



  GtkTextBuffer *textbuffer = NULL ;

  if (widget != NULL) {

    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));

  }



  if (textbuffer != NULL) {



    GtkTextIter main_iter ;

    /** Getting cursor position. **/
    GtkTextMark *mark         = gtk_text_buffer_get_insert(textbuffer) ;

    if (mark != NULL) {



      gtk_text_buffer_get_iter_at_mark(textbuffer, &main_iter, mark);



    }



    GtkTextIter *rows_iter    =  gtk_text_iter_copy(&main_iter) ;
    GtkTextIter *cols_iter    =  gtk_text_iter_copy(&main_iter) ;



    gchar *current_row_col_text = NULL ;

    if (rows_iter != NULL && cols_iter != NULL) {

      /** Setting up the current cursor position line and column numbers label text **/
      current_row_col_text = g_strdup_printf( _("Line: %d ; Column: %d ;"), gtk_text_iter_get_line(rows_iter) + 1, gtk_text_iter_get_line_offset(cols_iter) ) ;

      if (current_row_col_text != NULL) {

        if (gui->bottom_bar->current_row_col != NULL) {

          gtk_label_set_text(GTK_LABEL(gui->bottom_bar->current_row_col), current_row_col_text) ;

        }


      }

    }




    /** Setting up the current total number of lines and characters label text . **/
    gchar *total_rows_cols_text = g_strdup_printf( _("Total lines: %d ; Total chars: %d ;"), gtk_text_buffer_get_line_count(textbuffer), gtk_text_buffer_get_char_count(textbuffer) ) ;

    if (total_rows_cols_text != NULL) {

      if (gui->bottom_bar->total_rows_cols != NULL) {

        gtk_label_set_text(GTK_LABEL(gui->bottom_bar->total_rows_cols), total_rows_cols_text) ;

     }

    }



    g_free(current_row_col_text)   ;
    g_free(total_rows_cols_text)   ;



    gtk_text_iter_free(rows_iter) ;
    gtk_text_iter_free(cols_iter) ;



    GtkTextIter start_iter ;
    GtkTextIter end_iter ;

    gtk_text_buffer_get_start_iter(textbuffer, &start_iter);

    gtk_text_buffer_get_end_iter(textbuffer, &end_iter);



    /** Despite this is a very slow we needed it here ; Because else the Textview cannot display the syntax highlight correctly all the time. **/
    gtk_source_buffer_ensure_highlight(GTK_SOURCE_BUFFER(textbuffer), &start_iter, &end_iter) ;



  }

  return  ;
}


void textbuffer_changed_modified(GtkTextBuffer *textbuffer, GtkWidget *tab_label) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  /** Display an asterisk next to the notebook page tab label if the editor content has change and has not be saved.
    * To mark that the editor content is not saved.
    ***************************************************************************************************************/

  if (gtk_text_buffer_get_modified(textbuffer)) {
      /** The editor content has change: **/

      /** Getting current notebook page filename. **/
      gchar *filepath = g_object_get_data(G_OBJECT(textbuffer), "filepath") ;

      /** Getting the current notebook page tab text. **/
      const gchar *tab_label_text = gtk_label_get_text(GTK_LABEL(tab_label)) ;

      if (tab_label_text[0] == '*') {

          /** The tab is already mark as changed with an asterix. **/
          remove_searching_tag() ;
        
          gtk_text_buffer_set_modified(GTK_TEXT_BUFFER(textbuffer), TRUE) ;
        
          return ;
      }
      else {

          /** We must set the tab text without asterisk. **/

          if (g_strcmp0(tab_label_text,"New") == 0 ) {
              /** The default New named file. **/

              gtk_label_set_text(GTK_LABEL(tab_label),"*New") ;

              remove_searching_tag() ;
            
              gtk_text_buffer_set_modified(GTK_TEXT_BUFFER(textbuffer), TRUE) ;
            
              return ;
          }
          else {

              /** File opened in the program. **/
            
              if (filepath != NULL) {


                gchar *basename = g_path_get_basename(filepath) ;
                gchar *tab_label_text = g_strdup_printf("*%s", basename) ;
                g_free(basename) ;
                gtk_label_set_text(GTK_LABEL(tab_label), tab_label_text) ;
                g_free(tab_label_text) ;
              
                gtk_text_buffer_set_modified(GTK_TEXT_BUFFER(textbuffer), TRUE) ;

              }


              remove_searching_tag() ;

              return ;

          }
      }
  }
  else {

    /** The file is marked with the not-modified flag. Using the asterisk as marker. **/

    /** We must erase the asterisk marker from the filename tab component. **/

    /** Getting current notebook page filename. **/
    gpointer filepath = g_object_get_data(G_OBJECT(textbuffer), "filepath") ;

    /** Getting the current notebook page tab text. **/
    const gchar *tab_label_text = gtk_label_get_text(GTK_LABEL(tab_label)) ;

    if (tab_label_text[0] == '*') {

        /** The tab is mark is changed with an asterix. **/

        if (g_strcmp0(tab_label_text,"*New") == 0 ) {

              /** The default New named file. **/

              gtk_label_set_text(GTK_LABEL(tab_label),"New") ;

              remove_searching_tag() ;
            
              gtk_text_buffer_set_modified(GTK_TEXT_BUFFER(textbuffer), FALSE) ;
            
              return ;

        }
        else {

          /** File opened in the program. **/

          if (filepath != NULL) {

            gchar *tab_label_text  = g_path_get_basename(filepath) ;

            gtk_label_set_text(GTK_LABEL(tab_label),tab_label_text) ;

            g_free(tab_label_text) ;
          
            gtk_text_buffer_set_modified(GTK_TEXT_BUFFER(textbuffer), FALSE) ;

          }

          remove_searching_tag() ;
          return ;
       }

    }

  }




  while (gtk_events_pending()) {

   gtk_main_iteration();

  }

  return  ;

}

gboolean textview_button_release_event(GtkWidget *textview, GdkEvent  *event) {
  /** Function to set the selected text matches highlight off. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkTextBuffer *textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)) ;

  gboolean get_selection = FALSE ;

  g_object_get(G_OBJECT(textbuffer), "has-selection", &get_selection, NULL) ;

  if (get_selection) {

    highlight_selection(GTK_TEXT_VIEW(textview)) ;

  }
  else {

    destroy_highlight_selection() ;

  }

  return FALSE ; /** Continue event propagation to other events handlers... **/

}

gboolean textview_button_press_event(GtkWidget *textview, GdkEvent  *event) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  gtk_source_view_set_highlight_current_line(GTK_SOURCE_VIEW(textview), FALSE) ; /** Case by go to line function line highlighting is currently on. **/

  /** Function to set the selected text matches highlight off. **/

  GtkTextBuffer *textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)) ;

  gboolean get_selection = FALSE ;

  g_object_get(G_OBJECT(textbuffer), "has-selection", &get_selection, NULL) ;

  if (! get_selection) {

    destroy_highlight_selection() ;

    remove_searching_tag() ;

  }

  return FALSE ; /** Continue event propagation to other events handlers... **/
}

void switch_spaces_tabs(GtkWidget *widget, gboolean *switcher) {

  /** Switch use of tabs callback. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  const gint number_of_pages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(widget)) ;

  gint16 c=0 ;

  for ( ; c < number_of_pages ; c++) {

    GtkWidget *notebook_page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(widget), c) ;

    GtkWidget *sourceview    = gtk_bin_get_child(GTK_BIN(notebook_page)) ;

    gtk_source_view_set_insert_spaces_instead_of_tabs(GTK_SOURCE_VIEW(sourceview), *switcher) ;

  }

  if (number_of_pages <= 0) {

    return ;

  }

  GtkWidget *sourceview = gtk_bin_get_child(GTK_BIN(gtk_notebook_get_nth_page(GTK_NOTEBOOK(widget), gtk_notebook_get_current_page(GTK_NOTEBOOK(widget))))) ;

  gtk_widget_grab_focus(sourceview) ;

  return ;
}
