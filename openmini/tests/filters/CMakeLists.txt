# Retrieve all filters tests source files

file(GLOB
     OPENMINI_FILTERS_TESTS_SRC
     *.cc
)

# Expose variables to parent CMake files
set(OPENMINI_FILTERS_TESTS_SRC
    ${OPENMINI_FILTERS_TESTS_SRC}
    PARENT_SCOPE
)

