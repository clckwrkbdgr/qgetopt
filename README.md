qgetopt
=======

Qt wrapper class for GNU getopt.

Usage:

* specify options to check via `addOption()` and `addOptionWithArg()` calls.
* run `parse()` with argument list or `parseApplicationArgs()`.
* check found options and it's arguments using `hasOption()`, `getArg()`, `getArgs()`, and `count()` methods.

Installation
------------

Just put `qgetopt.h` and `qgetopt.cpp` in source dir and add them to Qt project file.

Requirements: only QtCore module.

