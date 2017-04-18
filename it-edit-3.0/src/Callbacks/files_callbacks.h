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

#ifndef IT_EDIT_FILES_CALLBACKS_HH
#define IT_EDIT_FILES_CALLBACKS_HH

#include "../Headers/global_vars.h"

#include "./gui_callbacks.h"

#include "../Editor/editor.h"

#include "../GUI/dialogs/message_dialog.h"

#include "./editor_callbacks.h"

#include "../FilesIO/files_saver.h"

extern void process_selected_file(GFile *gfile) ;

extern void open_file(GtkWidget *widget) ;

extern void open_recent(GtkRecentChooser *chooser) ;

extern void save_as_file(GtkWidget *widget) ;

extern void save_file(GtkWidget *widget) ;

extern void save_all_file(GtkWidget *widget) ;

extern void close_all_files(GtkWidget *widget) ;

extern void new_file(GtkWidget *widget) ;

extern void close_file(GtkWidget *widget, gint *page_nb) ;

extern void reloading_file(GtkWidget *widget) ;

extern void reload_last_files(GtkWidget *widget) ;

extern void reload_last_documentation(GtkWidget *widget) ;

extern void reload_last_applications(GtkWidget *widget) ;

extern void reload_last_session(GtkWidget *widget) ;

extern void clear_session(GtkWidget *widget) ;

extern void register_files(void) ;

extern guint16 files_not_saved_check(void) ;

#endif
