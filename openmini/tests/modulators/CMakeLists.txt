# Retrieve all modulators tests source files

file(GLOB
     OPENMINI_MODULATORS_TESTS_SRC
     *.cc
)

# Expose variables to parent CMake files
set(OPENMINI_MODULATORS_TESTS_SRC
    ${OPENMINI_MODULATORS_TESTS_SRC}
    PARENT_SCOPE
)

