# @brief OpenMini root cmake file

add_subdirectory(src)

if (${OPENMINI_HAS_JUCE} STREQUAL "ON")
  add_subdirectory(implementation)
endif (${OPENMINI_HAS_JUCE} STREQUAL "ON")

if (${OPENMINI_HAS_GTEST} STREQUAL "ON")
  add_subdirectory(tests)
endif (${OPENMINI_HAS_GTEST} STREQUAL "ON")
