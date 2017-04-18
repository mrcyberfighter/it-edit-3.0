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

#ifndef IT_EDIT_SETTINGS_STRUCTURES_HH
#define IT_EDIT_SETTINGS_STRUCTURES_HH

typedef struct {

   /** Terminal settings: **/

   gdouble font_scale ;

   /** GUI settings: **/

   gdouble  side_terms_factor ;




   /** Editor settings: **/

   gchar *scheme ;

   gchar *editor_font    ;

   /** charset of the file encoding. **/
   gchar *charset ;

   /** language_code of the spellcheck. **/
   gchar *language_code ;


   /** Terminal settings: **/

   gchar *term_argv ;

   gchar *term_font      ;

   gchar  *user_shell    ;

   gchar  *start_dir     ;

   gchar *term_bg        ;

   gchar *term_fg        ;

   gchar *command ;

   gchar *cursor_color ;

   gchar *bold_color ;




   /** Editor settings: **/
   gint indent_width ;

   gint tabs_width    ;

   guint right_margin_value ;

   /** Files settings. **/
   gint save_file_mode ;

   gint newline_type ;

   gint session_mode ;


   /** Terminal settings: **/
   gint   term_font_size  ;

   gint  scrollback_lines ;





   /** GUI settings:  **/
   gboolean side_terms_on   ;

   gboolean big_term_on   ;

   gboolean big_term_div ;

   gboolean buttonbar_on ;

   gboolean fullscreen  ;


   /** Editor settings: **/

   gboolean backup_file  ;

   gboolean display_line_numbers ;

   gboolean display_tabs_chars   ;

   gboolean use_auto_indent ;

   gboolean use_spaces_as_tabs ;

   gboolean rm_trailing_spaces ;

   gboolean spellcheck_inline ;

   gboolean  display_all_spaces ;

   gboolean use_monospace_font ;

   gboolean  warn_read_only  ;

   gboolean  warn_file_open  ;

   gboolean  warn_file_save  ;

   gboolean  notifications ;

   #if 0
   gboolean smart_backspace ;
   #endif

   gboolean grid_background ;

   gboolean overwrite_anyway ;

   gboolean use_right_margin ;

   /** Terminals settings: **/
   gboolean scroll_on_output ;

   gboolean scroll_on_keystroke ;

   gboolean audible_bell ;

   gboolean pointer_autohide ;

   gboolean bold_allow ;


   /** Terminals settings: **/
   gint8 delete_binding : 3 ;

   gint8 backspace_binding : 3 ;

   gint8 cursor_shape : 3 ;

   gint8 cursor_blink : 2 ;

} Settings ;


typedef struct {

  gsize session_files_nb ;

  gsize session_doc_nb ;

  gsize session_app_nb ;

  gchar **session_files ;

  gchar **session_doc ;

  gchar **session_app ;

} Session ;

typedef struct {

  gsize nb_of_others ;

  /** Programing **/

  gchar *diff ;

  gchar *debugger ;

  gchar *python ;

  gchar *perl ;

  gchar *gui_designer ;

  gchar *devhelp ;

  /** Utils  **/
  gchar *calculator ;

  gchar *color_picker ;

  gchar *dictionary ;

  gchar *file_manager ;

  gchar *notes ;

  gchar *browser ;

  gchar *screenshot ;

  gchar *record_desktop ;

  gchar **others ;

} Gui_App ;

typedef struct {

  char *label ;  /** "Files Handler" functionnality button label. **/

  char *uri   ;  /** Associated file URI. **/

} Files_Manager ;

#endif
