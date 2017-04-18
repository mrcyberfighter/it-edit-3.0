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

#include "./setup_gui.h"

static void copy_current_filepath_to_clipboard(GtkWidget *widget) ;

static void copy_current_folderpath_to_clipboard(GtkWidget *widget) ;

static void close_the_file(GtkWidget *widget) ;

static void construct_recent_menu_items(GtkWidget *menu_recent) ;

static void construct_language_menu_items(GtkWidget *menu_syntax) ;

static void construct_scheme_menu_items(GtkWidget *menu_scheme) ;

static gchar *get_icon_path(const gchar *filename, gchar *filepath) ;

static gint compare_filenames(gconstpointer  *p1, gconstpointer  *p2, gpointer user_data) ;

static void reorder_notebook_pages(GtkWidget *widget) ;

typedef struct {

  GtkWidget *widget ;

  gchar *filename ;

} reorder_pages ;

static gchar *get_icon_path(const gchar *filename, gchar *filepath) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  g_free(filepath) ;

  filepath = g_strdup_printf("%s%s", PATH_TO_MENU_ICON, filename) ;

  return filepath ;

}

static void generate_accel_label(MenuItem *menuitem, guint accel_key, GdkModifierType accel_modifier, GtkAccelGroup *menu_files_accel_group) {

  /** @NOTE: The @gtk_smart_menu_item(s) are build with this stuff,
    *         this function is only present for:
    *         the addable menu item like the applications menu items.
    **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (gtk_accelerator_valid(accel_key, accel_modifier)) {

    /** Menu item has an shortcut **/

    gchar *tmp_accel_label = gtk_accelerator_get_label(accel_key, accel_modifier) ;

    menuitem->accel_label = gtk_accel_label_new(tmp_accel_label) ;

    g_free(tmp_accel_label) ;

    gtk_label_set_justify(GTK_LABEL(menuitem->accel_label), GTK_JUSTIFY_RIGHT) ;




    if (menu_files_accel_group != NULL) {


      gtk_widget_add_accelerator(menuitem->menuitem,
                                 "activate",
                                 menu_files_accel_group,
                                 accel_key,
                                 accel_modifier,
                                 GTK_ACCEL_VISIBLE);

    }


  }
  else {

    menuitem->accel_label = NULL ;
  }

  return ;

}

static void pack_menuitem(MenuItem *menuitem, const char *image) {

   /** @NOTE: The @gtk_smart_menu_item(s) are build with this stuff,
    *         this function is only present for:
    *         the addable menu item like the applications menu items.
    **/

   /** Pack a big part of my menu entries. **/

   #ifdef DEBUG
   DEBUG_FUNC_MARK
   #endif

   if (image) {
    // Menu item has an associate image

    gchar *image_filepath = g_strdup_printf ("%s%s", PATH_TO_MENU_ICON, image);

    menuitem->image = gtk_image_new_from_file(image_filepath) ;

    g_free(image_filepath) ;

  }

   /** Menu item container **/
  menuitem->hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0) ;

  if (image) {
    gtk_box_pack_start(GTK_BOX(menuitem->hbox), menuitem->image, FALSE, FALSE, 0) ;
  }

  gtk_box_pack_start(GTK_BOX(menuitem->hbox),   menuitem->label, FALSE, FALSE, 0) ;

  GtkWidget *dummy_label=gtk_label_new("") ; /** For expand between Text and shortcut text **/

  gtk_box_pack_start(GTK_BOX(menuitem->hbox),  dummy_label,      TRUE,  TRUE,  0) ;


  if (menuitem->accel_label != NULL) {
    gtk_box_pack_start(GTK_BOX(menuitem->hbox), menuitem->accel_label, FALSE, FALSE, 0) ;
  }
  else {
    gtk_box_pack_start(GTK_BOX(menuitem->hbox), gtk_label_new(""),      FALSE, FALSE, 0) ;
  }


  gtk_container_add(GTK_CONTAINER(menuitem->menuitem),menuitem->hbox) ;

  return ;

 }

void setup_menu_item(MenuItem *menuitem, const char type, const char *label, const char *image, GtkAccelGroup *menu_files_accel_group,  GdkModifierType accel_modifier,  guint accel_key) {

  /** Generate one menu item according to the given settings
    *
    * The @gtk_smart_menu_item(s) are build with this stuff,
    * this function is only present for:
    * the addable menu item like the applications menu items.
    *
  **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  switch (type) {

    case 'N' :

      menuitem->menuitem  = gtk_menu_item_new() ;
      break ;

    #if 0

    case 'C' :

      menuitem->menuitem  = gtk_check_menu_item_new() ;

      break ;

    #endif

  }



  menuitem->label = gtk_label_new(label) ;

  gtk_label_set_justify(GTK_LABEL(menuitem->label), GTK_JUSTIFY_LEFT) ;

  generate_accel_label(menuitem, accel_key, accel_modifier, menu_files_accel_group) ;

  pack_menuitem(menuitem, image) ;

  return ;
}


void initialize_menu(GtkWidget *menu_bar, AccelGroups *accels_group, Menus *menus, MenuItems *menuitems) {

  /** Generate all the menu items. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkWidget *menu_files   = gtk_menu_new() ;

  gchar *icon_filepath = NULL ;

  icon_filepath = get_icon_path("text-x-install.png", icon_filepath) ;

  GtkWidget *files_menubutton = gtk_smart_menu_item_new_all(_("  Files  "), icon_filepath, NULL, 0, 0) ;



  icon_filepath = get_icon_path("document-new.png", icon_filepath) ;

  GtkWidget *new_file_menuitem = gtk_smart_menu_item_new_all(_("  New file  "), icon_filepath, accels_group->menu_files_accel_group, GDK_CONTROL_MASK, GDK_KEY_n) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(new_file_menuitem), "activate", G_CALLBACK(new_file), NULL) ;




  icon_filepath = get_icon_path("document-open.png", icon_filepath) ;

  GtkWidget *open_files_menuitem = gtk_smart_menu_item_new_all(_("  Open file  "), icon_filepath, accels_group->menu_files_accel_group, GDK_CONTROL_MASK, GDK_KEY_o) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(open_files_menuitem), "activate", G_CALLBACK(open_file), NULL) ;




  GtkWidget *menu_recent = gtk_recent_chooser_menu_new() ;

  construct_recent_menu_items(menu_recent) ;

  icon_filepath = get_icon_path("document-open-recent.png", icon_filepath) ;

  GtkWidget *recent_files_menuitem = gtk_smart_menu_item_new_all(_("  Recent file(s)  "), icon_filepath, NULL, 0, 0) ;


  gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(recent_files_menuitem)), menu_recent) ;


  icon_filepath = get_icon_path("document-save.png", icon_filepath) ;

  GtkWidget *save_file_menuitem = gtk_smart_menu_item_new_all(_("  Save file  "), icon_filepath, accels_group->menu_files_accel_group, GDK_CONTROL_MASK, GDK_KEY_s) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(save_file_menuitem), "activate", G_CALLBACK(save_file), NULL) ;



  icon_filepath = get_icon_path("document-save-as.png", icon_filepath) ;

  GtkWidget *save_as_file_menuitem = gtk_smart_menu_item_new_all(_("  Save file as  "), icon_filepath, NULL, 0, 0) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(save_as_file_menuitem), "activate", G_CALLBACK(save_as_file), NULL) ;



  icon_filepath = get_icon_path("document-save-all.png", icon_filepath) ;

  GtkWidget *save_all_menuitem = gtk_smart_menu_item_new_all(_("  Save all files  "), icon_filepath, accels_group->menu_files_accel_group, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GDK_KEY_s) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(save_all_menuitem), "activate", G_CALLBACK(save_all_file), NULL) ;


  icon_filepath = get_icon_path("project-development-close-all.png", icon_filepath) ;

  GtkWidget *close_all_menuitem = gtk_smart_menu_item_new_all(_("  Close all files  "), icon_filepath, accels_group->menu_files_accel_group, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GDK_KEY_c) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(close_all_menuitem), "activate", G_CALLBACK(close_all_files), NULL) ;


  icon_filepath = get_icon_path("project-development-close.png", icon_filepath) ;

  GtkWidget *close_the_file_menuitem = gtk_smart_menu_item_new_all(_("  Close file  "), icon_filepath, accels_group->menu_files_accel_group, GDK_CONTROL_MASK | GDK_MOD1_MASK, GDK_KEY_c) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(close_the_file_menuitem), "activate", G_CALLBACK(close_the_file), NULL) ;




  GtkWidget *menu_reload = gtk_menu_new() ;

  icon_filepath = get_icon_path("preferences-system-session.png", icon_filepath) ;

  GtkWidget *last_session_menuitem = gtk_smart_menu_item_new_all(_("  Reload session  "), icon_filepath, NULL, 0, 0) ;



  icon_filepath = get_icon_path("preferences-system-session-services.png", icon_filepath) ;

  GtkWidget *last_all_menuitem = gtk_smart_menu_item_new_all(_("  Reload entire last session  "), icon_filepath, NULL, 0, 0) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(last_all_menuitem), "activate", G_CALLBACK(reload_last_session), NULL) ;




  icon_filepath = get_icon_path("document-export-table.png", icon_filepath) ;

  GtkWidget *last_files_menuitem = gtk_smart_menu_item_new_all(_("  Reload last files  "), icon_filepath, NULL, 0, 0) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(last_files_menuitem), "activate", G_CALLBACK(reload_last_files), NULL) ;




  icon_filepath = get_icon_path("documentation.png", icon_filepath) ;

  GtkWidget *last_documents_menuitem = gtk_smart_menu_item_new_all(_("  Reload last documentation  "), icon_filepath, NULL, 0, 0) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(last_documents_menuitem), "activate", G_CALLBACK(reload_last_documentation), NULL) ;



  icon_filepath = get_icon_path("applications-system.png", icon_filepath) ;

  GtkWidget *last_applications_menuitem = gtk_smart_menu_item_new_all(_("  Reload last applications  "), icon_filepath, NULL, 0, 0) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(last_applications_menuitem), "activate", G_CALLBACK(reload_last_applications), NULL) ;



  icon_filepath = get_icon_path("edit-clear.png", icon_filepath) ;

  GtkWidget *clear_session_menuitem = gtk_smart_menu_item_new_all(_("  Clear session  "), icon_filepath, NULL, 0, 0) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(clear_session_menuitem), "activate", G_CALLBACK(clear_session), NULL) ;





  icon_filepath = get_icon_path("reload_page.png", icon_filepath) ;

  GtkWidget *reload_page_menuitem = gtk_smart_menu_item_new_all(_("  Reload current file  "), icon_filepath, accels_group->menu_files_accel_group, GDK_CONTROL_MASK, GDK_KEY_r) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(reload_page_menuitem), "activate", G_CALLBACK(reloading_file), NULL) ;



  icon_filepath = get_icon_path("text-x-texinfo.png", icon_filepath) ;

  GtkWidget *file_info_menuitem = gtk_smart_menu_item_new_all(_("  File informations  "), icon_filepath, accels_group->menu_files_accel_group, GDK_CONTROL_MASK, GDK_KEY_i) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(file_info_menuitem), "activate", G_CALLBACK(display_file_informations_dialog), NULL) ;



  icon_filepath = get_icon_path("process-stop.png", icon_filepath) ;

  GtkWidget *quit_menuitem = gtk_smart_menu_item_new_all(_("  Quit  "), icon_filepath, accels_group->menu_files_accel_group, GDK_CONTROL_MASK, GDK_KEY_q) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(quit_menuitem), "activate", G_CALLBACK(quit_application), NULL) ;


  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), gtk_smart_menu_item_get_menuitem(files_menubutton)) ;

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(files_menubutton)), menu_files);


  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_smart_menu_item_get_menuitem(new_file_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_separator_menu_item_new() ) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_smart_menu_item_get_menuitem(open_files_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_smart_menu_item_get_menuitem(recent_files_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_smart_menu_item_get_menuitem(save_file_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_smart_menu_item_get_menuitem(save_as_file_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_smart_menu_item_get_menuitem(save_all_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_smart_menu_item_get_menuitem(close_the_file_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_smart_menu_item_get_menuitem(close_all_menuitem)) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_separator_menu_item_new()) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_smart_menu_item_get_menuitem(last_session_menuitem)) ;

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(last_session_menuitem)), menu_reload);

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_reload), gtk_smart_menu_item_get_menuitem(last_all_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_reload), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_reload), gtk_smart_menu_item_get_menuitem(last_files_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_reload), gtk_smart_menu_item_get_menuitem(last_documents_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_reload), gtk_smart_menu_item_get_menuitem(last_applications_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_reload), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_reload), gtk_smart_menu_item_get_menuitem(clear_session_menuitem)) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_smart_menu_item_get_menuitem(reload_page_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_smart_menu_item_get_menuitem(file_info_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_files), gtk_smart_menu_item_get_menuitem(quit_menuitem)) ;


  gtk_menu_set_accel_group(GTK_MENU(menu_files), accels_group->menu_files_accel_group) ;


  GtkWidget *menu_edition = gtk_menu_new() ;

  icon_filepath = get_icon_path("document-edit.png", icon_filepath) ;

  GtkWidget *edition_menubutton = gtk_smart_menu_item_new_all(_("  Edition  "), icon_filepath, NULL, 0, 0) ;



  icon_filepath = get_icon_path("edit-undo.png", icon_filepath) ;

  GtkWidget *undo_menuitem = gtk_smart_menu_item_new_all(_("  Undo         "), icon_filepath, NULL, GDK_CONTROL_MASK, GDK_KEY_z) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(undo_menuitem), "activate", G_CALLBACK(undo), NULL) ;


  icon_filepath = get_icon_path("edit-redo.png", icon_filepath) ;

  GtkWidget *redo_menuitem = gtk_smart_menu_item_new_all(_("  Redo         "), icon_filepath, NULL, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GDK_KEY_z) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(redo_menuitem), "activate", G_CALLBACK(redo), NULL) ;



  icon_filepath = get_icon_path("edit-find.png", icon_filepath) ;

  GtkWidget *find_menuitem = gtk_smart_menu_item_new_all( _("  Find text   "), icon_filepath, accels_group->menu_edition_accel_group, GDK_CONTROL_MASK, GDK_KEY_f) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(find_menuitem), "activate", G_CALLBACK(activate_search_accelator), NULL) ;



  icon_filepath = get_icon_path("system-search.png", icon_filepath) ;

  GtkWidget *find_all_menuitem = gtk_smart_menu_item_new_all( _("  Find all   "), icon_filepath, accels_group->menu_edition_accel_group, GDK_CONTROL_MASK  | GDK_SHIFT_MASK, GDK_KEY_f) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(find_all_menuitem), "activate", G_CALLBACK(search), NULL) ;



  icon_filepath = get_icon_path("go-down-search.png", icon_filepath) ;

  GtkWidget *find_next_menuitem = gtk_smart_menu_item_new_all( _("  Find next  "), icon_filepath,  accels_group->menu_edition_accel_group, GDK_CONTROL_MASK, GDK_KEY_KP_Add) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(find_next_menuitem), "activate", G_CALLBACK(search_next), NULL) ;


  icon_filepath = get_icon_path("go-up-search.png", icon_filepath) ;

  GtkWidget *find_prev_menuitem = gtk_smart_menu_item_new_all( _("  Find previous  "), icon_filepath,  accels_group->menu_edition_accel_group, GDK_CONTROL_MASK, GDK_KEY_KP_Subtract) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(find_prev_menuitem), "activate", G_CALLBACK(search_previous), NULL) ;




  icon_filepath = get_icon_path("edit-find-replace.png", icon_filepath) ;

  GtkWidget *replace_menuitem = gtk_smart_menu_item_new_all( _("  Replace text  "), icon_filepath,  accels_group->menu_edition_accel_group, GDK_CONTROL_MASK, GDK_KEY_KP_Enter) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(replace_menuitem), "activate", G_CALLBACK(replace_one_occurrence), NULL) ;


  icon_filepath = get_icon_path("edit-find-project.png", icon_filepath) ;

  GtkWidget *replace_all_menuitem = gtk_smart_menu_item_new_all( _("  Replace all            "), icon_filepath, accels_group->menu_edition_accel_group, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GDK_KEY_KP_Enter) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(replace_all_menuitem), "activate", G_CALLBACK(replace_all_occurrence), NULL) ;


  icon_filepath = get_icon_path("edit-find-mail.png", icon_filepath) ;

  GtkWidget *replace_in_all_files_menuitem = gtk_smart_menu_item_new_all( _("  Replace all in all files  "), icon_filepath,  accels_group->menu_edition_accel_group, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GDK_KEY_r) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(replace_in_all_files_menuitem), "activate", G_CALLBACK(activate_replace_all_in_all_files), NULL) ;



  icon_filepath = get_icon_path("edit-text-frame-update.png", icon_filepath) ;

  GtkWidget *go_to_line_menuitem = gtk_smart_menu_item_new_all( _("  Go to line number  "), icon_filepath, accels_group->menu_edition_accel_group, GDK_CONTROL_MASK, GDK_KEY_g) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(go_to_line_menuitem), "activate", G_CALLBACK(display_go_to_line_dialog), NULL) ;




  icon_filepath = get_icon_path("edit-cut.png", icon_filepath) ;

  GtkWidget *cut_menuitem = gtk_smart_menu_item_new_all( _("  Cut  "), icon_filepath, accels_group->menu_edition_accel_group, GDK_CONTROL_MASK, GDK_KEY_x) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(cut_menuitem), "activate", G_CALLBACK(cut), NULL) ;



  icon_filepath = get_icon_path("edit-copy.png", icon_filepath) ;

  GtkWidget *copy_menuitem = gtk_smart_menu_item_new_all( _("  Copy  "), icon_filepath, NULL, GDK_CONTROL_MASK, GDK_KEY_c) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(copy_menuitem), "activate", G_CALLBACK(copy), NULL) ;



  icon_filepath = get_icon_path("edit-paste.png", icon_filepath) ;

  GtkWidget *paste_menuitem = gtk_smart_menu_item_new_all( _("  Paste  "), icon_filepath, NULL, GDK_CONTROL_MASK, GDK_KEY_v) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(paste_menuitem), "activate", G_CALLBACK(paste), NULL) ;



  icon_filepath = get_icon_path("distribute-vertical-bottom.png", icon_filepath) ;

  GtkWidget *duplicate_menuitem = gtk_smart_menu_item_new_all( _("  Duplicate text  "), icon_filepath, accels_group->menu_edition_accel_group, GDK_CONTROL_MASK, GDK_KEY_d) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(duplicate_menuitem), "activate", G_CALLBACK(duplicate), NULL) ;




  icon_filepath = get_icon_path(settings.use_spaces_as_tabs ? "tabs-off.png" :  "tabs-on.png", icon_filepath) ;

  menuitems->switch_tabs_menuitem  = gtk_smart_menu_item_new_all( _("  Use space instead of tabs "), icon_filepath, accels_group->menu_edition_accel_group, GDK_CONTROL_MASK , GDK_KEY_p) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->switch_tabs_menuitem), "activate", G_CALLBACK(toggle_use_space_instead_of_tabs), NULL) ;


  #ifdef GSPELL_SUPPORT

  icon_filepath = get_icon_path("tools-check-spelling.png", icon_filepath) ;

  menuitems->inline_spellcheck_menuitem  = gtk_smart_check_menu_item_new_all( _(" Inline spellcheck "), TRUE, icon_filepath,   accels_group->menu_edition_accel_group, GDK_CONTROL_MASK , GDK_KEY_w) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->inline_spellcheck_menuitem), "activate", G_CALLBACK(inline_spellcheck_menuitem), NULL) ;
  #endif


  gtk_menu_set_accel_group(GTK_MENU(menu_files), accels_group->menu_edition_accel_group) ;



  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), gtk_smart_menu_item_get_menuitem(edition_menubutton)) ;

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(edition_menubutton)),menu_edition);


  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(undo_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(redo_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(find_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(find_all_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(find_prev_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(find_next_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(replace_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(replace_all_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(replace_in_all_files_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(go_to_line_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(cut_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(copy_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(paste_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(duplicate_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(menuitems->switch_tabs_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_separator_menu_item_new()) ;
  #ifdef GSPELL_SUPPORT
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_edition), gtk_smart_menu_item_get_menuitem(menuitems->inline_spellcheck_menuitem)) ;
  #endif

  GtkWidget *menu_action   = gtk_menu_new() ;

  icon_filepath = get_icon_path("system-run.png", icon_filepath) ;

  GtkWidget *actions_menubutton  = gtk_smart_menu_item_new_all( _("  Actions  "), icon_filepath, NULL, 0, 0) ;




  icon_filepath = get_icon_path("application-x-executable-script.png", icon_filepath) ;

  GtkWidget *exec_cmd_menuitem  = gtk_smart_menu_item_new_all( _("  Execute a command  "), icon_filepath, accels_group->menu_action_accel_group, GDK_CONTROL_MASK, GDK_KEY_e) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(exec_cmd_menuitem), "activate", G_CALLBACK(display_execute_command_dialog), NULL) ;


  icon_filepath = get_icon_path("object-order-front.png", icon_filepath) ;

  GtkWidget *reorder_pages_menuitem  = gtk_smart_menu_item_new_all( _("  Order pages  "), icon_filepath, accels_group->menu_action_accel_group, GDK_CONTROL_MASK | GDK_MOD1_MASK, GDK_KEY_o) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(reorder_pages_menuitem), "activate", G_CALLBACK(reorder_notebook_pages), NULL) ;



  icon_filepath = get_icon_path("copy-filepath-clipboard.png", icon_filepath) ;

  GtkWidget *cp_filepath_to_clipboard_menuitem  = gtk_smart_menu_item_new_all( _("  Copy file-path to clipboard  "), icon_filepath, accels_group->menu_action_accel_group, GDK_CONTROL_MASK, GDK_KEY_y) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(cp_filepath_to_clipboard_menuitem), "activate", G_CALLBACK(copy_current_filepath_to_clipboard), NULL) ;


  icon_filepath = get_icon_path("stock_navigator-insert-as-copy.png", icon_filepath) ;

  GtkWidget *cp_folderpath_to_clipboard_menuitem  = gtk_smart_menu_item_new_all( _("  Copy folder-path to clipboard  "), icon_filepath, accels_group->menu_action_accel_group, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GDK_KEY_y) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(cp_folderpath_to_clipboard_menuitem), "activate", G_CALLBACK(copy_current_folderpath_to_clipboard), NULL) ;



  icon_filepath = get_icon_path("utilities-log-viewer.png", icon_filepath) ;

  menuitems->switch_big_terms_menuitem = gtk_smart_menu_item_new_all( _("  Big terminals(s) switch      "), icon_filepath, accels_group->menu_action_accel_group, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GDK_KEY_b) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->switch_big_terms_menuitem), "activate", G_CALLBACK(toogle_between_big_terminal_features), NULL) ;



  icon_filepath = get_icon_path("utilities-system-monitor.png", icon_filepath) ;

  GtkWidget *add_term_menuitem  = gtk_smart_menu_item_new_all( _("  Sidebar terminals add tab         "), icon_filepath, accels_group->menu_action_accel_group, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GDK_KEY_t) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(add_term_menuitem), "activate", G_CALLBACK(add_new_terminals), NULL) ;



  icon_filepath = get_icon_path("project-open.png", icon_filepath) ;

  GtkWidget *files_handler_menuitem  = gtk_smart_menu_item_new_all( _("  File(s) handler  "), icon_filepath, accels_group->menu_action_accel_group, GDK_CONTROL_MASK, GDK_KEY_h) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(files_handler_menuitem), "activate", G_CALLBACK(display_file_handler_dialog), NULL) ;


  icon_filepath = get_icon_path("preferences-desktop-launch-feedback.png", icon_filepath) ;

  GtkWidget *app_launcher_menuitem  = gtk_smart_menu_item_new_all( _("  Application launcher  "), icon_filepath, accels_group->menu_action_accel_group, GDK_CONTROL_MASK, GDK_KEY_a) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(app_launcher_menuitem), "activate", G_CALLBACK(display_application_laucher_dialog), NULL) ;


  #ifdef GSPELL_SUPPORT


  icon_filepath = get_icon_path("tools-check-spellcheck-dialog.png", icon_filepath) ;

  GtkWidget *dialog_spellcheck_menuitem  = gtk_smart_menu_item_new_all( _("  Spellcheck Dialog  "),  icon_filepath,  accels_group->menu_action_accel_group, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GDK_KEY_w) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(dialog_spellcheck_menuitem), "activate", G_CALLBACK(spellcheck_dialog), NULL) ;
  #endif


  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), gtk_smart_menu_item_get_menuitem(actions_menubutton)) ;

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(actions_menubutton)),menu_action);

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_action), gtk_smart_menu_item_get_menuitem(exec_cmd_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_action), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_action), gtk_smart_menu_item_get_menuitem(reorder_pages_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_action), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_action), gtk_smart_menu_item_get_menuitem(cp_filepath_to_clipboard_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_action), gtk_smart_menu_item_get_menuitem(cp_folderpath_to_clipboard_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_action), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_action), gtk_smart_menu_item_get_menuitem(add_term_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_action), gtk_smart_menu_item_get_menuitem(menuitems->switch_big_terms_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_action), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_action), gtk_smart_menu_item_get_menuitem(files_handler_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_action), gtk_smart_menu_item_get_menuitem(app_launcher_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_action), gtk_separator_menu_item_new()) ;
  #ifdef GSPELL_SUPPORT
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_action), gtk_smart_menu_item_get_menuitem(dialog_spellcheck_menuitem)) ;
  #endif





  icon_filepath = get_icon_path("preferences-desktop-icons.png", icon_filepath) ;

  GtkWidget *app_menubutton = gtk_smart_menu_item_new_all( _("  Applications  "), icon_filepath, NULL, 0, 0) ;




  GtkWidget *app_main_menu = gtk_menu_new() ;



  icon_filepath = get_icon_path("system-software-update.png", icon_filepath) ;

  GtkWidget *app_programming_menuitem = gtk_smart_menu_item_new_all( _("  Programing  "), icon_filepath, NULL, 0, 0) ;





  GtkWidget *app_programming_menu = gtk_menu_new() ;


  icon_filepath = get_icon_path("text-x-ldif.png", icon_filepath) ;

  gchar *gui_app_diff = g_strdup_printf("  %s  ", g_path_get_basename(gui_app.diff)) ;

  menuitems->app_programming_diff_menuitem = gtk_smart_menu_item_new_all( gui_app_diff, icon_filepath, NULL, 0, 0) ;

  g_free(gui_app_diff) ;



  icon_filepath = get_icon_path("debug-run.png", icon_filepath) ;

  gchar *gui_app_debugger = g_strdup_printf("  %s  ", g_path_get_basename(gui_app.debugger)) ;

  menuitems->app_programming_debugger_menuitem = gtk_smart_menu_item_new_all( gui_app_debugger, icon_filepath, NULL, 0, 0) ;

  g_free(gui_app_debugger) ;





  icon_filepath = get_icon_path("python.png", icon_filepath) ;

  gchar *gui_app_python = g_strdup_printf("  %s  ", g_path_get_basename(gui_app.python)) ;

  menuitems->app_programming_python_menuitem = gtk_smart_menu_item_new_all( gui_app_python, icon_filepath, NULL, 0, 0) ;

  g_free(gui_app_python) ;



  icon_filepath = get_icon_path("kde-windows.png", icon_filepath) ;

  gchar *gui_app_gui_designer = g_strdup_printf("  %s  ", g_path_get_basename(gui_app.gui_designer)) ;

  menuitems->app_programming_gui_designer_menuitem = gtk_smart_menu_item_new_all( gui_app_gui_designer, icon_filepath, NULL, 0, 0) ;

  g_free(gui_app_gui_designer) ;



  icon_filepath = get_icon_path("devhelp.png", icon_filepath) ;

  gchar *gui_app_devhelp = g_strdup_printf("  %s  ", g_path_get_basename(gui_app.devhelp)) ;

  menuitems->app_programming_devhelp_menuitem = gtk_smart_menu_item_new_all( gui_app_devhelp, icon_filepath, NULL, 0, 0) ;

  g_free(gui_app_devhelp) ;




  gtk_menu_shell_append(GTK_MENU_SHELL(app_programming_menu), gtk_smart_menu_item_get_menuitem(menuitems->app_programming_diff_menuitem)) ;
  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->app_programming_diff_menuitem), "activate", G_CALLBACK(launch_application), gui_app.diff) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(app_programming_menu), gtk_smart_menu_item_get_menuitem(menuitems->app_programming_debugger_menuitem)) ;
  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->app_programming_debugger_menuitem), "activate", G_CALLBACK(launch_application), gui_app.debugger) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(app_programming_menu), gtk_smart_menu_item_get_menuitem(menuitems->app_programming_python_menuitem)) ;
  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->app_programming_python_menuitem), "activate", G_CALLBACK(launch_application), gui_app.python) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(app_programming_menu), gtk_smart_menu_item_get_menuitem(menuitems->app_programming_gui_designer_menuitem)) ;
  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->app_programming_gui_designer_menuitem), "activate", G_CALLBACK(launch_application), gui_app.gui_designer) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(app_programming_menu), gtk_smart_menu_item_get_menuitem(menuitems->app_programming_devhelp_menuitem)) ;
  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->app_programming_devhelp_menuitem), "activate", G_CALLBACK(launch_application), gui_app.devhelp) ;






  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), gtk_smart_menu_item_get_menuitem(app_menubutton)) ;

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(app_menubutton)), app_main_menu) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(app_main_menu), gtk_smart_menu_item_get_menuitem(app_programming_menuitem)) ;

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(app_programming_menuitem)), app_programming_menu) ;




  icon_filepath = get_icon_path("system-software-update.png", icon_filepath) ;

  GtkWidget *app_utils_menuitem = gtk_smart_menu_item_new_all( _("  Utilities  "), icon_filepath, NULL, 0, 0) ;


  GtkWidget *app_utils_menu = gtk_menu_new() ;



  icon_filepath = get_icon_path("accessories-calculator.png", icon_filepath) ;

  gchar *gui_app_calculator = g_strdup_printf("  %s  ", g_path_get_basename(gui_app.calculator)) ;

  menuitems->app_utils_calculator_menuitem = gtk_smart_menu_item_new_all( gui_app_calculator, icon_filepath, NULL, 0, 0) ;

  g_free(gui_app_calculator) ;



  icon_filepath = get_icon_path("kcolorchooser.png", icon_filepath) ;

  gchar *gui_app_color_picker = g_strdup_printf("  %s  ", g_path_get_basename(gui_app.color_picker)) ;

  menuitems->app_utils_color_picker_menuitem = gtk_smart_menu_item_new_all( gui_app_color_picker, icon_filepath, NULL, 0, 0) ;

  g_free(gui_app_color_picker) ;



  icon_filepath = get_icon_path("accessories-dictionary.png", icon_filepath) ;

  gchar *app_app_dictionary = g_strdup_printf("  %s  ", g_path_get_basename(gui_app.dictionary)) ;

  menuitems->app_utils_dictionary_menuitem = gtk_smart_menu_item_new_all( app_app_dictionary, icon_filepath, NULL, 0, 0) ;

  g_free(app_app_dictionary) ;



  icon_filepath = get_icon_path("system-file-manager.png", icon_filepath) ;

  gchar *gui_app_file_manager = g_strdup_printf("  %s  ", g_path_get_basename(gui_app.file_manager)) ;

  menuitems->app_utils_file_manager_menuitem = gtk_smart_menu_item_new_all( gui_app_file_manager, icon_filepath, NULL, 0, 0) ;

  g_free(gui_app_file_manager) ;



  icon_filepath = get_icon_path("knotes.png", icon_filepath) ;

  gchar *gui_app_notes = g_strdup_printf("  %s  ", g_path_get_basename(gui_app.notes)) ;

  menuitems->app_utils_notes_menuitem = gtk_smart_menu_item_new_all( gui_app_notes, icon_filepath, NULL, 0, 0) ;

  g_free(gui_app_notes) ;



  icon_filepath = get_icon_path("internet-web-browser.png", icon_filepath) ;

  gchar *gui_app_browser = g_strdup_printf("  %s  ", g_path_get_basename(gui_app.browser) ) ;

  menuitems->app_utils_browser_menuitem = gtk_smart_menu_item_new_all( gui_app_browser, icon_filepath, NULL, 0, 0) ;

  g_free(gui_app_browser) ;


  gtk_menu_shell_append(GTK_MENU_SHELL(app_utils_menu), gtk_smart_menu_item_get_menuitem(menuitems->app_utils_calculator_menuitem)) ;
  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->app_utils_calculator_menuitem), "activate", G_CALLBACK(launch_application), gui_app.calculator) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(app_utils_menu), gtk_smart_menu_item_get_menuitem(menuitems->app_utils_color_picker_menuitem)) ;
  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->app_utils_color_picker_menuitem), "activate", G_CALLBACK(launch_application), gui_app.color_picker) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(app_utils_menu), gtk_smart_menu_item_get_menuitem(menuitems->app_utils_dictionary_menuitem)) ;
  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->app_utils_dictionary_menuitem), "activate", G_CALLBACK(launch_application), gui_app.dictionary) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(app_utils_menu), gtk_smart_menu_item_get_menuitem(menuitems->app_utils_file_manager_menuitem)) ;
  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->app_utils_file_manager_menuitem), "activate", G_CALLBACK(launch_application), gui_app.file_manager) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(app_utils_menu), gtk_smart_menu_item_get_menuitem(menuitems->app_utils_notes_menuitem)) ;
  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->app_utils_notes_menuitem), "activate", G_CALLBACK(launch_application), gui_app.notes) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(app_utils_menu), gtk_smart_menu_item_get_menuitem(menuitems->app_utils_browser_menuitem)) ;
  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->app_utils_browser_menuitem), "activate", G_CALLBACK(launch_application), gui_app.browser) ;




  gtk_menu_shell_append(GTK_MENU_SHELL(app_main_menu), gtk_smart_menu_item_get_menuitem(app_utils_menuitem)) ;

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(app_utils_menuitem)), app_utils_menu) ;


  icon_filepath = get_icon_path("system-software-update.png", icon_filepath) ;

  menuitems->app_others_menuitem = gtk_smart_menu_item_new_all( _("  Others  "), icon_filepath, NULL, 0, 0) ;


  menus->app_others_menu = gtk_menu_new() ;

  guint16 c=0 ;

  for ( ; c < gui_app.nb_of_others ; ++c) {

    if (gui_app.others[c] != NULL) {

      gchar *menu_item_string =  g_strdup_printf("  %s  ", g_path_get_basename(gui_app.others[c])) ;

      icon_filepath = get_icon_path("applications-system.png", icon_filepath) ;

      GtkWidget *menu_item = gtk_smart_menu_item_new_all( menu_item_string, icon_filepath, NULL, 0, 0) ;

      g_free(menu_item_string) ;

      gtk_menu_shell_append(GTK_MENU_SHELL(menus->app_others_menu), gtk_smart_menu_item_get_menuitem(menu_item)) ;

      g_signal_connect(gtk_smart_menu_item_get_menuitem(menu_item), "activate", G_CALLBACK(launch_application), gui_app.others[c]) ;

    }

  }


  gtk_menu_shell_append(GTK_MENU_SHELL(app_main_menu), gtk_smart_menu_item_get_menuitem(menuitems->app_others_menuitem)) ;

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(menuitems->app_others_menuitem)), menus->app_others_menu) ;






  icon_filepath = get_icon_path("preferences-desktop-display-color.png", icon_filepath) ;

  GtkWidget *view_menubutton = gtk_smart_menu_item_new_all( _("  View  "), icon_filepath, NULL, 0, 0) ;


  icon_filepath = get_icon_path("applications-utilities.png", icon_filepath) ;

  menuitems->big_term_menuitem  = gtk_smart_check_menu_item_new_all( _("  Big terminal(s) show  "), TRUE, icon_filepath, accels_group->menu_view_accel_group, GDK_CONTROL_MASK, GDK_KEY_b) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->big_term_menuitem), "activate", G_CALLBACK(toggle_big_terminal_main_interface_menuitem), NULL) ;




  icon_filepath = get_icon_path("utilities-terminal.png", icon_filepath) ;

  menuitems->show_term_menuitem  = gtk_smart_check_menu_item_new_all( _("  Sidebar terminals show  "), TRUE, icon_filepath, accels_group->menu_view_accel_group, GDK_CONTROL_MASK, GDK_KEY_t) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(menuitems->show_term_menuitem), "activate", G_CALLBACK(toggle_display_sidebar_terminals_menuitem), NULL) ;



  icon_filepath = get_icon_path("configure-toolbars.png", icon_filepath) ;

  menuitems->display_buttonbar_menuitem = gtk_smart_check_menu_item_new_all( _("  Button bar show  "), TRUE, icon_filepath,  NULL, 0, 0) ;

  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(menuitems->display_buttonbar_menuitem)), settings.buttonbar_on) ;



  icon_filepath = get_icon_path("view-fullscreen.png", icon_filepath) ;

  menuitems->toggle_fullscreen_menuitem = gtk_smart_check_menu_item_new_all( _("  Full screen  "), TRUE, icon_filepath,  NULL, 0, 0) ;

  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(menuitems->toggle_fullscreen_menuitem)), settings.fullscreen) ;


  GtkWidget *menu_view = gtk_menu_new() ;


  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), gtk_smart_menu_item_get_menuitem(view_menubutton)) ;

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(view_menubutton)),menu_view);

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_view), gtk_smart_menu_item_get_menuitem(menuitems->big_term_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_view), gtk_smart_menu_item_get_menuitem(menuitems->show_term_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_view), gtk_separator_menu_item_new()) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_view), gtk_smart_menu_item_get_menuitem(menuitems->display_buttonbar_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_view), gtk_smart_menu_item_get_menuitem(menuitems->toggle_fullscreen_menuitem)) ;








  icon_filepath = get_icon_path("preferences-other.png", icon_filepath) ;

  GtkWidget *settings_menubutton = gtk_smart_menu_item_new_all( _("  Settings  "), icon_filepath, NULL, 0, 0) ;



  icon_filepath = get_icon_path("input-keyboard.png", icon_filepath) ;

  GtkWidget *syntax_highlight_menuitem = gtk_smart_menu_item_new_all( _("  Syntax highlight  "), icon_filepath, NULL, 0, 0) ;



  GtkWidget *menu_settings = gtk_menu_new() ;

  menus->menu_syntax   = gtk_menu_new() ;


  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), gtk_smart_menu_item_get_menuitem(settings_menubutton)) ;

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(settings_menubutton)),menu_settings);

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_settings), gtk_smart_menu_item_get_menuitem(syntax_highlight_menuitem)) ;

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(syntax_highlight_menuitem)),menus->menu_syntax);

  //g_object_set(G_OBJECT(menu_syntax),"monitor", 11, NULL) ;


  construct_language_menu_items(menus->menu_syntax) ;



  icon_filepath = get_icon_path("select-scheme.png", icon_filepath) ;

  GtkWidget *syntax_scheme_menuitem = gtk_smart_menu_item_new_all( _("  Editor schemes  "), icon_filepath, NULL, 0, 0) ;



  menus->menu_scheme   = gtk_menu_new() ;



  gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(settings_menubutton)), menu_settings);

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_settings), gtk_smart_menu_item_get_menuitem(syntax_scheme_menuitem)) ;

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(syntax_scheme_menuitem)), menus->menu_scheme);


  construct_scheme_menu_items(menus->menu_scheme) ;


  #ifdef GSPELL_SUPPORT

  icon_filepath = get_icon_path("preferences-desktop-locale.png", icon_filepath) ;

  GtkWidget *config_lang_spellcheck_menuitem = gtk_smart_menu_item_new_all( _("  Configure spell check language  "), icon_filepath, NULL, 0, 0) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(config_lang_spellcheck_menuitem), "activate", G_CALLBACK(select_spellcheck_language), NULL) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_settings), gtk_smart_menu_item_get_menuitem(config_lang_spellcheck_menuitem)) ;
  #endif


  icon_filepath = get_icon_path("preferences-desktop.png", icon_filepath) ;

  GtkWidget *program_setting_menuitem = gtk_smart_menu_item_new_all( _("  Configure program  "), icon_filepath, NULL, 0, 0) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(program_setting_menuitem), "activate", G_CALLBACK(display_configuration_dialog), NULL) ;

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_settings), gtk_smart_menu_item_get_menuitem(program_setting_menuitem)) ;









  GtkWidget *menu_about   = gtk_menu_new() ;


  icon_filepath = get_icon_path("system-help.png", icon_filepath) ;

  GtkWidget *about_menubutton = gtk_smart_menu_item_new_all( _("  About  "), icon_filepath, NULL, 0, 0) ;



  icon_filepath = get_icon_path("help-contents.png", icon_filepath) ;

  GtkWidget *readme_menuitem = gtk_smart_menu_item_new_all( _("  Notice  "), icon_filepath, NULL, 0, 0) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(readme_menuitem), "activate", G_CALLBACK(launch_readme_html), NULL) ;


  icon_filepath = get_icon_path("im-user-away.png", icon_filepath) ;

  GtkWidget *user_salut_menuitem = gtk_smart_menu_item_new_all( _("  Uptime  "), icon_filepath, NULL, 0, 0) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(user_salut_menuitem), "activate", G_CALLBACK(user_salutation), NULL) ;



  icon_filepath = get_icon_path("document-edit-sign.png", icon_filepath) ;

  GtkWidget *license_menuitem = gtk_smart_menu_item_new_all( _("  License  "), icon_filepath, NULL, 0, 0) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(license_menuitem), "activate", G_CALLBACK(launch_license_html), NULL) ;


  icon_filepath = get_icon_path("help-about.png", icon_filepath) ;

  GtkWidget *about_menuitem = gtk_smart_menu_item_new_all( _("  About  "), icon_filepath, NULL, 0, 0) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(about_menuitem), "activate", G_CALLBACK(display_about_dialog), NULL) ;


  g_free(icon_filepath) ;


  gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), gtk_smart_menu_item_get_menuitem(about_menubutton)) ;

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(gtk_smart_menu_item_get_menuitem(about_menubutton)),menu_about);

  gtk_menu_shell_append(GTK_MENU_SHELL(menu_about), gtk_smart_menu_item_get_menuitem(user_salut_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_about), gtk_smart_menu_item_get_menuitem(readme_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_about), gtk_smart_menu_item_get_menuitem(license_menuitem)) ;
  gtk_menu_shell_append(GTK_MENU_SHELL(menu_about), gtk_smart_menu_item_get_menuitem(about_menuitem)) ;


  gtk_widget_show_all(menu_bar) ;

  return ;

}

static void construct_recent_menu_items(GtkWidget *menu_recent) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  gtk_recent_chooser_set_show_icons(GTK_RECENT_CHOOSER(menu_recent), TRUE);

  gtk_recent_chooser_set_show_not_found(GTK_RECENT_CHOOSER(menu_recent), FALSE) ;

  gtk_recent_chooser_set_limit(GTK_RECENT_CHOOSER(menu_recent), 20) ;

  gtk_recent_chooser_set_show_tips(GTK_RECENT_CHOOSER(menu_recent), TRUE);

  GtkRecentFilter *recent_chooser_filter = gtk_recent_filter_new() ;

  const gchar * const * lang_ids = gtk_source_language_manager_get_language_ids(source_language_manager) ;

  gint c=0 ;

  while (lang_ids[c] != NULL) {

     GtkSourceLanguage *source_language = gtk_source_language_manager_get_language(source_language_manager, lang_ids[c]) ;


     if ( ( source_language == NULL) ||  ((g_strcmp0("HTML", gtk_source_language_get_name(source_language)) == 0) || (g_strcmp0("XML", gtk_source_language_get_name(source_language)) == 0)) ) {

        /** Per default HTML and XML file are ignore to be default files for it-edit.
          * So we don't want them into the recent file list.
          * Sorry for the few hand web developers.
        **/

        ++c ;

        continue ;


     }


     gchar **mimetypes = gtk_source_language_get_mime_types(source_language) ;

     if (mimetypes != NULL) {

       int cc=0 ;
       while (mimetypes[cc] != NULL) {

         gtk_recent_filter_add_mime_type(GTK_RECENT_FILTER(recent_chooser_filter), mimetypes[cc]) ;




         ++cc ;
       }

     }

     g_strfreev(mimetypes) ;



     gchar **globs = gtk_source_language_get_globs(source_language) ;

     if (globs != NULL) {

       int cc=0 ;

       while (globs[cc] != NULL) {

         if ( g_strcmp0(globs[cc], "") !=  0) {

          gtk_recent_filter_add_pattern(GTK_RECENT_FILTER(recent_chooser_filter), globs[cc]);

         }

         ++cc ;
       }

     }

     g_strfreev(globs) ;


     ++c ;
  }

  /** This append HTML and XML file to the recent file list... Sorry for the few hand web developers and for other text/plain files users.
    * gtk_recent_filter_add_mime_type(GTK_RECENT_FILTER(recent_chooser_filter), "text/plain") ;
  **/

  gtk_recent_chooser_set_filter(GTK_RECENT_CHOOSER(menu_recent),GTK_RECENT_FILTER(recent_chooser_filter));

  gtk_recent_chooser_set_sort_type(GTK_RECENT_CHOOSER(menu_recent), GTK_RECENT_SORT_MRU);

  g_signal_connect(G_OBJECT(menu_recent), "item-activated", G_CALLBACK(open_recent), NULL) ;

}

static void construct_language_menu_items(GtkWidget *menu_syntax) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  guint c=0 ;

  gchar *lang_id = (gchar *) gtk_source_language_manager_get_language_ids(source_language_manager)[c] ;

  GSList *syntax_radiogroup = NULL ;

  GtkWidget *lang ;

  int col = 0 ;
  int row = 0 ;

  if (lang_id) {
    lang = gtk_radio_menu_item_new_with_label(syntax_radiogroup, lang_id) ;

    syntax_radiogroup = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(lang));

    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(lang), FALSE);

    g_signal_connect(G_OBJECT(lang), "button-press-event", G_CALLBACK(set_syntax_highlight), lang_id) ;

    g_object_set_data(G_OBJECT(lang), "lang_id", lang_id) ;

    gtk_menu_attach (GTK_MENU(menu_syntax),lang, row, row+1, col, col+1);


    while (1) {
      c++ ;
      col++ ;

      if (col == 22) {
        col=0 ;
        row++ ;
      }

      lang_id = (char *) gtk_source_language_manager_get_language_ids(source_language_manager)[c] ;

      if (lang_id == NULL) {
        break ;
      }

      lang = gtk_radio_menu_item_new_with_label(syntax_radiogroup, lang_id) ;
      syntax_radiogroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM(lang));
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(lang), FALSE);

      g_signal_connect(G_OBJECT(lang), "button-press-event", G_CALLBACK(set_syntax_highlight), lang_id) ;
      g_object_set_data(G_OBJECT(lang), "lang_id", lang_id) ;

      gtk_menu_attach (GTK_MENU(menu_syntax),lang, row, row+1, col, col+1);
    }
  }

  lang = gtk_radio_menu_item_new_with_label(syntax_radiogroup, "text") ;

  syntax_radiogroup = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM(lang));

  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(lang), TRUE);

  g_signal_connect(G_OBJECT(lang), "button-press-event", G_CALLBACK(set_syntax_highlight), lang_id) ;
  g_object_set_data(G_OBJECT(lang), "lang_id", "text") ;

  gtk_menu_attach (GTK_MENU(menu_syntax),lang, row, row+1, col, col+1);

  return ;

}

static void construct_scheme_menu_items(GtkWidget *menu_scheme) {

  /** NOTE:
      *
      * The different schemes files are not they provide by the package manager
      * but provide from the program sources because they are more schemes available in the last version from gtksourceview-3.0 and on the web
      * as they provide from the current version on my system.
      * So if in any time their will be more schemes available in futures versions from gtksourceview-3.0 the programm will not provide them.
      * I will maybe fix it in the future.
      ***************************************************************************************************************************************/


  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  gtk_source_style_scheme_manager_append_search_path(source_style_language_manager, PATH_TO_SCHEME);

  gtk_source_style_scheme_manager_force_rescan(source_style_language_manager) ;

  #if 0 /** Change the 0 to 1 to get others schemes. **/
  const gchar * const *schemes_string =  gtk_source_style_scheme_manager_get_scheme_ids(source_style_language_manager) ;
  #else
  /** Set of schemes wich all works into light or dark theme for the searching highlight. **/
  const char *schemes_string[] = { "builder", "builder-dark", "classic", "cobalt", "dark", "kate", "matrix", "oblivion", "slate", "solarized-dark", "solarized-light", "tango",  "vsdark", NULL } ;
  #endif

  int c=0 ;

  char *scheme = (char *) schemes_string[0] ;

  GtkWidget *widget = NULL ;

  while (scheme != NULL) {

    if ( c == 0) {

      widget = gtk_radio_menu_item_new_with_label(NULL, scheme) ;
    }
    else {

      widget = gtk_radio_menu_item_new_with_label_from_widget(GTK_RADIO_MENU_ITEM(widget), scheme) ;
    }

    g_signal_connect(G_OBJECT(widget), "activate", G_CALLBACK(set_scheme_highlight), scheme) ;

    gtk_menu_shell_append(GTK_MENU_SHELL(menu_scheme), widget) ;

    g_object_set_data(G_OBJECT(widget), "scheme_id", scheme) ;

    if (g_strcmp0(scheme, settings.scheme) == 0) {
       gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), TRUE) ;
    }
    else {
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), FALSE) ;
    }

    c++ ;

    scheme = (char *) schemes_string[c] ;

  }

}

void setup_search_and_replace_bar(Search_And_Replace *search_and_replace) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  search_and_replace->search_and_replace_main_vbox  = gtk_box_new(GTK_ORIENTATION_VERTICAL,   0) ;

  search_and_replace->search_and_replace_main_hbox  = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;


  search_and_replace->search_and_replace_label_vbox    = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  search_and_replace->search_and_replace_entry_vbox    = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  search_and_replace->search_and_replace_button_vbox_1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  search_and_replace->search_and_replace_button_vbox_2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;


  gtk_box_set_homogeneous(GTK_BOX(search_and_replace->search_and_replace_label_vbox), TRUE) ;

  gtk_box_set_homogeneous(GTK_BOX(search_and_replace->search_and_replace_entry_vbox), TRUE) ;

  gtk_box_set_homogeneous(GTK_BOX(search_and_replace->search_and_replace_button_vbox_1), TRUE) ;

  gtk_box_set_homogeneous(GTK_BOX(search_and_replace->search_and_replace_button_vbox_2), TRUE) ;



  search_and_replace->search_button = gtk_button_new_with_label( _(" Search ") ) ;

  search_and_replace->search_image  = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "system-search.png") ;

  gtk_button_set_image(GTK_BUTTON(search_and_replace->search_button), search_and_replace->search_image);

  g_object_set(G_OBJECT(search_and_replace->search_button),"always-show-image",TRUE,NULL) ;

  gtk_widget_set_tooltip_text(search_and_replace->search_button, "Highlight all matching occurrence(s)") ;

  g_signal_connect(G_OBJECT(search_and_replace->search_button), "clicked", G_CALLBACK(search), NULL ) ;



  search_and_replace->replace_button = gtk_button_new_with_label( _(" Replace") ) ;

  search_and_replace->replace_image  = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "edit-find-replace.png") ;

  gtk_button_set_image(GTK_BUTTON(search_and_replace->replace_button), search_and_replace->replace_image);

  g_object_set(G_OBJECT(search_and_replace->replace_button),"always-show-image",TRUE,NULL) ;

  g_signal_connect(G_OBJECT(search_and_replace->replace_button),"clicked", G_CALLBACK(replace_one_occurrence), NULL) ;


  search_and_replace->search_entry   = gtk_search_entry_new() ;

  g_signal_connect(G_OBJECT(search_and_replace->search_entry), "key-press-event", G_CALLBACK(search_history_callback), NULL) ;

  g_signal_connect(G_OBJECT(search_and_replace->search_entry), "activate",        G_CALLBACK(search_next), NULL) ;




  search_and_replace->replace_entry  = gtk_entry_new() ;

  search_and_replace->search_next_button  = gtk_button_new_with_label( _("Next") ) ;

  search_and_replace->search_next_image   = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "go-down-search.png") ;

  gtk_button_set_image(GTK_BUTTON(search_and_replace->search_next_button), search_and_replace->search_next_image);

  gtk_widget_set_tooltip_markup(search_and_replace->search_next_button, "Highlight <b>Next</b> matching occurrence(s) -> [ <b>Ctrl ++</b> (KP) ]") ;

  g_object_set(G_OBJECT(search_and_replace->search_next_button),"always-show-image",TRUE,NULL) ;

  g_signal_connect(G_OBJECT(search_and_replace->search_next_button),"clicked", G_CALLBACK(search_next), NULL) ;


  search_and_replace->search_prev_button  = gtk_button_new_with_label( _("Previous") ) ;

  search_and_replace->search_next_image   = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "go-up-search.png") ;

  gtk_button_set_image(GTK_BUTTON(search_and_replace->search_prev_button), search_and_replace->search_next_image);

  gtk_widget_set_tooltip_markup(search_and_replace->search_prev_button, "Highlight <b>Previous</b> matching occurrence(s) -> [ <b>Ctrl +-</b> (KP) ]") ;

  g_object_set(G_OBJECT(search_and_replace->search_prev_button),"always-show-image",TRUE,NULL) ;

  g_signal_connect(G_OBJECT(search_and_replace->search_prev_button), "clicked", G_CALLBACK(search_previous), NULL) ;


  search_and_replace->replace_one_button      = gtk_button_new_with_label( _("Replace") ) ;

  gtk_widget_set_tooltip_markup(search_and_replace->replace_one_button, "<b>Replace</b> current highlight occurrence with replacement term -> [ <b>Ctrl + Enter</b> (KP) ]") ;

  g_signal_connect(G_OBJECT(search_and_replace->replace_one_button),"clicked", G_CALLBACK(replace_one_occurrence), NULL) ;


  search_and_replace->replace_all_button      = gtk_button_new_with_label( _("Replace all") ) ;

  gtk_widget_set_tooltip_markup(search_and_replace->replace_all_button, "<b>Replace all</b> occurrence(s) with replacement term -> [ <b>Ctrl + Shift + Enter</b> (KP) ]") ;

  g_signal_connect(G_OBJECT(search_and_replace->replace_all_button), "clicked", G_CALLBACK(replace_all_occurrence), NULL) ;




  search_and_replace->search_settings_image   = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "preferences-system.png") ;

  search_and_replace->search_settings_button  = gtk_button_new_with_label( _(" Mode    ") ) ;

  g_object_set(G_OBJECT(search_and_replace->search_settings_button),"always-show-image",TRUE,NULL) ;

  gtk_widget_set_tooltip_markup(search_and_replace->search_settings_button, "Searching modes: <b>Raw</b>, <b>Word boundary</b>, <b>REGEX</b> (<i>GLib (Perl compatible) regular expression(s)</i>)\nSee GLib documentation for further informations.") ;

  gtk_button_set_image(GTK_BUTTON(search_and_replace->search_settings_button), search_and_replace->search_settings_image);


  search_and_replace->search_settings_hbox     = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  search_and_replace->search_settings_combobox = gtk_combo_box_text_new_with_entry() ;

  gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(search_and_replace->search_settings_combobox),"1", _("Raw text") ) ;
  gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(search_and_replace->search_settings_combobox),"2", _("Word boundary") ) ;
  gtk_combo_box_text_append (GTK_COMBO_BOX_TEXT(search_and_replace->search_settings_combobox),"3", _("Regular expression") ) ;

  gtk_combo_box_set_active (GTK_COMBO_BOX(search_and_replace->search_settings_combobox), 0) ;

  gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(search_and_replace->search_settings_combobox), GTK_SENSITIVITY_AUTO );

  g_signal_connect(G_OBJECT(search_and_replace->search_settings_combobox),"changed",G_CALLBACK(set_search_mode), NULL) ;

  GtkWidget *combobox_entry = gtk_bin_get_child(GTK_BIN(search_and_replace->search_settings_combobox)) ;

  g_object_set(G_OBJECT(combobox_entry),"editable", FALSE, NULL) ;


  search_and_replace->search_settings_padding_1 = gtk_label_new("") ;

  search_and_replace->search_settings_case_checkbox = gtk_check_button_new_with_label( _("Case-sensitive") )  ;

  g_signal_connect(G_OBJECT(search_and_replace->search_settings_case_checkbox),"toggled", G_CALLBACK(set_case_sensitive), NULL) ;

  search_and_replace->search_settings_wrap_checkbox = gtk_check_button_new_with_label( _("Wrap around") ) ;

  g_signal_connect(G_OBJECT(search_and_replace->search_settings_wrap_checkbox),"toggled", G_CALLBACK(set_wrap_around), NULL) ;

  g_object_set(G_OBJECT(search_and_replace->search_settings_case_checkbox), "margin-end", 8,    NULL) ;
  g_object_set(G_OBJECT(search_and_replace->search_settings_wrap_checkbox), "margin-end", 8,    NULL) ;

  g_object_set(G_OBJECT(search_and_replace->search_settings_case_checkbox), "margin-start", 8,  NULL) ;
  g_object_set(G_OBJECT(search_and_replace->search_settings_wrap_checkbox), "margin-start", 8,  NULL) ;



  search_and_replace->search_settings_padding_2 = gtk_label_new("") ;

  gtk_box_pack_start(GTK_BOX(search_and_replace->search_settings_hbox), search_and_replace->search_settings_combobox,       FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(search_and_replace->search_settings_hbox), search_and_replace->search_settings_padding_1,      TRUE,  TRUE,  0) ;
  gtk_box_pack_start(GTK_BOX(search_and_replace->search_settings_hbox), search_and_replace->search_settings_case_checkbox,  FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(search_and_replace->search_settings_hbox), search_and_replace->search_settings_wrap_checkbox,  FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(search_and_replace->search_settings_hbox), search_and_replace->search_settings_padding_2,      TRUE,  TRUE, 0) ;

  search_and_replace->search_settings_match_label         = gtk_label_new("") ;

  search_and_replace->search_settings_close_image         = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "window-close.png") ;

  search_and_replace->search_settings_close_button        = gtk_button_new_with_label( _(" Close    ") ) ;

  g_object_set(G_OBJECT(search_and_replace->search_settings_close_button),"always-show-image",TRUE,NULL) ;

  gtk_button_set_image(GTK_BUTTON(search_and_replace->search_settings_close_button), search_and_replace->search_settings_close_image);

  g_signal_connect(G_OBJECT(search_and_replace->search_settings_close_button),"clicked",G_CALLBACK(close_search_and_replace_bar), NULL) ;


  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_label_vbox), search_and_replace->search_button,          FALSE, FALSE, 0 ) ;
  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_label_vbox), search_and_replace->replace_button,         FALSE, FALSE, 0 ) ;
  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_label_vbox), search_and_replace->search_settings_button, FALSE, FALSE, 0 ) ;

  gtk_box_set_spacing(GTK_BOX(search_and_replace->search_and_replace_label_vbox), 4) ;

  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_entry_vbox), search_and_replace->search_entry,         FALSE, FALSE, 0 ) ;
  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_entry_vbox), search_and_replace->replace_entry,        FALSE, FALSE, 0 ) ;
  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_entry_vbox), search_and_replace->search_settings_hbox, FALSE, FALSE, 0 ) ;

  gtk_box_set_spacing(GTK_BOX(search_and_replace->search_and_replace_entry_vbox), 4) ;

  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_button_vbox_1), search_and_replace->search_next_button,           FALSE, FALSE, 0 ) ;
  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_button_vbox_1), search_and_replace->replace_one_button,           FALSE, FALSE, 0 ) ;
  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_button_vbox_1), search_and_replace->search_settings_match_label,  FALSE, FALSE, 0 ) ;

  gtk_box_set_spacing(GTK_BOX(search_and_replace->search_and_replace_button_vbox_1), 4) ;

  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_button_vbox_2), search_and_replace->search_prev_button,            FALSE, FALSE, 0 ) ;
  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_button_vbox_2), search_and_replace->replace_all_button,            FALSE, FALSE, 0 ) ;
  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_button_vbox_2), search_and_replace->search_settings_close_button,  FALSE, FALSE, 0 ) ;

  gtk_box_set_spacing(GTK_BOX(search_and_replace->search_and_replace_button_vbox_2), 4) ;

  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_main_hbox), search_and_replace->search_and_replace_label_vbox,     FALSE, FALSE, 0 ) ;
  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_main_hbox), search_and_replace->search_and_replace_entry_vbox,     TRUE, TRUE, 0 ) ;
  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_main_hbox), search_and_replace->search_and_replace_button_vbox_1,  FALSE, FALSE, 0 ) ;
  gtk_box_pack_start(GTK_BOX(search_and_replace->search_and_replace_main_hbox), search_and_replace->search_and_replace_button_vbox_2,  FALSE, FALSE, 0 ) ;

  return ;

}

void initialize_button_box(Buttons *buttons) {
  /** Initialize the button box. **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif



  buttons->open_file_button = gtk_smart_icon_button_new_all(PATH_TO_BUTTON_ICON "document-open.png", _("Open a file"), GDK_KEY_o, GDK_CONTROL_MASK) ;

  g_signal_connect(G_OBJECT(buttons->open_file_button),"clicked", G_CALLBACK(open_file),NULL) ;



  buttons->save_file_button = gtk_smart_icon_button_new_all(PATH_TO_BUTTON_ICON "document-save.png", _("Save current editor content to current file"), GDK_KEY_s, GDK_CONTROL_MASK) ;

  g_signal_connect(G_OBJECT(buttons->save_file_button),"clicked",G_CALLBACK(save_file),NULL) ;



  buttons->save_file_as_button = gtk_smart_icon_button_new_all(PATH_TO_BUTTON_ICON "document-save-as.png", _("Save current editor content to the selected file"), GDK_KEY_s, GDK_CONTROL_MASK) ;

  g_signal_connect(G_OBJECT(buttons->save_file_as_button),"clicked",G_CALLBACK(save_as_file),NULL) ;




  buttons->save_file_all_button = gtk_smart_icon_button_new_all(PATH_TO_BUTTON_ICON "document-save-all.png", _("Save all files"), GDK_KEY_s, GDK_CONTROL_MASK | GDK_SHIFT_MASK ) ;

  g_signal_connect(G_OBJECT(buttons->save_file_all_button), "clicked", G_CALLBACK(save_all_file), NULL) ;



  #ifdef GSPELL_SUPPORT
  buttons->inline_spellcheck_button = gtk_smart_icon_toggle_button_new_all(PATH_TO_BUTTON_ICON "tools-check-spelling.png", _("Enable/Disable inline spellcheck"), GDK_KEY_w, GDK_CONTROL_MASK) ;

  g_signal_connect(G_OBJECT(buttons->inline_spellcheck_button), "clicked", G_CALLBACK(inline_spellcheck_button), NULL) ;

  #endif

  buttons->tabs_button = gtk_smart_icon_button_new_all(settings.use_spaces_as_tabs ? PATH_TO_BUTTON_ICON "tabs-off.png"  : PATH_TO_BUTTON_ICON "tabs-on.png", _("Use space instead of tabs"), GDK_KEY_p, GDK_CONTROL_MASK) ;

  g_signal_connect(G_OBJECT(buttons->tabs_button), "clicked", G_CALLBACK(toggle_use_space_instead_of_tabs), NULL) ;


  buttons->cut_button = gtk_smart_icon_button_new_all(PATH_TO_BUTTON_ICON "edit-cut.png", _("Cut selected text and copy it to clipboard"), GDK_KEY_x, GDK_CONTROL_MASK ) ;

  g_signal_connect(G_OBJECT(buttons->cut_button), "clicked", G_CALLBACK(cut), NULL) ;


  buttons->copy_button = gtk_smart_icon_button_new_all(PATH_TO_BUTTON_ICON "edit-copy.png", _("Copy selected to clipboard"), GDK_KEY_c, GDK_CONTROL_MASK) ;

  g_signal_connect(G_OBJECT(buttons->copy_button), "clicked", G_CALLBACK(copy), NULL) ;



  buttons->paste_button = gtk_smart_icon_button_new_all(PATH_TO_BUTTON_ICON "edit-paste.png", _("Paste clipboard content at cursor position in the current editor"), GDK_KEY_v, GDK_CONTROL_MASK) ;

  g_signal_connect(G_OBJECT(buttons->paste_button), "clicked", G_CALLBACK(paste), NULL) ;



  buttons->duplicate_button = gtk_smart_icon_button_new_all(PATH_TO_BUTTON_ICON "distribute-vertical-bottom.png", _("Duplicate the selected text or the line at current cursor position"), GDK_KEY_d, GDK_CONTROL_MASK) ;

  g_signal_connect(G_OBJECT(buttons->duplicate_button),"clicked",G_CALLBACK(duplicate),NULL) ;


  #ifdef GSPELL_SUPPORT
  buttons->spellcheck_button = gtk_smart_icon_button_new_all(PATH_TO_BUTTON_ICON "tools-check-spellcheck-dialog.png", _("Perform a spellcheck on the current file"), GDK_KEY_w, GDK_CONTROL_MASK | GDK_SHIFT_MASK ) ;

  g_signal_connect(G_OBJECT(buttons->spellcheck_button), "clicked", G_CALLBACK(spellcheck_dialog), NULL) ;

  #endif

  buttons->go_to_line_button = gtk_smart_icon_button_new_all(PATH_TO_BUTTON_ICON "edit-text-frame-update.png", _("Go to the given line number"), GDK_KEY_g, GDK_CONTROL_MASK) ;

  buttons->go_to_line_image  = gtk_image_new_from_file(PATH_TO_BUTTON_ICON "edit-text-frame-update.png") ;

  g_signal_connect(G_OBJECT(buttons->go_to_line_button),"clicked",G_CALLBACK(display_go_to_line_dialog),NULL) ;



  buttons->search_and_replace_button = gtk_smart_icon_toggle_button_new_all(PATH_TO_BUTTON_ICON "system-search.png", _("Show | Hide the search &amp; replace bar"), GDK_KEY_f, GDK_CONTROL_MASK) ;

  g_signal_connect(G_OBJECT(buttons->search_and_replace_button),"toggled",G_CALLBACK(toggle_display_search_and_replace_bar),NULL) ;


  buttons->replace_all_in_all_files  = gtk_smart_icon_button_new_all(PATH_TO_BUTTON_ICON "edit-find-mail.png", _("Replace all occurrences in all files"), GDK_KEY_r, GDK_CONTROL_MASK | GDK_SHIFT_MASK) ;

  g_signal_connect(G_OBJECT(buttons->replace_all_in_all_files),"clicked",G_CALLBACK(activate_replace_all_in_all_files),NULL) ;


  buttons->exec_button       = gtk_smart_icon_button_new_all(PATH_TO_BUTTON_ICON "system-run.png", _("Run a command in a terminal (per example man [section] page)"), GDK_KEY_e, GDK_CONTROL_MASK) ;

  g_signal_connect(G_OBJECT(buttons->exec_button), "clicked", G_CALLBACK(display_execute_command_dialog), NULL) ;


  buttons->big_terminal_button = gtk_smart_icon_toggle_button_new_all(PATH_TO_BUTTON_ICON "applications-utilities.png", _("Switch between big terminal(s) and main interface"), GDK_KEY_b, GDK_CONTROL_MASK) ;

  g_signal_connect(G_OBJECT(buttons->big_terminal_button),"clicked", G_CALLBACK(toggle_big_terminal_main_interface_toggle_button), NULL) ;



  buttons->terminals_show_button = gtk_smart_icon_toggle_button_new_all(PATH_TO_BUTTON_ICON "utilities-terminal.png", _("Show | Hide the terminals sidebar"), GDK_KEY_t, GDK_CONTROL_MASK) ;

  g_signal_connect(G_OBJECT(buttons->terminals_show_button),"toggled",G_CALLBACK(toggle_display_sidebar_terminals_toggle_button),NULL) ;

  return ;
}

static void copy_current_filepath_to_clipboard(GtkWidget *widget) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  const gint nb_of_pages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) ;

  if (nb_of_pages <= 0) {

    return ;

  }

  gchar *filepath = g_object_get_data(G_OBJECT(current_editor.current_buffer), "filepath") ;

  if (filepath != NULL) {

    copy_filepath_to_clipboard(NULL, g_strdup(filepath)) ;
  }

  return ;

}

static void copy_current_folderpath_to_clipboard(GtkWidget *widget) {

  const gint nb_of_pages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) ;

  if (nb_of_pages <= 0) {

    return ;

  }

  gchar *filepath = g_object_get_data(G_OBJECT(current_editor.current_buffer), "filepath") ;

  if (filepath != NULL) {

    copy_filepath_to_clipboard(NULL, g_path_get_dirname(filepath)) ;
  }

  return ;

}

static void close_the_file(GtkWidget *widget) {

  const gint nb_of_pages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) ;

  if (nb_of_pages <= 0) {

    return ;

  }

  GtkWidget *notebook_page      = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->editor_notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(gui->editor_notebook))) ;

  /** Get the notebook label tab **/
  GtkWidget *notebook_tab = gtk_notebook_get_tab_label(GTK_NOTEBOOK(gui->editor_notebook), notebook_page);

  /** The tab contains an mime-type icon, the filename and the page closing button. **/
  GList *tab_box_list = gtk_container_get_children(GTK_CONTAINER(notebook_tab)) ;

  GtkWidget *tab_button =  g_list_nth_data(tab_box_list, 3) ;

  gint *page_number = (gint *) g_object_get_data(G_OBJECT(tab_button), "page_number") ;

  g_list_free(tab_box_list) ;

  close_file(NULL, page_number) ;

  return ;

}

static gint compare_filenames(gconstpointer  *p1, gconstpointer  *p2, gpointer user_data) {

  return g_strcmp0( ((reorder_pages *) p1)->filename, ((reorder_pages *) p2)->filename) ;

}



static void reorder_notebook_pages(GtkWidget *widget) {

  const gint16 page_nb = gtk_notebook_get_n_pages(GTK_NOTEBOOK(gui->editor_notebook)) ;

  if (page_nb < 2) {

    return ;

  }

  GList *glist_reordering_pages = NULL ;

  gint16 c = 0 ;

  for ( ; c < page_nb ; ++c) {

    GtkWidget *child_widget   = gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->editor_notebook), c) ;

    GtkWidget *textview       = gtk_bin_get_child(GTK_BIN(child_widget)) ;

    GtkTextBuffer *buffer     = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview)) ;


    gchar *filepath     =  g_object_get_data(G_OBJECT(buffer), "filepath") ;

    reorder_pages *page = g_malloc(sizeof(reorder_pages)) ;

    page->filename = g_path_get_basename(filepath) ;

    page->widget = child_widget ;

    glist_reordering_pages = g_list_insert_sorted_with_data(glist_reordering_pages, page, (GCompareDataFunc) compare_filenames, NULL) ;

  }

  c=0 ;

  while (glist_reordering_pages != NULL) {

    if (glist_reordering_pages->data != NULL) {

      gtk_notebook_reorder_child(GTK_NOTEBOOK(gui->editor_notebook), ((reorder_pages *) (glist_reordering_pages->data))->widget, c) ;

      g_free(((reorder_pages *) (glist_reordering_pages->data))->filename) ;

      g_free(glist_reordering_pages->data) ;

      ++c ;

    }

    if (glist_reordering_pages->next != NULL) {

      glist_reordering_pages = glist_reordering_pages->next ;
    }
    else {

      break ;
    }

  }

  g_list_free(glist_reordering_pages) ;

  update_page_number(gui->editor_notebook) ;

  return ;

}









