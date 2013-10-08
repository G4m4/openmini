OpenMini - an open source attempt at creating a Minimoog-like digital synthesizer
======================

OpenMini is a modern C++ implementation of a Minimoog-like digital synthesizer.
It aims at showing a simple, cross-platform yet efficient implementation of state of the art
low-aliasing audio synthesis algorithms.

As its main goal is education, information sharing and (hopefully) inspiring other people
to hack and spread audio synthesis tools and art, it is licensed under GPLv3 license.
It basically means that every one can use, copy and modify it with the sole condition
that the source code must be made public.

* Website: <http://openmini.github.com>

Building OpenMini - Using CMake
==============================

OpenMini currently has only been tested on Windows platforms, but its code aims at being portable.
Unix-like systems, including Mac, soon to be tested continuously.

On most systems you can build the project using the following commands

	$ mkdir build && cd build
	$ cmake ..
	$ cmake --build .

Alternatively you can point the CMake GUI tool to the CMakeLists.txt file and generate platform
specific build project or IDE workspace.


License
==================================
OpenMini is under GPLv3

See the COPYING file for the full license text.
