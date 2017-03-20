.. sectionauthor:: Brüggemann Eddie <mrcyberfighter@gmail.com>

Editor
======

The editor has other functionalities like:

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
   
