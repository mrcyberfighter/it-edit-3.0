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

#include "./Headers/includes.h"
#include "./Headers/defines.h"

#include "./main.h"

#include "./Headers/global_vars.h"
#include "./Configuration/configuration.h"
#include "./Callbacks/configuration_callbacks.h"
#include "./Callbacks/edition_callbacks.h"
#include "./Callbacks/editor_callbacks.h"
#include "./Callbacks/files_callbacks.h"
#include "./Callbacks/gui_callbacks.h"
#include "./Callbacks/highlight_selection_callbacks.h"
#include "./Callbacks/search_settings_callbacks.h"
#include "./Callbacks/terminals_callbacks.h"

#include "./Editor/editor.h"

#include "./GUI/dialogs/message_dialog.h"

#include "./GUI/configure/configure_gui_elements.h"

#include "./GUI/configure/configuration_dialog.h"

#include "./GUI/dialogs/dialogs.h"

#include "./GUI/setup/setup_gui.h"

#include "./Types/GtkItTerm.h"

#ifdef DEBUG
  #include "./DEBUG/debug_search_by_signals.h" /** Debugging utilities **/
#endif

/** GtkApplication open callback function: for cmdline file(s) or folder opening. **/
static void open_files(GApplication  *application, GFile **files, gint n_files, const gchar *hint) ;

/** GtkApplication activate callback function. **/
static void activate(GApplication *application) ;

/** GtkApplication startup callback function. **/
static void startup(GApplication *application, gpointer user_data) ;

/** GtkApplication shutdown callback function. **/
static void shutdown_app(GApplication *application, gpointer user_data) ;

#ifdef G_OS_UNIX

/** GtkApplication (application menu) open file callback function. **/
static void open_file_activated(GSimpleAction *action, GVariant *parameter, gpointer app) ;

/** GtkApplication (application menu) preferences callback function. **/
static void preferences_activated(GSimpleAction *action, GVariant *parameter, gpointer app) ;

/** GtkApplication (application menu) About callback function. **/
static void about_activated(GSimpleAction *action, GVariant *parameter, gpointer app) ;

/** GtkApplication (application menu) quit callback function. **/
static void quit_activated(GSimpleAction *action, GVariant *parameter, gpointer app) ;
#endif

static void destroy(GtkWidget *widget ,gpointer pointer) ;

static gboolean delete_event(GtkWidget *widget,GdkEvent *event,gpointer pointer) ;

static void check_applications(char *app_name, GtkWidget *menu_item) ;

static void on_size_allocated(GtkWidget *widget, GdkRectangle *allocation, gpointer user_data) ;

static void at_exit_handler(void) ;

int main(int argc, char *argv[]) {

  #ifdef MINI_MULTIPURPOSE_EDITOR_SUPPORT /** This feature is too much unstable. **/
  todo_settings = g_malloc(sizeof(Todo_Settings)) ;
  #endif


  /** Loading default configuration at first launch and then your configuration. **/
  get_main_configuration() ;

  /** Loading G.U.I applications. **/
  get_app_configuration() ;

  session = g_malloc(sizeof(Session)) ;

  GKeyFile *conf_file = g_key_file_new() ;

  g_key_file_load_from_file(conf_file, PATH_TO_SESSION_FILE, G_KEY_FILE_NONE, NULL) ;

  session->session_files  = g_key_file_get_string_list(conf_file, "Session", "session_files", &session->session_files_nb, NULL) ;
  session->session_doc    = g_key_file_get_string_list(conf_file, "Session", "session_doc",   &session->session_doc_nb,   NULL) ;
  session->session_app    = g_key_file_get_string_list(conf_file, "Session", "session_app",   &session->session_app_nb,   NULL) ;

  g_key_file_unref(conf_file) ;

  #ifdef GETTEXT_SUPPORT

  setlocale(LC_ALL, "");

  bindtextdomain(PACKAGE, LOCALEDIR);
  bind_textdomain_codeset(PACKAGE, "UTF-8") ;
  textdomain(PACKAGE);

  #ifdef INFO
  fprintf(stdout,"PACKAGE location: %s\n", PACKAGE) ;

  fprintf(stdout,"LOCALEDIR location: %s\n", LOCALEDIR) ;
  #endif

  #endif


  #ifdef MAC_INTEGRATION /** The library gtk-mac-integration-gtk3 define this. **/

  gtkosx_application_attention_request(gtkosx_application_get(), CRITICAL_REQUEST);

  gtkosx_application_ready(gtkosx_application_get()) ;

  /** I don't know if it is a good idea to set it on @TRUE, because the radio and check buttons doesn't work with value @TRUE. **/
  gtkosx_application_set_use_quartz_accelerators(gtkosx_application_get(), FALSE) ;

  #ifdef INFO
  fprintf(stdout,"Mac app use quartz accels: %d\n", gtkosx_application_use_quartz_accelerators(gtkosx_application_get()) ;
  #endif

  g_set_prgname("it-edit") ; /** Not automatically set from the GtkApplication. **/

  #endif

  atexit(&at_exit_handler) ;

  g_set_application_name(PRGNAME) ;

  const char *app_id = "it.edit.mrcyberfighter" ; /** @Hint: For using Gtk notification you will have create a *.desktop file named has the id (prgname.org in this case) and getting a dbus connection. ; **/

  if ( ! g_application_id_is_valid(app_id) ) {

    fprintf(stderr, "Wrong app id\n") ;
    exit(EXIT_FAILURE) ;

  }

  int status;

  int app_flags = G_APPLICATION_NON_UNIQUE | G_APPLICATION_SEND_ENVIRONMENT | G_APPLICATION_HANDLES_OPEN ;

  app = gtk_application_new(app_id, app_flags) ;

  g_signal_connect( G_APPLICATION(app),  "activate",              G_CALLBACK(activate),       NULL) ;
  g_signal_connect( G_APPLICATION(app),  "open",                  G_CALLBACK(open_files),     NULL) ;
  g_signal_connect( G_APPLICATION(app),  "startup",               G_CALLBACK(startup),        NULL) ;
  g_signal_connect( G_APPLICATION(app),  "shutdown",              G_CALLBACK(shutdown_app),   NULL) ;

  GError *error = NULL ;

  bool registered = g_application_register(G_APPLICATION(app), NULL, &error) ;

  if (error != NULL || (! registered)) {

    fprintf(stderr,"Cannot register app: %s\n", error->message) ;

    g_error_free(error) ;

    exit(EXIT_FAILURE) ;

  }






  #ifdef G_OS_UNIX

  if (gtk_application_prefers_app_menu(app)) {

    GtkBuilder *builder = NULL  ;
    GMenuModel *app_menu = NULL ;

    static GActionEntry app_entries[] = {

      { "openfile",     open_file_activated,    NULL, NULL, NULL },
      { "preferences",  preferences_activated,  NULL, NULL, NULL },
      { "about",        about_activated,        NULL, NULL, NULL },
      { "quit",         quit_activated,         NULL, NULL, NULL }

    };

    g_action_map_add_action_entries(G_ACTION_MAP(app), app_entries, G_N_ELEMENTS(app_entries), app);

    builder = gtk_builder_new_from_string("<?xml version=\"1.0\"?>"
                                            "<interface>"
                                              "<menu id=\"appmenu\">"
                                                "<section>"
                                                  "<item>"
                                                    "<attribute name=\"label\" translatable=\"no\">_Open file</attribute>"
                                                    "<attribute name=\"action\">app.openfile</attribute>"
                                                  "</item>"
                                               "</section>"
                                               "<section>"
                                                  "<item>"
                                                    "<attribute name=\"label\" translatable=\"no\">Preferences</attribute>"
                                                    "<attribute name=\"action\">app.preferences</attribute>"
                                                  "</item>"
                                                "</section>"
                                                "<section>"
                                                  "<item>"
                                                    "<attribute name=\"label\" translatable=\"no\">About</attribute>"
                                                    "<attribute name=\"action\">app.about</attribute>"
                                                  "</item>"
                                                "</section>"
                                                "<section>"
                                                  "<item>"
                                                    "<attribute name=\"label\" translatable=\"no\">_Quit</attribute>"
                                                    "<attribute name=\"action\">app.quit</attribute>"
                                                 "</item>"
                                               "</section>"
                                              "</menu>"
                                            "</interface>",

                                            -1) ;

    app_menu = G_MENU_MODEL(gtk_builder_get_object(builder, "appmenu"));

    gtk_application_set_app_menu(GTK_APPLICATION(app), app_menu);

    g_object_unref(builder) ;

  }

  #endif

  #ifdef INFO /** Supported source file(s) encoding. **/
  GSList *gslist_encoding = gtk_source_encoding_get_all() ; /** Available text encoding. **/

  gslist_encoding = g_slist_nth(gslist_encoding,0) ;

  while (gslist_encoding != NULL) {

    if (gslist_encoding->data != NULL) {

      fprintf(stdout,"\n%s\n\n", gtk_source_encoding_get_name( (const GtkSourceEncoding *) gslist_encoding->data)) ;
      fprintf(stdout,"* %s\n\n", gtk_source_encoding_get_charset( (const GtkSourceEncoding *) gslist_encoding->data)) ;
      fprintf(stdout,"---\n") ;

    }


    if (gslist_encoding->next != NULL) {

      gslist_encoding = gslist_encoding->next ;
    }
    else {

      break ;
    }
  }
  #endif

  #ifdef INFO /** Supported filetypes: **/
  const gchar * const * lang_ids = gtk_source_language_manager_get_language_ids(gtk_source_language_manager_get_default()) ;

  uint16_t c=0 ;

  while (lang_ids[c] != NULL) {

     fprintf(stdout,"\n") ;

     fprintf(stdout, "%s:\n", gtk_source_language_get_name(gtk_source_language_manager_get_language(gtk_source_language_manager_get_default(), lang_ids[c])) ) ;


     uint16_t cc=0 ;
     gchar **mimetypes = gtk_source_language_get_mime_types(gtk_source_language_manager_get_language(gtk_source_language_manager_get_default(), lang_ids[c])) ;

     if (mimetypes != NULL) {

       fprintf(stdout,"\n") ;

       while (mimetypes[cc] != NULL) {

         if (cc == 0) {

            fprintf(stdout,"  %s",mimetypes[cc] ) ;
         }
         else {
           fprintf(stdout,", %s",mimetypes[cc] ) ;
         }


         ++cc ;
       }

     }

     fprintf(stdout,"\n\n") ;



     cc = 0 ;
     gchar **globs = gtk_source_language_get_globs(gtk_source_language_manager_get_language(gtk_source_language_manager_get_default(), lang_ids[c])) ;

     if (globs != NULL) {

       while (globs[cc] != NULL) {

         fprintf(stdout,"  + \\%s\n\n", globs[cc] ) ;

         ++cc ;
       }

       fprintf(stdout,"---\n") ;

     }

     ++c ;
  }

  #endif


  #ifdef INFO
  #ifdef GSPELL_SUPPORT

  const GList *language_list = gspell_language_get_available() ;  /** Available spell-check languages !" **/

  while ( language_list != NULL) {

    if (language_list->data != NULL) {

      fprintf(stderr,"lang: %s [%s]\n", gspell_language_get_name(language_list->data), gspell_language_get_code(language_list->data)) ;
    }

    if (language_list->next != NULL) {

      language_list = language_list->next ;
    }
    else {

      break ;
    }

 }

 #endif
 #endif



  /** The search history double-linked list
    * which purpose is to navigate into the previous search terms,
    * through the UP and DOWN arrow when the search entry has the focus.
    *********************************************************************/

  extern GList *search_history ;
  extern gboolean search_history_start ;

  search_history = NULL ;

  search_history_start = TRUE ;


  /** Files Handler file managing single-linked list: **/
  files_manager_list =  NULL ;


  /** GUI structure initialization. **/
  GUI    pgui     ;  /** Pointer initialisation corrected by:  **/
  extern GUI *gui ;  /** Mickaël <Mickaël_mail@gmail.com>      **/
  gui = &pgui     ;

  /** main window **/
  gui->main_window = gtk_application_window_new(app)  ;

  gui->editor_notebook = NULL ;


  gtk_window_set_default_icon_from_file(PATH_TO_ICON, NULL) ;
  gtk_window_maximize(GTK_WINDOW(gui->main_window));
  gtk_window_set_title(GTK_WINDOW(gui->main_window), PRGNAME) ;

  g_object_set(G_OBJECT(gui->main_window), "border-width", 8, NULL) ;

  g_signal_connect_after(G_OBJECT(gui->main_window), "delete-event", G_CALLBACK(delete_event), NULL) ;
  g_signal_connect(G_OBJECT(gui->main_window), "destroy",      G_CALLBACK(destroy),      NULL) ;




  source_language_manager = gtk_source_language_manager_get_default() ;

  source_style_language_manager = gtk_source_style_scheme_manager_get_default() ;

  /** Menu. **/
  gui->menus = g_slice_new(Menus) ;

  gui->menuitems = g_slice_new(MenuItems) ;

  gui->accels_group = g_slice_new(AccelGroups) ;

  gui->accels_group->menu_files_accel_group    = gtk_accel_group_new() ;
  gui->accels_group->menu_edition_accel_group  = gtk_accel_group_new() ;
  gui->accels_group->menu_action_accel_group   = gtk_accel_group_new() ;
  gui->accels_group->menu_view_accel_group     = gtk_accel_group_new() ;


  gui->menu_bar = gtk_menu_bar_new() ;

  initialize_menu(gui->menu_bar, gui->accels_group, gui->menus, gui->menuitems) ;


  gtk_window_add_accel_group(GTK_WINDOW(gui->main_window), gui->accels_group->menu_files_accel_group)   ;
  gtk_window_add_accel_group(GTK_WINDOW(gui->main_window), gui->accels_group->menu_edition_accel_group) ;
  gtk_window_add_accel_group(GTK_WINDOW(gui->main_window), gui->accels_group->menu_action_accel_group)  ;
  gtk_window_add_accel_group(GTK_WINDOW(gui->main_window), gui->accels_group->menu_view_accel_group)    ;





  /** Buttons. **/
  gui->buttons = g_slice_new(Buttons) ;

  gui->buttons_accel_group   = gtk_accel_group_new() ;

  initialize_button_box(gui->buttons) ;

  /** Buttons containers. **/
  gui->left_buttonbox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;
  gtk_box_set_spacing(GTK_BOX(gui->left_buttonbox),    8) ;

  gui->edition_buttonbox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;
  gtk_button_box_set_layout(GTK_BUTTON_BOX(gui->edition_buttonbox), GTK_BUTTONBOX_CENTER);
  gtk_box_set_spacing(GTK_BOX(gui->edition_buttonbox), 8) ;

  gui->action_buttonbox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;
  gtk_button_box_set_layout(GTK_BUTTON_BOX(gui->action_buttonbox), GTK_BUTTONBOX_CENTER);
  gtk_box_set_spacing(GTK_BOX(gui->action_buttonbox),  8) ;

  gui->right_buttonbox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;
  gtk_button_box_set_layout(GTK_BUTTON_BOX(gui->right_buttonbox), GTK_BUTTONBOX_END) ;
  gtk_box_set_spacing(GTK_BOX(gui->right_buttonbox),   8) ;

  gtk_box_pack_start(GTK_BOX(gui->left_buttonbox), gui->buttons->open_file_button,             FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui->left_buttonbox), gui->buttons->save_file_button,             FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui->left_buttonbox), gui->buttons->save_file_as_button,          FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui->left_buttonbox), gui->buttons->save_file_all_button,         FALSE, FALSE, 0) ;

  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->left_buttonbox), gui->buttons->open_file_button,     TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->left_buttonbox), gui->buttons->save_file_button,     TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->left_buttonbox), gui->buttons->save_file_as_button,  TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->left_buttonbox), gui->buttons->save_file_all_button, TRUE) ;


  /** Pack buttons into the button bar. */
  #ifdef GSPELL_SUPPORT
  gtk_box_pack_start(GTK_BOX(gui->edition_buttonbox), gui->buttons->inline_spellcheck_button,  FALSE, FALSE, 0) ;
  #endif
  gtk_box_pack_start(GTK_BOX(gui->edition_buttonbox), gui->buttons->tabs_button,               FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui->edition_buttonbox), gui->buttons->cut_button,                FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui->edition_buttonbox), gui->buttons->copy_button,               FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui->edition_buttonbox), gui->buttons->paste_button,              FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui->edition_buttonbox), gui->buttons->duplicate_button,          FALSE, FALSE, 0) ;




  /** This are image button which must set non-homogeneous for not expand for having an square size. **/
  #ifdef GSPELL_SUPPORT
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->edition_buttonbox),gui->buttons->inline_spellcheck_button,   TRUE) ;
  #endif
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->edition_buttonbox),gui->buttons->tabs_button,                TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->edition_buttonbox),gui->buttons->cut_button,                 TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->edition_buttonbox),gui->buttons->copy_button,                TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->edition_buttonbox),gui->buttons->paste_button,               TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->edition_buttonbox),gui->buttons->duplicate_button,           TRUE) ;


  #ifdef MINI_MULTIPURPOSE_EDITOR_SUPPORT /** This feature is too much unstable. **/
  gtk_box_pack_start(GTK_BOX(gui->action_buttonbox), gui->buttons->todo_window_button,          FALSE, FALSE, 0) ;
  #endif

  #ifdef GSPELL_SUPPORT
  gtk_box_pack_start(GTK_BOX(gui->action_buttonbox), gui->buttons->spellcheck_button,           FALSE, FALSE, 0) ;
  #endif
  gtk_box_pack_start(GTK_BOX(gui->action_buttonbox), gui->buttons->go_to_line_button,           FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui->action_buttonbox), gui->buttons->search_and_replace_button,   FALSE, FALSE, 0) ;

  /** This are image button which must set non-homogeneous for not expand for having an square size. **/
  #ifdef MINI_MULTIPURPOSE_EDITOR_SUPPORT /** This feature is too much unstable. **/
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->action_buttonbox),gui->buttons->todo_window_button,        TRUE);
  #endif
  #ifdef GSPELL_SUPPORT
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->action_buttonbox),gui->buttons->spellcheck_button,         TRUE);
  #endif
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->action_buttonbox),gui->buttons->go_to_line_button,         TRUE);
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->action_buttonbox),gui->buttons->search_and_replace_button, TRUE);


  #ifdef DEBUG
  GtkWidget *debug_search_and_replace_button = gtk_button_new_with_label("Find Replace debug") ;

  g_signal_connect(G_OBJECT(debug_search_and_replace_button), "clicked", G_CALLBACK(debug_searching_and_replace_by_signal_emit) , NULL) ;

  gtk_box_pack_start(GTK_BOX(gui->right_buttonbox), debug_search_and_replace_button,                FALSE, FALSE, 0) ;
  #endif



  gtk_box_pack_start(GTK_BOX(gui->right_buttonbox), gui->buttons->exec_button,                  FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui->right_buttonbox), gui->buttons->big_terminal_button,          FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui->right_buttonbox), gui->buttons->terminals_show_button,        FALSE, FALSE, 0) ;


  /** This are image button which must set non-homogeneous for not expand for having an square size. **/
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->right_buttonbox),gui->buttons->exec_button,           TRUE);
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->right_buttonbox),gui->buttons->big_terminal_button,   TRUE);
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(gui->right_buttonbox),gui->buttons->terminals_show_button, TRUE);



  gtk_window_add_accel_group(GTK_WINDOW(gui->main_window), gui->buttons_accel_group);

  /** Main buttons box toolbar container: **/
  gui->buttonbox_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0) ;

  gtk_box_set_spacing(GTK_BOX(gui->buttonbox_hbox), 8) ;

  gtk_box_pack_start(GTK_BOX(gui->buttonbox_hbox), gui->left_buttonbox,       FALSE, FALSE, 0 )  ;
  gtk_box_pack_start(GTK_BOX(gui->buttonbox_hbox), gtk_label_new(""),         TRUE,  TRUE,  0 )  ;
  gtk_box_pack_start(GTK_BOX(gui->buttonbox_hbox), gui->edition_buttonbox,    FALSE, FALSE, 0 )  ;
  gtk_box_pack_start(GTK_BOX(gui->buttonbox_hbox), gtk_label_new(""),         TRUE,   TRUE,  0 )  ;
  gtk_box_pack_start(GTK_BOX(gui->buttonbox_hbox), gui->action_buttonbox,     FALSE, FALSE, 0 )  ;
  gtk_box_pack_start(GTK_BOX(gui->buttonbox_hbox), gtk_label_new(""),         TRUE,  TRUE,  0 )  ;
  gtk_box_pack_start(GTK_BOX(gui->buttonbox_hbox), gui->right_buttonbox,      FALSE, FALSE, 0 )  ;

  g_object_set(G_OBJECT(gui->buttonbox_hbox), "margin", 4, NULL) ;




  gui->go_to_line_dialog = g_slice_new(Go_To_Line_Dialog) ;



  gui->editor_notebook = gtk_notebook_new() ;

  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(gui->editor_notebook), GTK_POS_TOP) ;
  gtk_notebook_set_show_border(GTK_NOTEBOOK(gui->editor_notebook), TRUE);
  gtk_notebook_set_scrollable(GTK_NOTEBOOK(gui->editor_notebook), TRUE);
  gtk_notebook_popup_enable(GTK_NOTEBOOK(gui->editor_notebook));

  g_signal_connect(G_OBJECT(gui->editor_notebook), "switch-page", G_CALLBACK(change_current_page), NULL) ;

  /** Page reorder callback: we update the "page_number" data from every notebook page. **/
  g_signal_connect(G_OBJECT(gui->editor_notebook), "page-reordered", G_CALLBACK(page_reorder), NULL) ;



  /** Main editor container **/
  gui->editor_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;

  gui->bottom_bar  = g_slice_new(BottomBar) ;

  /** Editor bottom info bar: currrent line,column \t filename \t total lines, total characters ; **/
  gui->bottom_bar->info_bottom_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;

  g_object_set(G_OBJECT(gui->bottom_bar->info_bottom_bar), "margin-start",  4,  NULL) ;
  g_object_set(G_OBJECT(gui->bottom_bar->info_bottom_bar), "margin-end",    4,  NULL) ;

  gui->bottom_bar->current_row_col  = gtk_label_new("") ;
  gui->bottom_bar->padding_left     = gtk_label_new("") ;
  gui->bottom_bar->filename_label   = gtk_label_new("") ;
  gui->bottom_bar->padding_right    = gtk_label_new("") ;
  gui->bottom_bar->total_rows_cols  = gtk_label_new("") ;


  gtk_widget_set_size_request(gui->bottom_bar->current_row_col, -1, 8) ;
  gtk_widget_set_size_request(gui->bottom_bar->info_bottom_bar, -1, 8) ;



  gtk_box_pack_start(GTK_BOX(gui->bottom_bar->info_bottom_bar), gui->bottom_bar->current_row_col,  FALSE, FALSE, 0 ) ;
  gtk_box_pack_start(GTK_BOX(gui->bottom_bar->info_bottom_bar), gui->bottom_bar->padding_left,     TRUE,  TRUE,  0 ) ;
  gtk_box_pack_start(GTK_BOX(gui->bottom_bar->info_bottom_bar), gui->bottom_bar->filename_label,   FALSE, FALSE, 0 ) ;
  gtk_box_pack_start(GTK_BOX(gui->bottom_bar->info_bottom_bar), gui->bottom_bar->padding_right,    TRUE,  TRUE,  0 ) ;
  gtk_box_pack_start(GTK_BOX(gui->bottom_bar->info_bottom_bar), gui->bottom_bar->total_rows_cols,  FALSE, FALSE, 0 ) ;


  /** Search and replace functionnality. **/

  /** Search settings. **/
  search_settings = g_malloc(sizeof(Search_Settings)) ;

  set_search_settings_default(search_settings) ;

  searching.search_text       = NULL  ;
  searching.search_tag        = NULL  ;
  searching.complete_search   = FALSE ;
  searching.get_an_occurrence = FALSE ;
  searching.restart_search    = FALSE ;


  searching.source_search_settings = gtk_source_search_settings_new() ;


  gui->search_and_replace = g_slice_new(Search_And_Replace) ;

  setup_search_and_replace_bar(gui->search_and_replace) ;

  gtk_box_set_spacing(GTK_BOX(gui->search_and_replace->search_and_replace_main_vbox), 4) ;

  g_object_set(G_OBJECT(gui->search_and_replace->search_and_replace_main_vbox), "margin", 4, NULL) ;

  gtk_box_set_spacing(GTK_BOX(gui->search_and_replace->search_and_replace_main_hbox), 4) ;

  gtk_box_pack_start(GTK_BOX(gui->search_and_replace->search_and_replace_main_vbox), gui->search_and_replace->search_and_replace_main_hbox, FALSE, FALSE, 0) ;


  /** Pack editor containers. **/
  gtk_box_pack_start(GTK_BOX(gui->editor_vbox), gui->editor_notebook,                                  TRUE,  TRUE,  0) ;
  gtk_box_pack_start(GTK_BOX(gui->editor_vbox), gui->search_and_replace->search_and_replace_main_vbox, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui->editor_vbox), gui->bottom_bar->info_bottom_bar,                      FALSE, FALSE, 0) ;




  /** Hard GUI coded terminals notebook container: **/
  gui->terminal_notebook = gtk_notebook_new() ;

  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(gui->terminal_notebook), GTK_POS_TOP) ;
  gtk_notebook_set_show_border(GTK_NOTEBOOK(gui->terminal_notebook), TRUE);
  gtk_notebook_set_scrollable(GTK_NOTEBOOK(gui->terminal_notebook),  FALSE);
  gtk_notebook_popup_enable(GTK_NOTEBOOK(gui->terminal_notebook));

  add_new_terminals(NULL) ;

  while (g_main_context_pending(NULL)) {

    g_main_context_iteration(NULL, FALSE) ;

  }

  while (gtk_events_pending()) {

    gtk_main_iteration() ;

  }

  /** We preset an arbitrary size for displaying problems patching and resize it into the function: on_size_allocated ;
      Because else we won't have enough place and get terminal prompt displaying problems. **/
  gtk_widget_set_size_request(gui->terminal_notebook, 724,-1) ;



  /** Initialized and configure the big terminal **/
  gui->big_terminal = gtk_itterm_new(TRUE, FALSE) ;

  /** Big terminal divided into 4 terminals. **/
  gui->big_four_terminals = g_malloc(sizeof(FTerms)) ;

  /** Initialized and configure all terminals **/
  gui->big_four_terminals->terminal_1 = gtk_itterm_new(TRUE, FALSE) ;
  gui->big_four_terminals->terminal_2 = gtk_itterm_new(TRUE, FALSE) ;
  gui->big_four_terminals->terminal_3 = gtk_itterm_new(TRUE, FALSE) ;
  gui->big_four_terminals->terminal_4 = gtk_itterm_new(TRUE, FALSE) ;


  gui->big_four_terminals->vertical_paned_up   = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL) ;
  gui->big_four_terminals->vertical_paned_down = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL) ;

  gui->big_four_terminals->horizontal_paned    = gtk_paned_new(GTK_ORIENTATION_VERTICAL)   ;


  g_object_set(G_OBJECT(gui->big_four_terminals->vertical_paned_up),    "wide-handle", TRUE, NULL) ;
  g_object_set(G_OBJECT(gui->big_four_terminals->vertical_paned_down),  "wide-handle", TRUE, NULL) ;

  g_object_set(G_OBJECT(gui->big_four_terminals->horizontal_paned),     "wide-handle", TRUE, NULL) ;

  gtk_paned_pack1(GTK_PANED(gui->big_four_terminals->vertical_paned_up),   gui->big_four_terminals->terminal_1, TRUE, TRUE);
  gtk_paned_pack2(GTK_PANED(gui->big_four_terminals->vertical_paned_up),   gui->big_four_terminals->terminal_2, TRUE, TRUE);

  gtk_paned_pack1(GTK_PANED(gui->big_four_terminals->vertical_paned_down), gui->big_four_terminals->terminal_3, TRUE,TRUE);
  gtk_paned_pack2(GTK_PANED(gui->big_four_terminals->vertical_paned_down), gui->big_four_terminals->terminal_4, TRUE,TRUE);

  gtk_paned_pack1(GTK_PANED(gui->big_four_terminals->horizontal_paned),    gui->big_four_terminals->vertical_paned_up,         TRUE, TRUE) ;
  gtk_paned_pack2(GTK_PANED(gui->big_four_terminals->horizontal_paned),    gui->big_four_terminals->vertical_paned_down,       TRUE, TRUE) ;


  /** Main window middle resizing between editor and terminals. **/
  gui->middle_paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL) ;

  g_object_set(G_OBJECT(gui->middle_paned), "wide-handle",  TRUE, NULL) ;

  gtk_paned_pack1(GTK_PANED(gui->middle_paned),gui->editor_vbox,       TRUE,  FALSE) ;
  gtk_paned_pack2(GTK_PANED(gui->middle_paned),gui->terminal_notebook, FALSE, TRUE)  ;


  /** Main notebook will permit to switch between big terminal and editor with side terminals. **/
  gui->main_notebook = gtk_notebook_new() ;

  gtk_notebook_set_show_tabs(GTK_NOTEBOOK(gui->main_notebook),   FALSE) ;

  gtk_notebook_set_show_border(GTK_NOTEBOOK(gui->main_notebook), TRUE)  ;


  gtk_notebook_append_page(GTK_NOTEBOOK(gui->main_notebook), gui->middle_paned,                         FALSE) ;
  gtk_notebook_append_page(GTK_NOTEBOOK(gui->main_notebook), gui->big_terminal,                         FALSE) ;
  gtk_notebook_append_page(GTK_NOTEBOOK(gui->main_notebook), gui->big_four_terminals->horizontal_paned, FALSE) ;

  gtk_widget_show_all(gui->main_notebook) ;



  /** Main GUI container. **/
  gui->main_hbox   = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;


  gtk_box_pack_start(GTK_BOX(gui->main_hbox),gui->main_notebook,  TRUE,  TRUE,  0) ;


  /** Main GUI container containing all the widgets. **/
  gui->main_vbox   = gtk_box_new(GTK_ORIENTATION_VERTICAL,   0) ;

  gtk_box_pack_start(GTK_BOX(gui->main_vbox),gui->menu_bar,       FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui->main_vbox),gui->buttonbox_hbox, FALSE, FALSE, 0) ;
  gtk_box_pack_start(GTK_BOX(gui->main_vbox),gui->main_hbox,      TRUE,  TRUE,  0) ;



  gtk_container_add(GTK_CONTAINER(gui->main_window), gui->main_vbox) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(gui->menuitems->display_buttonbar_menuitem), "activate", G_CALLBACK(view_buttonbar), gui->buttonbox_hbox) ;

  g_signal_connect(gtk_smart_menu_item_get_menuitem(gui->menuitems->toggle_fullscreen_menuitem), "activate", G_CALLBACK(toggle_fullscreen), gui->main_window) ;



  gtk_widget_show_all(gui->main_window) ;


  /** Launch a command in the terminals if defined from user. **/
  Terminals *terminals =  (Terminals *) g_object_get_data(G_OBJECT(gtk_notebook_get_nth_page(GTK_NOTEBOOK(gui->terminal_notebook), 0)) , "terminals") ;


  set_terminal_start_cmd(terminals->terminal_1) ;
  set_terminal_start_cmd(terminals->terminal_2) ;

  set_terminal_start_cmd(gui->big_terminal) ;

  set_terminal_start_cmd(gui->big_four_terminals->terminal_1) ;
  set_terminal_start_cmd(gui->big_four_terminals->terminal_2) ;
  set_terminal_start_cmd(gui->big_four_terminals->terminal_3) ;
  set_terminal_start_cmd(gui->big_four_terminals->terminal_4) ;



  /** Check if applications detected if not then hide the menuitem widget. **/
  check_applications(gui_app.diff,         gtk_smart_menu_item_get_menuitem(gui->menuitems->app_programming_diff_menuitem)) ;
  check_applications(gui_app.debugger,     gtk_smart_menu_item_get_menuitem(gui->menuitems->app_programming_debugger_menuitem)) ;
  check_applications(gui_app.python,       gtk_smart_menu_item_get_menuitem(gui->menuitems->app_programming_python_menuitem)) ;
  check_applications(gui_app.gui_designer, gtk_smart_menu_item_get_menuitem(gui->menuitems->app_programming_gui_designer_menuitem)) ;
  check_applications(gui_app.devhelp,      gtk_smart_menu_item_get_menuitem(gui->menuitems->app_programming_devhelp_menuitem)) ;
  check_applications(gui_app.calculator,   gtk_smart_menu_item_get_menuitem(gui->menuitems->app_utils_calculator_menuitem)) ;
  check_applications(gui_app.color_picker, gtk_smart_menu_item_get_menuitem(gui->menuitems->app_utils_color_picker_menuitem)) ;
  check_applications(gui_app.dictionary,   gtk_smart_menu_item_get_menuitem(gui->menuitems->app_utils_dictionary_menuitem)) ;
  check_applications(gui_app.file_manager, gtk_smart_menu_item_get_menuitem(gui->menuitems->app_utils_file_manager_menuitem)) ;
  check_applications(gui_app.notes,        gtk_smart_menu_item_get_menuitem(gui->menuitems->app_utils_notes_menuitem)) ;
  check_applications(gui_app.browser,      gtk_smart_menu_item_get_menuitem(gui->menuitems->app_utils_browser_menuitem)) ;









  /** Check if their are user defined applications, if not hide the menuitem widget. **/
  if (gui_app.nb_of_others == 0) {
    gtk_widget_hide(gtk_smart_menu_item_get_menuitem(gui->menuitems->app_others_menuitem)) ;
  }

  /** Hide the search and replace bar at application start. **/
  gtk_widget_hide(gui->search_and_replace->search_and_replace_main_vbox) ;

  /** Hide the terminals at application start. **/
  gtk_widget_hide(gui->terminal_notebook) ;



  /** Set editor notebook page. **/
  gtk_notebook_set_current_page(GTK_NOTEBOOK(gui->main_notebook), 0) ;



  /** This function set the terminals default size in relationship to your screen relationship because the application is maximize at start. And set startup settings. **/
  g_signal_connect(G_OBJECT(gui->main_window), "size-allocate", G_CALLBACK(on_size_allocated), NULL) ;




  if (argc == 1) {

    /** it-edit started without any file()s given as argument:
      * We set the default file into the editor at start.
      *******************************************************/

    File_Editor *start_file = g_malloc0(sizeof(File_Editor)) ;

    gchar *tmp_name = NULL ;

    gint ret = g_file_open_tmp("New_XXXXXX", &tmp_name, NULL) ;

    g_close(ret, NULL);

    GFile *new_g_file = g_file_new_for_path(tmp_name);

    g_free(tmp_name) ;

    add_file_to_editor(gui->editor_notebook, start_file, new_g_file, true) ;

  }
  else {

    /** Activate the application. **/
    g_application_activate(G_APPLICATION(app)) ;

  }


  while (g_main_context_pending(NULL)) { // gtk_events_pending()

    g_main_context_iteration(NULL, FALSE) ;

  }

  while (gtk_events_pending()) {

    gtk_main_iteration() ;

  }

  /** Enter GUI mainloop. **/
  status = g_application_run(G_APPLICATION(app), argc, argv) ;

  g_object_unref(app) ;

  return status ;

}

static void check_applications(char *app_name, GtkWidget *menu_item) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (g_strcmp0(app_name, "") == 0) {
    gtk_widget_hide(menu_item) ;
    g_signal_handlers_disconnect_by_func(menu_item, launch_application, app_name) ;
  }

  return ;
}


static void on_size_allocated(GtkWidget *widget, GdkRectangle *allocation, gpointer user_data) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  static bool is_on = false ;

  if (is_on) {

    g_signal_handlers_disconnect_by_func(widget, on_size_allocated, NULL) ;

    return ;

  }


  /** Set the default sidebar terminals on 80 characters wide.
    * On a resolution of 1920/1080.
    * But you can easily change the factor and the terminal font to use it-edit efficiently. 
  **/

  gtk_widget_set_size_request(gui->terminal_notebook, settings.side_terms_factor * ( 80 * 8 + 24), -1) ;

  is_on = true ;

  return ;

}


void set_terminal_start_cmd(GtkWidget *itterm) {

  /** Execute a command at start in all terminals. **/

  GtkWidget *vteterminal = gtk_itterm_get_vte_terminal(itterm) ;

  if (gtk_widget_is_visible(vteterminal)) {

    if ((settings.command != NULL) && (g_strcmp0(settings.command, "") != 0) ) {


      char *command_string ;
      command_string = g_strdup_printf("%s \r", settings.command) ;

      /** Launch the wanted command in the terminal. **/
      vte_terminal_feed_child(VTE_TERMINAL(vteterminal), command_string, strlen(command_string)) ;

      free(command_string) ;

      /** Wait for the forking from the shell has been done.  **/
      while (g_main_context_pending(NULL)) {

        g_main_context_iteration(NULL, FALSE) ;

      }

    }

  }

  return ;

}

static void activate(GApplication *application) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  /** This signal is receive by calling: g_application_run() function,
    *
    * so the just by starting the application to run by calling the g_application_activate() function.
    *
  **/

  #ifdef MAC_INTEGRATION /** The library gtk-mac-integration-gtk3 define this. **/
  /** @NOTE: the mac top application menu doesn't update correctly the menu radio buttons ! **/
  gtkosx_application_set_menu_bar(gtkosx_application_get(), GTK_MENU_SHELL(gui->menu_bar)) ;
  gtkosx_application_sync_menubar(gtkosx_application_get());
  #endif

  #ifdef G_OS_UNIX
  GDBusConnection *dbus_connection = g_application_get_dbus_connection(G_APPLICATION(app)) ;

  if (dbus_connection != NULL && G_IS_DBUS_CONNECTION(dbus_connection) ) {

    g_dbus_connection_start_message_processing(dbus_connection) ;

  }

  /** Send a notification at startup and at exit. **/
 
  /** @NOTE: not done finally but if wanted
    *        you can simply edit the it-edity.conf file to get it.
    **************************************************************/
 
  if (settings.notifications) {

    GNotification *notification = g_notification_new( g_get_application_name() ) ;

    g_notification_set_title(notification, "Ready to work with it-edit...") ;

    g_notification_set_default_action(notification, "app.actions") ;

    gchar *notification_body = g_strdup("Enjoy to perform the art of programmatic with it-edit.") ;

    g_notification_set_body(notification, notification_body) ;

    g_notification_set_priority(notification, G_NOTIFICATION_PRIORITY_NORMAL) ;

    g_application_send_notification(G_APPLICATION(app), "at_it_edit_startup", notification);

    g_free(notification_body) ;

  }

  g_application_set_default(G_APPLICATION(app)) ;

  /** 15 seconds before it-edit becomes idle. **/
  g_application_set_inactivity_timeout(G_APPLICATION(app), 15000) ;



  #ifdef DEBUG
  fprintf(stdout,"GtkApplication DBUS path: %s\n", g_application_get_dbus_object_path(G_APPLICATION(app)) ) ;
  #endif

  #endif

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui->buttons->terminals_show_button), (settings.side_terms_on) ? TRUE : FALSE) ;

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui->buttons->big_terminal_button), (settings.big_term_on) ? TRUE : FALSE) ;

  if (settings.big_term_div) {

    gtk_widget_activate(gtk_smart_menu_item_get_menuitem(gui->menuitems->switch_big_terms_menuitem)) ;

  }

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui->buttons->big_terminal_button), (settings.big_term_on) ? TRUE : FALSE) ;

  if (! settings.buttonbar_on) {

    gtk_widget_hide(gui->buttonbox_hbox) ;

  }


  if (settings.fullscreen) {

    gtk_window_fullscreen(GTK_WINDOW(gui->main_window)) ;

  }

  return ;

}

static void open_files(GApplication  *application, GFile **files, gint n_files, const gchar *hint) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  /** GtkApplication command line open folder or files argument treatment function:
    * This will permit the g_win32_get_command_line() function to be automatically called internally.
    * And many others things.
    *************************************************************************************************/

  int i = 0 ;

  for ( ; i < n_files ; ++i) {

     process_selected_file(g_file_dup(files[i])) ;

  }

  register_files() ;

  return ;

}

static void startup(GApplication *application, gpointer user_data) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  while ( ! g_application_get_is_registered(application) ) {

    while (g_main_context_pending(NULL)) {

      g_main_context_iteration(NULL, FALSE) ;

    }
  }

  return ;
}

#ifdef G_OS_UNIX
static void about_activated(GSimpleAction *action, GVariant *parameter, gpointer app) {

  display_about_dialog(NULL) ;

  return ;

}

static void open_file_activated(GSimpleAction *action, GVariant *parameter, gpointer app) {

  open_file(NULL) ;

  return ;

}

static void preferences_activated(GSimpleAction *action, GVariant *parameter, gpointer app) {

  display_configuration_dialog(NULL, NULL) ;

  return ;

}

static void quit_activated(GSimpleAction *action, GVariant *parameter, gpointer app) {

  delete_event(NULL, NULL, NULL) ;

  return ;

}
#endif

static void shutdown_app(GApplication *application, gpointer user_data) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  delete_event(NULL, NULL, NULL) ;

}

static void at_exit_handler(void) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD) ;

  gtk_clipboard_store(clipboard);

  gboolean register_session = FALSE ;

  switch (settings.session_mode) {

    case -1 :

      /** Automatic session registering (default) **/

      register_session = TRUE ;

      break ;

    case 1 :

      /** Asked for session registering **/

      if (! get_answers_message_dialog( _("Register session"),
                                        _( "Do you want to register this session ?\n"
                                           "  + All opened files.\n"
                                           "  + All opened documentation (through the File handler).\n"
                                           "  + All opened applications through the Applications menu items."
                                           "\n\n"
                                           "See the documentation for further informations."),
                                         _("Register session"),
                                         _("Don't register"),
                                         GTK_MESSAGE_QUESTION))
                                                  
      {

        exit(EXIT_SUCCESS) ;


      }
      else {

        register_session = TRUE ;

      }

      break ;

  }
                                                         

  if (register_session) {

    GError *error = NULL ;

    GKeyFile *conf_file = g_key_file_new() ;

    if (session->session_files_nb > 0) {

      g_key_file_set_string_list(conf_file, "Session", "session_files", (const gchar **) session->session_files, session->session_files_nb) ;

    }

    if (session->session_doc_nb > 0) {

      g_key_file_set_string_list(conf_file, "Session", "session_doc", (const gchar **) session->session_doc, session->session_doc_nb) ;

    }

    if (session->session_app_nb > 0) {

      g_key_file_set_string_list(conf_file, "Session", "session_app", (const gchar **) session->session_app, session->session_app_nb) ;

    }

    g_key_file_save_to_file(conf_file, PATH_TO_SESSION_FILE, &error);


    if (error != NULL) {

      display_message_dialog( _("Error saving session !"), error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

      g_error_free(error) ;

      return ;

    }


    g_key_file_unref(conf_file) ;

  }



  /** Send a notification at startup and at exit. **/
 
  /** @NOTE: not done finally but if wanted
    *        you can simply edit the it-edity.conf file to get it.
    **************************************************************/
 
  if (settings.notifications) {

    GNotification *notification = g_notification_new( g_get_application_name() ) ;

    g_notification_set_title(notification, "Quit it-edit") ;

    g_notification_set_default_action(notification, "app.actions") ;

    gchar *notification_body = g_strdup("Bye bye, see you soon for a new it-edit session !") ;

    g_notification_set_body(notification, notification_body) ;

    g_notification_set_priority(notification, G_NOTIFICATION_PRIORITY_NORMAL) ;

    g_application_send_notification(G_APPLICATION(app), "at_exit_it-edit", notification);

    g_free(notification_body) ;

  }


  return ;

}

static void destroy(GtkWidget *widget, gpointer pointer) {

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  g_application_quit(G_APPLICATION(app)) ;


  exit(EXIT_SUCCESS) ;

}



static gboolean delete_event(GtkWidget *widget,GdkEvent *event,gpointer pointer) {
  /** Check if any file still unsaved before exit the application **/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  if (files_not_saved_check() > 0) {
    /** Any file is not saved by exiting the application. **/

    /** Display an last chance saving window
      * and return an boolean value according user choice
      * which eventually prevent to close the application.
      ****************************************************/

    return display_unsaved_files_dialog() ;

  }

  return FALSE ;
}
