# @brief OpenMini root cmake file

add_subdirectory(src)

  add_subdirectory(plugin)
if (${OPENMINI_HAS_JUCE} STREQUAL "ON")
endif (${OPENMINI_HAS_JUCE} STREQUAL "ON")

if (${OPENMINI_HAS_GTEST} STREQUAL "ON")
  add_subdirectory(tests)
endif (${OPENMINI_HAS_GTEST} STREQUAL "ON")
