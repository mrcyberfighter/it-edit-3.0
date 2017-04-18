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

#ifndef GTK_SMART_TOOLTIP_HH
#define GTK_SMART_TOOLTIP_HH

/**
  *  The @GtkSmartTooltip type is:
  *
  *  simply a highly personalize tool-tip.
  *
  *  TODO: You have to use the Gtk-3 CSS to personalize into the gtk_smart_tooltip_initialize(...),
  *  you have to call before using what you do with this template.
  *
  *  @NOTE: it-edit does finally not use this widget, use it as you want.
  *
  *************************************************************************************************/

#include <gtk/gtk.h>

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

G_BEGIN_DECLS

#define GTK_SMART_TOOLTIP(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_TYPE_SMART_TOOLTIP, GtkSmartFileTooltip))
#define GTK_SMART_TOOLTIP_CLASS(class)    (G_TYPE_CHECK_CLASS_CAST((class), GTK_TYPE_SMART_TOOLTIP, GtkSmartFileTooltipClass))
#define IS_GTK_SMART_TOOLTIP(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_TYPE_SMART_TOOLTIP))
#define IS_GTK_SMART_TOOLTIP_CLASS(class) (G_TYPE_CHECK_CLASS_TYPE((class), GTK_TYPE_SMART_TOOLTIP))
#define GTK_SMART_TOOLTIP_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_SMART_TOOLTIP, GtkSmartFileTooltipClass))

typedef struct _GtkSmartFileTooltip        GtkSmartFileTooltip;
typedef struct _GtkSmartFileTooltipClass   GtkSmartFileTooltipClass;

#define GTK_TYPE_SMART_TOOLTIP (gtk_smart_tooltip_get_type())

/** This structure is defined into the *.c file **/
typedef struct _GtkSmartFileTooltipPrivate GtkSmartFileTooltipPrivate;

struct _GtkSmartFileTooltipClass {

   GObjectClass parent_class;

};

struct _GtkSmartFileTooltip {

  GObject parent;

  GtkSmartFileTooltipPrivate *priv;

};

GType gtk_smart_tooltip_get_type(void) G_GNUC_CONST ;

/** ************************************************************************ **/



/** Getters of the @GtkSmartTooltip members: **/

/** Getter for the image widget. **/
extern GtkWidget* gtk_smart_tooltip_get_image(GtkWidget *smart_tooltip) ;


/** Contructors of the @GtkSmartTooltip type: **/

extern void gtk_smart_tooltip_initialize(void) ;

extern GtkWidget* gtk_smart_tooltip_add(GtkWidget *widget, const gchar *filepath, const gchar *image_filepath) ;

#if 0
/** Another simple constructor. **/
extern GtkWidget *gtk_smart_tooltip_new(const gchar *filepath) ;
extern GtkWidget *gtk_smart_icon_toggle_button_new(const gchar *filepath) ;
#endif


/** Setters of the @GtkSmartTooltip type: **/

#if 0
/** Setters to use with the simple constructor you can either add a tooltip_text or change the buttons image. **/
extern void gtk_smart_tooltip_set_tooltip_text(const gchar *tooltip_text) ;

extern void gtk_smart_tooltip_set_tooltip_text_markup(const gchar *markup) ;

extern void gtk_smart_tooltip_set_icon_from_file(const gchar *filepath) ;
#endif

G_END_DECLS

#endif

