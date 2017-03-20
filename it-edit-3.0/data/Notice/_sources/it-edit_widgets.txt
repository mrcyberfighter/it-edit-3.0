it-edit smart widgets
=====================

:program:`it-edit` implement some few self build widget:

.. note:: If you get interest into Gtk-3 widget building

  you can broad the source to understand How-To build GtkWidget(s) for Gtk-3 from your own.

GtkSmartMenuItem
----------------

A simple menu item containing:

+ An icon.

+ A label.

+ An universal shortcut text.

Constructors
~~~~~~~~~~~~

.. c:function:: GtkWidget* gtk_smart_menu_item_new_all(const gchar *label, const gchar *icon_filepath, GtkAccelGroup *accel_group, const GdkModifierType accel_modifier, const guint accel_key) ;
 
  :param  label: The label to display into the menu item.
  :type   label: :c:type:`const gchar *`
  :param  icon_filepath: The menu item icon file-path.
  :type   icon_filepath: :c:type:`const gchar *`
  :param  accel_group: The shortcut accelerator group.
  :type   accel_group: :c:type:`GtkAccelGroup *`
  :param  accel_modifier: The shortcut modifier.
  :type   accel_modifier: :c:type:`const GdkModifierType`
  :param  accel_key: The shortcut accelerator key.
  :type   accel_key: :c:type:`const guint`
  :rtype: :c:type:`GtkWidget *` 
  :return: A pointer to the :c:type:`GtkSmartMenuItem`.
 
.. c:function:: GtkWidget* gtk_smart_check_menu_item_new_all(const gchar *label, const gboolean draw_as_radio, const gchar *icon_filepath, GtkAccelGroup *accel_group, const GdkModifierType accel_modifier, const guint accel_key) ; 
 
  :param  label: The label to display into the menu item.
  :type   label: :c:type:`const gchar *`
  :param  draw_as_radio: draw_as_radio
  :type   draw_as_radio: :c:type:`const gboolean`
  :param  icon_filepath: The menu item icon file-path.
  :type   icon_filepath: :c:type:`const gchar *`
  :param  accel_group: The shortcut accelerator group.
  :type   accel_group: :c:type:`GtkAccelGroup *`
  :param  accel_modifier: The shortcut modifier.
  :type   accel_modifier: :c:type:`const GdkModifierType`
  :param  accel_key: The shortcut accelerator key.
  :type   accel_key: :c:type:`const guint`
  :rtype: :c:type:`GtkWidget *` 
  :return: A pointer to the :c:type:`GtkSmartMenuItem` check button.

.. note:: You can pass a :c:type:`NULL` pointer or ``0`` to the parameters :

  * icon_filepath
 
  * accel_group
 
  * accel_modifier
 
  * accel_key.
 
:note: You can build others constructors if you have understand How-To build this kind of widgets.     
 
Getters
~~~~~~~

.. c:function:: GtkWidget* gtk_smart_menu_item_get_image(GtkWidget *smart_menu_item) ;
 
  :param smart_menu_item: The return value from the constructors.
  :type  smart_menu_item: :c:type:`GtkWidget *`
  :rtype: :c:type:`GtkWidget *`
  :return: A pointer to the :c:type:`GtkImage` widget.
 
.. c:function:: GtkWidget* gtk_smart_menu_item_get_menuitem(GtkWidget *smart_menu_item) ;

  :param smart_menu_item: The return value from the constructors.
  :type  smart_menu_item: :c:type:`GtkWidget *`
  :rtype: :c:type:`GtkWidget *`
  :return: A pointer to the :c:type:`GtkMenuItem` widget.

.. c:function:: GtkWidget* gtk_smart_menu_item_get_label(GtkWidget *smart_menu_item) ;

  :param smart_menu_item: The return value from the constructors.
  :type  smart_menu_item: :c:type:`GtkWidget *`
  :rtype: :c:type:`GtkWidget *`
  :return: A pointer to the :c:type:`GtkLabel` widget.

.. c:function:: GtkWidget* gtk_smart_menu_item_get_accel_label(GtkWidget *smart_menu_item) ;  
 
  :param smart_menu_item: The return value from the constructors.
  :type  smart_menu_item: :c:type:`GtkWidget *`
  :rtype: :c:type:`GtkWidget *`
  :return: A pointer to the :c:type:`GtkAccelLabel` widget.
 
GtkSmartIconButton
------------------

A simple button with an icon without label and tool-tip which embed an universal short-cut text.

Constructors
~~~~~~~~~~~~

.. c:function:: GtkWidget* gtk_smart_icon_button_new_all(const gchar *filepath, const gchar *tooltip_text, const guint accel_key, const GdkModifierType accel_modifier) ;

  :param filepath: The filepath to the image to use as icon.
  :type  filepath: :c:type:`const gchar *`
  :param tooltip_text: The tool-tip text without the accelerator label.
  :type  tooltip_text: :c:type:`const gchar *`
  :param  accel_key: The shortcut accelerator key.
  :type   accel_key: :c:type:`const guint`
  :param  accel_modifier: The shortcut modifier.
  :type   accel_modifier: :c:type:`const GdkModifierType`
  :rtype: :c:type:`GtkWidget *`
  :return: A pointer to the :c:type:`GtkSmartIconButton` widget.
 
.. c:function::  GtkWidget* gtk_smart_icon_toggle_button_new_all(const gchar *filepath, const gchar *tooltip_text, const guint accel_key, const GdkModifierType accel_modifier) ;

  :param filepath: The filepath to the image to use as icon.
  :type  filepath: :c:type:`const gchar *`
  :param tooltip_text: The tool-tip text without the accelerator label.
  :type  tooltip_text: :c:type:`const gchar *`
  :param  accel_key: The shortcut accelerator key.
  :type   accel_key: :c:type:`const guint`
  :param  accel_modifier: The shortcut modifier.
  :type   accel_modifier: :c:type:`const GdkModifierType`
  :rtype: :c:type:`GtkWidget *`
  :return: A pointer to the :c:type:`GtkSmartIconButton` toggle button widget.
 
Getters
~~~~~~~

.. c:function:: GtkWidget* gtk_smart_icon_button_get_image(GtkWidget *smart_icon_button) ;

  :param smart_icon_button: The return value from the constructor of a :c:type:`GtkSmartIconButton`.
  :type  smart_icon_button: :c:type:`GtkWidget *`
  :rtype: :c:type:`GtkWidget *`
  :return: A pointer to the :c:type:`GtkImage` widget.     
 
GtkItTerm
---------

Not reusable like this.

.. note:: I've learned how to implement self builded Gtk-3 widgets

  If you get interest in building widgets take a look at the source for basics.   
 
