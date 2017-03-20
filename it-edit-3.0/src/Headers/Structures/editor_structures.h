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

#ifndef IT_EDIT_EDITOR_STRUCTURES_HH
#define IT_EDIT_EDITOR_STRUCTURES_HH


#include "../includes.h"

typedef struct {

  /** Pointers on current file editing widgets. **/

  GtkWidget *current_notebook_page ;

  GtkWidget *current_textview ;

  GtkTextBuffer *current_buffer ;

  #ifdef GSPELL_SUPPORT
  GspellTextView *current_gspell_view ;
  #endif

} Current_Editor ;

typedef struct  {

  /** Pointers on current file editing "Highlight selection match" functionnality. **/

  GtkSourceSearchSettings *source_search_settings ;

  GtkSourceSearchContext  *source_search_highlight_context ;

} Highlight_Selection ;

enum SEARCH_MODES { RAW_TEXT, WORD_BOUNDARIES, REGEX } ;

typedef struct {

  /** Searching settings. **/

  gboolean case_sensitive ;

  gboolean wrap_around    ;

  guint8 mode ;

} Search_Settings ;

typedef struct  {

  /** Search and replace functionnality. **/

  GtkTextIter start_search   ;

  GtkTextIter start_match    ;

  GtkTextIter  end_match     ;



  GtkSourceSearchSettings *source_search_settings ;

  GtkSourceSearchContext  *source_search_steps_context  ;

  GtkSourceSearchContext  *source_search_highlight_context  ;


  GtkTextTag *search_tag     ;


  char *search_text ;


  gint start_search_offset    ;

  gint start_match_offset    ;

  gint end_match_offset    ;



  gboolean complete_search   ;

  gboolean only_highlight    ;

  gboolean get_an_occurrence  ;

  gboolean restart_search     ;

  gboolean has_selection      ;



} Searching ;

struct Unsaved_File {

  /** At exit unsaved files filepath and page number. **/

  char *filepath  ;

  int page_number ;

}  ;

#endif
