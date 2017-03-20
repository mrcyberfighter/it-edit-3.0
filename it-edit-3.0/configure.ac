#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# fdl (C) 2015-2017 Brüggemann Eddie.
# Permission is granted to copy, distribute and/or modify this document
# under the terms of the GNU Free Documentation License, Version 1.3
# or any later version published by the Free Software Foundation;
# with no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts.
# A copy of the license is included in the section entitled "GNU
# Free Documentation License".

AC_PREREQ([2.69])
AC_INIT([it-edit], [3.0], [mrcyberfighter@gmail.com], , [http://www.open-source-projects.net/it-edit/it-edit])
AC_CONFIG_SRCDIR([.])
AC_CONFIG_HEADERS([config.h])

AC_LANG([C])

AC_COPYRIGHT([GPLv3])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_CC_STDC
AC_PROG_CC_C99

AC_C_CONST

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([foreign -Wall -Werror])

case $host_os in

  *linux-gnu*)
    platform_linux=yes
    platform_win32=no
    native_win32=no
    platform_darwin=no
    ;;

  pw32* | *cygwin*)
    # Could work on cygwin but not tested.
    platform_win32=yes
    native_win32=no
    platform_linux=no
    platform_darwin=no
    ;;

  *darwin*)
    # Could work on cygwin but not tested.
    platform_darwin=yes
    platform_linux=no
    platform_win32=no
    native_win32=no
    ;;

esac

AM_CONDITIONAL(PLATFORM_WIN32, test x"$platform_win32" = "xyes")
AM_CONDITIONAL(NATIVE_WIN32, test x"$native_win32" = "xyes")
AM_CONDITIONAL(PLATFORM_LINUX, test x"$platform_linux" = "xyes")
AM_CONDITIONAL(PLATFORM_DARWIN, test x"$platform_darwin" = "xyes")

# Checks for programs

# POSIX programs detection.
AC_PROG_INSTALL
AC_PROG_MKDIR_P
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_SED

# coreutils package
AC_PATH_PROG([CHMOD],         [chmod],        [no])
AC_PATH_PROG([RM],            [rm],           [no])
AC_PATH_PROG([MV],            [mv],           [no])
AC_PATH_PROG([RMDIR],         [rmdir],        [no])
AC_PATH_PROG([SED],           [sed],          [no])
AC_PATH_PROG([WC],            [wc],           [no])
AC_PATH_PROG([INSTALL_INFO],  [install-info], [no])

if test x"$CHMOD" = xno ; then

  AC_MSG_ERROR(Install the coreutils package (chmod is missing) !, 1)

fi


if test x"$RM" = xno ; then

  AC_MSG_ERROR(Install the coreutils package (rm is missing) !, 1)

fi


if test x"$MV" = xno ; then

  AC_MSG_ERROR(Install the coreutils package (mv is missing) !, 1)

fi

if test x"$RMDIR" = xno ; then

  AC_MSG_ERROR(Install the coreutils package (rmdir is missing) !, 1)

fi

if test x"$SED" = xno ; then

  AC_MSG_WARN(Install the sed binary !, 1)

fi

if test x"$WC" = xno ; then

  AC_MSG_WARN(Install the wc binary !, 1)

fi


# desktop-file-utils package:
AC_PATH_PROG([DESKTOP_FILE_INSTALL],     [desktop-file-install],      [no])
AC_PATH_PROG([UPDATE_DESKTOP_DATABASE],  [update-desktop-database],   [no])
# xdg-utils package:
AC_PATH_PROG([XDG_DESKTOP_MENU],         [xdg-desktop-menu],          [no])

if test x"$DESKTOP_FILE_INSTALL" = xno ; then
if test x"$XDG_DESKTOP_MENU" = xno ; then

  AC_MSG_WARN(Install either the xdg-utils or desktop-file-utils package !)

  AC_MSG_WARN(This is not absolutely required.)

  AC_MSG_WARN(But it is recommend to install desktop files.)

fi
fi

destop_file_utils=no

if test x"$DESKTOP_FILE_INSTALL" = xno ; then

  AC_MSG_NOTICE(The desktop-file-install tool is used to install the *.desktop file.)
  AC_MSG_NOTICE(The desktop-file-install tool is used to update the cache database)
  AC_MSG_NOTICE(of the MIME types handled by desktop files)

else

desktop_file_utils=yes

fi

xdg_utils=no

if test x"$XDG_DESKTOP_MENU" = xno ; then

  AC_MSG_NOTICE(The xdg-desktop-menu tool is used to install the *.desktop file.)
  AC_MSG_NOTICE(The xdg-desktop-menu tool is used to update the cache database)
  AC_MSG_NOTICE(of the MIME types handled by desktop files)

else

xdg_utils=yes

fi

AM_CONDITIONAL(DESTOP_FILE_UTILS, test x"$destop_file_utils" = "xyes")
AM_CONDITIONAL(XDG_UTILS,         test x"$xdg_utils" = "xyes")





# The program is translatable but I do not think that a programming editor have to be translated.
#AC_PATH_PROG([MSGFMT], [msgfmt],                        AC_MSG_WARN(Install the gettext package !      ))
#AC_SUBST(PRG_USER, $USER)

# Checks for libraries.
PKG_CHECK_MODULES([GTK], [gtk+-3.0], , AC_MSG_ERROR(   gtk+3.0 library not detected !!!

   ******************* GTK 3 library required for it-edit *****************
   Install the GTK 3 library from your package manager or from source.

   WARNING: Take care to install the development files from gtk-3 !!!
            Often call: libgtk-3-dev or libgtk-3-devel.

   Then retry to launch the ./configure script...
   ************************************************************************
   , 1))

PKG_CHECK_MODULES([GTKSOURCEVIEW], [gtksourceview-3.0], , AC_MSG_ERROR(   gtksourceview-3.0 library not detected !!!

   ************* gtksourceview-3.0 library required for it-edit ***********
   Install the gtksourceview-3.0 library from your package manager or
   from source.

   You must have the package: libgtksourceview-3.0-common installed too.

   This package provide common files for the GTK+ syntax highlighting
   widget GtkSourceView: this is a text widget that extends the standard
   GTK+ 3.x text widget GtkTextView.

   It improves GtkTextView by implementing syntax highlighting and
   other features typical of a source editor.

   WARNING: Take care to install the development files from gtksourceview !
            Often call: libgtksourceview-3.0-dev or
                        libgtksourceview-3.0-devel.

   Then retry to launch the ./configure script...
   ************************************************************************
   , 1))


PKG_CHECK_MODULES([VTE_2_91], [vte-2.91], , AC_MSG_ERROR(   vte-2.91 library not detected !!!

   ****************** vte-2.91 library required for it-edit ****************
   Install the libvte-2.91 library from your package manager or from source.

   WARNING: Take care to install the development files from vte-2.91 !!!
            Often call: libvte-2.91-dev or libvte-2.91-devel.

   Then retry to launch the ./configure script...
   ************************************************************************
   , 1))


AC_ARG_ENABLE([gspell-1], [AS_HELP_STRING([--disable-gspell-1], [disable it-edit spell check feature.]) ], [gspell=no], [gspell=yes])

if test x"$gspell" = xyes ; then

PKG_CHECK_MODULES([GSPELL_1], [gspell-1], , AC_MSG_ERROR(   gspell-1 library not detected !

   ****************** gspell-1 library required for it-edit ****************
   Install the libgspell-1 library from your package manager or from source.

   WARNING: Take care to install the development files from gspell-1 !!!
            Often call: libgspell-1-1-dev or libgspell-1-1-devel.

   NOTE: You can get the libgspell-1 library through the ppa
     
         gnome-3-staging
     
         Else you can build the gspell-1 library with >= gtk+-3.20.

   You can disable the spell check support by using:

   $ ./configure --disable-gspell-1
     
   Then retry to launch the ./configure script...
   ************************************************************************
   , 1))

fi

AM_CONDITIONAL(GSPELL_OK, test x"$gspell" = "xyes")

if test x"$platform_darwin" = xyes ; then

  PKG_CHECK_MODULES([GTK_MAC_INTEGRATION], [gtk-mac-integration-gtk3], [], AC_MSG_ERROR(   gtk-mac-integration-gtk3 library not detected !!!

   *** gtk-mac-integration-gtk3 library required for it-edit **************

   Install the gtk-mac-integration-gtk3 library.

   You can easily get it through the package manager brew on Mac OSX.

   $ brew install gtk-mac-integration

   Then retry to launch the ./configure script...
   ************************************************************************
   , 1))

  PKG_CHECK_MODULES([GTK_QUARTZ], [gdk-quartz-3.0], [], AC_MSG_ERROR(   gdk-quartz-3.0 library not detected !!!

   ******** gdk-quartz-3.0 library required for it-edit *******************

   Install the gdk-quartz-3.0 library.

   You can easily get it through the package manager brew on Mac OSX.

   $ brew install gtk+3 # gdk-quartz-3.0 depends from gtk+3

   Then retry to launch the ./configure script...
   ************************************************************************
   , 1))

fi

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h locale.h stdint.h stdlib.h string.h sys/time.h unistd.h utime.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_UINT16_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atexit gettimeofday realpath setlocale strchr strrchr])


AC_CONFIG_FILES([Makefile
                src/Makefile
                man/Makefile
                texi/Makefile
                data/Makefile
                doc/Makefile
                desktop/Makefile
                ])


# Final output:
AC_OUTPUT

AC_MSG_NOTICE(System check OK !!!)

echo "
  ************************** ${PACKAGE_STRING} ************************

    ${PACKAGE_NAME} a programming text editor for command line fans.

    Package dependencies: gtk+-3, gtksourceview-3, gspell-1.

    Host:     ${host}
    Compiler: ${CC} ${CFLAGS} ${CPPFLAGS}
    Prefix:   ${prefix}

    Author: Brüggemann Eddie <${PACKAGE_BUGREPORT}>.

    Now enter:

    $ make
    $ sudo make install

    To build and install ${PACKAGE_NAME}.

    You can launch the program with the command line:
    $ it-edit [File(s)]
    Or use simply the icon.

    Enjoy using it-edit.
"
