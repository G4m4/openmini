# @brief OpenMini root cmake file

add_subdirectory(src)

if (${OPENMINI_BUILD_PLUGINS} STREQUAL "ON")
  add_subdirectory(plugin)
endif (${OPENMINI_BUILD_PLUGINS} STREQUAL "ON")

if (${OPENMINI_BUILD_TESTS} STREQUAL "ON")
  add_subdirectory(tests)
endif (${OPENMINI_BUILD_TESTS} STREQUAL "ON")
