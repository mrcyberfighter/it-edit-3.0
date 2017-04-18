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

#ifndef GTK_SMART_MENU_ITEM_HH
#define GTK_SMART_MENU_ITEM_HH

/**
  *  The @GtkSmartMenuItem type is:
  *
  *  A (check) menu item with a menu item label, an icon and an universal accelerator label.
  *
  *  Having a @GtkBox has parent widget.
  *
  *  @note: If an accelerator is provided he is automatically connected to the "activate" signal
  *         to the given @GtkAccelGroup.
  *
  *         Else it will only display the accelerator label if you don't provide a @GtkAccelGroup.
  *
  ***********************************************************************************************/

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GTK_SMART_MENU_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_TYPE_SMART_MENU_ITEM, GtkSmartMenuItem))
#define GTK_SMART_MENU_ITEM_CLASS(class)    (G_TYPE_CHECK_CLASS_CAST((class), GTK_TYPE_SMART_MENU_ITEM, GtkSmartMenuItemClass))
#define IS_GTK_SMART_MENU_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_TYPE_SMART_MENU_ITEM))
#define IS_GTK_SMART_MENU_ITEM_CLASS(class) (G_TYPE_CHECK_CLASS_TYPE((class), GTK_TYPE_SMART_MENU_ITEM))
#define GTK_SMART_MENU_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_SMART_MENU_ITEM, GtkSmartMenuItemClass))

typedef struct _GtkSmartMenuItem        GtkSmartMenuItem;
typedef struct _GtkSmartMenuItemClass   GtkSmartMenuItemClass;

#define GTK_TYPE_SMART_MENU_ITEM (gtk_smart_menu_item_get_type())

/** This structure is defined into the *.c file **/
typedef struct _GtkSmartMenuItemPrivate GtkSmartMenuItemPrivate;

struct _GtkSmartMenuItem {

  GtkBox parent;

  GtkSmartMenuItemPrivate *priv;

};

struct _GtkSmartMenuItemClass {

   GtkBoxClass parent_class;

};

GType gtk_smart_menu_item_get_type(void) G_GNUC_CONST ;

#if 0
/**
  * You can broad this new type as you want by implementing functions as like the following definitions...
**/

/** Constructors examples definition:
  * you must return an object from type GtkSmartMenuItem: I mean you have to return the entire object, for further usage.
  * Cast to a GtkWidget pointer from the constructor for compatibility.
**/

/** Contructor which provide only a menu item label (You will have to provide setters for the others members). **/
GtkWidget* gtk_smart_menu_item_new(const gchar *label);

/** Contructor with only a label and and an icon given as Pixbuf (You will have to provide setters for the others members).  **/
GtkWidget* gtk_smart_menu_item_new_from_pixbuf(const gchar *label, GdkPixbuf *pixbuf);

/** Contructor with only a label and an icon given as image widget (You will have to provide setters for the others members). **/
GtkWidget* gtk_smart_menu_item_new_with_image(const gchar *label, GtkWidget image);

/** Contructor with a label and an accel_modifier label (You will have to provide setters for the others members). **/
GtkWidget* gtk_smart_menu_item_new_with_accel(const gchar *label, GtkAccelGroup *accel_group,  GdkModifierType accel_modifier, guint accel_key);

/** And to provide to adapted setters,
**/ completing this uncompleted constructors. At you to see if you want to personalize this new type.


#endif

/** Contructors of the @GtkSmartMenuItem type: **/

extern GtkWidget* gtk_smart_menu_item_new_all(const gchar *label, const gchar *icon_filepath, GtkAccelGroup *accel_group, const GdkModifierType accel_modifier, const guint accel_key) ;

extern GtkWidget* gtk_smart_check_menu_item_new_all(const gchar *label, const gboolean draw_as_radio, const gchar *icon_filepath, GtkAccelGroup *accel_group, const GdkModifierType accel_modifier, const guint accel_key) ;

/** Getters of the @GtkSmartMenuItem members: **/

extern GtkWidget* gtk_smart_menu_item_get_image(GtkWidget *smart_menu_item) ;

extern GtkWidget* gtk_smart_menu_item_get_menuitem(GtkWidget *smart_menu_item) ;

extern GtkWidget* gtk_smart_menu_item_get_label(GtkWidget *smart_menu_item) ;

extern GtkWidget* gtk_smart_menu_item_get_accel_label(GtkWidget *smart_menu_item) ;

G_END_DECLS

#endif
