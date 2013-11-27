# Build OpenMini tests executable

include_directories(
  SYSTEM
  ${GTEST_INCLUDE_DIRS}
)

include_directories(
  ${OPENMINI_INCLUDE_DIR}
)

# Include all subdirectories tests source files
add_subdirectory(filters)
add_subdirectory(generators)
add_subdirectory(synthesizer)

# Group sources
source_group("filters"
  FILES
  ${OPENMINI_FILTERS_TESTS_SRC}
)
source_group("generators"
  FILES
  ${OPENMINI_GENERATORS_TESTS_SRC}
)
source_group("synthesizer"
  FILES
  ${OPENMINI_SYNTHESIZER_TESTS_SRC}
)

# Source files
set(OPENMINI_TESTS_SRC
    main.cc
    tests.cc
    ${OPENMINI_FILTERS_TESTS_SRC}
    ${OPENMINI_GENERATORS_TESTS_SRC}
    ${OPENMINI_SYNTHESIZER_TESTS_SRC}
)
set(OPENMINI_TESTS_HDR
    debug.h
    tests.h
)

# Target
add_executable(openmini_tests
  ${OPENMINI_TESTS_SRC}
  ${OPENMINI_TESTS_HDR}
)

set_target_mt(openmini_tests)

if (${OPENMINI_ENABLE_COVERAGE} STREQUAL "ON")
  target_link_libraries(openmini_tests
    openmini_lib
    gtest_main
  )
  add_test(openmini_coverage
    openmini_tests
  )
else()
  target_link_libraries(openmini_tests
    openmini_lib
    gtest_main
  )
endif (${OPENMINI_ENABLE_COVERAGE} STREQUAL "ON")
