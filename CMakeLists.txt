# Required for building OpenMini

cmake_minimum_required(VERSION 2.8)

project(openmini)

# Paths definitions
set(OPENMINI_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(GTEST_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/../gtest-1.6.0
)

set(GTEST_BUILD_DIR
  ${CMAKE_CURRENT_BINARY_DIR}/gtest
)

set(GTEST_INCLUDE_DIR
  ${GTEST_DIR}/include
)

# MSVC-specific option for known GTest/MSVC 2011 issue
if(MSVC)
  # VS2012 doesn't support correctly the tuples yet
  add_definitions(/D _VARIADIC_MAX=10)
endif(MSVC)

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
endif(UNIX)

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

  get_property(COMP_FLAGS TARGET ${_TARGET_NAME}
               PROPERTY COMPILE_FLAGS
               )
  # manual appending
  set(COMP_FLAGS
      "${COMP_FLAGS} ${_FLAGS}"
      )

  set_property(TARGET ${_TARGET_NAME}
               PROPERTY COMPILE_FLAGS ${COMP_FLAGS}
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

add_subdirectory(${GTEST_DIR} ${GTEST_BUILD_DIR})
add_subdirectory(openmini)
