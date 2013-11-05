# @brief OpenMini implementation root cmake file

# These have to be defined since JUCE needs it.
# Sometimes they're not passed to the compiler,
# so we're doing it here explicitly.
if (UNIX)
  add_definitions(-DLINUX)
elseif (MSVC)
  add_definitions(-D_WIN32)
endif ()

add_subdirectory(common)
add_subdirectory(plugin)
add_subdirectory(standalone)
