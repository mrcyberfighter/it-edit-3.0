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

#ifndef IT_EDIT_TERMINALS_CALLBACKS_HH
#define IT_EDIT_TERMINALS_CALLBACKS_HH

#include "../Headers/global_vars.h"

#include "../GUI/configure/configure_gui_elements.h"


extern void update_page_number_terminal(GtkWidget *notebook) ;

extern void terminals_page_reorder(GtkNotebook *notebook, GtkWidget   *child, guint page_num, gpointer user_data) ;

extern gboolean display_clipboard_menu(GtkWidget *widget, GdkEvent  *event, GtkWidget *clipboard_menu) ;

extern void clipboard_copy(GtkMenuItem *menuitem, gpointer user_data) ;

extern void clipboard_paste(GtkMenuItem *menuitem, VteTerminal *user_data) ;

extern void reset_terminal(GtkMenuItem *menuitem, gpointer vteterminal) ;

extern void increase_font_size(VteTerminal *vte_terminal, VteTerminal *vteterminal) ;

extern void decrease_font_size(VteTerminal *vte_terminal, VteTerminal *vteterminal) ;

extern void remove_terminal_tabs_from_clipboard(GtkWidget *widget, gint *page_num) ;

extern void remove_terminal_tabs(GtkWidget *widget, GtkWidget *user_data) ;

#endif
