/** ***********************************************************************************
  * it-edit the Integrated Terminal Editor: a text editor with severals               *
  * integrated functionalities.                                                      *
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
#include "./configure_gui_elements.h"


void setting_sourceview_settings(GtkSourceView *view) {
  /** Configure the @GtkSourceView according to configuration. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (view == NULL) {

    return ;

  }

  gtk_source_view_set_auto_indent(view, settings.use_auto_indent) ;

  gtk_source_view_set_indent_on_tab(view, TRUE);

  gtk_source_view_set_indent_width(view, settings.indent_width) ;

  gtk_source_view_set_insert_spaces_instead_of_tabs(view, settings.use_spaces_as_tabs);

  /** FIXME: ??? gtk_source_view_set_smart_home_end(view, GTK_SOURCE_SMART_HOME_END_BEFORE);
    *        ??? gtk_source_view_set_smart_backspace(GtkSourceView *view, gboolean smart_backspace);
  **/

  gtk_source_view_set_show_line_numbers(view, settings.display_line_numbers) ;

  gtk_source_view_set_tab_width(view, settings.tabs_width) ;

  if (settings.display_tabs_chars) {
    gtk_source_view_set_draw_spaces(view, GTK_SOURCE_DRAW_SPACES_TAB) ;
  }

  if (settings.display_all_spaces) {

    gtk_source_view_set_draw_spaces(view,  GTK_SOURCE_DRAW_SPACES_ALL) ;

  }

  gtk_text_view_set_monospace(GTK_TEXT_VIEW(view), TRUE);



  #if 0
  GtkCssProvider *provider = gtk_css_provider_new();



  fprintf(stdout,"%s\n", settings.editor_font) ;

  gchar *text_css = g_strconcat("GtkTextView {\n"
                                 "font‑family: ",
                                 settings.editor_font,
                                 "font-size:5px"
                                 "}\n",
                                 NULL) ;
                         
  gtk_css_provider_load_from_data(provider, text_css, -1, NULL) ;

  g_free(text_css) ;


  GdkDisplay *display = gdk_display_get_default() ;

  GdkScreen *screen = gdk_display_get_default_screen(display) ;

  gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER) ;



  g_object_unref(provider) ;
  #endif

  return ;
}


void apply_editor_change(void) {
  /** Apply changes to every noetbbok page. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  gint16 c=0 ;

  for ( ; c < gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) ; c++) {

    GtkWidget *notebook_child              = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->editor_notebook), c) ;
    GtkWidget *current_textview            = gtk_bin_get_child(GTK_BIN(notebook_child)) ;

    setting_sourceview_settings(GTK_SOURCE_VIEW(current_textview)) ;

  }

  return ;

}

void apply_terminal_change(void) {

  for (gint16 c=0 ; c < gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->terminal_notebook)) ; c++) {

    Terminals *terminals = g_object_get_data(G_OBJECT(gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->terminal_notebook),c)), "terminals") ;

    gtk_itterm_configure_terminal(terminals->terminal_1, FALSE) ;
    gtk_itterm_configure_terminal(terminals->terminal_2, FALSE) ;
  }

  gtk_widget_set_size_request(gui->terminal_notebook, settings.side_terms_factor * ( 80 * 8 + 24), -1) ;


  gtk_itterm_configure_terminal(gui->big_terminal, FALSE) ;

  gtk_itterm_configure_terminal(gui->big_four_terminals->terminal_1, FALSE) ;
  gtk_itterm_configure_terminal(gui->big_four_terminals->terminal_2, FALSE) ;
  gtk_itterm_configure_terminal(gui->big_four_terminals->terminal_3, FALSE) ;
  gtk_itterm_configure_terminal(gui->big_four_terminals->terminal_4, FALSE) ;

  return ;
}
