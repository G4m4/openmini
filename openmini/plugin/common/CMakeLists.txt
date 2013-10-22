# Retrieve all plugin common source files

file(GLOB
     OPENMINI_PLUGIN_SRC
     *.cpp
)

# Expose variables to parent CMake files
set(OPENMINI_PLUGIN_SRC
    ${OPENMINI_PLUGIN_SRC}
    PARENT_SCOPE
)

file(GLOB
     OPENMINI_PLUGIN_HDR
     *.h
)

# Expose variables to parent CMake files
set(OPENMINI_PLUGIN_HDR
    ${OPENMINI_PLUGIN_HDR}
    PARENT_SCOPE
)
