OpenMini - Open homage to Minimoog
==================================

[![Build Status](https://travis-ci.org/G4m4/openmini.svg?branch=master)](https://travis-ci.org/G4m4/openmini)

OpenMini is a modern C++ implementation of a Minimoog-like digital synthesizer.
It rely on the [SoundTailor library](https://github.com/G4m4/soundtailor) which provide a simple, cross-platform yet efficient implementation of state of the art low-aliasing audio synthesis algorithms.

As its main goal is education, information sharing and (hopefully) inspiring other people to hack and spread audio synthesis tools and art, it is licensed under GPLv3 license.
It basically means that every one can use, copy and modify it with the sole condition that the source code must be made public.

Organization
------------

OpenMini is organized in two parts as follows:
- A "synthesizer" part, which is the actual synthesizer boilerplate - managing synthesis algorithms parameterization, user input and so on.
- An "implementation" part, relying heavily on the open source [JUCE framework](http://www.juce.com/), wraps the latter synthesizer into an actual audio plugin that can easily be used in various audio editor. For now only VST is available, RTAS should follow. Lastly, a standalone version is available using the same code as the plugin but without any dependencies to the proprietary Steinberg's VST SDK.

All of this code is heavily tested using the Google Test Framework.
It is documented following Doxygen convention, and strictly follows Google style - cpplint script is included for this purpose.
All the code is standard C++: no OS-specific headers.

Note that OpenMini is under continuous integration, building under Linux (using gcc and Clang) at each push with [Travis CI](https://travis-ci.org/G4m4/openmini).
The Windows build is continuously tested as well.

Building OpenMini library
-------------------------

OpenMini build system is Cmake.

Its most basic use would be, from the root directory openmini/ :

    mkdir build && cd build
    cmake ..
    cmake --build .

This only build the library - nothing else. You can also build OpenMini tests and implementations as explained below.

Building OpenMini tests
-----------------------

Tests are using the [Google Test Framework](http://code.google.com/p/googletest/).
To retrieve the library files in order to build tests, the following must be done from the root folder:

    git submodule init
    git submodule update

Once this is done, you only have to set the flag OPENMINI_HAS_GTEST to ON (caps matters) when invoking cmake:

    cmake -DOPENMINI_HAS_GTEST=ON ../

Building OpenMini implementations
---------------------------------

"Implementation" means here the plugins as well as a standalone version of OpenMini. Both are made using the [JUCE framework](https://github.com/julianstorer/JUCE), referenced as a git submodule.

To retrieve the JUCE files in order to build implementation, the following must be done from the root folder:

    git submodule init
    git submodule update

JUCE requires a few additional dependencies:

    sudo apt-get -y install libfreetype6-dev
    sudo apt-get -y install libx11-dev
    sudo apt-get -y install libxinerama-dev
    sudo apt-get -y install libxcursor-dev
    sudo apt-get -y install mesa-common-dev
    sudo apt-get -y install libasound2-dev
    sudo apt-get -y install freeglut3-dev
    sudo apt-get -y install libxcomposite-dev

Once all of this is done, you only have to set the flag OPENMINI_HAS_JUCE to ON (caps matters) when invoking cmake:

    cmake -DOPENMINI_HAS_JUCE=ON ../

Eventually, if you wish to build the VST plugin, you will have to get the (proprietary) [Steinberg VST SDK](http://www.steinberg.net/en/company/developer.html) and set the environment variable VST_ROOT to its location.
Then set the flag OPENMINI_HAS_VST to ON (caps matters) when invoking cmake.

License
==================================
OpenMini is under GPLv3.

See the COPYING file for the full license text.
