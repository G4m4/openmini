# Retrieve all generators tests source files

file(GLOB
     OPENMINI_GENERATORS_TESTS_SRC
     *.cc
)

# Expose variables to parent CMake files
set(OPENMINI_GENERATORS_TESTS_SRC
    ${OPENMINI_GENERATORS_TESTS_SRC}
    PARENT_SCOPE
)

