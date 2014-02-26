# Required for building OpenMini

cmake_minimum_required(VERSION 2.8)

project(openmini)

# Paths definitions
set(OPENMINI_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Default parameters

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Various detection, to be used as shortcuts below

# Compilers
set(COMPILER_IS_CLANG
    0)
set(COMPILER_IS_GCC
    0)
set(COMPILER_IS_MSVC
    0)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(COMPILER_IS_CLANG
      1)
  message("Compiler detected as Clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(COMPILER_IS_GCC
      1)
  message("Compiler detected as gcc")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(COMPILER_IS_MSVC
      1)
  message("Compiler detected as MSVC")
endif()

# Systems
set(SYSTEM_IS_WINDOWS
    0)
set(SYSTEM_IS_MACOSX
    0)
set(SYSTEM_IS_LINUX
    0)
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(SYSTEM_IS_WINDOWS
      1)
  message("System detected as Windows")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(SYSTEM_IS_MACOSX
      1)
  message("System detected as Mac OS X")
else()
  # ${CMAKE_SYSTEM_NAME} MATCHES "Linux" may be true on Mac!
  set(SYSTEM_IS_LINUX
      1)
  message("System detected as Linux")
endif()

# Build configuration
set(BUILD_IS_DEBUG
    0)
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
  set(BUILD_IS_DEBUG
      1)
  message("Build configuration set to Debug")
else()
  message("Build configuration set to Release")
endif()

# Functions

# @brief For the specified target, append compiler flags to the existing ones
#
# @param _TARGET_NAME           target name
# @param _FLAGS                 flags to set
function(add_compiler_flags _TARGET_NAME _FLAGS)
  set_property(TARGET ${_TARGET_NAME}
               APPEND_STRING PROPERTY COMPILE_FLAGS ${_FLAGS}
               )
endfunction(add_compiler_flags)

# @brief For the specified target, append linker flags to the existing ones
#
# @param _TARGET_NAME           target name
# @param _FLAGS                 flags to set
function(add_linker_flags _TARGET_NAME _FLAGS)
  set_property(TARGET ${_TARGET_NAME}
               APPEND PROPERTY LINK_FLAGS ${_FLAGS}
               )
endfunction(add_linker_flags)

# @brief For the whole project, append debug compiler flags
#
# @param _FLAGS                 flags to set
function(add_debug_flags _FLAGS)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${_FLAGS}"
      PARENT_SCOPE)
endfunction(add_debug_flags)

# @brief For the whole project, append release compiler flags
#
# @param _FLAGS                 flags to set
function(add_release_flags _FLAGS)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${_FLAGS}"
      PARENT_SCOPE)
  # Same for the "symbol" configuration
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${_FLAGS}"
      PARENT_SCOPE)
endfunction(add_release_flags)

# @brief For the whole project, append release linker flags
#
# @param _FLAGS                 flags to set
function(add_release_linker_flags _FLAGS)
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${_FLAGS}"
      PARENT_SCOPE)
  # Same for the "symbol" configuration
  set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} ${_FLAGS}"
      PARENT_SCOPE)
endfunction(add_release_linker_flags)

# @brief For the given list of targets, disable all warnings
#
# @param _TARGETS      List of targets
function(disable_warnings_list _TARGETS)
 if (${COMPILER_IS_GCC} OR ${COMPILER_IS_CLANG})
   set(FLAG
       "-w")
 else()
   set(FLAG
       "/W0")
 endif()
 SET_SOURCE_FILES_PROPERTIES(${_TARGETS}
                             PROPERTIES COMPILE_FLAGS ${FLAG}
                             )
endfunction(disable_warnings_list)

# @brief (MSVC only) Set one specific target to use MT runtime library,
# or MTd in debug configuration
#
# @param _TARGET_NAME           target name
function(set_target_mt _TARGET_NAME)
  if(${COMPILER_IS_MSVC})
    # Cannot use "add_CONFIG_flags" function here due to variable scoping
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd"
        PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT"
        PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT"
        PARENT_SCOPE)
  endif(${COMPILER_IS_MSVC})
endfunction(set_target_mt)

option(OPENMINI_HAS_GTEST "Allowing to use GTest framework (should be present into environment variable GTEST_ROOT." OFF)
message(STATUS "GTest framework: ${OPENMINI_HAS_GTEST}")

option(OPENMINI_HAS_JUCE "Allowing to use Juce framework (should be present into environment variable JUCE_ROOT, and the VST SDK into VST_ROOT)." OFF)
message(STATUS "Juce framework: ${OPENMINI_HAS_JUCE}")

option(OPENMINI_ENABLE_COVERAGE "Enables coverage build (using gcov)." OFF)
message(STATUS "Coverage: ${OPENMINI_ENABLE_COVERAGE}")

option(OPENMINI_ENABLE_SIMD "Allowing to use SIMD instructions: SSE on x86, etc." OFF)
message(STATUS "Simd instructions use: ${OPENMINI_ENABLE_SIMD}")

# Project-wide various options
if (${COMPILER_IS_MSVC})
  # Multithreaded build
  add_definitions("/MP")
endif (${COMPILER_IS_MSVC})

# Project-wide options (SIMD, if enabled)
if (${OPENMINI_ENABLE_SIMD} STREQUAL "ON")
  if (${COMPILER_IS_GCC} OR ${COMPILER_IS_CLANG})
    add_definitions("-msse2")
  else()
    add_release_flags("/arch:SSE2")
  endif (${COMPILER_IS_GCC} OR ${COMPILER_IS_CLANG})
else()
  add_definitions(-D_DISABLE_SIMD)
endif (${OPENMINI_ENABLE_SIMD} STREQUAL "ON")

# Project-wide warning options
if(${COMPILER_IS_GCC} OR ${COMPILER_IS_CLANG})
  add_definitions(-pedantic)
  add_definitions(-Wall)
  add_definitions(-Wextra)
  add_definitions(-Wpointer-arith)
  add_definitions(-Wcast-qual)
  add_definitions(-ggdb)
elseif(${COMPILER_IS_MSVC})
  add_definitions(-W4)
  # This disable useless msvc warnings about checked iterators
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif(${COMPILER_IS_GCC} OR ${COMPILER_IS_CLANG})

# Debug-only options
if(${COMPILER_IS_GCC} OR ${COMPILER_IS_CLANG})
  add_debug_flags("-ggdb")
elseif(${COMPILER_IS_MSVC})
  add_debug_flags("/Zi")
endif(${COMPILER_IS_GCC} OR ${COMPILER_IS_CLANG})

# Release-only options
if(${COMPILER_IS_GCC} OR ${COMPILER_IS_CLANG})
  add_release_flags("-Ofast")
  add_release_flags("-march=native")
  add_release_flags("-mfpmath=sse")
  add_release_flags("-Ofast")
  # More informations about vectorization
  add_release_flags("-ftree-vectorizer-verbose=10")
elseif(${COMPILER_IS_MSVC})
  add_release_linker_flags("/LTCG")
  add_release_flags("/Ox")
  add_release_flags("/Ob2")
  add_release_flags("/Oi")
  add_release_flags("/Ot")
  add_release_flags("/GL")
  add_release_flags("/fp:fast")
endif(${COMPILER_IS_GCC} OR ${COMPILER_IS_CLANG})

# C++11 related options:
# If using Clang we have to add the flag "std=c++11"
# if using gcc, the c++11 flag change depending on the version
# Nothing special with msvc
if (${COMPILER_IS_CLANG})
  add_definitions(-std=c++11)
elseif (${COMPILER_IS_GCC})
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                  OUTPUT_VARIABLE GCC_VERSION)
  if ((GCC_VERSION VERSION_LESS 4.6) OR (GCC_VERSION VERSION_EQUAL 4.6))
    add_definitions(-std=c++0x)
  else()
    add_definitions(-std=c++11)
  endif()
endif()

# C++11 on Mac: we have to use libc++ as well, because the libstdc++ provided
# with Macs is based on 4.2, with no C++11 support.
if (${SYSTEM_IS_MACOSX})
  add_definitions(-stdlib=libc++)
endif (${SYSTEM_IS_MACOSX})

# Coverage only available on Linux, with debug configuration
if (${OPENMINI_ENABLE_COVERAGE} STREQUAL "ON")
  if (UNIX)
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
      include(CTest)
      enable_testing()
      add_definitions("--coverage")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    endif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  endif(UNIX)
endif (${OPENMINI_ENABLE_COVERAGE} STREQUAL "ON")

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
  # Not displaying gtest warnings
  add_compiler_flags(gtest " -w")
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
