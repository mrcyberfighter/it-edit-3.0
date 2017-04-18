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

#include "GtkSmartMenuItem.h"

typedef struct _GtkSmartMenuItemPrivate {

  /** This structure contains all the members of the new type: **/

  GtkWidget *image;

  GtkWidget *label;

  GtkWidget *accel_label;

  GtkWidget *menuitem;

} GtkSmartMenuItemPrivate;

static void gtk_smart_menu_item_class_init(GtkSmartMenuItemClass *class);
static void gtk_smart_menu_item_init(GtkSmartMenuItem *smart_menu_item); // Constructor
static void gtk_smart_menu_item_finalize(GObject *object); // Destructor

G_DEFINE_TYPE_WITH_PRIVATE(GtkSmartMenuItem, gtk_smart_menu_item, GTK_TYPE_BOX)

static void gtk_smart_menu_item_class_init(GtkSmartMenuItemClass *class) {

  /** Class initialize. **/

  g_return_if_fail(class != NULL);

  G_OBJECT_CLASS(class)->finalize = gtk_smart_menu_item_finalize ; // Destructor

}

static void gtk_smart_menu_item_init(GtkSmartMenuItem *smart_menu_item) {

  /** Initialize the members. **/

  g_return_if_fail(smart_menu_item != NULL);

  /** Permit to access object private data. **/
  GtkSmartMenuItemPrivate *priv = gtk_smart_menu_item_get_instance_private(smart_menu_item);

  /** Initialize private data. **/
  priv->menuitem    = NULL ;  /** The menu item               **/
  priv->label       = NULL ;  /** The menu label              **/
  priv->image       = NULL ;  /** The menu icon image         **/
  priv->accel_label = NULL ;  /** The menu accelerator label  **/

}



static void gtk_smart_menu_item_finalize(GObject *object) {

  GtkSmartMenuItemPrivate *priv = gtk_smart_menu_item_get_instance_private(GTK_SMART_MENU_ITEM(object));

  if (priv->menuitem != NULL) {
    gtk_widget_destroy(priv->menuitem) ;
    g_object_unref(priv->menuitem) ;
  }

  if (priv->image != NULL) {
    gtk_widget_destroy(priv->image) ;
    g_object_unref(priv->image) ;
  }

  if (priv->label != NULL) {
    gtk_widget_destroy(priv->label) ;
    g_object_unref(priv->label) ;
  }

  if (priv->accel_label != NULL) {
    gtk_widget_destroy(priv->accel_label) ;
    g_object_unref(priv->accel_label) ;
  }

  /** Private data destructor if necessary. **/
  G_OBJECT_CLASS(gtk_smart_menu_item_parent_class)->finalize(object);

}

/** Local functions: **/
static void generate_accel_label(GtkSmartMenuItemPrivate *priv, const guint accel_key, const GdkModifierType accel_modifier, GtkAccelGroup *accel_group) ;

static void add_smart_menu_item_icon(GtkSmartMenuItemPrivate *priv, const gchar *icon_filepath) ;

static void pack_menuitem(GtkSmartMenuItemPrivate *priv, GtkSmartMenuItem *box) ;


static void generate_accel_label(GtkSmartMenuItemPrivate *priv, const guint accel_key, const GdkModifierType accel_modifier, GtkAccelGroup *accel_group) {

  if (gtk_accelerator_valid(accel_key, accel_modifier)) {

    /** @GtkSmartMenuItem has an accelerator and so we generate the accelerator label. **/

    gchar *tmp_accel_label = gtk_accelerator_get_label(accel_key, accel_modifier);

    priv->accel_label = gtk_accel_label_new(tmp_accel_label);

    g_free(tmp_accel_label);

    gtk_label_set_justify(GTK_LABEL(priv->accel_label), GTK_JUSTIFY_RIGHT) ;


    if (accel_group != NULL) {


      /** @GtkSmartMenuItem has an associated accelerator group and so we connect it to the activate signal. **/

      gtk_widget_add_accelerator(priv->menuitem,
                                 "activate",
                                 accel_group,
                                 accel_key,
                                 accel_modifier,
                                 GTK_ACCEL_VISIBLE);

    }

  }
  else {

    /** @GtkSmartMenuItem has no accelerator and so no accelerator label **/

    priv->accel_label = NULL;

  }

  return ;

}


static void add_smart_menu_item_icon(GtkSmartMenuItemPrivate *priv, const gchar *icon_filepath) {

   /** Add an icon to the @GtkSmartMenuItem if a valid file path is provided. **/

  if (g_file_test(icon_filepath, G_FILE_TEST_IS_REGULAR)) {

    /** @GtkSmartMenuItem has an associated icon image **/

    priv->image = gtk_image_new_from_file(icon_filepath) ;

    return ;

  }

  /** @GtkSmartMenuItem has an no associated icon image **/

  priv->image = NULL ;

  return ;

}

static void pack_menuitem(GtkSmartMenuItemPrivate *priv, GtkSmartMenuItem *box) {

  /** Pack all the component from the @GtkSmartMenuItem together. **/

  if (priv->image) {

    /** Add the icon to the @GtkSmartMenuItem the parent container.  **/
    gtk_box_pack_start(GTK_BOX(box),priv->image,FALSE,FALSE,0);

  }


  /** Add the @GtkSmartMenuItem label to the parent container.  **/
  gtk_box_pack_start(GTK_BOX(box),priv->label,FALSE,FALSE,0);

  /** Add padding to the @GtkSmartMenuItem between the @label and the @accel_label.  **/
  gtk_box_pack_start(GTK_BOX(box),gtk_label_new(""),TRUE,TRUE,0);

  if (priv->accel_label != NULL) {

    /** Add the @accel_label to the @GtkSmartMenuItem if @accel_key, @accel_modifier provided. **/
    gtk_box_pack_start(GTK_BOX(box),priv->accel_label,FALSE,FALSE,0);

  }
  else {

    gtk_box_pack_start(GTK_BOX(box),gtk_label_new(""),FALSE, FALSE,0);

  }

  gtk_container_add(GTK_CONTAINER(priv->menuitem), GTK_WIDGET(box));

  return ;

}

GtkWidget* gtk_smart_menu_item_new_all(const gchar *label, const gchar *icon_filepath, GtkAccelGroup *accel_group, const GdkModifierType accel_modifier, const guint accel_key) {


  /** Create the new object. **/
  GtkSmartMenuItem *smart_menu_item = NULL ;

  smart_menu_item = g_object_new(GTK_TYPE_SMART_MENU_ITEM, NULL);

  /** Accessing private data **/
  GtkSmartMenuItemPrivate *priv = gtk_smart_menu_item_get_instance_private(smart_menu_item);

  /** Initialize private data. **/
  priv->menuitem = gtk_menu_item_new(); /** The menu_item   **/

  priv->label = gtk_label_new(label);   /** The menu label  **/

  gtk_label_set_justify(GTK_LABEL(priv->label), GTK_JUSTIFY_LEFT);

  /** Generate the accelerator label if @accel_key is a valid key and eventually if a modifier is given.
    *
    * @if @accel_group != @NULL then the accelerator is connected to the "activate" signal, through the given @accel_group.
    *
    * @note: You will have to connect the signal you want, to the @menuitem member. (At best simply the "activate" signal.)
    *
    ***********************************************************************************************************************/

  generate_accel_label(priv, accel_key, accel_modifier, accel_group);

  /** Add the menu item icon **/
  add_smart_menu_item_icon(priv, icon_filepath);

  /** Pack all the @GtkSmartMenuItem together **/
  pack_menuitem(priv, smart_menu_item);

  /** Return the entire @GtkSmartMenuItem object as @GtkWidget pointer (for compatibility). **/
  return GTK_WIDGET(smart_menu_item) ;
}

GtkWidget* gtk_smart_check_menu_item_new_all(const gchar *label, const gboolean draw_as_radio, const gchar *icon_filepath, GtkAccelGroup *accel_group, const GdkModifierType accel_modifier, const guint accel_key) {

  /** Create the new object. **/
  GtkSmartMenuItem *smart_menu_item = NULL ;

  smart_menu_item = g_object_new(GTK_TYPE_SMART_MENU_ITEM, NULL);

  /** Accessing private data. **/
  GtkSmartMenuItemPrivate *priv = gtk_smart_menu_item_get_instance_private(smart_menu_item);

  /** Initialize private data. **/
  priv->menuitem = gtk_check_menu_item_new(); /** The menu_item **/

  /** Draw the @GtkSmartCheckMenuItem as radio or not. **/
  gtk_check_menu_item_set_draw_as_radio(GTK_CHECK_MENU_ITEM(priv->menuitem), draw_as_radio) ;

  priv->label = gtk_label_new(label); /** The menu label **/

  gtk_label_set_justify(GTK_LABEL(priv->label), GTK_JUSTIFY_LEFT);

  /** Generate the accelerator label if @accel_key is a valid key and eventually if a modifier is given.
    *
    * @if @accel_group != @NULL then the accelerator is connected to the "activate" signal, through the given @accel_group.
    *
    * @note: You will have to connect the signal you want, to the @menuitem member. (At best simply the "activate" signal.)
    *
    ***********************************************************************************************************************/

  generate_accel_label(priv, accel_key, accel_modifier, accel_group);

  /** Add the menu item icon **/
  add_smart_menu_item_icon(priv, icon_filepath);

  /** Pack all the @GtkSmartMenuItem together **/
  pack_menuitem(priv, smart_menu_item);

  /** Return the entire @GtkSmartMenuItem object as @GtkWidget pointer (for compatibility). **/
  return GTK_WIDGET(smart_menu_item);
}

GtkWidget* gtk_smart_menu_item_get_image(GtkWidget *smart_menu_item) {

  if ( smart_menu_item == NULL || ! IS_GTK_SMART_MENU_ITEM(smart_menu_item) ) {

    g_printerr("%s(...) error: *smart_menu_item is not a valid pointer on a GtkSmartMenuItem object.\n", __func__);

    return NULL ;

  }

  GtkSmartMenuItemPrivate *priv = gtk_smart_menu_item_get_instance_private(GTK_SMART_MENU_ITEM(smart_menu_item));

  return GTK_WIDGET(priv->image);
}

GtkWidget* gtk_smart_menu_item_get_menuitem(GtkWidget *smart_menu_item) {

  if ( smart_menu_item == NULL || ! IS_GTK_SMART_MENU_ITEM(smart_menu_item) ) {

    g_printerr("%s(...) error: *smart_menu_item is not a valid pointer on a GtkSmartMenuItem object.\n", __func__);

    return NULL ;

  }

  GtkSmartMenuItemPrivate *priv = gtk_smart_menu_item_get_instance_private(GTK_SMART_MENU_ITEM(smart_menu_item));

  return GTK_WIDGET(priv->menuitem);
}

GtkWidget* gtk_smart_menu_item_get_label(GtkWidget *smart_menu_item) {

  if ( smart_menu_item == NULL || ! IS_GTK_SMART_MENU_ITEM(smart_menu_item) ) {

    g_printerr("%s(...) error: *smart_menu_item is not a valid pointer on a GtkSmartMenuItem object.\n", __func__);

    return NULL ;

  }

  GtkSmartMenuItemPrivate *priv = gtk_smart_menu_item_get_instance_private(GTK_SMART_MENU_ITEM(smart_menu_item));

  return GTK_WIDGET(priv->label);
}

GtkWidget* gtk_smart_menu_item_get_accel_label(GtkWidget *smart_menu_item) {

  if ( smart_menu_item == NULL || ! IS_GTK_SMART_MENU_ITEM(smart_menu_item) ) {

    g_printerr("%s(...) error: *smart_menu_item is not a valid pointer on a GtkSmartMenuItem object.\n", __func__);

    return NULL ;

  }

  GtkSmartMenuItemPrivate *priv = gtk_smart_menu_item_get_instance_private(GTK_SMART_MENU_ITEM(smart_menu_item));

  return GTK_WIDGET(priv->accel_label);
}


