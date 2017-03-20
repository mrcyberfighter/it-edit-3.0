.. _Files:

Files
=====

New file
--------

You can open a New file, this will create a random named ``New_XXXXXX`` file into your **TEMPDIR** folder, and erase it immediately but the file-path is kept.

You will surely write inside the new create buffer and surely save it after (surely not into the **TEMPDIR** folder). 

Use the menu item :menuselection:`Files --> New file` or the shortcut :kbd:`Ctrl + N` to create a new buffer as describe above.

File(s) opening
---------------

They are several ways for opening file(s) into :program:`it-edit`:

+ By using the open file(s)

  + The menu item: :menuselection:`Files --> Open file`.

  + The button.

  + The shortcut :kbd:`Ctrl + O`.

This will present you a file selector to open the wanted file(s).

.. note:: The current tab influate the file-selector behaviour:

  + The active editor page will influence into which folder the file-selector gets open.

    Because the file-selector will be launched into the folder from the current edited file location.

      You can held the mouse over the tab (which content the current filename) to sea the absolute file path in a tool-tip.
 
      The same mechanic is by placing the mouse over the filename into the bottom bar.

  + You can configure to get the current edited file selected (highlighted into the file-selector) into the editor or not.

    :note: This can be practice if per example you want to open the header file from a source file or inversed.

+ By using the :menuselection:`recent file` menu-item to open a recent used file.

.. Make a link to the session notice page.

+ You can open the last registered files, most often the files opened in the last session, by using the the menu item :menuselection:`Reload Session --> Reload last files`.

  This will open automatically all the last registered files.

  :note: This can be practice if you work on a single project during some time.

Saving files
------------

They are several ways of saving files into :program:`it-edit`:

1. You can save the current file simply using:

  + The menu item: :menuselection:`Files --> Save file`.

  + The button.

  + The shortcut :kbd:`Ctrl + S`

  This will save the file at is current location.

2. You can save a "``New``" file or the edited file into another location by using:

  + The menu item: :menuselection:`Files --> Save file as`.

  + The button.

  :warning: They is no shortcut for this purpose, but if you save (:menuselection:`Save file` :kbd:`Ctrl + S`) simply a "``New``" file this will act as a :menuselection:`Save file as`.    

3. You can save all the open files using:

  + The menu item: :menuselection:`Files --> Save all files`.

  + The button.

  + The shortcut :kbd:`Ctrl + Shift + S`

  This will save all the unsaved files at their current location.

.. note::

  You can distinguish if a file is currently save or modified by looking at the **name** *in the tab*:

  if their is an **asterisk** '``*``' *before* the **file name** this mean that the *file* is currently **not saved** on the *disk*.

Reload file
-----------

You can reload a file from disk with :program:`it-edit` by using:

+ The menu item: :menuselection:`Files --> Reload current file`.

+ The shortcut :kbd:`Ctrl + R`.

:note: This can be practice if per example you have redirect you compilation process to a file for debugging compilation errors.

File informations
-----------------

You can get and change files informations by using the menu item :menuselection:`Files --> File informations` :kbd:`Ctrl + I`.

This will display a **top-level window** presenting following **informations** and **action** to *process* on the *file*:

1. A frame named: ``Main informations`` will display:

  + A nice image from the **mime type** of your current edited file.

  + The programming **language** of the file.

  + The **mime type** verbatim.

  + The file **extension**.

2. A frame named: ``Mode`` will display a file **permissions** table like this:


  ::

    +---+---+---+---+
    |   | R | W | X |
    +---+---+---+---+
    | U | * | * |   |
    +---+---+---+---+
    | G | * | * |   |
    +---+---+---+---+
    | O | * |   |   |
    +---+---+---+---+

    U -> User.      R -> Read.

    G -> Group.     W -> Write.

    O -> Others.    X -> Execute.

  The cells of the table contains **check-boxes** representing the current **permissions** of the file.

  By simply (un)checking the **check-boxes** you change the **permissions** of the file on disk.

  .. Put a link to the configuration Notice page.
 
  :warning: By saving your file you will set the permissions according to your configuration into :program:`it-edit` for files saving.

3. A frame named: ``File counts`` display some few statistics of the file:

  + *The number* of **lines**.

  + *The number* of **characters**.

  + *The number* of **words**.

  Of the file on the disk.

  :note: :program:`it-edit` use the program :program:`wc` to gets this informations.

  :warning: The number of line(s) and character(s) into your current edited buffer is visible into the bottom bar.

4. A frame named: ``Timestamps`` display the:

  + **Last status change** *date and time*.

  + **Last access** *date and time*.

  + **Last modification** *date and time*.
     
  Of the file on the disk.

  Near of every information is a button named ``Modify`` which permit you to change the timestamps.

  Which will present you a **calendar** for the date

  and **3** *spin buttons*:

  + **Hours**

  + **Minutes**

  + **Seconds**

  Which permit you to change the timestamps easily.

  :note: This can be useful per example if you have change your system clock and you use the :program:`make` tool,...

5. A frame named: ``Checksum`` will display the:

  + **MD 5** hash.

  + **SHA 1** hash.

  + **SHA 256** hash.

  + **SHA 512** hash.

  of your file.

  :note: The *checksums* are displayed into **hexadecimal** values.
 
6. A frame named: ``File actions`` will present you:

  The file name and 4 buttons, named:

  + Rename file

  + Copy file

  + Compress file

  + Copy file-path to clipboard

  The functionalities of this buttons are clear as their name.

      
