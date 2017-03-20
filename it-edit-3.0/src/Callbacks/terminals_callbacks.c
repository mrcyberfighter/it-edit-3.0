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

#include "terminals_callbacks.h"

void update_page_number_terminal(GtkWidget *notebook) {

  /** Update the page number data from the tab_button. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  gint c=0 ;

  for ( ; c < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; c++) {

    gint *page_number = g_object_get_data(G_OBJECT(gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), c)), "term_page_number") ;

    *page_number = c ;

    g_object_set_data(G_OBJECT(gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), c)), "term_page_number", page_number) ;

  }

  return ;

}

void terminals_page_reorder(GtkNotebook *notebook, GtkWidget *child, guint page_num, gpointer user_data) {

  /** Sidebar terminals page-reordering callback. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  update_page_number_terminal(GTK_WIDGET(notebook)) ;

  return ;

}

gboolean display_clipboard_menu(GtkWidget *widget, GdkEvent  *event, GtkWidget *clipboard_menu) {
  /** Display the clipboard menu from the terminals. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if ((event->type == GDK_BUTTON_PRESS) && (((GdkEventButton*)event)->button == 3)) {

    #if (GTK_MINOR_VERSION < 22)
    gtk_menu_popup(GTK_MENU(clipboard_menu),
                   NULL,
                   NULL,
                   NULL,
                   NULL,
                   ((GdkEventButton*)event)->button,
                   gtk_get_current_event_time());

    #else

      gtk_menu_popup_at_pointer(GTK_MENU(clipboard_menu), event);

    #endif




    return TRUE ;
  }

  return FALSE ;
}



void clipboard_copy(GtkMenuItem *menuitem, gpointer user_data) {
  /** Copy the selected text from an terminal to the clipboard. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  vte_terminal_copy_clipboard(VTE_TERMINAL(user_data));

  return ;

}

void clipboard_paste(GtkMenuItem *menuitem, VteTerminal *user_data) {
  /** Paste the clipboard content into an terminal. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  /** @NOTE: It were better to format the content of the clipboard
    *        for a better paste mechanic.
    *        Because it can come to problem in relationship where you cut or copy
    *        and where you paste the text. According the leading spaces.
  **/

  vte_terminal_paste_clipboard(VTE_TERMINAL(user_data)) ;

  return ;
}

void increase_font_size(VteTerminal *vte_terminal, VteTerminal *vteterminal) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  double font_size = vte_terminal_get_font_scale(vteterminal);


  if  (font_size < 4) {
    font_size += 0.1 ;
    vte_terminal_set_font_scale(vteterminal, font_size) ;
  }

  return ;

}

void decrease_font_size(VteTerminal *vte_terminal, VteTerminal *vteterminal) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  double font_size = vte_terminal_get_font_scale(vteterminal);

  if  (font_size > 0.25) {

    font_size -= 0.1 ;

    vte_terminal_set_font_scale(vteterminal, font_size) ;
  }

  return ;

}

void reset_terminal(GtkMenuItem *menuitem, gpointer vteterminal) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  vte_terminal_reset(VTE_TERMINAL(vteterminal), TRUE, TRUE) ;

  /** Configure terminal partially according to the configuration stored settings. **/
  gtk_itterm_configure_terminal(vteterminal, FALSE) ;


  char *argv_cmd[2] = {settings.user_shell, NULL} ;

  /** Fork a new process: your default shell. The configurations files like .bashrc will be reading. **/
  GPid child_pid = -1 ;

  gchar **envp = g_get_environ() ;

  vte_terminal_spawn_sync(VTE_TERMINAL(vteterminal),
                          VTE_PTY_DEFAULT,
                          settings.start_dir,
                          argv_cmd,
                          envp,
                          G_SPAWN_DEFAULT, NULL, NULL, &child_pid, NULL,  NULL);


  g_strfreev(envp) ;

  vte_terminal_watch_child(VTE_TERMINAL(vteterminal), child_pid) ;

  GPid *shell_pid = g_object_get_data(G_OBJECT(vteterminal), "Pid") ;

  g_spawn_close_pid(*shell_pid) ;

  /** Wait for the forking from the shell has been done. **/
  while (child_pid == -1) {

    g_main_context_iteration(NULL, FALSE) ;

  }

  g_object_set_data(G_OBJECT(vteterminal), "Pid", &child_pid) ;

  /** Wait for the forking from the shell has been done. **/
  while (g_main_context_pending(NULL)) {
    g_main_context_iteration(NULL, FALSE) ;
  }



  gchar *command_string = g_strdup_printf("%s \r", settings.command) ;

  vte_terminal_feed_child(VTE_TERMINAL(vteterminal), command_string, strlen(command_string) );

  g_free(command_string) ;

  return ;
}

void remove_terminal_tabs_from_clipboard(GtkWidget *widget, gint *page_num) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->terminal_notebook)) == 1) {

    return ;

  }

  GtkWidget *page = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->terminal_notebook), *page_num) ;

  Terminals *terminals = g_object_get_data(G_OBJECT(page), "terminals") ;

  GPid *shell_pid_1 = g_object_get_data(G_OBJECT(gtk_itterm_get_vte_terminal(terminals->terminal_1)), "Pid") ;
  GPid *shell_pid_2 = g_object_get_data(G_OBJECT(gtk_itterm_get_vte_terminal(terminals->terminal_2)), "Pid") ;

  g_spawn_close_pid(*shell_pid_1) ;
  g_spawn_close_pid(*shell_pid_2) ;

  g_free(terminals) ;

  gtk_notebook_remove_page(GTK_NOTEBOOK(gui->terminal_notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(gui->terminal_notebook))) ;

  update_page_number_terminal(gui->terminal_notebook) ;

  free(page_num) ;

  return ;

}


void remove_terminal_tabs(GtkWidget *widget, GtkWidget *user_data) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->terminal_notebook)) == 1) {
    return ;
  }

  Terminals *terminals = g_object_get_data(G_OBJECT(user_data), "terminals") ;

  GPid *shell_pid_1 = g_object_get_data(G_OBJECT(gtk_itterm_get_vte_terminal(terminals->terminal_1)), "Pid") ;
  GPid *shell_pid_2 = g_object_get_data(G_OBJECT(gtk_itterm_get_vte_terminal(terminals->terminal_2)), "Pid") ;

  g_spawn_close_pid(*shell_pid_1) ;
  g_spawn_close_pid(*shell_pid_2) ;

  free(terminals) ;

  gtk_notebook_remove_page(GTK_NOTEBOOK(gui->terminal_notebook), gtk_notebook_page_num(GTK_NOTEBOOK(gui->terminal_notebook), user_data) ) ;

  update_page_number_terminal(gui->terminal_notebook) ;

  return ;

}

