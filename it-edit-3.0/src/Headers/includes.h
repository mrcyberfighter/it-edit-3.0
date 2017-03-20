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

#ifndef IT_EDIT_INCLUDES_HH
#define IT_EDIT_INCLUDES_HH

/** Include gtk utilities **/
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <glib.h>
#include <glib-2.0/glib/gstdio.h>

#include <gio/gio.h>
#include <gio/gunixinputstream.h>

#include <gtksourceview/gtksource.h>
#include <gtksourceview/completion-providers/words/gtksourcecompletionwords.h>

#include <vte-2.91/vte/vte.h>

#ifdef GSPELL_SUPPORT
#include <gspell/gspell.h>
#endif

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <locale.h>
#include <math.h>
#include <errno.h>

#include <dirent.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <utime.h>

#ifdef MACOSX
// For Mac OS X menu and dock support.
#include <gtkmacintegration/gtkosxapplication.h>
#endif

#endif
