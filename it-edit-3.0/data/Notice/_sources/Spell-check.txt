.. _Spell-check:

.. sectionauthor:: Brüggemann Eddie <mrcyberfighter@gmail.com>

Spell check
===========

:program:`it-edit` spell check features
---------------------------------------

  :program:`it-edit` provide 2 different spell check methods:

  + In-line spell check:

    The in-line spell check mechanism is to underline the misspelled words and to provides corrections

    by setting the cursor over the misspell word and opening the contextual menu from :program:`it-edit`.

    The contextual menu include then a :guilabel:`&Spelling suggestion` menu item, from which you can choose

    to correct the misspelled word: the word is automatically replaced with the word you've chosen.

  + Spell check dialog:

    :program:`it-edit` provide a dialog window which will check the entire current edited file buffer.

Using :program:`it-edit` spell check
------------------------------------

  You can enable or disable the in-line spell check by using:

  + The menu item: :menuselection:`Edition --> Inline spell check`.      

  + The button.

  + The shortcut: :kbd:`Ctrl + W`.

  You can display the spell check dialog window using the:

  + The menu item: :menuselection:`Actions --> Spell check dialog`.      

  + The button.

  + The shortcut: :kbd:`Ctrl + Shift + W`.

:program:`it-edit` spell check support
--------------------------------------

  :program:`it-edit` use the :program:`gspell-1` library for providing spell check.

  .. seealso:: :program:`it-edit` and :program:`gspell-1` library:

    Actually the :program:`gspell-1` library is relative young, so not available in every repository.

    So where ever you get the spell check functionality into :program:`it-edit` depends on what version

    of :program:`gtk-3` you get.

    Because the :program:`gspell-1` library is only available with **>=** :program:`gtk-+3.20`.

    :note: You can get gtk+-3.22 and gspell-1 currently with the *ppa* ``gnome3-staging`` for debian packages.

    :program:`gspell-1` has the advantages:

    + To be compatible with the gtksourceview-3 library contextual menu.

    + To provide a spell check dialog has widget.

    + To have a good language selection mechanism.

  .. seealso:: :program:`gspell-1` library

    ::

      gspell provides a flexible API to add spell checking to a GTK+ application. It
      features:
      * GObject wrappers around Enchant
      * An inline spell checker for GtkTextView (enhanced version of GtkSpell)
      * A spell checker dialog for GtkTextView
      * Support of the no-spell-check tag defined by GtkSourceView
      * Language choosers (button and dialog)

    So we know that gspell is based on enchant:

    ::

      Enchant is a generic spell checking library which uses existing spell checker
      engines such as ispell, aspell and myspell as its backends.

      Enchant steps in to provide uniformity and conformity on top of these libraries,
      and implement certain features that may be lacking in any individual provider
      library.

    So for getting dictionaries compatibles with the :program:`gspell-1` library simply download either or:

    + aspell

    + ispell

    + myspell

    dictionnaries in the wanted language(s).


Thanks
------

  Big Thanks to the author of the gpsell-1 library **Sébastien Wilmet** which I get some form of familiarity within.

  :Sébastien Wilmet: is the author of the :program:`Texilla` Latex editor and maintainer of the gtksourceview-3 library and participate in many other projects like :program:`gedit`.
 

