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

#include "GtkSmartFileTooltip.h"

typedef struct _GtkSmartFileTooltipPrivate {

  /** This structure contains all the members of the new type: **/

  /** You can personalize the widgets contains in the @GtkSmartTooltip. **/

  /** NOTE: I have try to implement a tool-tip with a button to click-on to get the absolute file-path onto the notebook tab, but it's not reachable. **/

  GtkWidget *window;

  GtkWidget *filename_label;

  GtkWidget *filepath_label;

  GtkWidget *button_label;

  GtkWidget *button;

  GtkWidget *vbox;

  GtkWidget *hbox;

  GtkWidget *hbox2;

} GtkSmartFileTooltipPrivate;

static void gtk_smart_tooltip_class_init(GtkSmartFileTooltipClass *class);
static void gtk_smart_tooltip_init(GtkSmartFileTooltip *smart_tooltip); // Constructor
static void gtk_smart_tooltip_finalize(GObject *object); // Destructor

G_DEFINE_TYPE_WITH_PRIVATE(GtkSmartFileTooltip, gtk_smart_tooltip, G_TYPE_OBJECT)

static void gtk_smart_tooltip_class_init(GtkSmartFileTooltipClass *class) {

  /** Class initialize. **/

  g_return_if_fail(class != NULL);

  G_OBJECT_CLASS(class)->finalize = gtk_smart_tooltip_finalize ; // Destructor

}

static void gtk_smart_tooltip_init(GtkSmartFileTooltip *smart_tooltip) {

  /** Initialize the members. **/

  g_return_if_fail(smart_tooltip != NULL);

  /** Permit to access object private data. **/
  GtkSmartFileTooltipPrivate *priv = gtk_smart_tooltip_get_instance_private(smart_tooltip);

  /** Initialize private data. **/

  priv->window=NULL;

  priv->filename_label=NULL;

  priv->filepath_label=NULL;

  priv->button_label=NULL;

  priv->button=NULL;

  priv->vbox=NULL;

  priv->hbox=NULL;

  priv->hbox2=NULL;

}



static void gtk_smart_tooltip_finalize(GObject *object) {

  /** Private data destructor if necessary. **/
  G_OBJECT_CLASS(gtk_smart_tooltip_parent_class)->finalize(object);

}

void gtk_smart_tooltip_initialize(void) {

  /** TODO: All append in this function for the appearance of the @GtkSmartTooltip.
    *       On the other hand you can changes the widgets contains into the @GtkSmartTooltip.
    *****************************************************************************************/

  GtkCssProvider *provider = gtk_css_provider_new();

  GdkDisplay *display = gdk_display_get_default() ;
  GdkScreen *screen = gdk_display_get_default_screen(display);

  gtk_style_context_add_provider_for_screen(screen,
                                           GTK_STYLE_PROVIDER (provider),
                                           GTK_STYLE_PROVIDER_PRIORITY_USER) ; //GTK_STYLE_PROVIDER_PRIORITY_APPLICATION

  gtk_css_provider_load_from_data(provider,
                                   "#file_tooltip {\n"
                                   "color : rgba(0%,0%,0%,1.0);\n"
                                   "background-color: rgba(0%,100%,100%,0.75);\n"
                                   "}\n"
                                    , -1, NULL);
  g_object_unref(provider);

  return ;

}


static gboolean display_tooltip(GtkWidget *widget, gint x, gint y, gboolean keyboard_mode, GtkTooltip *tooltip, GtkWidget *window) {

  GtkAllocation *allocation = g_slice_new(GtkAllocation) ;

  //GtkAllocation allocation ; //= g_slice_new(GtkAllocation) ;

  gtk_widget_get_allocation(widget, allocation);



  gboolean ret = TRUE ;

  if ( ((x  < allocation->x) || (x > allocation->x+allocation->width)) && ((y  < allocation->y) || (y > allocation->y+allocation->height)) ) {

    ret = FALSE ;
  }

  return ret ; // RETURN FALSE hide the tooltip.

}

GtkWidget* gtk_smart_tooltip_add(GtkWidget *widget, const gchar *filepath, const gchar *image_filepath) {

  fprintf(stderr,"%s\n", __func__) ;

  if (filepath == NULL || image_filepath == NULL) {

    return NULL ;

  }

  char *abs_filepath = realpath(filepath, NULL) ;

  errno = 0 ;

  if (! g_file_test(abs_filepath, G_FILE_TEST_IS_REGULAR)) {

    g_printerr("%s", g_strerror(g_file_error_from_errno(errno)) ) ;

    return NULL ;

  }

  char *image_realpath = realpath(image_filepath, NULL) ;

  errno = 0 ;

  if (! g_file_test(image_realpath, G_FILE_TEST_IS_REGULAR)) {

    g_printerr("%s", g_strerror(g_file_error_from_errno(errno)) ) ;

    return NULL ;

  }


  /** Initialize private data. **/

  GtkSmartFileTooltip *smart_tooltip = NULL ;

  smart_tooltip = g_object_new(GTK_TYPE_SMART_TOOLTIP, NULL);

  /** Accessing private data **/
  GtkSmartFileTooltipPrivate *priv = gtk_smart_tooltip_get_instance_private(smart_tooltip);


  priv->window = gtk_window_new(GTK_WINDOW_POPUP) ;

  gtk_widget_set_name(priv->window,"file_tooltip") ;

  gchar *label_filename_text = g_path_get_basename(abs_filepath);

  priv->filename_label=gtk_label_new(label_filename_text);

  free(label_filename_text);

  priv->filepath_label=gtk_label_new(abs_filepath);

  free(abs_filepath);

  priv->button_label=gtk_label_new("");

  priv->button=gtk_button_new();

  gtk_button_set_image(GTK_BUTTON(priv->button),gtk_image_new_from_file(image_realpath));

  gtk_button_set_relief(GTK_BUTTON(priv->button), GTK_RELIEF_NONE) ;

  g_object_set(priv->button,"always-show-image",TRUE,NULL);

  priv->vbox=gtk_button_box_new(GTK_ORIENTATION_VERTICAL);

  priv->hbox=gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);





  gtk_box_pack_start(GTK_BOX(priv->hbox),priv->filename_label,FALSE,FALSE,0) ;
  //gtk_box_pack_start(GTK_BOX(priv->hbox),gtk_label_new(""),TRUE,TRUE,0) ;
  gtk_box_pack_start(GTK_BOX(priv->hbox),priv->button_label,TRUE,TRUE,0) ;
  gtk_box_pack_start(GTK_BOX(priv->hbox),priv->button,FALSE,FALSE,0) ;

  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(priv->hbox), priv->filename_label, TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(priv->hbox), priv->button_label, TRUE) ;
  gtk_button_box_set_child_non_homogeneous(GTK_BUTTON_BOX(priv->hbox), priv->button, TRUE) ;


  gtk_box_pack_start(GTK_BOX(priv->vbox),priv->hbox,FALSE,FALSE,0);
  gtk_box_pack_start(GTK_BOX(priv->vbox),priv->filepath_label,FALSE,FALSE,0);

  //gtk_box_set_homogeneous(GTK_BOX(priv->vbox), TRUE);

  gtk_container_add(GTK_CONTAINER(priv->window),priv->vbox) ;

  gtk_widget_show_all(priv->window) ;

  gtk_widget_hide(priv->window) ;

  g_object_set(widget, "has-tooltip", TRUE, NULL) ;

  gtk_widget_set_tooltip_window(widget, GTK_WINDOW(priv->window)) ;



  g_signal_connect(widget,"query-tooltip",G_CALLBACK(display_tooltip), priv->window) ;

  return priv->window ;

}
