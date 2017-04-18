/** ***********************************************************************************
  * it-edit the Integrated Terminal Editor: a text editor with severals               *
  * integrated functionalities.                                                       *
  *                                                                                   *
  * Copyright (C) 2015,2016 Brüggemann Eddie.                                         *
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

#ifndef IT_EDIT_DIALOGS_HH
#define IT_EDIT_DIALOGS_HH

#include "../../Headers/global_vars.h"

#include "../configure/configure_gui_elements.h"

#include "./message_dialog.h"

#include "../../Callbacks/terminals_callbacks.h"

#include "../../GUI/setup/setup_gui.h"

#include "../../Types/GtkItTerm.h"

extern void go_to_line(gint line_count) ;

extern void display_go_to_line_dialog(GtkWidget *button) ;

extern void display_execute_command_dialog(GtkWidget *button) ;

extern void display_application_laucher_dialog(GtkWidget *widget) ;

extern void display_file_handler_dialog(GtkWidget *widget) ;

extern gboolean display_unsaved_files_dialog(void) ;

extern void display_file_informations_dialog(GtkWidget *widget) ;

extern void copy_filepath_to_clipboard(GtkWidget *widget, gchar *filepath) ;

extern void display_about_dialog(GtkWidget *widget) ;

#ifdef GSPELL_SUPPORT
extern void select_spellcheck_language(GtkWidget *widget) ;
#endif

extern void activate_replace_all_in_all_files(GtkWidget *widget) ;

extern void user_salutation(GtkWidget *widget) ;

#endif
