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

#ifndef IT_EDIT_EDITOR_CALLBACKS_HH
#define IT_EDIT_EDITOR_CALLBACKS_HH

#include "../Headers/global_vars.h"

#include "../Headers/defines.h"

#include "../Callbacks/highlight_selection_callbacks.h"
#include "../Callbacks/search_settings_callbacks.h"
#include "../Callbacks/gui_callbacks.h"

#include "../Callbacks/terminals_callbacks.h"

#include "../Editor/editor.h"

#ifdef GSPELL_SUPPORT
extern void inline_spellcheck_menuitem(GtkWidget *widget) ;

extern void inline_spellcheck_button(GtkWidget *widget) ;

extern void spellcheck_dialog(GtkWidget *widget) ;
#endif

extern void update_page_number(GtkWidget *notebook) ;

extern void change_current_page(GtkNotebook *notebook, GtkWidget *page, guint page_num) ;

extern void page_reorder(GtkNotebook *notebook, GtkWidget   *child, guint page_num) ;

extern void update_rows_cols_info(GtkWidget *widget, GdkEvent  *event) ;

extern void textbuffer_changed_modified(GtkTextBuffer *textbuffer, GtkWidget *tab_label) ;

extern gboolean textview_button_release_event(GtkWidget *textview, GdkEvent  *event) ; // inline

extern gboolean textview_button_press_event(GtkWidget *textview, GdkEvent  *event) ;

extern void switch_spaces_tabs(GtkWidget *widget, gboolean *switcher) ;

#endif
