OpenMini - Open homage to Minimoog
==================================

OpenMini is a modern C++ implementation of a Minimoog-like digital synthesizer.
It aims at showing a simple, cross-platform yet efficient implementation of state of the art low-aliasing audio synthesis algorithms.

As its main goal is education, information sharing and (hopefully) inspiring other people to hack and spread audio synthesis tools and art, it is licensed under GPLv3 license.
It basically means that every one can use, copy and modify it with the sole condition that the source code must be made public.

Organization
------------

OpenMini is organized in third parts as follows:
- A "generators" part, offering numerous signal generators for various forms (sin, square, triangle...) each one implemented using differents algorithms (DPW, BLIT-derived methods...)
A lot of these algorithms theory comes from Institute for the 'Digital Sound Generation' online book, published by the Computer Music and Sound Technology. The implementation itself is first prototyped in Python - scripts are made public in OpenMini, under the same license.
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
To build the test, set the environment variable GTEST_ROOT to the directory where you put the GTest sources.
On Linux, this can be done as follows:

    sudo apt-get install libgtest-dev
    export GTEST_ROOT=/usr/src/gtest

On Windows, this is done by going to Control Panel->System->Advanced system settings->Environment variables.

Once this is done, you only have to set the flag OPENMINI_HAS_GTEST to ON (caps matters) when invoking cmake:

    cmake -DOPENMINI_HAS_GTEST=ON ../

Building OpenMini implementations
---------------------------------

"Implementation" means here the plugins as well as a standalone version of OpenMini.
Both are made using the [JUCE framework](https://github.com/julianstorer/JUCE).
To build the implementations, set the environment variable JUCE_ROOT to the directory where you put the JUCE sources.
On Linux, this can be done as follows~:

    cd ~/
    mkdir juce
    cd juce
    git clone --depth 1 git://github.com/julianstorer/JUCE.git
    export JUCE_ROOT=~/juce/

JUCE requires a few additional dependencies:

    sudo apt-get -y install libfreetype6-dev
    sudo apt-get -y install libx11-dev
    sudo apt-get -y install libxinerama-dev
    sudo apt-get -y install libxcursor-dev
    sudo apt-get -y install mesa-common-dev
    sudo apt-get -y install libasound2-dev
    sudo apt-get -y install freeglut3-dev
    sudo apt-get -y install libxcomposite-dev

Lastly, if you wish to build the VST plugin, you will have to get the (proprietary) [Steinberg VST SDK](http://www.steinberg.net/en/company/developer.html) and set the environment variable VST_ROOT to its location.

Once all of this is done, you only have to set the flag OPENMINI_HAS_JUCE to ON (caps matters) when invoking cmake:

    cmake -DOPENMINI_HAS_JUCE=ON ../

Note that both these flags can be used simultaneously.

License
==================================
OpenMini is under GPLv3.

See the COPYING file for the full license text.
