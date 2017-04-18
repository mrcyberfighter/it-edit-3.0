====================================
it-edit (Integrated Terminal Editor)
====================================

:program: it-edit

:version: 3.0

:author: Brüggemann Eddie

:contact: <mrcyberfighter@gmail.com>

:license: GPLv3

:website: <http://www.open-source-projects.net/it-edit/it-edit>

:release: |today|

.. sectionauthor:: Brüggemann Eddie <mrcyberfighter@gmail.com>


------------------------------
Motivation for writing it-edit
------------------------------

I think they are 2 sort of programmers in their habits of writing programs:

  * They which use an :abbr:`I.D.E (Integrated Development Environment)` with full integrated functionalities, and master it to respond to most of their requirements.

  * They which use an text editor to write their programs and a terminal to compile, debug or launch them and some other tools separately.

Because I'm an programmer from the second category and because i remark that i often use additional tools than the editor and the terminal.

I decide to write my own text editor program which provide me all the functionalities that i need to get a development environment fully adapt to my requirement.

So in fact :program:`it-edit` is more than a basic programming text editor, but a powerful tool which I hope you will agree the concept.

.. note::

  In fact in conjunction with my project generator `mk-project <http://www.open-source-projects.net/mk-project/mk-project>`_ which generate me a big **do all** ``Makefile``.

  I enjoy using it-edit every time !

  Even when I only to type few targets like (make, make exec, make ddebug, make gdb,...) :program:`it-edit` is useful for all task to do in a terminal.

  All this with an accompanied editor.

  :note: By the way `mk-project <http://www.open-source-projects.net/mk-project/mk-project>`_ can be useful for every :program:`vim` or :abbr:`T.U.I (Terminal User Interface)` editor user.

---------------------------------
Requirement of :program:`it-edit`
---------------------------------

:program:`it-edit` requires

+ gtk-3 as :abbr:`G.U.I (Graphical User Interface)`

+ gtksourceview-3.

+ vte-2.91 for the virtual terminals emulation as a gtk-3 widget.

+ Optionally gspell-1 as :ref:`spell checker <Spell-check>`.

:warning: You must install the development packages of all required library because it will be compiled and installed on the target host.

-------------------------------------
What :program:`it-edit` does for me !
-------------------------------------

:program:`it-edit` consist of a basic programming text editor with all basic functionalities with

intelligently integrated terminals the best positioned so that they are ``easy to reach`` into the main interface of the program.

With many others practices functionalities.

.. Make it better:  Easy access to documentation of any sort and easy launching GUI applications for the purpose you need.

---

:program:`it-edit` integrated terminals in the best way I have thought for me.

A side bar terminal which you can add and remove items, a big full-screen terminal, the same divided into 4 terminals and as many top-level terminals as you want.

---

:program:`it-edit` provide an easy file access...

In fact it was thought for registering **HTML** documentation and so accessing it **easily**.

|

But you can register your winner song if you want when you have terminate all your assertions well,

to celebrate your victory **!**

:program:`it-edit` will open the registered file with the default program for it if any available.

|

  Saving and restoring your file(s) list.

  .. code-block:: bash

    $ cp /usr/local/share/it-edit/Files_handler/Files_handler.conf $HOME

  To restore your file(s) list after upgrade per example.

  .. code-block:: bash

    $ cp $HOME/Files_handler.conf /usr/local/share/it-edit/Files_handler/Files_handler.conf

---

You can launch graphical applications with :program:`it-edit` very easily:

+ At first it-edit will check your system, at first start, for some predefined applications and register it into their related category.

  :note: You can change the application(s), if found or not, as you want.

+ Then you can choose to register and delete the applications you want, into the category ``Other``.

---

The version **3**.**0** of :program:`it-edit` embedded a configurable spell checker utility which you can use to write documentations

reachable as

+ **in-line spell-check** (*the misspelled words are underline*)

or

+ through launching a **dialog spell-check** window which permit you to correct all misspelled words of the current document.



.. OLD: Which consist of a terminal integrated text-editor with which you can access all you require directly from the editor: documentation, GUI applications, ...

.. Editor description:

.. * syntax highlight for many languages.

..  * Cut, Copy, Paste functionalities available from severals places in the main interface:

..    * From the menu items.

..    * From a button into the button bar.

..    * From the contextual menu


-------------------------
:program:`it-edit` spirit
-------------------------

:program:`it-edit` offers:

A programming editor
^^^^^^^^^^^^^^^^^^^^

A text-editor without favoring any language.

With all the basic text editor functionalities and overall useful :ref:`shortcuts <shortcuts-table>` which you didn't have to know all.

And some unusual like **duplicate text** or **copy to clipboard** the current edited absolute **file-path** (:kbd:`Ctrl + Shift + Y`).

You will be able to open, open a recent, save, save as, save all, file(s).

Or reload your last session file(s) as documentation and launched applications.

:program:`it-edit` provides some informations like:

+ Line

+ Column

+ Total lines

+ Total characters

+ The filename (hold the mouse over  the notebook tab, you will know the absolute file-path).

By opening the file informations you will get more informations and can do some basic functions on (and strictly over) the file on disk (**not** the current edited buffer):

+ The file base-name.

+ Language, Mime-type and Extension.

+ Lines, Words and Chars count.

+ MD5, SHA1, SHA256 and SHA512 checksum in hexadecimal notation.

You will be able to see and to modify:

+ The file permissions.

:warning: By saving the ``configured mask`` will overwrite your changes.

+ The last

  + Access time

  + Modification time (can be useful with make and system clock disturbing).

  + Last status change time (this one isn't modifiable).

+ :guilabel:`&Rename`, :guilabel:`&Copy`, :guilabel:`&Compress` (*using the gzip algorithm*), :guilabel:`&Move file`.

--------------------
Terminal integration
--------------------

:program:`it-edit` provides the best terminals integration for everyone:

+ The **sidebar terminals** can be useful by editing and requiring immediately a terminal in the same window.

+ The **single "Big Terminal"** can be useful if you need space: you can mask the button bar and made it full-screen.

+ The **4 divided "Big Terminal"** can be useful if you need severals terminals on the screen.

+ The **top level terminal** is re-sizable and in a separate window.

:note: It's recommended to use :program:`it-edit` with a ``Makefile`` (easy self-build or not) for compiled languages, else enjoy the terminals for launching your scripts and commands.

``make makes the life easier !``

All this terminals have a good contextual menu and I had problems to add some items, as ``sync current directory`` in every terminal, and so on.


---------------------------------
:program:`it-edit` writing spirit
---------------------------------

:program:`it-edit` is written in C (-std=c99) using gtk-3 and related libraries.

:program:`it-edit` writing style takes cares of:

  * structure variables order: so that no unneeded padding is added from the compiler.

  * cache optimizing: so that we get the best cache-hits we can.

:program:`it-edit` make strong usage of the gtk-3 types.

:program:`it-edit` provide an optional spell checker useful for writing documentation or simply comments.

:program:`it-edit` make usage of ``/** **/`` comments so for commenting out a code section, by hacking it-edit, use the preprocessor: ``#if 0 ... #endif``.

:note: The spirit of :program:`it-edit` says that you have to do a thing a single time then it's automatize.

----------
Conclusion
----------

Control all your system with :program:`it-edit` and show us that you can dialog with it through the best medium: the terminal.


