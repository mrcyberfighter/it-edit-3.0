.. sectionauthor:: Brüggemann Eddie <mrcyberfighter@gmail.com>

Sessions
========

Configure your sessions recovering
----------------------------------

Their are 3 different modes for registering your session at your convenience.

Files, documentation, applications registering:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

+ Asked for registering the current session at quitting.

+ Automatic registering.

+ Disable session registering.


The **automatic** ``session`` mechanism
---------------------------------------

How does it work ?

Registering:
~~~~~~~~~~~~

+ Every time you open or close a file into the editor the registered files list is recomputed.

  :note: The path is registered.

+ Every time you launch a file through the :menuselection:`File handler` (:kbd:`Ctrl + H`), the file is registered.

  :note: The URI is registered.

+ Every time you launch an application, the application is registered.

  :note: The application path is registered.

+ Nothing is erase if you don't clear the session :menuselection:`Files --> Reload session --> Clear session`.

+ If you reload something it's automatically stored again for next session.

Reloading a session
~~~~~~~~~~~~~~~~~~~

You can reload the entire session by activating the menu item:

  + :menuselection:`Reload all last session`

    This will activate the reloading of the last registered:

      + Files.

      + Documentation files.

      + Applications.

Or reload the different items singular.

Clear a session
~~~~~~~~~~~~~~~

:warning: To know that every **non-empty** registered list can be relaunch at the next session or into the same session.

The only way to clear all the list is to activate the menu item :menuselection:`Files --> Reload session --> Clear session`.

Into a session.

This permit to clear the lists and

if you want to reconstruct a new session by reactivating the concern session registering mechanism.

.. warning:: Application launching Note:

  When you launch an application per the :menuselection:`Actions --> Application launch` it won't be registered as applications

  because I consider that we need the application only now not very often.

  But if you open an application per the menu items :menuselection:`Applications` where you can register your personal applications you use often.

  They are are registered because you will use them often with the :program:`it-edit` easy application access menus.


