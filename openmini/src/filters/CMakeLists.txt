# Retrieve all filters source files

file(GLOB
     OPENMINI_FILTERS_SRC
     *.cc
)

# Expose variables to parent CMake files
set(OPENMINI_FILTERS_SRC
    ${OPENMINI_FILTERS_SRC}
    PARENT_SCOPE
)

file(GLOB
     OPENMINI_FILTERS_HDR
     *.h
)

# Expose variables to parent CMake files
set(OPENMINI_FILTERS_HDR
    ${OPENMINI_FILTERS_HDR}
    PARENT_SCOPE
)
