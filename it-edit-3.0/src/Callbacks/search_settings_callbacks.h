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

#ifndef SEARCH_SETTINGS_CALLBACKS_HH
#define SEARCH_SETTINGS_CALLBACKS_HH

#include "../Headers/global_vars.h"

#include "./highlight_selection_callbacks.h"

extern void set_search_settings_default(Search_Settings *search_settings) ;

extern void remove_searching_tag(void) ;

extern void set_search_mode(GtkComboBox *widget) ;

extern void set_case_sensitive(GtkToggleButton *togglebutton) ;

extern void set_wrap_around(GtkToggleButton *togglebutton) ;

extern void reset_search(void) ;

extern gboolean search_history_callback(GtkWidget *widget,  GdkEventKey  *event, gpointer   user_data) ;

extern void get_iter_offset(void) ;

extern void get_iter_at_offset(void) ;

extern void activate_search_accelator(GtkWidget *widget) ;

extern void search(GtkWidget *widget) ;

extern void search_next(GtkWidget *button) ;

extern void search_previous(GtkWidget *button) ;

extern void replace_one_occurrence(GtkWidget *widget) ;

extern void replace_all_occurrence(GtkWidget *widget) ;

extern guint replace_all_occurrence_in_all_files(void) ;

#endif
