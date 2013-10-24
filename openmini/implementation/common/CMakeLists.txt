# Retrieve all implementation common source files

file(GLOB
     OPENMINI_IMPLEMENTATION_SRC
     *.cpp
)

# Expose variables to parent CMake files
set(OPENMINI_IMPLEMENTATION_SRC
    ${OPENMINI_IMPLEMENTATION_SRC}
    PARENT_SCOPE
)

file(GLOB
     OPENMINI_IMPLEMENTATION_HDR
     *.h
)

# Expose variables to parent CMake files
set(OPENMINI_IMPLEMENTATION_HDR
    ${OPENMINI_IMPLEMENTATION_HDR}
    PARENT_SCOPE
)
