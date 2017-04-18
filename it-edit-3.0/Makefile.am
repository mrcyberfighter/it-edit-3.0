# fdl (C) 2015-2017 Brüggemann Eddie.
# Permission is granted to copy, distribute and/or modify this document
# under the terms of the GNU Free Documentation License, Version 1.3
# or any later version published by the Free Software Foundation;
# with no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts.
# A copy of the license is included in the section entitled "GNU
# Free Documentation License".

SUBDIRS = . src data man texi doc desktop

nobase_dist_pkgdata_DATA   =  ./src/DEBUG/debug_search_by_signals.c \
                              ./src/Types/Enjoy-Using/GtkSmartTooltip.c \
                              ./src/Types/GtkSmartIconButton.c \
                              ./src/Types/GtkItTerm.c \
                              ./src/Types/GtkSmartMenuItem.c \
                              ./src/main.c \
                              ./src/Editor/editor.c \
                              ./src/GUI/configure/configuration_dialog.c \
                              ./src/GUI/configure/configure_gui_elements.c \
                              ./src/GUI/setup/setup_gui.c \
                              ./src/GUI/dialogs/message_dialog.c \
                              ./src/GUI/dialogs/dialogs.c \
                              ./src/Callbacks/gui_callbacks.c \
                              ./src/Callbacks/edition_callbacks.c \
                              ./src/Callbacks/terminals_callbacks.c \
                              ./src/Callbacks/configuration_callbacks.c \
                              ./src/Callbacks/highlight_selection_callbacks.c \
                              ./src/Callbacks/editor_callbacks.c \
                              ./src/Callbacks/files_callbacks.c \
                              ./src/Callbacks/search_settings_callbacks.c \
                              ./src/FilesIO/files_saver.c \
                              ./src/Configuration/configuration.c \
                              ./src/Headers/global_vars.c \
                              ./src/DEBUG/debug_search_by_signals.h \
                              ./src/Types/GtkSmartIconButton.h \
                              ./src/Types/Enjoy-Using/GtkSmartTooltip.h \
                              ./src/Types/GtkItTerm.h \
                              ./src/Types/GtkSmartMenuItem.h \
                              ./src/main.h \
                              ./src/Editor/editor.h \
                              ./src/GUI/configure/configuration_dialog.h \
                              ./src/GUI/configure/configure_gui_elements.h \
                              ./src/GUI/setup/setup_gui.h \
                              ./src/GUI/dialogs/dialogs.h \
                              ./src/GUI/dialogs/message_dialog.h \
                              ./src/Callbacks/edition_callbacks.h \
                              ./src/Callbacks/terminals_callbacks.h \
                              ./src/Callbacks/gui_callbacks.h \
                              ./src/Callbacks/configuration_callbacks.h \
                              ./src/Callbacks/editor_callbacks.h \
                              ./src/Callbacks/highlight_selection_callbacks.h \
                              ./src/Callbacks/files_callbacks.h \
                              ./src/Callbacks/search_settings_callbacks.h \
                              ./src/FilesIO/files_saver.h \
                              ./src/Configuration/configuration.h \
                              ./src/Headers/Structures/GUI_structures.h \
                              ./src/Headers/Structures/setting_structure.h \
                              ./src/Headers/Structures/editor_structures.h \
                              ./src/Headers/global_vars.h \
                              ./src/Headers/includes.h \
                              ./src/Headers/defines.h \
                              ./Makefile.mk
                     
EXTRA_DIST = ./Makefile.mk ./texi/it-edit.info ./texi/it-edit.texi ./CONTRIBUTORS

install-data-hook:
	$(MV) $(DESTDIR)$(pkgdatadir)/Makefile.mk $(DESTDIR)$(pkgdatadir)/Makefile
	- if `test ! -d $(HOME)/.local/share` ; then $(MKDIR_P) $(HOME)/.local/share ; $(TOUCH) $(HOME)/.local/share/recently-used.xbel ; fi

