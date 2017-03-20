# fdl (C) 2015-2017 Brüggemann Eddie.
# Permission is granted to copy, distribute and/or modify this document
# under the terms of the GNU Free Documentation License, Version 1.3
# or any later version published by the Free Software Foundation;
# with no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts.
# A copy of the license is included in the section entitled "GNU
# Free Documentation License".


#  Make invocation
#
#  Make -f [--file ] filename -q [--question] -n [--just-print] ...
#

# Introduction:
#
# They are 2 sort of Makefile (i Think)
# 1. They for making you the life easier.
# 2. They for distributing stuff (see automake from the autotools or cmake).
#

####### Variables. #######
#
# $@  Target filename with extension
# $% Target meber name if the target is an archive member.
# $<  First dependencies in first rule.
# $?  List of all dependencies newer than the target.
# $^  List of all dependencies without repetition.
# $+  List of all dependencies with repetition.
# $*  The steam which match implicit rules.



####### Assigments #######
# define var     value  # Value definition (used for multiline).
# define var =   value  # indirect. (the value change at the next assignment for the final variable value.)
# define var :=  value  # direct.   (the value doesn't change at the next assignment for the final variable value.)
# define var ::= value  # retro and inter compatibility with other make tools.
# define var +=  value  # incr   assigment operator.
# define var ?=  value  # ifndef assigment operator.

# endif # End of a definition.

# target : dependencies    # target can be a target name or a file...
# \t RULE                  # dependencies can be files to update.
#                          # starting with a tab or whatever define in the .RECIPEPREFIX built-in variables.



# CC        is defined per default as the system c-compiler.
# CXX       is defined per default as the system c++-compiler.
#
# COMPILER_FLAGS    isn't defined per default.  # By distributing think about that the user can overwrite it.
# CXXFLAGS  isn't defined per default.  # By distributing think about that the user can overwrite it.
# CPPFLAGS  isn't defined per default.  # By distributing think about that the user can overwrite it.
#
# LDFLAGS   ins't defined per default.  # By distributing think about that the user can overwrite it,
# LDLIBS    isn't defined per default   # but should if he desn't fork the project with additional.

# .PHONY: targets are run everytime you call it with:
# $ make phony_target

# --no-print-directory -> I dislike directory printing behaviour of make: it look's like an error message cause of formatting --print-directory...
# -j       -> This enable the job option the compilation will be faster because it start a job for command simultaneous.
# -j [int] -> You can add an integer to limit the number of jobs you want. 4 jobs is good.
# -O [target|line|recurse|none] -> Type of output synchronizing by using jobs
MAKE_OPTS = --no-print-directory -j4

MAKEFLAGS += $(MAKE_OPTS)

SHELL = /bin/bash

################################################################################

########## [START] directory variables [START] #################################

prefix = /usr/local

exec_prefix = $(prefix)

bindir  = $(prefix)/bin

sbindir = $(prefix)/sbin

libexecdir = $(exec_prefix)/libexec

datarootdir = $(prefix)/share

datadir     = $(datarootdir)

sysconfdir = $(prefix)/etc

sharedstatedir = $(prefix)/com

localstatedir = $(prefix)/var

includedir   = $(prefix)/include

oldincludedir = /usr/include

docdir = $(datarootdir)/doc

infodir = $(datarootdir)/info

htmldir = $(docdir)

dvidir = $(docdir)

pdfdir = $(docdir)

psdir = $(docdir)

libdir  = $(exec_prefix)/lib

lispdir =$(datarootdir)/emacs/site-lisp

localedir = $(datarootdir)/locale

mandir  = $(datarootdir)/man

man1dir = $(mandir)/man1

man2dir = $(mandir)/man2

man3dir = $(mandir)/man3

man4dir = $(mandir)/man4

man5dir = $(mandir)/man5

man6dir = $(mandir)/man6

man7dir = $(mandir)/man7

man7dir = $(mandir)/man8

man9dir = $(mandir)/man9


################################################################################

pkgdatadir      = $(datarootdir)/$(PRGNAME)

pkgincludedir   = $(includedir)/$(PRGNAME)

pkglibdir       = $(libdir)/$(PRGNAME)

pkglibexecdir   = $(libexecdir)/$(PRGNAME)

################################################################################

################################################################################

srcdir  = .

################################################################################

TMP_DIR   = /tmp

# $(prefix)/share/locale
LOCALE_DIR = ./share/locale

GETTEXT_BKUP_DIR = ./gettext_backup

#################### [Start] functions [Start] #################################

# Define a function to check the presence of an executable through his binary path.
BINARY_CHECK = $(shell which $1 2> /dev/null)

BINARY_EXIST = $(if $(call BINARY_CHECK, $1 ), $(call BINARY_CHECK, $1 ))

# Define a function to check if a file exist.
FILE_EXIST   = $(if `test -f $1`,T,F)

PKG_CONFIG_EXIST =  $(if `pkg-config --exists $1`,T,F)

################################################################################


############### [START] Check GNU make tool purpose [START] ####################

# NOTE: Use the defined $(MAKE) variable for calling the make tool in recipes.

MAKE    = $(call BINARY_EXIST, make)

############### [END] Check GNU make tool purpose [END] ########################


############### [START] Check coreutils tools [START] ##########################

MKDIR   = $(call BINARY_EXIST, mkdir)

MKDIR_P = $(MKDIR) -p

BASENAME = $(call BINARY_EXIST, basename)

LS = $(call BINARY_EXIST, ls)

LS_SIZE = $(LS) -s -h

CP = $(call BINARY_EXIST, cp)

CP_R = $(CP) -R

RM = $(call BINARY_EXIST, rm)

RM_R = $(RM) -R

MV  =  $(call BINARY_EXIST, mv)

CHMOD = $(call BINARY_EXIST, chmod)

TOUCH = $(call BINARY_EXIST, touch)

CD = $(call BINARY_EXIST, cd)

LN = $(call BINARY_EXIST, ln)

LN_S = $(LN) -s -T

############### [END] Check coreutils tools [END] ##############################


########## [START] installation variables [START] ##############################

INSTALL  = $(call BINARY_EXIST, install)

INSTALL_PROGRAM = ${INSTALL} -c
INSTALL_DATA    = ${INSTALL} -c -m 644
INSTALL_DATA_RW = ${INSTALL} -c -m 666

DESTDIR =

################################################################################

#################### [Start] Other needed binaries [Start] #####################

# Install the findutils package if missing.
FIND = $(call BINARY_EXIST, find)

# Install the file package if missing.
FILE = $(call BINARY_EXIST, file)

# Package ???
SIZE = $(call BINARY_EXIST, size)

# Package ???
STRIP = $(call BINARY_EXIST, strip)

# Package ???
GREP  = $(call BINARY_EXIST, grep)

# Package ???
SED = $(call BINARY_EXIST, sed)

# Package ???
HEXDUMP = $(call BINARY_EXIST, hexdump)

# Install the install-info package if missing.
INSTALL_INFO = $(call BINARY_EXIST, install-info)

#################### [End] Other needed binaries [End] #########################

########################### [Start]  *.texi tools [Start] ######################

# Install the texinfo package if missing.
MAKEINFO = $(call BINARY_EXIST, makeinfo)

# Install the texlive package if missing.
TEXI2DVI = $(call BINARY_EXIST, texi2dvi)

# *.info files viewer.
INFO = $(call BINARY_EXIST, info)

################################################################################

################## [Start] Documentation viewer checks [Start] #################

XDG_OPEN = $(call BINARY_EXIST, xdg-open)

INFO = $(call BINARY_EXIST, info)

MAN  = $(call BINARY_EXIST, man)

BROWSER = $(call BINARY_EXIST, firefox)

ifeq ($(BROWSER), )
BROWSER = $(call BINARY_EXIST, chromium-browser)
endif

ifeq ($(BROWSER), )
BROWSER = $(call BINARY_EXIST, konqueror)
endif


ODT = $(call BINARY_EXIST, libreoffice)


PDF = $(call BINARY_EXIST, evince)

ifeq ($(PDF), )
PDF = $(call BINARY_EXIST, xpdf)
endif

ifeq ($(PDF), )
PDF = $(call BINARY_EXIST, mupdf)
endif

ifeq ($(PDF), )
PDF = $(call BINARY_EXIST, pdfcube)
endif

ifeq ($(PDF), )
PDF = $(call BINARY_EXIST, pdf-viewer)
endif

EPUB = $(call BINARY_EXIST, fbreader)

ifeq ($(EPUB), )
EPUB = $(call BINARY_EXIST, okular)
endif



# Rescue case for unfounded programs:
ifneq ($(XDG_OPEN), )

ifeq ($(BROWSER), )
BROWSER = $(XDG_OPEN)
endif

ifeq ($(PDF), )
PDF = $(XDG_OPEN)
endif

ifeq ($(ODT), )
ODT = $(XDG_OPEN)
endif

ifeq ($(EPUB), )
EPUB = $(XDG_OPEN)
endif


ifeq ($(INFO), )
INFO = $(XDG_OPEN)
endif


ifeq ($(MAN), )
MAN = $(XDG_OPEN)
endif

endif


#################### [END] Check documentation tools [END] #####################

#################### [START] Check debugging and profiling tools [START] #######
# DEBUGGING AND PROFILING programs and settings

# gprof binary check.
GPROF  = $(call BINARY_EXIST, gprof)

# nm binary check.
# nm: display symbols of an object file or a program (named a.out) per default.
NM      = $(call BINARY_EXIST, nm)


# readelf binary check.
# readelf: display informations about a program (a ELF (Executable Linkable Format) binary).
READELF = $(call BINARY_EXIST, readelf)

# strace binary check.
# strace: Intercepts  and  records  the  system  calls  which are called by a
#         process and the signals which are received by a process.
STRACE  = $(call BINARY_EXIST, strace)


# ltrace binary check.
# ltrace: Trace library calls of a given program.
LTRACE  = $(call BINARY_EXIST, ltrace)


# objdump binary check.
# objdump: Display the informations about the object file(s).
OBJDUMP = $(call BINARY_EXIST, objdump)

# ldd: ldd  print the  shared  libraries  required by each program or shared
#      library specified on the command line.
#
# NOTE: A  safer alternative when dealing with untrusted executables is:
# $ objdump -p /path/to/program | grep NEEDED
LDD = $(call BINARY_EXIST, ldd)


# diff: diff print difference between 2 or more files
#
#
# NOTE: diff can work recursively on a folder.
# SUGGEST: the kompare diff-gui program.
DIFF = $(call BINARY_EXIST, diff)

#################################################################################


#################### [START] gettext support binaries [START] ##################

XGETTEXT = $(call BINARY_EXIST, xgettext)

MSGFMT = $(call BINARY_EXIST, msgfmt)

MSGUNIQ = $(call BINARY_EXIST, msguniq)

################################################################################


#################### [START] archiving and compressing utilities [START] #######

ZIP     = $(call BINARY_EXIST, zip)

TAR     = $(call BINARY_EXIST, tar)

GZ      = $(call BINARY_EXIST, gzip)

BZ2     = $(call BINARY_EXIST, bzip2)

#################### [END] archiving and compressing utilities [END] ###########

############### Compiler settings ##################

# Programming language choosen by the user.

# IMPORTANT: Do not comment this variables and remove trailing spaces.
PRG_LANG = c

# IMPORTANT: Do not comment this variables and remove trailing spaces.
EXT_SRC = .c

# IMPORTANT: Do not comment this variables and remove trailing spaces.
EXT_HDR = .h

# Compiler according user choice or automatique to language.
COMPILER = $(CC)



WALL =  -Wall

WEXTRA = -Wextra

WARNS = $(WALL) $(WEXTRA)

#ifeq ($(CC),gcc)
WARNS += -Wno-unused-parameter -Wno-missing-field-initializers
#endif

# Use the increment operator so that the user can define the flags he want.
CFLAGS += -std=c99 -g -O2 -finline-functions

#ifeq ($(CC),gcc)
#CFLAGS += -fstack-protector -finline-functions-called-once -Waggressive-loop-optimizations
#endif

CPPFLAGS =

GTK3 = $(call PKG_CONFIG_EXIST, gtk+-3.0)

SOURCEVIEW3 = $(call PKG_CONFIG_EXIST, gtksourceview-3.0)

VTE_2_91 = $(call PKG_CONFIG_EXIST, vte-2.91)

GSPELL1 = $(call PKG_CONFIG_EXIST, gspell-1)

LDFLAGS =
LDLIBS  =


GSPELL_SUPPORT =

ifeq ($(GTK3), T)
LDFLAGS += `pkg-config --cflags gtk+-3.0`
LDLIBS  += `pkg-config --libs gtk+-3.0`
endif

ifeq ($(SOURCEVIEW3), T)
LDFLAGS += `pkg-config --cflags gtksourceview-3.0`
LDLIBS  += `pkg-config --libs gtksourceview-3.0`
endif

ifeq ($(VTE_2_91), T)
LDFLAGS += `pkg-config --cflags vte-2.91`
LDLIBS  += `pkg-config --libs vte-2.91`
endif

ifeq ($(GSPELL1), T)
LDFLAGS += `pkg-config --cflags gspell-1`
LDLIBS  += `pkg-config --libs gspell-1`
GSPELL_SUPPORT = -DGSPELL_SUPPORT
else

ifeq ($(MAKECMDGOALS),)
$(warning )
$(warning "gpsell-1 not detected !")
$(warning "Install the library gspell-1 (if available) and reinstall it-edit.")
$(warning )
endif

endif



#LDFLAGS = `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gtksourceview-3.0` `pkg-config --cflags vte-2.91` `pkg-config --cflags gspell-1`
#LDLIBS  = `pkg-config --libs gtk+-3.0` `pkg-config --libs gtksourceview-3.0`  `pkg-config --libs vte-2.91` `pkg-config --libs gspell-1`




# IMPORTANT: Do not comment this variables and remove trailing spaces.
USE_PRG_LIB = F


BIN_DIR = ./bin

PRGNAME = it-edit

PRGPATH = $(BIN_DIR)/$(PRGNAME)

VERSION = 3.0.x




SRC_DIR   = ./src

SRC_FILES = $(SRC_DIR)/main.c \
            $(SRC_DIR)/Callbacks/configuration_callbacks.c \
            $(SRC_DIR)/Callbacks/edition_callbacks.c \
            $(SRC_DIR)/Callbacks/editor_callbacks.c \
            $(SRC_DIR)/Callbacks/files_callbacks.c \
            $(SRC_DIR)/Callbacks/gui_callbacks.c \
            $(SRC_DIR)/Callbacks/highlight_selection_callbacks.c \
            $(SRC_DIR)/Callbacks/search_settings_callbacks.c \
            $(SRC_DIR)/Callbacks/terminals_callbacks.c \
            $(SRC_DIR)/GUI/configure/configuration_dialog.c \
            $(SRC_DIR)/GUI/configure/configure_gui_elements.c \
            $(SRC_DIR)/GUI/dialogs/message_dialog.c \
            $(SRC_DIR)/GUI/dialogs/dialogs.c \
            $(SRC_DIR)/GUI/setup/setup_gui.c \
            $(SRC_DIR)/Editor/editor.c \
            $(SRC_DIR)/Configuration/configuration.c \
            $(SRC_DIR)/Headers/global_vars.c \
            $(SRC_DIR)/Types/GtkItTerm.c \
            $(SRC_DIR)/Types/GtkSmartMenuItem.c \
            $(SRC_DIR)/Types/GtkSmartIconButton.c \
            $(SRC_DIR)/FilesIO/files_saver.c \
            $(SRC_DIR)/DEBUG/debug_search_by_signals.c

#           $(SRC_DIR)/Types/GtkSmartFileTooltip.c \
#           $(SRC_DIR)/Types/GtkSmartFileTooltip.c \

HDR_DIR   =

HDR_FILES =   $(SRC_DIR)/main.h \
              $(SRC_DIR)/Callbacks/configuration_callbacks.h \
              $(SRC_DIR)/Callbacks/edition_callbacks.h \
              $(SRC_DIR)/Callbacks/editor_callbacks.h \
              $(SRC_DIR)/Callbacks/files_callbacks.h \
              $(SRC_DIR)/Callbacks/gui_callbacks.h \
              $(SRC_DIR)/Callbacks/highlight_selection_callbacks.h \
              $(SRC_DIR)/Callbacks/search_settings_callbacks.h \
              $(SRC_DIR)/Callbacks/terminals_callbacks.h \
              $(SRC_DIR)/GUI/configure/configuration_dialog.h \
              $(SRC_DIR)/GUI/configure/configure_gui_elements.h \
              $(SRC_DIR)/GUI/dialogs/message_dialog.h \
              $(SRC_DIR)/GUI/dialogs/dialogs.h \
              $(SRC_DIR)/GUI/setup/setup_gui.h \
              $(SRC_DIR)/Editor/editor.h \
              $(SRC_DIR)/Configuration/configuration.h \
              $(SRC_DIR)/DEBUG/debug_search_by_signals.h \
              $(SRC_DIR)/Headers/global_vars.h \
              $(SRC_DIR)/Types/GtkItTerm.h \
              $(SRC_DIR)/Types/GtkSmartMenuItem.h \
              $(SRC_DIR)/Types/GtkSmartIconButton.h \
              $(SRC_DIR)/FilesIO/files_saver.h \
              $(SRC_DIR)/DEBUG/debug_search_by_signals.h


#              $(SRC_DIR)/Types/GtkSmartFileTooltip.h \
#              $(SRC_DIR)/Types/GtkSmartIconButton.h \

# Don't work as expect.
#ifeq ($(MAKECMDGOALS),ddebug)
#SRC_FILES += $(SRC_DIR)/DEBUG/debug_search_by_signals.c
#endif


S_FILES   = $(SRC_FILES:$(EXT_SRC)=.s)

CPP_FILES = $(SRC_FILES:$(EXT_SRC)=.i)

OBJECTS = $(SRC_FILES:.c=.o)



#NOTE: not GNU make tools have less capabilitites than the original tool.
ifneq ($(notdir $(MAKE)),make)
$(warning You don't use the GNU make tool: this can go into incompatiblities with this Makefile)
endif






# The default or all target is the default target (wenn you only enter $ make) or the first target encounter in the Makefile.
#all: $(PRGPATH)

#################### [START] Program library target [START] #####################

ifeq ($(USE_PRG_LIB),T)

LIB_NAME = lib$(PRGNAME)

LIB_SRC_PATH = $(SRC_DIR)/$(LIB_NAME)

LIB_PATH = $(LIB_SRC_PATH)/$(LIB_NAME)

$(LIB_NAME):
  cd $(LIB_SRC_PATH) ; $(MAKE)

else

LIB_PATH =

endif

################################################################################


# First target appearing is the implicte variable: .DEFAULT: target.
$(PRGPATH):  $(BIN_DIR) $(LIB_PATH) $(OBJECTS)
	$(COMPILER)   $(DEBUG_FLAG) $(CPPFLAGS) $(GSPELL_SUPPORT) $(CFLAGS) $(WARNS) $(LDFLAGS) $(DEBUG_OPTION)  $(PG_OPTION) -o $(PRGPATH) $(OBJECTS) $(LIB_PATH) $(LDLIBS)


.PHONY: $(BIN_DIR)
$(BIN_DIR):
	- @if `test ! -d $(BIN_DIR)` ; then  $(MKDIR_P) $(BIN_DIR) ; fi


%.o: %$(EXT_SRC)
	$(COMPILER)  $(DEBUG_FLAG) $(CPPFLAGS) $(GSPELL_SUPPORT) $(CFLAGS) $(WARNS) $(LDFLAGS) $(DEBUG_OPTION)  $(PG_OPTION)  -c $< -o $@ $(LDLIBS)


ifeq ($(MAKECMDGOALS),as-att)

AS_SYNTAX = att

else

ifeq ($(MAKECMDGOALS),as-intel)

AS_SYNTAX = intel

else

ifeq ($(MAKECMDGOALS),as-power)

AS_SYNTAX = power

endif
endif
endif


SYNTAX_AS_OPT = -masm $(AS_SYNTAX)

AS_OPTS =

as-intel : assembly

as-att : assembly

as-power: assembly

assembly: $(S_FILES)

%.s : %$(EXT_SRC)
	$(COMPILER) $(CPPFLAGS) $(LDFLAGS) -S $(SYNTAX_AS_OPTS)  $(AS_OPTS) $< -o $@ $(LDLIBS)


preprocessor: $(CPP_FILES)

%.i : %$(EXT_SRC)
	$(COMPILER) $(CPPFLAGS) $(LDFLAGS) -E $< -o $@ $(LDLIBS)

################################################################################

sphinx-html:
	cd ./documentation/Notice ; $(MAKE) html

sphinx-show-html:
	$(BROWSER) ./documentation/Notice/build/html/index.html

sphinx-info:
	cd ./documentation/Notice ; $(MAKE) info

sphinx-show-info:
	cd ./documentation/Notice/build/texinfo ; $(INFO) -f ./it-edit.info

sphinx-man:
	cd ./documentation/manpage ; $(MAKE) man

sphinx-show-man:
	cd ./documentation/manpage/build/man ; $(MAN) -l it-edit.1


#################### [START] gettext support [START] ###########################



ifneq ($(XGETTEXT), )
XGETTEXT_OPTS = -L C --keyword=_ --keyword=N_ --keyword=Q_ -d $(PRGNAME)
.PHONY: xgettext
xgettext:
	$(XGETTEXT) $(XGETTEXT_OPTS) $(SRC_FILES)
	$(MV) messages.po $(PRGNAME).pot
endif

lang=


ifneq ($(MSGFMT), )
MSGFMT_OPTS = -v -f --strict
.PHONY: msgfmt
msgfmt:
ifneq ($(lang), )
	if `test ! -d $(LOCALE_DIR)/$(lang)/LC_MESSAGES` ; then $(MKDIR_P) $(LOCALE_DIR)/$(lang)/LC_MESSAGES ; fi
	$(MSGFMT) $(MSGFMT_OPTS) $ $(PRGNAME).pot -o $(LOCALE_DIR)/$(lang)/LC_MESSAGES/$(PRGNAME).mo
	if `test ! -d $(GETTEXT_BKUP_DIR)/$(lang)` ;       then $(MKDIR_P) $(GETTEXT_BKUP_DIR)/$(lang)       ; fi
	$(CP) $(PRGNAME).pot $(GETTEXT_BKUP_DIR)/$(lang)/
	@echo
	@echo "Now you can try your translation"
	@echo "by setting the value of the \$$LANGUAGE variable:"
	@echo "$$ export LANGUAGE=fr:en      # Language on French with English fallback."
	@echo "$$ export set LANGUAGE=fr:en  # Language on French with English fallback."
	@echo "$$ declare LANGUAGE=fr:en     # Language on French with English fallback."
else
	@echo  "You must define the lang variable on the commandline."
	@echo  "Per example \"make msgfmt lang=fr\" for french."
endif

endif


################################################################################

#################### [START] Debugging targets [START] #########################

.PHONY: fdebug ddebug gdebug gdb

# File debug.
fdebug:
	- $(RM) -f ./debug.txt
	$(MAKE) $(PRGPATH) 2> ./debug.txt

# Define -DDEBUG
ddebug:
	$(MAKE) -B DEBUG_FLAG=-DDEBUG

# Compile with -g debugging option
gdebug:
	$(MAKE) -B $(PRGPATH) DEBUG_OPTION=-g

# Launch interactive gdb (gnu debugger).
gdb:
	cd $(BIN_DIR) ; gdb ./$(PRGNAME)

#################### [END] Debugging targets [END] #############################


#################### [START] program analyze tools targets [START] #############

# Default -pg option filename.
PG_FILE = $(BIN_DIR)/gmon.out

ifeq ($(MAKECMDGOALS),gprof)
test_gmon = $(call FILE_EXIST, $(PG_FILE))
ifneq ($(test_gmon),T)
$(info  )
$(info ***************************************************************)
$(info You must first execute your program so that it can be profiled,)
$(info it generate the profiling file gmon.out by runtime,)
$(info then you can analyse it content with gprof...)
$(info ***************************************************************)
$(info  )
$(error $(PG_FILE) missing)
endif
endif


.PHONY: pg gprof
pg:
	$(MAKE) -B clean_pg
	$(MAKE) clean
	$(MAKE) PG_OPTION=-pg

ifneq ($(GPROF), )
# You must execute your program to generate the gmon.out file.
# Edit the wanted options to give to gprof program.
GPROF_OPTS =
gprof: $(PG_FILE)
	$(GPROF) $(GPROF_OPTS) $(PRGPATH) $(PG_FILE)
endif

ifneq ($(STRACE), )
# Edit the wanted options to give to strace program.
STRACE_OPTS =
.PHONY: strace
strace:
	$(STRACE) $(STRACE_OPTS) $(PRGPATH)
endif

ifneq ($(LTRACE), )
# Edit the wanted options to give to strace program.
LTRACE_OPTS =
.PHONY: ltrace
ltrace:
	$(LTRACE) $(STRACE_OPTS) $(PRGPATH)
endif

ifneq ($(NM), )
# Edit the wanted options to give to nm program.
NM_OPTS =
.PHONY: nm
nm:
	$(NM) $(NM_OPTS) $(PRGPATH)
endif

ifneq ($(READELF), )
# Edit the wanted options to give to readelf program.
READELF_OPTS =
.PHONY: readelf
readelf:
	$(READELF) $(READELF_OPTS) $(PRGPATH)
endif

ifneq ($(OBJDUMP), )
# Edit the wanted options to give to objdump program. # One is required from the program.
OBJDUMP_OPTS = -p
# objdump require an option.
.PHONY: objdump
objdump:
	$(OBJDUMP) $(OBJDUMP_OPTS) $(PRGPATH)
endif

ifneq ($(LDD), )
# Edit the wanted options to give to ldd program.
LDD_OPTS =
.PHONY: ldd
ldd:
	$(LDD) $(LDD_OPTS) $(PRGPATH)
endif

ifneq ($(HEXDUMP), )
# Edit the wanted options to give to hexdump program.
HEXDUMP_OPTS =
.PHONY: hexdump
hexdump:
	$(HEXDUMP) $(HEXDUMP_OPTS) $(PRGPATH)
endif

ifneq ($(DIFF), )
# Edit the wanted options to give to diff program.
# -q -y  -t --suppress-blank-empty --suppress-common-lines --show-c-function
DIFF_OPTS =
.PHONY: diff
diff:
ifneq ($(argv), )
	$(DIFF) $(DIFF_OPTS) "$(argv)"
else
	@echo "You must provide 2 filepath given on the cmdline:"
	@echo "$ make diff argv=\"filepath1 filepath2\""
endif
endif

#################### [END] program analyze tools targets [END] #################


CUR_PATH = .


#################### [START] Distributing targets [START] ######################

COMPRESSION = 9

DIST_DIR  = ./dist

DIST_NAME = $(PRGNAME)-$(VERSION)

DIST_EXCLUDE_FOLDER = bin dist

TAR_EXCLUDE_CMD = $(foreach var, $(DIST_EXCLUDE_FOLDER), --exclude=$(var)/*)

ZIP_EXCLUDE_CMD = $(foreach var, $(DIST_EXCLUDE_FOLDER), -x $(var)/\*)

# .PHONY: targets are run everytime you call it with:
# $ make phony_target

.PHONY: $(DIST_DIR) dist-zip dist-tar dist-tar-gz dist-tar-bz2

$(DIST_DIR):
	@- if `test ! -d $(DIST_DIR)` ; then $(MKDIR_P) $(DIST_DIR) ; fi

ifneq ($(ZIP), )
dist-zip: $(DIST_DIR)
	$(MAKE) clean
	$(ZIP) -r  $(ZIP_EXCLUDE_CMD)  -$(COMPRESSION) -o -v  $(TMP_DIR)/$(DIST_NAME).zip .
	$(ZIP) $(TMP_DIR)/$(DIST_NAME).zip $(DIST_EXCLUDE_FOLDER)
	$(CP) $(TMP_DIR)/$(DIST_NAME).zip $(DIST_DIR)
	$(ZIP) -T $(DIST_DIR)/$(DIST_NAME).zip
	- $(RM) -f $(TMP_DIR)/$(DIST_NAME).zip
	ls -s -h $(DIST_DIR)/$(DIST_NAME).zip
endif

ifneq ($(TAR), )
$(DIST_NAME).tar: $(DIST_DIR)
	$(MAKE) clean
	$(TAR) $(TAR_EXCLUDE_CMD) -W -cvf  $(TMP_DIR)/$(DIST_NAME).tar .


dist-tar: $(DIST_NAME).tar
	$(CP) $(TMP_DIR)/$(DIST_NAME).tar $(DIST_DIR)
	$(TAR) --test-label $(DIST_DIR)/$(DIST_NAME).tar
	ls -s -h $(DIST_DIR)/$(DIST_NAME).tar
endif

ifneq ($(GZ), )
dist-tar-gz: $(DIST_DIR) $(DIST_NAME).tar
	$(MAKE) clean
	$(GZ) -v -$(COMPRESSION) $(TMP_DIR)/$(DIST_NAME).tar
	$(CP) $(TMP_DIR)/$(DIST_NAME).tar.gz $(DIST_DIR)
	$(GZ) -v -t $(DIST_DIR)/$(DIST_NAME).tar.gz
	- $(RM) -f $(TMP_DIR)/$(DIST_NAME).tar $(TMP_DIR)/$(DIST_NAME).tar.gz
	ls -s -h $(DIST_DIR)/$(DIST_NAME).tar.gz
endif

ifneq ($(BZ2), )
dist-tar-bz2: $(DIST_DIR) $(DIST_NAME).tar
	$(MAKE) clean
	$(BZ2) -v -$(COMPRESSION) $(TMP_DIR)/$(DIST_NAME).tar
	$(CP) $(TMP_DIR)/$(DIST_NAME).tar.bz2 $(DIST_DIR)
	$(BZ2) -v -t $(DIST_DIR)/$(DIST_NAME).tar.bz2
	- $(RM) -f $(TMP_DIR)/$(DIST_NAME).tar $(TMP_DIR)/$(DIST_NAME).tar.bz2
	ls -s -h $(DIST_DIR)/$(DIST_NAME).tar.bz2
endif

#################### [END] Distributing targets [END] ##########################


#################### [START] cleaning targets [START] ##########################

.PHONY: clean_all clean_o clean_lib clean_save clean_prg clean_pg clean clean_doc clean_s clean_cpp

clean_prg:
	- $(RM) -f $(PRGPATH)

ifneq ($(FIND), )
.PHONY: clean_save
clean_save:
	$(FIND) . -iregex .*~$$ -exec rm -f {} \;
endif

clean_pg:
	- $(RM) -f $(PG_FILE)

clean_o:
	- $(RM) -f $(OBJECTS)

clean_lib:
	- $(RM) -f $(LIB_PATH)

clean_cpp:
	- $(RM) -f $(CPP_FILES)

clean_s:
	- $(RM) -f $(S_FILES)

.PHONY: clean_doc
clean_doc:
	- $(RM) -f $(INFO_FILE) $(TEXT_FILE) $(PDF_FILE) $(HTML_FILE) $(HTML5_FILE) $(TEXI_FILE) $(ODT_FILE) $(XML_FILE) $(LATEX_FILE) $(ASCIIDOC_FILE) $(EPUB_FILE)

.PHONY: clean_doc_gen
clean_doc_gen:
	- $(RM) -f $(TEXI_FILE) $(TEXI_MAN_FILE) $(TEXI_INFO_FILE) $(TEXI_PDF_FILE) $(TEXI_HTML_FILE) $(TEXI_HTML5_FILE) $(TEXI_ODT_FILE) $(TEXI_XML_FILE) $(TEXI_LATEX_FILE) $(TEXI_ASCIIDOC_FILE) $(TEXI_EPUB_FILE) $(TEXI_PLAIN_FILE) \
	                        $(RST_MAN_FILE)  $(RST_INFO_FILE)  $(RST_PDF_FILE)  $(RST_HTML_FILE)  $(RST_HTML5_FILE)  $(RST_ODT_FILE)  $(RST_XML_FILE)  $(RST_LATEX_FILE)  $(RST_ASCIIDOC_FILE)  $(RST_EPUB_FILE)  $(RST_PLAIN_FILE) \
	                        $(MD_MAN_FILE)   $(MD_INFO_FILE)   $(MD_PDF_FILE)   $(MD_HTML_FILE)   $(MD_HTML5_FILE)   $(MD_ODT_FILE)   $(MD_XML_FILE)   $(MD_LATEX_FILE)   $(MD_ASCIIDOC_FILE)   $(MD_EPUB_FILE)   $(MD_PLAIN_FILE)



clean_all: clean_o clean_prg clean_save clean_pg clean_s clean_lib clean_cpp

clean: clean_o clean_prg

#################### [END] cleaning targets [END] ##############################


#################### [START] utilities targets [START] #########################

.PHONY: strip info exec

ifneq ($(FIND), )
.PHONY: list-project
list-project:
	$(FIND) * -exec file {} \;
endif



ifneq ($(STRIP), )
strip:
	strip $(PRGPATH)
endif

# Put the informations you need in this target.
info:
	@echo
	@ls -l $(PRGPATH) ; # Must make a personnal tool displaying file size in K but with the comma rest.
	@echo
ifneq ($(FILE), )
	@file $(PRGPATH)  ;
	@echo
endif
ifneq ($(SIZE), )
	@size $(PRGPATH)  ;
	@echo
endif
	@ls -s -h $(PRGPATH) ;
	@echo



.PHONY: set_x
set_x:
	@ if `test ! -x $(PRGPATH)` ; then $(CHMOD) u+x $(PRGPATH) ; fi


# Overwrite on the cmdline or set it in Makefile if your program need some args.
# or call make by defining this variable:
# $ make argv="foo bar" exec
exec:
# The execute bit removed can append by sharing executable on different device or file system.
	@ $(MAKE) set_x
	cd $(BIN_DIR) ; ./$(PRGNAME) $(argv)


ifneq ($(GREP), )
GREP_OPTS = -n --color
.PHONY: search-pattern
search-pattern:
	cd $(SRC_DIR) ; $(GREP) $(GREP_OPTS) -r "$(argv)"
endif

ifneq ($(FIND), )
ifneq ($(SED), )
.PHONY: rm-trailing-spaces
rm-trailing-spaces:
	$(FIND) $(SRC_DIR) -iregex .*\$(EXT_SRC) -exec $(SED) --in-place "s/[[:space:]]$$//" {} \;
	$(FIND) $(SRC_DIR) -iregex .*\$(EXT_HDR) -exec $(SED) --in-place "s/[[:space:]]$$//" {} \;
endif
endif


ifneq ($(SED), )
.PHONY: replace-in-file
replace-in-file:
ifneq ($(pattern), )
	@if `test -f $(filepath)` ; \
	then $(SED) --in-place "s/$(pattern)/$(replace)/" $(filepath) ; \
	else echo "$(filepath) is not a valid filepath" ; fi
else
	@echo "You must provide a pattern to replace !"
	@echo "$ make replace-in-file pattern=\"\" replace=\"\" filepath=\"\""
endif
endif

ifneq ($(FIND), )
ifneq ($(SED), )
.PHONY: replace-all
replace-all:
ifneq ($(pattern), )
	$(FIND) $(SRC_DIR) -iregex .*\$(EXT_SRC) -exec $(SED) --in-place "s/$(pattern)/$(replace)/" {} \;
	$(FIND) $(SRC_DIR) -iregex .*\$(EXT_HDR) -exec $(SED) --in-place "s/$(pattern)/$(replace)/" {} \;
else
	@echo "You must provide a pattern to replace !"
	@echo "$ make replace-all pattern=\"\" replace=\"\""
endif
endif
endif

.PHONY: prepend-license
prepend-license:
ifneq ($(BOILERPLATE_PATH), )
ifneq ($(PRG_LANG), )
ifneq ($(PYTHON), )
ifneq ($(CHMOD), )
	$(CHMOD) a+rx $(PREPEND_LICENSE_SCRIPT)
	$(PYTHON) $(PREPEND_LICENSE_SCRIPT) -d $(SRC_DIR) -f $(BOILERPLATE_PATH) -l $(PRG_LANG)
#	$(PYTHON) $(PREPEND_LICENSE_SCRIPT) -d $(HDR_DIR) -f $(BOILERPLATE_PATH) -l $(PRG_LANG)
endif
endif
endif
endif


#### [START] test installation targets [START] ####

.PHONY: desktop install-info install-man install-doc install-locale install

desktopdir  = /usr/share/applications

desktop:
	@echo "[Desktop Entry]" > "$(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"
	@echo "Name=it-edit" >> "$(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"
	@echo "GenericName=Integrated Terminals Editor" >> "$(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"
	@echo "Comment=Integrated terminals programming text editor with severals others functionnalities." >> "$(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"
	@echo "Exec=$(bindir)/it-edit %U" >> "$(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"
	@echo "Icon=$(pkgdatadir)/Icons/it-edit_icon.png" >> "$(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"
	@echo "Terminal=false" >> "$(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"
	@echo "X-MultipleArgs=false" >> "$(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"
	@echo "StartupNotify=true" >> "$(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"
	@echo "Type=Application" >> "$(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"
	@echo "MimeType=text/x-actionscript;text/x-ada;text/x-adasrc;text/x-asp;application/x-asp;application/x-asap;application/x-awk;text/x-bibtex;text/x-boo;text/x-c;text/x-csrc;image/x-xpixmap;text/x-csharpsrc;text/x-csharp;text/x-c++;text/x-cpp;text/x-c++src;text/x-chdr;text/x-c++hdr;text/x-changelog;text/css;text/x-dsrc;application/x-gnome-app-info;application/x-desktop;text/x-diff;text/x-patch;application/docbook+xml;text/x-dpatch;text/x-dtd;text/x-eiffel;text/x-erlang;text/x-fsharp;text/x-forth;text/x-fortran;text/x-gap;text/x-po;text/x-pot;text/x-pox;text/x-gettext-translation;text/x-gettext-translation-template;text/vnd.graphviz;text/x-gtkrc;text/x-haskell;text/html;text/x-idl;text/x-ini-file;application/x-ini-file;text/x-java;application/javascript;application/x-javascript;text/x-javascript;text/javascript;text/x-js;text/x-tex;text/x-libtool;text/x-literate-haskell;text/x-lua;application/x-m4;text/x-makefile;text/x-markdown;text/x-octave;text/x-matlab;text/x-modelica;text/x-nemerle;text/x-netrexx;text/x-objcsrc;text/x-objective-j;text/x-ocaml;text/x-ocl;text/x-pascal;text/x-perl;application/x-perl;text/x-php;application/x-php;text/x-php-source;application/x-php-source;text/x-pkg-config;text/x-prolog;text/x-protobuf;text/x-python;application/x-python;text/x-R;text/x-rpm-spec;application/x-ruby;text/x-ruby;text/x-scala;text/x-scheme;text/x-shellscript;application/x-shellscript;text/x-sh;application/sparql-query;text/x-sql;text/x-tcl;application/x-tcl;text/x-texinfo;text/x-vala;text/x-vbnet;text/x-vb;text/x-verilog-src;text/x-vhdl;application/xml;text/xml;application/xslt+xml;text/x-yacc;text/x-bison;"  >> "$(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"
	@echo "Keywords=ITE;Development;Programming;Editor;Terminal;Text;Plaintext;Write;"  >> "$(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"
	@echo "Categories=Development;IDE;GTK;"  >> "$(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"
	@echo "desktop file generated at: $(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"

XDG_DESKTOP_MENU = $(call BINARY_EXIST, xdg-desktop-menu)

update-desktop:
	- $(XDG_DESKTOP_MENU) install --novendor --mode system "$(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"
	- $(XDG_DESKTOP_MENU) forceupdate --mode system

unupdate-desktop:
	- $(XDG_DESKTOP_MENU)  uninstall --novendor --mode system "$(DESTDIR)$(desktopdir)/it.edit.mrcyberfighter.desktop"
	- $(XDG_DESKTOP_MENU) forceupdate --mode system

install-info:
	- if `test ! -d "$(DESTDIR)$(infodir)"` ; then $(MKDIR_P) "$(DESTDIR)$(infodir)" ; fi
	- $(INSTALL) ./texi/it-edit.info "$(DESTDIR)$(infodir)"

install-man:
	- if `test ! -d "$(DESTDIR)$(man1dir)"` ; then $(MKDIR_P) "$(DESTDIR)$(man1dir)" ; fi
	- $(INSTALL) ./man/it-edit.1 "$(DESTDIR)$(man1dir)"

install-doc:
	- if `test ! -d $(DESTDIR)$(docdir)` ; then $(MKDIR_P) "$(DESTDIR)$(docdir)" ; fi
	- $(INSTALL) ./doc/it-edit.html "$(DESTDIR)$(docdir)"
	- $(INSTALL) ./doc/it-edit.pdf  "$(DESTDIR)$(docdir)"

install-locale:
	- if `test ! -d "$(localedir)/fr/LC_MESSAGES"` ; then $(MKDIR_P) $(localedir)/fr/LC_MESSAGES ; fi
	- if `test ! -d "$(prefix)/share/locale/de/LC_MESSAGES"` ; then $(MKDIR_P) $(prefix)/share/locale/de/LC_MESSAGES ; fi
	- if `test -f   "$(srcdir)/share/locale/de/LC_MESSAGES/it-edit.mo"` ; then $(INSTALL_DATA) "$(srcdir)/share/locale/de/LC_MESSAGES/it-edit.mo" "$(localedir)/de/LC_MESSAGES" ; fi
	- if `test -f   "$(srcdir)/share/locale/fr/LC_MESSAGES/it-edit.mo"` ; then $(INSTALL_DATA) "$(srcdir)/share/locale/fr/LC_MESSAGES/it-edit.mo" "$(localedir)/fr/LC_MESSAGES" ; fi


mkdir-data:
	if `test ! -d "$(DESTDIR)$(pkgdatadir)/Configuration"` ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/Configuration" ; fi
	if `test ! -d "$(DESTDIR)$(pkgdatadir)/Files_handler"` ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/Files_handler" ; fi
	if `test ! -d "$(DESTDIR)$(pkgdatadir)/Icons"`         ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/Icons"         ; fi
	if `test ! -d "$(DESTDIR)$(pkgdatadir)/License"`       ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/License"       ; fi
	if `test ! -d "$(DESTDIR)$(pkgdatadir)/README"`        ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/README"        ; fi
#	if `test ! -d "$(DESTDIR)$(pkgdatadir)/Session"`       ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/Session"       ; fi
	if `test ! -d "$(DESTDIR)$(pkgdatadir)/Styles"`        ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/Styles"        ; fi

cp-data:
	$(CP_R) $(srcdir)/share/$(PRGNAME)/Icons/*    "$(DESTDIR)$(pkgdatadir)/Icons/"
	$(CP_R) $(srcdir)/share/$(PRGNAME)/License/*  "$(DESTDIR)$(pkgdatadir)/License/"
	$(CP_R) $(srcdir)/share/$(PRGNAME)/Configuration/*  "$(DESTDIR)$(pkgdatadir)/Configuration/"
#	 $(CP_R) $(srcdir)/share/$(PRGNAME)/README/*   "$(DESTDIR)$(pkgdatadir)/README/"
#	$(CP_R) $(srcdir)/share/$(PRGNAME)/Session/*   "$(DESTDIR)$(pkgdatadir)/Session/"
#	$(CP_R) $(srcdir)/share/$(PRGNAME)/Styles/*   "$(DESTDIR)$(pkgdatadir)/Styles/"

chmod-data:
	$(CHMOD) a+rwx "$(DESTDIR)$(pkgdatadir)/Configuration"
	$(CHMOD) a+rwx "$(DESTDIR)$(pkgdatadir)/Files_handler"
#	$(CHMOD) a+rwx "$(DESTDIR)$(pkgdatadir)/Session"
	$(CHMOD) -R a+rwx "$(DESTDIR)$(pkgdatadir)/Icons"
	$(CHMOD) -R 0755 "$(DESTDIR)$(pkgdatadir)/Styles"

.PHONY: gspell-warning

gspell-warning:
	@echo
	@echo gspell-1 not detected !
	@echo Install the library gspell-1 and reinstall it-edit
	@echo for getting spell check support.
	@echo By debian packages try the ppa gnome3-staging for getting the gspell-1 library.
	@echo To know that gspell-1 is available >= gtk-+3.20.
	@echo You must install the *-dev or *-devel package of the gspell-1 library.
	@echo Else enjoy using it-edit without spell check support !
	@echo

install:
# If you don't define PREFIX you can modify compile and test the program here.
	- $(MAKE) -B CPPFLAGS=-D'PREFIX=\"$(DESTDIR)$(prefix)\"'
	- $(INSTALL) $(PRGPATH) "$(DESTDIR)$(bindir)"
# Targets used by the development of it-edit:
#	- $(MAKE) mkdir-data
#	- $(MAKE) chmod-data
#	- $(MAKE) cp-data
#	 - $(MAKE) install-locale
#	- $(MAKE) desktop
#	- $(MAKE) update-desktop
#	- $(MAKE) clean
#ifeq ($(GSPELL1),F)
#	- $(MAKE) gspell-warning
#endif

#	 - if `test ! -d "$(DESTDIR)$(pkgdatadir)/Files_handler"` ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/Files_handler" ; fi ;
#	 - if `test ! -f "$(DESTDIR)$(pkgdatadir)/Files_handler/Files_handler.conf"` ; then $(TOUCH) "$(DESTDIR)$(pkgdatadir)/Files_handler/Files_handler.conf" ; fi
#	 - if `test ! -f "$(DESTDIR)$(pkgdatadir)/Configuration/menus.ui"` ; then $(INSTALL) "./share/it-edit/Configuration/menus.ui" "$(DESTDIR)$(pkgdatadir)/Configuration/menus.ui" ; fi
#	 - if `test ! -d "$(DESTDIR)$(pkgdatadir)/Icons"` ; then $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/Icons" ; fi
#	 - $(CP_R) "./share/it-edit/Icons" "$(DESTDIR)$(pkgdatadir)/Icons" ;
#	 - $(CHMOD) a+w "$(DESTDIR)$(pkgdatadir)/Files_handler"
#	 - $(CHMOD) a+w "$(DESTDIR)$(pkgdatadir)/Files_handler/Files_handler.conf"
#	 - $(MAKE) install-locale
# Following permit you to hack my code locally with this Makefile without problems:
#	 - $(MKDIR_P) "$(DESTDIR)$(pkgdatadir)/bin"
#	 - $(CHMOD) -R a+rwx "$(DESTDIR)$(pkgdatadir)/source"
#	 - $(CHMOD) -R a+rwx "$(DESTDIR)$(pkgdatadir)/bin"
# We continue with installing the *.desktop file...
#	 - $(MAKE) desktop
#	 - $(MAKE) update-desktop
# Installing the documentation.
#	 - $(MAKE) install-info
#	 - $(MAKE) install-man
#	 - $(MAKE) install-doc
#	 - $(MAKE) clean

uninstall:
	- $(MAKE) unupdate-desktop
	- $(RM) -f "$(DESTDIR)$(bindir)/$(PRGNAME)"
	- $(RM) -f "$(DESTDIR)$(infodir)/$(PRGNAME).info"
	- $(RM) -f "$(DESTDIR)$(man1dir)/$(PRGNAME).1"
	- $(RM) -f "$(DESTDIR)$(docdir)/$(PRGNAME).html"
	- $(RM) -f "$(DESTDIR)$(docdir)/$(PRGNAME).pdf"
	- $(RM) -f "$(desktopdir)/it.edit.mrcyberfighter.desktop"
	- $(RM_R) -f $(pkgdatadir)

###### [END] test installation target [END] #######

.PHONY: help

help:
	@echo "make build      # Build the binary."
	@echo "make exec       # Execute the binary."
	@echo
	@echo "make ddebug     # Build the binary with DEBUG flag defined."
	@echo "make gdebug     # Build the binary with the debugging -g option."
	@echo "make gdb        # Launch gdb with the binary given as argument."
	@echo "make fdebug     # Build the binary and generate a debug.txt file from the build."
	@echo "make pg         # Build the binary with the profiling -pg option."
	@echo "make gprof      # Launch the gprof tool."
	@echo
	@echo "make as-att     # Generate assembly files (AT&T syntax)  from src files."
	@echo "make as-intel   # Generate assembly files (Intel syntax) from src files."
	@echo "make as-power   # Generate assembly files (Power syntax) from src files."
	@echo
	@echo "make preproc    # Generate preprocessor file (*.i) from source files."
	@echo
ifneq ($(HEXDUMP), )
	@echo "make hexdump    # Process a hexdump on the program."
	@echo
endif
	@echo "make info       # Print binary file informations."
	@echo
ifneq ($(STRIP), )
	@echo "make strip      # strip the binary."
	@echo
endif
	@echo "make clean_o    # Remove all object files."
	@echo "make clean_prg  # Remove the binary file."
	@echo "make clean      # Remove all object files and the binary."
	@echo "make clean_lib  # Remove the static library (ar) file if any."
	@echo "make clean_s    # Remove all assembly generated files."
	@echo "make clean_cpp  # Remove all preprocessor files."
	@echo "make clean_pg   # Remove gmon.out file."
	@echo "make clean_all  # Remove all generated files."
	@echo
ifneq ($(FIND), )
	@echo "make clean_save # Remove backup files."
	@echo
endif
ifneq ($(GREP), )
	@echo "make search-pattern argv=pattern # Search for pattern in files."
	@echo
endif
ifneq ($(SED), )
	@echo "make replace-in-file pattern=\"\" replace=\"\" filepath=\"\" # Replace in file."
	@echo
endif
ifneq ($(FIND), )
ifneq ($(SED), )
	@echo "make replace-all pattern=\"\" replace=\"\" # Replace all in all src files."
	@echo
endif
endif
ifneq ($(SED), )
	@echo "make rm-trailing-spaces # Remove trailing spaces from source and headers."
	@echo
endif
	@echo "make prepend-license    # Prepend license to all source and headers files."
	@echo
ifneq ($(DIFF), )
	@echo "make diff argv=file1 file2 # Make a diff between the given files."
	@echo
endif
ifneq ($(XGETTEXT), )
	@echo "make xgettext           # Generate a *.pot file from source files."
endif
ifneq ($(MSGFMT), )
	@echo "make msgfmt lang=[LANG] # Generate a *.mo file and set it into locale directory"
endif
ifneq ($(MSGUNIQ), )
	@echo "make msguniq            # Remove duplicate in a translation file (*.po, *.pot)"
	@echo
endif
	@echo
ifneq ($(STRACE), )
	@echo "make strace      # trace the binary."
endif
ifneq ($(LTRACE), )
	@echo "make ltrace      # ltrace the binary."
endif
ifneq ($(NM), )
	@echo "make nm          # process the binary the nm tool."
endif
ifneq ($(READELF), )
	@echo "make readelf     # display informations about the ELF binary."
endif
ifneq ($(OBJDUMP), )
	@echo "make objdump     # Process the binary the objdump tool."
endif
ifneq ($(LDD), )
	@echo "make ldd         # Process the binary with the ldd tool."
endif
	@echo
	@echo
ifneq ($(TAR), )
	@echo "make dist-tar     # Generate a tar archive from the project."
ifneq ($(GZ), )
	@echo "make dist-tar-gz  # Generate a tar.gz archive from the project."
endif
ifneq ($(BZ2), )
	@echo "make dist-tar-bz2 # Generate a tar.bz2 archive from the project."
endif
endif
ifneq ($(ZIP), )
	@echo "make dist-zip     # Generate a zip archive from the project."
endif
	@echo
	@echo "make help         # Display all available targets."

#################### [END] print available targets [END] #######################
