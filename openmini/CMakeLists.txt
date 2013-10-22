# @brief OpenMini root cmake file

add_subdirectory(src)

if (${OPENMINI_BUILD_TESTS} STREQUAL "ON")
  add_subdirectory(tests)
endif (${OPENMINI_BUILD_TESTS} STREQUAL "ON")
