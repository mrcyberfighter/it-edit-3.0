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

#ifndef IT_EDIT_GUI_CALLBACKS_HH
#define IT_EDIT_GUI_CALLBACKS_HH

#include "../Headers/global_vars.h"

#include "../Headers/defines.h"

#include "./highlight_selection_callbacks.h"

#include "./search_settings_callbacks.h"

#include "./configuration_callbacks.h"

#include "../GUI/dialogs/dialogs.h"

#include "../Editor/editor.h"

#include "../Callbacks/files_callbacks.h"

#include "../Callbacks/editor_callbacks.h"

#include "../Types/GtkSmartMenuItem.h"

extern void toggle_display_sidebar_terminals_menuitem(GtkWidget *widget) ;

extern void toggle_display_sidebar_terminals_toggle_button(GtkWidget *widget) ;

extern void add_new_terminals(GtkWidget *widget) ;

extern void toogle_between_big_terminal_features(GtkWidget *widget) ;

extern void toggle_big_terminal_main_interface_menuitem(GtkWidget *widget) ;

extern void toggle_big_terminal_main_interface_toggle_button(GtkWidget *widget) ;

extern void toggle_display_search_and_replace_bar(GtkToggleButton *togglebutton) ;

extern void close_search_and_replace_bar(GtkWidget *button) ;

extern void toggle_use_space_instead_of_tabs(GtkWidget *widget) ;

extern void set_syntax_highlight_radio(const gchar *id) ;

extern gboolean set_syntax_highlight(GtkMenuItem *menuitem, GdkEvent  *event, const char *lang_id) ;

extern void set_scheme_highlight(GtkMenuItem *menuitem, char *scheme_id) ;

extern void launch_application(GtkWidget *widget, const char *cmdline) ;

extern void launch_readme_html(GtkWidget *widget) ;

extern void launch_license_html(GtkWidget *widget) ;

extern void quit_application(GtkWidget *widget) ;

#endif
