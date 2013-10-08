# Retrieve all synthesizer source files

file(GLOB
     OPENMINI_SYNTHESIZER_SRC
     *.cc
)

# Expose variables to parent CMake files
set(OPENMINI_SYNTHESIZER_SRC
    ${OPENMINI_SYNTHESIZER_SRC}
    PARENT_SCOPE
)

file(GLOB
     OPENMINI_SYNTHESIZER_HDR
     *.h
)

# Expose variables to parent CMake files
set(OPENMINI_SYNTHESIZER_HDR
    ${OPENMINI_SYNTHESIZER_HDR}
    PARENT_SCOPE
)
