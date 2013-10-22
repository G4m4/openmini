# @brief OpenMini plugin root cmake file

# On Linux these are required in order to build an audio plugin
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  find_package(Freetype REQUIRED)
  find_package(X11 REQUIRED)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

set(OPENMINI_PLUGIN_DIR
    ${OPENMINI_INCLUDE_DIR}/openmini/plugin
)

include_directories(SYSTEM
  ${FREETYPE_INCLUDE_DIRS}  # Linux-specific stuff
  ${X11_INCLUDE_DIRS}  # Linux-specific stuff
  ${VST_INCLUDE_DIR}  # VST SDK root path
  ${JUCE_INCLUDE_DIR}  # Juce library root path
  # Those will be null on anything else than Mac
  ${FLATCARBON}  # Mac-specific stuff
)

include_directories(
  ${OPENMINI_INCLUDE_DIR}
  # This is required since Juce-generated files are looking for files there
  ${OPENMINI_PLUGIN_DIR}/vst
)

# Source files declaration
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # On Mac we have to pass through Juce wrappers (.mm files)
  set(OPENMINI_JUCE_SRC
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_basics/juce_audio_basics.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_devices/juce_audio_devices.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_formats/juce_audio_formats.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_processors/juce_audio_processors.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_utils/juce_audio_utils.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_core/juce_core.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_cryptography/juce_cryptography.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_data_structures/juce_data_structures.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_events/juce_events.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_graphics/juce_graphics.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_gui_basics/juce_gui_basics.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_gui_extra/juce_gui_extra.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp
  )
else()
  set(OPENMINI_JUCE_SRC
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_basics/juce_audio_basics.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_devices/juce_audio_devices.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_formats/juce_audio_formats.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_processors/juce_audio_processors.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_utils/juce_audio_utils.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_core/juce_core.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_cryptography/juce_cryptography.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_data_structures/juce_data_structures.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_events/juce_events.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_graphics/juce_graphics.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_gui_basics/juce_gui_basics.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_gui_extra/juce_gui_extra.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp
  )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Include all subdirectories plugin source files
add_subdirectory(common)
add_subdirectory(vst)

source_group("juce_externals" FILES ${OPENMINI_JUCE_SRC})
source_group("openmini" FILES ${OPENMINI_SRC} ${OPENMINI_HDR})
source_group("openmini_plugin" FILES ${OPENMINI_PLUGIN_SRC} ${OPENMINI_PLUGIN_HDR})
source_group("vst_specifics" FILES ${OPENMINI_PLUGIN_VST_SRC} ${OPENMINI_PLUGIN_VST_HDR})

add_library(openmini_vst SHARED
  ${OPENMINI_JUCE_SRC}
  ${OPENMINI_SRC}
  ${OPENMINI_HDR}
  ${OPENMINI_PLUGIN_SRC}
  ${OPENMINI_PLUGIN_HDR}
  ${OPENMINI_PLUGIN_VST_SRC}
  ${OPENMINI_PLUGIN_VST_HDR}
)

target_link_libraries(openmini_vst
                      openmini_lib)

set_target_mt(openmini_vst)
