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

#include "./files_saver.h"

static GError *error = NULL ;

static gboolean source_view_saver_success = FALSE ;

static gboolean saver_as_finished = FALSE ;

#ifdef DEBUG
static void file_saver_progress_callback(goffset current_num_bytes, goffset total_num_bytes, gpointer user_data) {

  fprintf(stdout,"N° of bytes offset: %li loaded from total: %li\n", current_num_bytes, total_num_bytes) ;

  return ;

}

#endif

static void file_saver_result_callback(GObject *source_object, GAsyncResult *res, gpointer saver) {


  source_view_saver_success = gtk_source_file_saver_save_finish(saver, res, &error) ;

  if (error != NULL) {

    display_message_dialog( _("Error file saving !"), error->message, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE) ;

    g_error_free(error) ;

    error = NULL ;

  }

  g_object_unref(source_object) ;

  saver_as_finished = TRUE ;

  return ;

}


static void destoy_notify_asio_load(gpointer data) {

  g_free(data) ;

  return ;


}


gboolean source_file_saver(GtkSourceBuffer *buffer, GtkSourceFile *file) {

  /** File saving function used in file "save" case.
    ************************************************/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  source_view_saver_success = FALSE ;

  saver_as_finished = FALSE ;

  GtkSourceFileSaver *saver = gtk_source_file_saver_new(GTK_SOURCE_BUFFER(buffer), file) ;

  gtk_source_file_saver_set_encoding(saver, gtk_source_encoding_get_from_charset(settings.charset)) ;

  gtk_source_file_saver_set_newline_type(saver, settings.newline_type) ;

  GtkSourceFileSaverFlags flags = GTK_SOURCE_FILE_SAVER_FLAGS_IGNORE_INVALID_CHARS  ;

  if (settings.backup_file) {

    flags |= GTK_SOURCE_FILE_SAVER_FLAGS_CREATE_BACKUP ;

  }

  if (settings.overwrite_anyway) {

    flags |= GTK_SOURCE_FILE_SAVER_FLAGS_IGNORE_MODIFICATION_TIME ;

  }

  gtk_source_file_saver_set_flags(saver, flags) ;

  #ifdef DEBUG
  gtk_source_file_saver_save_async(saver, G_PRIORITY_DEFAULT, NULL, &file_saver_progress_callback, NULL, &destoy_notify_asio_load, &file_saver_result_callback, saver) ;
  #else
  gtk_source_file_saver_save_async(saver, G_PRIORITY_DEFAULT, NULL, NULL, NULL, &destoy_notify_asio_load, &file_saver_result_callback, saver) ;
  #endif

  while (! saver_as_finished) {

    /** Waiting the asynchron call has completed. **/

    gtk_main_iteration_do(FALSE) ;

    g_main_context_iteration(NULL, FALSE) ;

  }

  return source_view_saver_success ;

}

gboolean source_file_saver_with_target(GtkSourceBuffer *buffer, GtkSourceFile *file, GFile *target_location, gboolean compress) {

  /** File saving function used in file "save as" case.
    * Extended for the file compressing usage.
    ***************************************************/

  #ifdef DEBUG
  DEBUG_FUNC_MARK
  #endif

  source_view_saver_success = FALSE ;

  saver_as_finished = FALSE ;

  GtkSourceFileSaver *saver = gtk_source_file_saver_new_with_target(GTK_SOURCE_BUFFER(buffer), file, target_location) ;

  g_object_unref(target_location) ;

  gtk_source_file_saver_set_encoding(saver, gtk_source_encoding_get_from_charset(settings.charset)) ;

  gtk_source_file_saver_set_newline_type(saver, settings.newline_type) ;

  GtkSourceFileSaverFlags flags = GTK_SOURCE_FILE_SAVER_FLAGS_IGNORE_INVALID_CHARS ;

  if (settings.overwrite_anyway) {

    flags |= GTK_SOURCE_FILE_SAVER_FLAGS_IGNORE_MODIFICATION_TIME ;

  }

  if (compress) {

    gtk_source_file_saver_set_compression_type(saver, GTK_SOURCE_COMPRESSION_TYPE_GZIP) ;
 
  }
  

  gtk_source_file_saver_set_flags(saver, flags) ;

  #ifdef DEBUG
  gtk_source_file_saver_save_async(saver, G_PRIORITY_DEFAULT, NULL, &file_saver_progress_callback, NULL, &destoy_notify_asio_load, &file_saver_result_callback, saver) ;
  #else
  gtk_source_file_saver_save_async(saver, G_PRIORITY_DEFAULT, NULL, NULL, NULL, &destoy_notify_asio_load, &file_saver_result_callback, saver) ;
  #endif


  while (! saver_as_finished) {

    /** Waiting the asynchron call has completed. **/

    gtk_main_iteration_do(FALSE) ;

    g_main_context_iteration(NULL, FALSE) ;

  }

  return source_view_saver_success ;

}


