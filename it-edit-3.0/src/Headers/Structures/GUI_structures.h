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

#ifndef IT_EDIT_GUI_STRUCTURES_HH
#define IT_EDIT_GUI_STRUCTURES_HH

#include "../includes.h"

typedef struct {

    /** Structure for holding one file open in the editor **/

    GStatBuf file_info ;

    GtkWidget *scrolled_window ;

    GtkWidget *textview ;

    GtkSourceBuffer *buffer ;

    GtkSourceFile *source_file ;

    #ifdef GSPELL_SUPPORT
    GspellTextView *gspell_view ;
    #endif

} File_Editor ;

typedef struct {

   /** Hard coded @GtkItTerm. **/

   GtkWidget *terminal_1 ;

   GtkWidget *terminal_2 ;

   GtkWidget *terminals_vertical_paned ;

} Terminals ;

typedef struct {

  /** Four @GtkItTerm terminals **/

  GtkWidget *terminal_1 ;

  GtkWidget *terminal_2 ;

  GtkWidget *terminal_3 ;

  GtkWidget *terminal_4 ;

  GtkWidget *vertical_paned_up   ;

  GtkWidget *vertical_paned_down ;

  GtkWidget *horizontal_paned ;

} FTerms ;



typedef struct {

    /** Structure for one menu item. **/

    GtkWidget *image ;

    GtkWidget *label ;

    GtkWidget *accel_label ;

    GtkWidget *menuitem ;

    GtkWidget *hbox ;

} MenuItem ;


typedef struct {

  GtkWidget *menu_syntax ;

  GtkWidget *app_others_menu ;

  GtkWidget *menu_scheme ;

} Menus ;

typedef struct {

  GtkWidget *app_programming_diff_menuitem ;

  GtkWidget *app_programming_debugger_menuitem ;

  GtkWidget *app_programming_python_menuitem ;

  GtkWidget *app_programming_gui_designer_menuitem ;

  GtkWidget *app_programming_devhelp_menuitem ;

  GtkWidget *app_utils_calculator_menuitem ;

  GtkWidget *app_utils_color_picker_menuitem ;

  GtkWidget *app_utils_dictionary_menuitem ;

  GtkWidget *app_utils_file_manager_menuitem ;

  GtkWidget *app_utils_notes_menuitem ;

  GtkWidget *app_utils_browser_menuitem ;


  GtkWidget *show_term_menuitem ;

  GtkWidget *display_buttonbar_menuitem ;

  GtkWidget *toggle_fullscreen_menuitem ;

  GtkWidget *app_others_menuitem ;

  GtkWidget *switch_big_terms_menuitem ;

  GtkWidget *big_term_menuitem ;

  GtkWidget *switch_tabs_menuitem ;

  GtkWidget *inline_spellcheck_menuitem ;

} MenuItems ;

typedef struct {

  GtkAccelGroup *menu_files_accel_group ;

  GtkAccelGroup *menu_edition_accel_group ;

  GtkAccelGroup *menu_action_accel_group ;

  GtkAccelGroup *menu_view_accel_group ;

} AccelGroups ;

typedef struct {

    /** Buttons structure **/

    GtkWidget *open_file_button ;

    GtkWidget *open_file_image ;

    GtkWidget *save_file_as_button ;

    GtkWidget *save_file_as_image ;

    GtkWidget *save_file_button ;

    GtkWidget *save_file_image ;

    GtkWidget *save_file_all_button ;

    GtkWidget *save_file_all_image ;


    #ifdef MINI_MULTIPURPOSE_EDITOR_SUPPORT  /** This feature is too much unstable. **/
    GtkWidget *todo_window_button ;

    GtkWidget *todo_window_image ;
    #endif
 
    #ifdef GSPELL_SUPPORT
    GtkWidget *inline_spellcheck_button ;

    GtkWidget *inline_spellcheck_image ;
    #endif

    GtkWidget *tabs_button ;

    GtkWidget *tabs_image ;

    GtkWidget *cut_button ;

    GtkWidget *cut_image ;

    GtkWidget *copy_button ;

    GtkWidget *copy_image ;

    GtkWidget *paste_button ;

    GtkWidget *paste_image ;

    GtkWidget *duplicate_button ;

    GtkWidget *duplicate_image ;

    #ifdef GSPELL_SUPPORT
    GtkWidget *spellcheck_button ;

    GtkWidget *spellcheck_image ;
    #endif

    GtkWidget *go_to_line_button ;

    GtkWidget *go_to_line_image  ;

    GtkWidget *search_and_replace_button ;

    GtkWidget *search_and_replace_image  ;


    GtkWidget *exec_button ;

    GtkWidget *exec_image  ;

    GtkWidget *big_terminal_button ;

    GtkWidget *big_terminal_image ;

    GtkWidget *terminals_show_button ;

    GtkWidget *terminals_show_image ;

} Buttons ;

typedef struct {

    /** Editor bottom info bar. **/

    GtkWidget *info_bottom_bar       ;

    GtkWidget *current_row_col       ;

    GtkWidget *padding_left          ;

    GtkWidget *filename_label        ;

    GtkWidget *padding_right         ;

    GtkWidget *total_rows_cols       ;

} BottomBar ;

typedef struct {

  /** Main container **/
  GtkWidget *search_and_replace_main_vbox   ;

  /** search and replace main container **/
  GtkWidget *search_and_replace_main_hbox   ;

  /** Search and replace containers **/

  GtkWidget *search_and_replace_label_vbox  ;

  GtkWidget *search_and_replace_entry_vbox  ;

  GtkWidget *search_and_replace_button_vbox_1 ;

  GtkWidget *search_and_replace_button_vbox_2 ;



  GtkWidget *search_button   ;

  GtkWidget *search_image    ;


  GtkWidget *replace_button  ;

  GtkWidget *replace_image   ;


  GtkWidget *search_entry   ;

  GtkWidget *replace_entry  ;


  GtkWidget *search_next_button ;

  GtkWidget *search_next_image  ;

  GtkWidget *search_prev_button ;

  GtkWidget *search_prev_image  ;


  GtkWidget *replace_one_button ;

  GtkWidget *replace_all_button ;


  /** Search settings **/
  GtkWidget *search_settings_main_hbox ;

  GtkWidget *search_settings_button_hbox ;

  GtkWidget *search_settings_button ;

  GtkWidget *search_settings_image ;

  GtkWidget *search_settings_label ;

  GtkWidget *search_settings_hbox  ;

  GtkWidget *search_settings_combobox ;

  GtkWidget *search_settings_padding_1     ;

  GtkWidget *search_settings_case_checkbox ;

  GtkWidget *search_settings_wrap_checkbox ;

  GtkWidget *search_settings_padding_2     ;

  GtkWidget *search_settings_match_label   ;

  GtkWidget *search_settings_close_button  ;

  GtkWidget *search_settings_close_image   ;


} Search_And_Replace ;

typedef struct {

  /** Go to line functionnality. **/

  GtkWidget *go_to_line_dialog  ;

  GtkWidget *go_to_line_content ;

  GtkWidget *go_to_line_vbox    ;

  GtkWidget *go_to_line_label   ;

  GtkWidget *go_to_line_entry   ;

  GtkWidget *go_to_line_apply   ;

  GtkWidget *go_to_line_close   ;

} Go_To_Line_Dialog ;


typedef struct {

    /** Main GUI structure **/

    GtkWidget *main_window ;

    GtkWidget *main_vbox ;

    GtkWidget *main_hbox ;

    GtkWidget *menu_bar ;

    MenuItems *menuitems ;

    AccelGroups *accels_group ;

    Buttons *buttons ;

    Menus *menus ;

    GtkAccelGroup *buttons_accel_group ;

    GtkWidget *buttonbox_hbox      ;

    GtkWidget *left_buttonbox      ;

    GtkWidget *edition_buttonbox   ;

    GtkWidget *action_buttonbox    ;

    GtkWidget *right_buttonbox     ;


    Go_To_Line_Dialog *go_to_line_dialog   ;


    GtkWidget *big_terminal    ;

    FTerms   *big_four_terminals ;

    GtkWidget *middle_paned      ;

    GtkWidget *editor_vbox       ;

    Search_And_Replace *search_and_replace ;

    BottomBar *bottom_bar        ;


    GtkWidget *editor_notebook  ;

    GtkWidget *terminal_notebook ;

    GtkWidget *main_notebook ;

} GUI ;

#endif
