.. sectionauthor:: Brüggemann Eddie <mrcyberfighter@gmail.com>

Editor
======

The editor has as functionalities :

:ref:`Files management <Files>`.
--------------------------------

+ New file.

+ Open file(s) or a recent file.

+ Save file(s).

+ Close file.

+ Close all file(s).

+ File informations.

+ Reload file (from disk).

Text edition functionalities.
-----------------------------

+ Undo/Redo.

+ Search and Replace.

+ Go to line number.

+ Cut/Copy/Paste.

+ Duplicate text.

+ Use spaces instead of tabs.

+ Enable/Disable in-line spell-check.

Contextual menu from editor
---------------------------

+ Undo

+ Redo

---

+ Cut

+ Copy

+ Paste

+ Erase

---

+ Select all

+ Change case

  + All to uppercase

  + All to lowercase

  + Invert case

  + First letter uppercase

:note: If you're using the ``In-line spell check`` functionality an item with a *sub-menu* of **suggestions** is added to the **contextual menu** of the editor.

:ref:`Spell-check <Spell-check>` functionality
----------------------------------------------

+ In-line spell-check (:kbd:`Ctrl + w`) : the misspelled words are highlight. 

+ Spell-check dialog (:kbd:`Ctrl + Maj + w`) : scan all the text buffer for misspelled words, with a dialog window for correct them or not.

:warning: Install the optional :program:`gspell-1` ``-dev`` or ``-devel`` package, before installing :program:`it-edit`.

Go to line number
-----------------

+ Use the menu item, the button or the shortcut: :kbd:`Ctrl + g`.

  A window will appear asking you for a valid line number.

  Enter a valid line number and press :kbd:`Enter` or the :guilabel:`&Apply` button to move the editor to the wanted line,

  which will be highlight according to the current scheme.

  :note: This is very useful to go to a specific line.

Find and replace
----------------

+ You can *show* | *hide* the ``Find and replace`` bar by using the toggle button.

  The ``Search and Replace`` bar has following functionalities:

  + **Search** button: this will highlight all the matching occurrences from the search term and moving the editor to the first occurrence position.
      
  + **Next** button: highlight the next matching occurrence from the search term.

    Pressing **Enter** when the search entry field has the focus has the same effect.

    The search will start at selection or at the cursor position.
        
    If you select some text with the mouse and using the shortcut :kbd:`Ctrl + f`.

    The search field will toggle on if not visible and will contains the selected text as search term.

    :note: You can use the shortcut :kbd:`Ctrl + + (KP)` to activate the **Next** functionality.


  + **Previous** button: highlight the previous matching occurrence from the search term.

    The search will start at selection or at the cursor position.

    If you select some text with the mouse and use the shortcut :kbd:`Ctrl + f`.

    The search field will toggle on if not visible and will contains the selected text as search term.

    :note: You can use the shortcut :kbd:`Ctrl + - (KP)` to activate the **Previous** functionality.

  + **Replace** button: replace the current matching occurrence, which is highlight, with the content of the replace field.

    :note: You can use the shortcut :kbd:`Ctrl + Enter (KP)`: instead of the button.
          
  + **Replace all** button: replace all the matching occurrence in the current file.

    :note: You can use the shortcut :kbd:`Ctrl + Shift + Enter (KP)` instead of the button.

  + **Mode**: You can select how your search term(s) will be interpreted.

    + **Raw text**: all search terms matching.

    + **Word boundary**: The search term must be a complete word, not a part but an variable with separators like underscores or points will work too.

    + **Regular expression**: *Perl* compatible regular expression (**REGEX**).

    :note: For **REGEX** read the **GLib Regex** documentation which is contains into the ``gtk-doc`` directory.

  + **Close** button: hide the search and replace bar and clear the highlight.                                                         

:note: If you select some text with the mouse and use the :kbd:`Ctrl + f` shortcut, then:

  + The search and replace bar will be show.

  + The search field will be filled with your selection.

  :note: The search will begin at your selection position if you hit the **Next** or **Previous** button.

.. note:: **The search terms history:**

  Every search term you make a search for will be register into the history.

  + You can use the :kbd:`Up` key to start the history search from the beginning.

  + You can use the :kbd:`Down` key to start the history search from the end.

  It will flow through the search terms history but not wrap around, simply end at the other end.

Replace in all files
--------------------

You can use the button, menu item or shortcut (:kbd:`Ctrl + Shift + R`),

to replace all occurrence(s) from a pattern according the settings:

+ Case sensitive

+ Mode

  + Raw text
 
  + Word boundary
 
  + REGEX
 
Which are all settable, like the pattern and the replacement text, into the appearing top-level window.  

Copy to clipboard
-----------------

You can copy to clipboard either :

+ The current absolute file-path (:kbd:`Ctrl + y`).

+ The current absolute folder-path (:kbd:`Ctrl + Shift + Y`).

Order page
----------

You can use menu item or the shortcut (:kbd:`Ctrl + Shift + O`) to reorder all the pages lexicographically. 

Editor usage more
-----------------

+ You can use the page up and page down to scroll faster than with the arrows.

+ You can use the key :kbd:`Insert` for changing the form of the cursor.

+ You can use the combination :kbd:`Control +  Left | Right` to move to previous | next sequence of characters.

+ You can use the Erase key to erase the current selection.

+ You can use the shortcut :kbd:`Ctrl + Backspace` to remove an entire characters sequence.

+ You can use the shortcut :kbd:`Ctrl + Start` and :kbd:`Ctrl + End` to move the cursor to the beginning or end of the document.  

+ The search-replace shortcuts are clever set on the keypad:

  + Find next : :kbd:`Ctrl + + (KP)`.

  + Find previous : :kbd:`Ctrl + - (KP)`.

  + Replace : :kbd:`Ctrl + Enter (KP)`.

  + Replace all : :kbd:`Ctrl + Shift + Enter (KP)`.

  For a better and faster search and replace feature.  

+ By all top-level windows you can use the shortcuts:

  + :kbd:`Escape` to close the window.
 
  + :kbd:`Enter` to confirm.  
 

