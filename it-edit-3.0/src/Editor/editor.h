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

#ifndef IT_EDIT_EDITOR_HH
#define IT_EDIT_EDITOR_HH

#include "../Headers/includes.h"

#include "../GUI/configure/configure_gui_elements.h"

#include "../GUI/setup/setup_gui.h"

#include "../GUI/dialogs/message_dialog.h"

#include "../GUI/dialogs/dialogs.h"

#include "../Callbacks/gui_callbacks.h"

#include "../Callbacks/editor_callbacks.h"

#include "../Configuration/configuration.h"

#include "../Types/GtkItTerm.h"

typedef struct {

  GtkWidget *todo_window ;

  GtkSourceBuffer *buffer ;

  GtkWidget *textview ;

  #ifdef GSPELL_SUPPORT
  GspellTextView *gspell_view ;

  GspellTextBuffer *gspell_buffer ;
  #endif

} Todo_Content ;

extern gchar* get_mimetype_image_filepath(GtkSourceFile *source_file, const char *filepath_prefix) ;

extern void add_file_to_editor(GtkWidget *editor_notebook, File_Editor  *file_editor, GFile *g_file, bool start) ;

extern void reload_file(GtkSourceBuffer *buffer, const char *filepath,  File_Editor *file_editor) ;

extern void display_todo_window(GtkWidget *widget, gpointer user_data) ;

#endif
