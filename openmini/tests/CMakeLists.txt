# Build OpenMini tests executable

include_directories(
  SYSTEM
  ${GTEST_INCLUDE_DIRS}
)

include_directories(
  ${OPENMINI_INCLUDE_DIR}
)

# Include all subdirectories tests source files
add_subdirectory(generators)
add_subdirectory(synthesizer)

# Group sources
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
    ${OPENMINI_GENERATORS_TESTS_SRC}
    ${OPENMINI_SYNTHESIZER_TESTS_SRC}
)
set(OPENMINI_TESTS_HDR
    tests.h
)

# Target
add_executable(openmini_tests
  ${OPENMINI_TESTS_SRC}
  ${OPENMINI_TESTS_HDR}
)

set_target_mt(openmini_tests)

target_link_libraries(openmini_tests
  openmini_lib
  gtest_main
)
