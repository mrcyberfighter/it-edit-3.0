# fdl (C) 2015-2017 Brüggemann Eddie.
# Permission is granted to copy, distribute and/or modify this document
# under the terms of the GNU Free Documentation License, Version 1.3
# or any later version published by the Free Software Foundation;
# with no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts.
# A copy of the license is included in the section entitled "GNU
# Free Documentation License".

AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = it-edit

it_edit_SOURCES = ./Callbacks/gui_callbacks.c \
                  ./Callbacks/edition_callbacks.c \
                  ./Callbacks/terminals_callbacks.c \
                  ./Callbacks/configuration_callbacks.c \
                  ./Callbacks/highlight_selection_callbacks.c \
                  ./Callbacks/editor_callbacks.c \
                  ./Callbacks/files_callbacks.c \
                  ./Callbacks/search_settings_callbacks.c \
                  ./Configuration/configuration.c \
                  ./DEBUG/debug_search_by_signals.c \
                  ./Editor/editor.c \
                  ./FilesIO/files_saver.c \
                  ./GUI/configure/configuration_dialog.c \
                  ./GUI/configure/configure_gui_elements.c \
                  ./GUI/setup/setup_gui.c \
                  ./GUI/dialogs/message_dialog.c \
                  ./GUI/dialogs/dialogs.c \
                  ./Headers/global_vars.c \
                  ./main.c \
                  ./Types/GtkSmartIconButton.c \
                  ./Types/GtkItTerm.c \
                  ./Types/GtkSmartMenuItem.c


              
EXTRA_DIST =  ./Callbacks/edition_callbacks.h \
              ./Callbacks/terminals_callbacks.h \
              ./Callbacks/gui_callbacks.h \
              ./Callbacks/configuration_callbacks.h \
              ./Callbacks/editor_callbacks.h \
              ./Callbacks/highlight_selection_callbacks.h \
              ./Callbacks/files_callbacks.h \
              ./Callbacks/search_settings_callbacks.h \
              ./Configuration/configuration.h \
              ./DEBUG/debug_search_by_signals.h \
              ./Editor/editor.h \
              ./FilesIO/files_saver.h \
              ./GUI/configure/configuration_dialog.h \
              ./GUI/configure/configure_gui_elements.h \
              ./GUI/setup/setup_gui.h \
              ./GUI/dialogs/dialogs.h \
              ./GUI/dialogs/message_dialog.h \
              ./Headers/Structures/GUI_structures.h \
              ./Headers/Structures/setting_structure.h \
              ./Headers/Structures/editor_structures.h \
              ./Headers/global_vars.h \
              ./Headers/includes.h \
              ./Headers/defines.h \
              ./main.h \
              ./Types/GtkSmartIconButton.h \
              ./Types/Enjoy-Using/GtkSmartTooltip.h \
              ./Types/Enjoy-Using/GtkSmartTooltip.c \
              ./Types/GtkItTerm.h \
              ./Types/GtkSmartMenuItem.h
            

          
          
# The warnings inhibitor are used for setting -Wall and -Wextra but without the unwanted warnings: I want a clean output !          
it_edit_CFLAGS = -Wno-unused-parameter -Wno-missing-field-initializers $(GTK_CFLAGS) $(GTKSOURCEVIEW_CFLAGS) $(VTE_2_91_CFLAGS) $(GSPELL_1_CFLAGS)

it_edit_LDADD = $(GTK_LIBS) $(GTKSOURCEVIEW_LIBS) $(VTE_2_91_LIBS) $(GSPELL_1_LIBS)    


if GSPELL_OK

it_edit_CPPFLAGS = -DPREFIX=\"$(DESTDIR)$(prefix)\" -DGSPELL_SUPPORT

else

it_edit_CPPFLAGS = -DPREFIX=\"$(DESTDIR)$(prefix)\"

endif

if PLATFORM_DARWIN

# Add the definition of MACOSX and the gtk-mac-integration and quartz-gtk3 cflags
it_edit_CPPFLAGS += -DMACOSX ${GTK_MAC_INTEGRATION_CFLAGS} ${GTK_QUARTZ_CFLAGS}


#add the gtk-mac-integration and quartz-gtk3 libs
it_edit_LDADD += ${GTK_MAC_INTEGRATION_LIBS} ${GTK_QUARTZ_LIBS}

endif


          
                           
