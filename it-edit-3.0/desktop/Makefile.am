# fdl (C) 2015-2017 Brüggemann Eddie.
# Permission is granted to copy, distribute and/or modify this document
# under the terms of the GNU Free Documentation License, Version 1.3
# or any later version published by the Free Software Foundation;
# with no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts.
# A copy of the license is included in the section entitled "GNU
# Free Documentation License".

desktop_file_name_base_on_id = it.edit.mrcyberfighter.desktop

desktopdir  = $(prefix)/share/applications

desktop:
	@echo "[Desktop Entry]" > "$(DESTDIR)$(desktopdir)/$(desktop_file_name_base_on_id)"
	@echo "Name=it-edit" >> "$(DESTDIR)$(desktopdir)/$(desktop_file_name_base_on_id)"
	@echo "GenericName=Integrated Terminals Editor" >> "$(DESTDIR)$(desktopdir)/$(desktop_file_name_base_on_id)"
	@echo "Comment=Integrated terminals programming text editor with severals others functionnalities." >> "$(DESTDIR)$(desktopdir)/$(desktop_file_name_base_on_id)"
	@echo "Exec=$(bindir)/it-edit %U" >> "$(DESTDIR)$(desktopdir)/$(desktop_file_name_base_on_id)"
	@echo "Icon=$(pkgdatadir)/Icons/it-edit_icon.png" >> "$(DESTDIR)$(desktopdir)/$(desktop_file_name_base_on_id)"
	@echo "Terminal=false" >> "$(DESTDIR)$(desktopdir)/$(desktop_file_name_base_on_id)"
	@echo "X-MultipleArgs=false" >> "$(DESTDIR)$(desktopdir)/$(desktop_file_name_base_on_id)"
	@echo "StartupNotify=true" >> "$(DESTDIR)$(desktopdir)/$(desktop_file_name_base_on_id)"
	@echo "Type=Application" >> "$(DESTDIR)$(desktopdir)/$(desktop_file_name_base_on_id)"
	@echo "MimeType=text/x-actionscript;text/x-ada;text/x-adasrc;text/x-asp;application/x-asp;application/x-asap;application/x-awk;text/x-bibtex;text/x-boo;text/x-c;text/x-csrc;image/x-xpixmap;text/x-csharpsrc;text/x-csharp;text/x-c++;text/x-cpp;text/x-c++src;text/x-chdr;text/x-c++hdr;text/x-changelog;text/css;text/x-dsrc;application/x-gnome-app-info;application/x-desktop;text/x-diff;text/x-patch;application/docbook+xml;text/x-dpatch;text/x-dtd;text/x-eiffel;text/x-erlang;text/x-fsharp;text/x-forth;text/x-fortran;text/x-gap;text/x-po;text/x-pot;text/x-pox;text/x-gettext-translation;text/x-gettext-translation-template;text/vnd.graphviz;text/x-gtkrc;text/x-haskell;text/html;text/x-idl;text/x-ini-file;application/x-ini-file;text/x-java;application/javascript;application/x-javascript;text/x-javascript;text/javascript;text/x-js;text/x-tex;text/x-libtool;text/x-literate-haskell;text/x-lua;application/x-m4;text/x-makefile;text/x-markdown;text/x-octave;text/x-matlab;text/x-modelica;text/x-nemerle;text/x-netrexx;text/x-objcsrc;text/x-objective-j;text/x-ocaml;text/x-ocl;text/x-pascal;text/x-perl;application/x-perl;text/x-php;application/x-php;text/x-php-source;application/x-php-source;text/x-pkg-config;text/x-prolog;text/x-protobuf;text/x-python;application/x-python;text/x-R;text/x-rpm-spec;application/x-ruby;text/x-ruby;text/x-scala;text/x-scheme;text/x-shellscript;application/x-shellscript;text/x-sh;application/sparql-query;text/x-sql;text/x-tcl;application/x-tcl;text/x-texinfo;text/x-vala;text/x-vbnet;text/x-vb;text/x-verilog-src;text/x-vhdl;application/xml;text/xml;application/xslt+xml;text/x-yacc;text/x-bison;"  >> "$(DESTDIR)$(desktopdir)/$(desktop_file_name_base_on_id)"
	@echo "Keywords=IDE;Development;Programming;Editor;Terminal;Text;Plaintext;Write;GTK;"  >> "$(DESTDIR)$(desktopdir)/$(desktop_file_name_base_on_id)"
	@echo "Categories=Development;IDE;GTK;"  >> "$(DESTDIR)$(desktopdir)/$(desktop_file_name_base_on_id)"
	@echo "desktop file generated at: $(DESTDIR)$(desktopdir)/$(desktop_file_name_base_on_id)"

update-desktop:
if DESTOP_FILE_UTILS
	- $(DESKTOP_FILE_INSTALL) --rebuild-mime-info-cache "$(desktopdir)/$(desktop_file_name_base_on_id)" ;
	- $(UPDATE_DESKTOP_DATABASE) -q ;
else if XDG_UTILS
	- $(XDG_DESKTOP_MENU)  install --noupdate --novendor --mode system "$(desktopdir)/$(desktop_file_name_base_on_id)" ;
	- $(XDG_DESKTOP_MENU) forceupdate --mode system ;
endif


unupdate-desktop:
if DESTOP_FILE_UTILS
	- $(XDG_DESKTOP_MENU) forceupdate --mode system ;
else if XDG_UTILS
	- $(UPDATE_DESKTOP_DATABASE) -q
endif


install-data-hook:
	- if `test ! -d $(desktopdir)` ; then $(MKDIR_P) $(desktopdir) ; fi
	- $(MAKE) desktop
	- $(MAKE) update-desktop
	$(CHMOD) -R a+rx "$(DESTDIR)$(pkgdatadir)"
	- if `test -f "$(DESTDIR)$(pkgdatadir)/Configuration/it-edit.conf"` ; then $(RM) "$(DESTDIR)$(pkgdatadir)/Configuration/it-edit.conf" ; fi
	- if `test ! -d "$(DESTDIR)$(pkgdatadir)/Configuration"` ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/Configuration" ; fi
	$(CHMOD) -R a+rwx "$(DESTDIR)$(pkgdatadir)/Configuration"
	- if `test ! -d "$(DESTDIR)$(pkgdatadir)/Files_handler"` ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/Files_handler" ; fi
	$(CHMOD) -R a+rwx "$(DESTDIR)$(pkgdatadir)/Files_handler"
	- if `test ! -d "$(DESTDIR)$(pkgdatadir)/Session"` ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/Session" ; fi
	$(CHMOD) -R a+rwx "$(DESTDIR)$(pkgdatadir)/Session"
	- if `test ! -d "$(DESTDIR)$(pkgdatadir)/Icons"` ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/Icons" ; fi
	$(CHMOD) -R a+rwx "$(DESTDIR)$(pkgdatadir)/Icons"
	- if `test ! -d "$(DESTDIR)$(pkgdatadir)/src"` ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/src" ; fi
	$(CHMOD) -R a+rwx "$(DESTDIR)$(pkgdatadir)/src"
	- if `test ! -d "$(DESTDIR)$(pkgdatadir)/bin"` ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/bin" ; fi
	$(CHMOD) -R a+rwx "$(DESTDIR)$(pkgdatadir)/bin"
	@echo "***********************"
	@echo "Enjoy using it-edit !!!"
	@echo "***********************"



uninstall-hook:
	- $(RM) -f "$(desktopdir)/$(desktop_file_name_base_on_id)"
	- $(MAKE) unupdate-desktop
	- $(RM) -R -f "$(DESTDIR)$(pkgdatadir)"


