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

#ifndef GTK_ITTERM_HH
#define GTK_ITTERM_HH

#include <gtk/gtk.h>
#include <vte-2.91/vte/vte.h>

G_BEGIN_DECLS

/** Thanks to M. Gérald Dumas <http://linechec.sourceforge.net/, http://m3dlib.tuxfamily.org/>
  *
  * For the seed idea of creating a GtkItTerm type.
  *
  * The code of the GtkItTerm type (*.h and *.c files) was originally entirely written by M. Gérald Dumas.
  ********************************************************************************************************
  *
  * M. Gérald Dumas has analyze my code of it-edit (version 2.91)
  * and send me, per the forum from developpez.net located in France,
  * on which I have present it-edit (version 2.91),
  * a wonder full complete template code for writing the GtkItTerm type.
  *
  ********************************************************************************************************
  *
  * In it-edit (version 2.91) the vteterminal was included into a GtkBox with aside a GtkScrollbar.
  *
  * In the new version the vteterminal is included in a single GtkScrolledWindow.
  * So I only had to made few changes to make the originally code from M. Gérald Dumas working,
  * with deriving the GtkItTerm from a GtkScrolledWindow.
  ********************************************************************************************************
  *
  * We don't forget to thanks M. Gérald Dumas for his precious help at the begin of the project it-edit.
  * By rereading entirely my code from it-edit (version 1.0.0) and pointing all the worse memory leaks
  * I have made in it-edit (version 1.0.0) so that the first release version of it-edit was the
  * it-edit 1.0.1 (the version after having correct all the errors pointed by M. Gérald Dumas).
  * Thanks to my friend gerald.
  * And to all the others having react by emitting ideas and correct some piece of code along
  * the development of it-edit, which isn't finish.
  *
  ********************************************************************************************************/


#define GTK_ITTERM(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_TYPE_ITTERM, GtkItTerm))
#define GTK_ITTERM_CLASS(class)    (G_TYPE_CHECK_CLASS_CAST((class),  GTK_TYPE_ITTERM, GtkItTermClass))
#define IS_GTK_ITTERM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_TYPE_ITTERM))
#define IS_GTK_ITTERM_CLASS(class) (G_TYPE_CHECK_CLASS_TYPE((class),  GTK_TYPE_ITTERM))
#define GTK_ITTERM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GTK_TYPE_ITTERM, GtkItTermClass))

typedef struct _GtkItTerm       GtkItTerm;
typedef struct _GtkItTermClass  GtkItTermClass;

#define GTK_TYPE_ITTERM (gtk_itterm_get_type())

/** Definition to find in the *.c file **/
typedef struct _GtkItTermPrivate GtkItTermPrivate;

/** GtkItTerm definition. **/
struct _GtkItTerm {

  GtkScrolledWindow parent;

  GtkItTermPrivate *priv;

};

/** Class definition. **/
struct _GtkItTermClass {

   GtkScrolledWindowClass parent_class;

};


GType gtk_itterm_get_type(void) G_GNUC_CONST ;

/** ************************************************************************ **/



#include "../Headers/global_vars.h"

#include "../Callbacks/terminals_callbacks.h"
#include "../Callbacks/gui_callbacks.h"

/** New @GtkItTerm instance creation. **/
extern GtkWidget *gtk_itterm_new(gboolean initialize, gboolean is_sidebar_term);

/** (vte) terminal getter. **/
extern GtkWidget *gtk_itterm_get_vte_terminal(GtkWidget *itterm);
extern GtkWidget *gtk_itterm_get_clipboard_menu(GtkWidget *itterm);

/** Configuration of the (vte) terminal. **/
extern void gtk_itterm_configure_terminal(GtkWidget *itterm, gboolean initialize);

G_END_DECLS

#endif
