# Retrieve all modulators source files

file(GLOB
     OPENMINI_MODULATORS_SRC
     *.cc
)

# Expose variables to parent CMake files
set(OPENMINI_MODULATORS_SRC
    ${OPENMINI_MODULATORS_SRC}
    PARENT_SCOPE
)

file(GLOB
     OPENMINI_MODULATORS_HDR
     *.h
)

# Expose variables to parent CMake files
set(OPENMINI_MODULATORS_HDR
    ${OPENMINI_MODULATORS_HDR}
    PARENT_SCOPE
)
