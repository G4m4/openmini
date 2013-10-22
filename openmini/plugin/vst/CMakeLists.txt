# Retrieve all VST-specifics source files

file(GLOB
     OPENMINI_PLUGIN_VST_SRC
     *.cc
)

# Expose variables to parent CMake files
set(OPENMINI_PLUGIN_VST_SRC
    ${OPENMINI_PLUGIN_VST_SRC}
    PARENT_SCOPE
)

file(GLOB
     OPENMINI_PLUGIN_VST_HDR
     *.h
)

# Expose variables to parent CMake files
set(OPENMINI_PLUGIN_VST_HDR
    ${OPENMINI_PLUGIN_VST_HDR}
    PARENT_SCOPE
)
