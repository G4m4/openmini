# Retrieve all synthesizer tests source files

file(GLOB
     OPENMINI_SYNTHESIZER_TESTS_SRC
     *.cc
)

# Expose variables to parent CMake files
set(OPENMINI_SYNTHESIZER_TESTS_SRC
    ${OPENMINI_SYNTHESIZER_TESTS_SRC}
    PARENT_SCOPE
)

