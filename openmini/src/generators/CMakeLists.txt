# Retrieve all generators source files

file(GLOB
     OPENMINI_GENERATORS_SRC
     *.cc
)

# Expose variables to parent CMake files
set(OPENMINI_GENERATORS_SRC
    ${OPENMINI_GENERATORS_SRC}
    PARENT_SCOPE
)

file(GLOB
     OPENMINI_GENERATORS_HDR
     *.h
)

# Expose variables to parent CMake files
set(OPENMINI_GENERATORS_HDR
    ${OPENMINI_GENERATORS_HDR}
    PARENT_SCOPE
)
