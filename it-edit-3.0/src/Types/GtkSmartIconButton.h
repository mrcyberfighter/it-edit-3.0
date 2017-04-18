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

#ifndef GTK_SMART_ICON_BUTTON_HH
#define GTK_SMART_ICON_BUTTON_HH

/**
  *  The @GtkSmartIconButton type is:
  *
  *  simply a button with only an icon and a formatted tooltip text.
  *
  *  @note: If accelerators data's are given to the constructor, i.e the key and the modifier,
  *         the tooltip text is automatic formatted to contains an universal accelerator label.
  *
  *         Else it's only a button containing an icon.
  *
  * @Warning: The accelerator datas are not connected to a signal not used anyway else.
  *
  ***********************************************************************************************/

#include <gtk/gtk.h>

#include <errno.h>
#include <limits.h>
#include <stdlib.h>



G_BEGIN_DECLS

#define GTK_SMART_ICON_BUTTON(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_TYPE_SMART_ICON_BUTTON, GtkSmartIconButton))
#define GTK_SMART_ICON_BUTTON_CLASS(class)    (G_TYPE_CHECK_CLASS_CAST((class), GTK_TYPE_SMART_ICON_BUTTON, GtkSmartIconButtonClass))
#define IS_GTK_SMART_ICON_BUTTON(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_TYPE_SMART_ICON_BUTTON))
#define IS_GTK_SMART_ICON_BUTTON_CLASS(class) (G_TYPE_CHECK_CLASS_TYPE((class), GTK_TYPE_SMART_ICON_BUTTON))
#define GTK_SMART_ICON_BUTTON_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_SMART_ICON_BUTTON, GtkSmartIconButtonClass))

typedef struct _GtkSmartIconButton        GtkSmartIconButton;
typedef struct _GtkSmartIconButtonClass   GtkSmartIconButtonClass;

#define GTK_TYPE_SMART_ICON_BUTTON (gtk_smart_icon_button_get_type())

/** This structure is defined into the *.c file **/
typedef struct _GtkSmartIconButtonPrivate GtkSmartIconButtonPrivate;

struct _GtkSmartIconButtonClass {

   GtkButtonClass parent_class;

};

struct _GtkSmartIconButton {

  GtkButton parent;

  GtkSmartIconButtonPrivate *priv;

};

GType gtk_smart_icon_button_get_type(void) G_GNUC_CONST ;

/** ************************************************************************ **/



/** Getters of the @GtkSmartIconButton members: **/

/** Getter for the image widget. **/
extern GtkWidget* gtk_smart_icon_button_get_image(GtkWidget *smart_icon_button) ;


/** Contructors of the @GtkSmartIconButton type: **/

extern GtkWidget* gtk_smart_icon_button_new_all(const gchar *filepath, const gchar *tooltip_text, const guint accel_key, const GdkModifierType accel_modifier) ;

extern GtkWidget* gtk_smart_icon_toggle_button_new_all(const gchar *filepath, const gchar *tooltip_text, const guint accel_key, const GdkModifierType accel_modifier) ;

#if 0
/** Another simple constructor. **/
extern GtkWidget *gtk_smart_icon_button_new(const gchar *filepath) ;
extern GtkWidget *gtk_smart_icon_toggle_button_new(const gchar *filepath) ;
#endif


/** Setters of the @GtkSmartIconButton type: **/

#if 0
/** Setters to use with the simple constructor you can either add a tooltip_text or change the buttons image. **/
extern void gtk_smart_icon_button_set_tooltip_text(const gchar *tooltip_text) ;

extern void gtk_smart_icon_button_set_tooltip_text_markup(const gchar *markup) ;

extern void gtk_smart_icon_button_set_icon_from_file(const gchar *filepath) ;
#endif

G_END_DECLS

#endif

