# @brief OpenMini root cmake file

add_subdirectory(src)

if (OPENMINI_HAS_JUCE)
  add_subdirectory(implementation)
endif (OPENMINI_HAS_JUCE)

if (OPENMINI_HAS_GTEST)
  add_subdirectory(tests)
endif (OPENMINI_HAS_GTEST)
