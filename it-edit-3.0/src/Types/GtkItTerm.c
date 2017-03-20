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

#include "GtkItTerm.h"

/** Thanks to M. Gérald Dumas <http://linechec.sourceforge.net/, http://m3dlib.tuxfamily.org/>
  *
  * For the seed idea of creating a GtkItTerm type.
  *
  * The code of the GtkItTerm type (*.h and *.c files) was originally entirely written by M. Gérald Dumas.
  ********************************************************************************************************
  *
  * M. Gérald Dumas has analyze my code of it-edit (version 2.91)
  * and send me, per the forum from developpez.net located in France,
  * on which I have present it-edit (version 2.91),
  * a wonder full complete template code for writing the GtkItTerm type.
  *
  ********************************************************************************************************
  *
  * In it-edit (version 2.91) the vteterminal was included into a GtkBox with aside a GtkScrollbar.
  *
  * In the new version the vteterminal is included in a single GtkScrolledWindow.
  * So I only had to made few changes to make the originally code from M. Gérald Dumas working,
  * with deriving the GtkItTerm from a GtkScrolledWindow.
  ********************************************************************************************************
  *
  * We don't forget to thanks M. Gérald Dumas for his precious help at the begin of the project it-edit.
  * By rereading entirely my code from it-edit (version 1.0.0) and pointing all the worse memory leaks
  * I have made in it-edit (version 1.0.0) so that the first release version of it-edit was the
  * it-edit 1.0.1 (the version after having correct all the errors pointed by M. Gérald Dumas).
  * Thanks to my friend gerald.
  * And to all the others having react by emitting ideas and correct some piece of code along
  * the development of it-edit, which isn't finish.
  *
  ********************************************************************************************************/


/** Private structure definition. **/
struct _GtkItTermPrivate {

  GtkWidget *vteterm ;

  GtkWidget *clipboard_menu ;

};

static void gtk_itterm_class_init(GtkItTermClass *class); /** Initialize  **/
static void gtk_itterm_init(GtkItTerm *itterm);           /** Constructor **/
static void gtk_itterm_finalize(GObject *object);         /** Destructor  **/
static void gtk_itterm_dispose(GObject *object) ;

G_DEFINE_TYPE_WITH_PRIVATE(GtkItTerm, gtk_itterm, GTK_TYPE_SCROLLED_WINDOW)

static void gtk_itterm_class_init(GtkItTermClass *class) {

  g_return_if_fail(class != NULL);

  G_OBJECT_CLASS(class)->dispose = gtk_itterm_dispose;

  G_OBJECT_CLASS(class)->finalize = gtk_itterm_finalize ;

}

static void gtk_itterm_init(GtkItTerm *itterm) {

  g_return_if_fail(itterm != NULL);

  /** Permit to access object private data. **/
  GtkItTermPrivate *priv = gtk_itterm_get_instance_private(itterm);

  /** Initialize private datas. **/
  priv->vteterm = vte_terminal_new() ;
  priv->clipboard_menu = gtk_menu_new() ;

}

static void gtk_itterm_dispose(GObject *object) {

  GtkItTermPrivate *priv = gtk_itterm_get_instance_private(GTK_ITTERM(object));

  /* In dispose(), you are supposed to free all types referenced from this
   * object which might themselves hold a reference to self. Generally,
   * the most simple solution is to unref all members on which you own a
   * reference.
   */

  /* dispose() might be called multiple times, so we must guard against
   * calling g_object_unref() on an invalid GObject by setting the member
   * NULL; g_clear_object() does this for us.
   */

  priv->clipboard_menu = g_object_ref_sink(priv->clipboard_menu) ;

  priv->vteterm = g_object_ref_sink(priv->vteterm) ;

  /* Always chain up to the parent class; there is no need to check if
   * the parent class implements the dispose() virtual function: it is
   * always guaranteed to do so
   */
  G_OBJECT_CLASS(gtk_itterm_parent_class)->dispose(object);

}


static void gtk_itterm_finalize(GObject *object) {

  /** Permit to access object private data. **/
  GtkItTermPrivate *priv = gtk_itterm_get_instance_private(GTK_ITTERM(object));

  g_clear_object(&priv->vteterm);

  GList *clipboard_menu_child_list = gtk_container_get_children(GTK_CONTAINER(priv->clipboard_menu)) ;

  clipboard_menu_child_list = g_list_nth (clipboard_menu_child_list, 0) ;

  while (clipboard_menu_child_list != NULL) {

    if (clipboard_menu_child_list->data != NULL) {

      gtk_widget_destroy(GTK_WIDGET(clipboard_menu_child_list->data)) ;

    }

    if (clipboard_menu_child_list->next != NULL) {

      clipboard_menu_child_list = clipboard_menu_child_list->next ;
    }
    else {

      break ;
    }

  }
  
  g_clear_object(&priv->clipboard_menu);

  /** Private datas destructor if necessary. **/
  G_OBJECT_CLASS(gtk_itterm_parent_class)->finalize(object);

  #ifdef DEBUG
  fprintf(stderr,"GtkItTerm: %s (priv->vteterm)         -> %p\n", __func__, priv->vteterm) ;
  fprintf(stderr,"GtkItTerm: %s (priv->clipboard_menu)  -> %p\n", __func__, priv->clipboard_menu) ;
  #endif

}

void gtk_itterm_setup(GtkWidget *itterm, gboolean is_sidebar) {

  char *argv_cmd[2] = {settings.user_shell, NULL} ;

  /** Fork a new process: your default shell or the one you have configurated.
    *
    * The process is killed per example if you closing a terminals tab or by resetting the terminal
    * else it is automatic killed by exiting the application.
    *
    * So we let no zoombies after us.
    *
    ***********************************************************************************************/

  GtkWidget *vteterminal = gtk_itterm_get_vte_terminal(itterm) ;
  GtkWidget *clipboard_menu = gtk_itterm_get_clipboard_menu(itterm) ;

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

  #if 0
  while (child_pid == -1) {

    fprintf(stderr,"wait pid \n") ;

    /** Wait until the fork has been completed, i.e until the pid from the child process is set. **/

    g_main_context_iteration(NULL, FALSE) ;

  }
  #endif

  /** Build a clipboard menu connecting to the terminal. **/

  GtkWidget *copy_label  = gtk_menu_item_new_with_label( _("Copy from terminal") ) ;
  GtkWidget *paste_label = gtk_menu_item_new_with_label( _("Paste  to terminal") ) ;


  GtkWidget *new_tab_label    = gtk_menu_item_new_with_label( _("Open new tab") ) ;
  GtkWidget *close_tab_label  = gtk_menu_item_new_with_label( _("Close tab") ) ;

  GtkWidget *decr_font_label = gtk_menu_item_new_with_label( _("Decrease font-scale") ) ;
  GtkWidget *incr_font_label = gtk_menu_item_new_with_label( _("Increase font-scale") ) ;

  GtkWidget *reset_label      = gtk_menu_item_new_with_label( _("Reset terminal") ) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(clipboard_menu), copy_label)  ;
  gtk_menu_shell_append(GTK_MENU_SHELL(clipboard_menu), paste_label) ;


  if (is_sidebar) {

    gtk_menu_shell_append(GTK_MENU_SHELL(clipboard_menu), gtk_separator_menu_item_new())     ;
    gtk_menu_shell_append(GTK_MENU_SHELL(clipboard_menu), new_tab_label)   ;
    gtk_menu_shell_append(GTK_MENU_SHELL(clipboard_menu), close_tab_label) ;

  }

  gtk_menu_shell_append(GTK_MENU_SHELL(clipboard_menu), gtk_separator_menu_item_new())       ;
  gtk_menu_shell_append(GTK_MENU_SHELL(clipboard_menu), decr_font_label)   ;
  gtk_menu_shell_append(GTK_MENU_SHELL(clipboard_menu), incr_font_label)   ;
  gtk_menu_shell_append(GTK_MENU_SHELL(clipboard_menu), gtk_separator_menu_item_new())       ;
  gtk_menu_shell_append(GTK_MENU_SHELL(clipboard_menu), reset_label)       ;

  GtkAccelGroup *accel_group = gtk_accel_group_new() ;

  gtk_window_add_accel_group(GTK_WINDOW(gui->main_window), accel_group);

  gtk_widget_add_accelerator(GTK_WIDGET(copy_label),  "activate", accel_group, GDK_KEY_Copy,   GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE) ;
  gtk_widget_add_accelerator(GTK_WIDGET(paste_label), "activate", accel_group, GDK_KEY_Insert, GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE) ;


  gtk_widget_add_accelerator(GTK_WIDGET(incr_font_label),  "activate", accel_group, GDK_KEY_plus,   GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE) ;
  gtk_widget_add_accelerator(GTK_WIDGET(decr_font_label),  "activate", accel_group, GDK_KEY_minus,  GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE) ;

  if (is_sidebar) {

    gtk_widget_add_accelerator(GTK_WIDGET(new_tab_label),  "activate", accel_group, GDK_KEY_t,      GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE) ;

  }


  gtk_widget_show_all(clipboard_menu) ;

  g_signal_connect(G_OBJECT(vteterminal), "button-press-event", G_CALLBACK(display_clipboard_menu), clipboard_menu) ;

  g_signal_connect(G_OBJECT(copy_label),  "activate", G_CALLBACK(clipboard_copy),  vteterminal) ;
  g_signal_connect(G_OBJECT(paste_label), "activate", G_CALLBACK(clipboard_paste), vteterminal) ;

  g_object_set_data(G_OBJECT(vteterminal), "Pid", &child_pid) ;

  g_signal_connect(G_OBJECT(reset_label), "activate", G_CALLBACK(reset_terminal),  vteterminal) ;

  g_signal_connect(G_OBJECT(incr_font_label),  "activate", G_CALLBACK(increase_font_size),  vteterminal) ;
  g_signal_connect(G_OBJECT(decr_font_label),  "activate", G_CALLBACK(decrease_font_size), vteterminal) ;

  g_signal_connect(G_OBJECT(vteterminal), "increase-font-size", G_CALLBACK(increase_font_size),  vteterminal) ;
  g_signal_connect(G_OBJECT(vteterminal), "decrease-font-size", G_CALLBACK(decrease_font_size),  vteterminal) ;

  if (is_sidebar) {

    g_signal_connect(G_OBJECT(new_tab_label),   "activate", G_CALLBACK(add_new_terminals),    NULL) ;

    gint *ret = g_malloc(sizeof(gint)) ;

    *ret = gtk_notebook_get_current_page(GTK_NOTEBOOK(gui->terminal_notebook)) ;

    ++(*ret) ;


    g_signal_connect(G_OBJECT(close_tab_label), "activate", G_CALLBACK(remove_terminal_tabs_from_clipboard),  ret ) ;

  }


  if (! is_sidebar) {

    gtk_widget_destroy(new_tab_label)  ;
    gtk_widget_destroy(close_tab_label)  ;

  }

  return ;

}

GtkWidget *gtk_itterm_new(gboolean initialize, gboolean is_sidebar_term) {

  /** Create a new (G_OBJECT) object. **/

  GtkItTerm *itterm = NULL;

  /** Create a new object. **/
  itterm  = g_object_new(GTK_TYPE_ITTERM, NULL);

  /** Get the private data pointer. **/
  GtkItTermPrivate *priv = gtk_itterm_get_instance_private(itterm);

  /** Configure the (vte) terminal parent widget: a @GTK_SCROLLED_WINDOW. **/
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(itterm), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  gtk_scrolled_window_set_overlay_scrolling(GTK_SCROLLED_WINDOW(itterm), FALSE) ;
  gtk_scrolled_window_set_vadjustment(GTK_SCROLLED_WINDOW(itterm), gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(priv->vteterm))) ;
  #if 0
  gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(itterm),   80 *  8  + 24); // 80 chars.
  gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(itterm),  24 * 16      );
  #endif

  gtk_widget_set_redraw_on_allocate(priv->vteterm , TRUE) ;

  /** Add the (vte) terminal into his parent container. **/
  gtk_container_add(GTK_CONTAINER(itterm), priv->vteterm) ;

  /** Configure the (vte) terminal. **/
  gtk_itterm_configure_terminal(GTK_WIDGET(itterm), initialize);

  /** setup @GtkItTerm **/
  gtk_itterm_setup(GTK_WIDGET(itterm), is_sidebar_term);

  /** Return the entire @GtkItTerm object as @GtkWidget pointer (for compatibility). **/
  return GTK_WIDGET(itterm) ;

}

GtkWidget* gtk_itterm_get_vte_terminal(GtkWidget *itterm) {

  if ( itterm == NULL || ! IS_GTK_ITTERM(itterm) ) {

      g_printerr("%s(...) error: *itterm is not a valid pointer on a GtkItTerm object\n", __func__);

      return NULL;
  }

  /** Getting a pointer on the private datas. **/
  GtkItTermPrivate *priv = gtk_itterm_get_instance_private (GTK_ITTERM(itterm));

  return priv->vteterm;
}

GtkWidget* gtk_itterm_get_clipboard_menu(GtkWidget *itterm) {

  if ( itterm == NULL || ! IS_GTK_ITTERM(itterm) ) {

      g_printerr("%s(...) error: *itterm is not a valid pointer on a GtkItTerm object\n", __func__);

      return NULL;
  }

  /** Getting a pointer on the private datas. **/
  GtkItTermPrivate *priv = gtk_itterm_get_instance_private(GTK_ITTERM(itterm));

  return priv->clipboard_menu ;
}

void gtk_itterm_configure_terminal(GtkWidget *itterm, gboolean initialize) {

  /** Configure the (vte) terminal according to the configuration. **/

  if ( itterm == NULL || ! IS_GTK_ITTERM(itterm) ) {                                 

    g_printerr("%s(...) error: *itterm is not a valid pointer on a GtkItTerm object.\n", __func__);

    return;

  }

  GtkWidget *vteterminal = gtk_itterm_get_vte_terminal(itterm);

  vte_terminal_set_default_colors(VTE_TERMINAL(vteterminal)) ;

  /** Font settings: **/
  PangoFontDescription *font_desc = pango_font_description_from_string(settings.term_font) ;

  g_object_set(G_OBJECT(vteterminal), "font-desc", font_desc, NULL) ;

  vte_terminal_set_font_scale(VTE_TERMINAL(vteterminal),  settings.font_scale) ;


  /** Cursor settings: **/
  vte_terminal_set_cursor_shape(VTE_TERMINAL(vteterminal), (settings.cursor_shape == 0) ? VTE_CURSOR_SHAPE_BLOCK : (settings.cursor_shape == 1) ? VTE_CURSOR_SHAPE_IBEAM : VTE_CURSOR_SHAPE_UNDERLINE) ;

  vte_terminal_set_cursor_blink_mode(VTE_TERMINAL(vteterminal), (settings.cursor_blink == 0) ? VTE_CURSOR_BLINK_SYSTEM : (settings.cursor_blink == 1) ? VTE_CURSOR_BLINK_ON : VTE_CURSOR_BLINK_OFF) ;

  vte_terminal_set_audible_bell(VTE_TERMINAL(vteterminal), settings.audible_bell) ;

  vte_terminal_set_mouse_autohide(VTE_TERMINAL(vteterminal), settings.pointer_autohide) ;

  /** Bold setting: **/

  vte_terminal_set_allow_bold(VTE_TERMINAL(vteterminal), settings.bold_allow) ;

  GdkRGBA bold_color ;
  gdk_rgba_parse(&bold_color, settings.bold_color) ;
  vte_terminal_set_color_bold(VTE_TERMINAL(vteterminal), &bold_color) ;



  /** Erasing keys binding. **/
  g_object_set(G_OBJECT(vteterminal), "backspace-binding", (settings.backspace_binding == 0) ? VTE_ERASE_AUTO :
                                                           (settings.backspace_binding == 1) ? VTE_ERASE_ASCII_BACKSPACE :
                                                           (settings.backspace_binding == 2) ? VTE_ERASE_ASCII_DELETE :
                                                           (settings.backspace_binding == 3) ? VTE_ERASE_DELETE_SEQUENCE :
                                                                                               VTE_ERASE_TTY,
                                                           NULL) ;

  g_object_set(G_OBJECT(vteterminal), "delete-binding", (settings.delete_binding == 0) ? VTE_ERASE_AUTO :
                                                        (settings.delete_binding == 1) ? VTE_ERASE_ASCII_BACKSPACE :
                                                        (settings.delete_binding == 2) ? VTE_ERASE_ASCII_DELETE :
                                                        (settings.delete_binding == 3) ? VTE_ERASE_DELETE_SEQUENCE :
                                                                                         VTE_ERASE_TTY,
                                                         NULL) ;



  /** Colors settings: **/
  GdkRGBA bg_terminal ;
  gdk_rgba_parse(&bg_terminal, settings.term_bg) ;
  GdkRGBA fg_terminal ;
  gdk_rgba_parse(&fg_terminal, settings.term_fg) ;

  vte_terminal_set_color_foreground(VTE_TERMINAL(vteterminal), &fg_terminal) ;

  vte_terminal_set_color_background(VTE_TERMINAL(vteterminal), &bg_terminal) ;


  /** Usual default settings: **/
  vte_terminal_set_scrollback_lines(VTE_TERMINAL(vteterminal),    settings.scrollback_lines) ;

  vte_terminal_set_scroll_on_output(VTE_TERMINAL(vteterminal),    settings.scroll_on_output) ;

  vte_terminal_set_scroll_on_keystroke(VTE_TERMINAL(vteterminal), settings.scroll_on_keystroke) ;

  if (initialize) {

    vte_terminal_set_encoding(VTE_TERMINAL(vteterminal), vte_terminal_get_encoding(VTE_TERMINAL(vteterminal)), NULL)  ;  // default

    /** Mouse left button double click word separators: The following characters are not register as word separators. **/
    vte_terminal_set_word_char_exceptions(VTE_TERMINAL(vteterminal), vte_terminal_get_word_char_exceptions(VTE_TERMINAL(vteterminal)) ) ;

    vte_terminal_set_rewrap_on_resize(VTE_TERMINAL(vteterminal), TRUE) ;

    vte_terminal_set_size(VTE_TERMINAL(vteterminal), 80, 24) ;

  }

  return ;
}
