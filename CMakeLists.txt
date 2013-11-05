# Required for building OpenMini

cmake_minimum_required(VERSION 2.8)

project(openmini)

# Paths definitions
set(OPENMINI_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Project-wide warning options
if(UNIX)
  add_definitions(-pedantic)
  add_definitions(-Wall)
  add_definitions(-Wextra)
  add_definitions(-Wpointer-arith)
  add_definitions(-Wcast-qual)
  add_definitions(-ggdb)
elseif(MSVC)
  add_definitions(-W4)
  # This disable useless msvc warnings about checked iterators
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif(UNIX)

# C++11 related options:
# If using Clang we have to add the flag "std=c++11"
# if using gcc, the c++11 flag change depending on the version
# Nothing special with msvc
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_definitions(-std=c++11)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                  OUTPUT_VARIABLE GCC_VERSION)
  if ((GCC_VERSION VERSION_LESS 4.6) OR (GCC_VERSION VERSION_EQUAL 4.6))
    add_definitions(-std=c++0x)
  else()
    add_definitions(-std=c++11)
  endif()
endif()

# Default parameters

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Functions

# @brief For the specified target, append compiler flags to the existing ones
#
# @param _TARGET_NAME           target name
# @param _FLAGS                 flags to set
function(add_compiler_flags _TARGET_NAME _FLAGS)
  set_property(TARGET ${_TARGET_NAME}
               APPEND PROPERTY COMPILE_FLAGS ${_FLAGS}
               )
endfunction(add_compiler_flags)

# @brief (MSVC only) Set one specific target to use MT runtime library,
# or MTd in debug configuration
#
# @param _TARGET_NAME           target name
function(set_target_mt _TARGET_NAME)
  if(MSVC)
    # Debug is the default when anything else than "Release" is set
    if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
      set(FLAG "/MT")
    else()
      set(FLAG "/MTd")
    endif()
    add_compiler_flags(${_TARGET_NAME} ${FLAG})
  endif(MSVC)
endfunction(set_target_mt)

# @brief Turns off warnings for one given file
# @param _FILE           File whose warnings will be ignored
function(disable_file_warnings _FILE)
  if (MSVC)
    # Windows
    set(FLAG "/W0")
  else()
    # Linux, Mac (gcc and Clang)
    set(FLAG "-w")
  endif(MSVC)

  SET_SOURCE_FILES_PROPERTIES(TARGET
               ${_FILE}
               PROPERTIES COMPILE_FLAGS ${FLAG}
               )
endfunction(disable_file_warnings)

option(OPENMINI_HAS_GTEST "Allowing to use GTest framework (should be present into environment variable GTEST_ROOT." OFF)
message(STATUS "GTest framework: ${OPENMINI_HAS_GTEST}")

option(OPENMINI_HAS_JUCE "Allowing to use Juce framework (should be present into environment variable JUCE_ROOT, and the VST SDK into VST_ROOT)." OFF)
message(STATUS "Juce framework: ${OPENMINI_HAS_JUCE}")


# Various includes


# Google Test framework
set(GTEST_DIR
  $ENV{GTEST_ROOT}
)
set(GTEST_BUILD_DIR
  ${CMAKE_CURRENT_BINARY_DIR}/gtest
)
set(GTEST_INCLUDE_DIRS
  ${GTEST_DIR}/include
)
if(EXISTS ${GTEST_DIR})
  # MSVC-specific issues
  if(MSVC)
    # Known GTest/MSVC 2011 issue: VS2012 doesn't support correctly the tuples yet
    add_definitions(-D _VARIADIC_MAX=10)
  endif(MSVC)

  add_subdirectory(${GTEST_DIR} ${GTEST_BUILD_DIR})
endif(EXISTS ${GTEST_DIR})


# JUCE
set(JUCE_DIR
  $ENV{JUCE_ROOT}
)
set(JUCE_BUILD_DIR
  ${CMAKE_CURRENT_BINARY_DIR}/juce
)
set(JUCE_INCLUDE_DIR
  ${JUCE_DIR}/JUCE
)


# VST SDK
set(VST_DIR
  $ENV{VST_ROOT}
)
set(VST_BUILD_DIR
  ${CMAKE_CURRENT_BINARY_DIR}/vst
)
set(VST_INCLUDE_DIR
  ${VST_DIR}
)


add_subdirectory(openmini)
