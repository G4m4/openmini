OpenMini - a Minimoog-like digital synthesizer
======================

OpenMini is a modern C++ implementation of a Minimoog-like digital synthesizer.
It aims at showing a simple, cross-platform yet efficient implementation of state of the art low-aliasing audio synthesis algorithms.

As its main goal is education, information sharing and (hopefully) inspiring other people to hack and spread audio synthesis tools and art, it is licensed under GPLv3 license.
It basically means that every one can use, copy and modify it with the sole condition that the source code must be made public.

Organization
==============================

OpenMini is organized in third parts as follows:
- A "generators" part, offering numerous signal generators for various forms (sin, square, triangle...) each one implemented using differents algorithms (DPW, BLIT-derived methods...)
A lot of these algorithms theory comes from Institute for the 'Digital Sound Generation' online book, published by the Computer Music and Sound Technology. The implementation itself is first prototyped in Python - scripts are made public in OpenMini, under the same license.
- A "synthesizer" part, which is the actual synthesizer boilerplate - managing synthesis algorithms parameterization, user input and so on.
- A "plugin" part, relying heavily on the open source JUCE framework, wraps the latter synthesizer into an actual audio plugin that can easily be used in various audio editor. For now only VST and Audio Unit are available, RTAS should follow.

All of this code is heavily tested using the Google Test Framework.
It is documented following Doxygen convention, and strictly follows Google style - cpplint script is included for this purpose.
Except the plugin part which rely on JUCE, all the code is standard C++: no OS-specific headers.

Building OpenMini - Using CMake
==============================

OpenMini currently has only been tested on Windows platforms, but its code aims at being portable.
Unix-like systems, including Mac, soon to be tested continuously.

On most systems you can build the project using the following commands

	$ mkdir build && cd build
	$ cmake ..
	$ cmake --build .

Alternatively you can point the CMake GUI tool to the CMakeLists.txt file and generate platform specific build project or IDE workspace.


License
==================================
OpenMini is under GPLv3.

See the COPYING file for the full license text.
