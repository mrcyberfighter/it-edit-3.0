# fdl (C) 2015-2017 Brüggemann Eddie.
# Permission is granted to copy, distribute and/or modify this document
# under the terms of the GNU Free Documentation License, Version 1.3
# or any later version published by the Free Software Foundation;
# with no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts.
# A copy of the license is included in the section entitled "GNU
# Free Documentation License".

AM_MAKEINFOFLAGS = --no-split
AM_MAKEINFOHTMLFLAGS = --no-headers --no-split

info_TEXINFOS = it-edit.texi

install-data-hook:
	- ${INSTALL_DATA} ./it-edit.info "$(DESTDIR)/${infodir}"
	- if `test -x $(INSTALL_INFO)` ; then $(INSTALL_INFO) --info-dir="$(DESTDIR)$(infodir)" "$(DESTDIR)$(infodir)/it-edit.info" ; fi

uninstall-hook:
	- if `test -f "$(DESTDIR)/${infodir}/it-edit.info"` ; then $(RM) -f "$(DESTDIR)/${infodir}/it-edit.info" ; fi
