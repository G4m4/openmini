# @brief OpenMini plugin root cmake file

# On Linux these are required in order to build an audio plugin
if (${SYSTEM_IS_LINUX})
  find_package(Freetype REQUIRED)
  find_package(X11 REQUIRED)
endif (${SYSTEM_IS_LINUX})

set(OPENMINI_PLUGIN_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(SYSTEM
  ${FLATCARBON}  # Mac-specific stuff
  ${FREETYPE_INCLUDE_DIRS}  # Linux-specific stuff
  ${JUCE_INCLUDE_DIR}  # Juce library root path
  ${VST_INCLUDE_DIR}  # VST SDK root path
  ${X11_INCLUDE_DIRS}  # Linux-specific stuff
)

include_directories(
  ${OPENMINI_INCLUDE_DIR}
  # This is required since Juce-generated files are looking for files there
  ${OPENMINI_PLUGIN_DIR}/vst
  ${SOUNDTAILOR_INCLUDE_DIR}
)

# Source files declaration
if(${SYSTEM_IS_MACOSX})
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
endif(${SYSTEM_IS_MACOSX})

# Disable warnings for external source
disable_warnings_list("${OPENMINI_JUCE_SRC}")

# Include all subdirectories plugin source files
add_subdirectory(vst)

source_group("juce_externals" FILES ${OPENMINI_JUCE_SRC})
source_group("openmini" FILES ${OPENMINI_SRC} ${OPENMINI_HDR})
source_group("openmini_implementation" FILES ${OPENMINI_IMPLEMENTATION_SRC} ${OPENMINI_IMPLEMENTATION_HDR})
source_group("vst_specifics" FILES ${OPENMINI_PLUGIN_VST_SRC} ${OPENMINI_PLUGIN_VST_HDR})

add_library(openmini_vst SHARED
  ${OPENMINI_JUCE_SRC}
  ${OPENMINI_SRC}
  ${OPENMINI_HDR}
  ${OPENMINI_IMPLEMENTATION_SRC}
  ${OPENMINI_IMPLEMENTATION_HDR}
  ${OPENMINI_PLUGIN_VST_SRC}
  ${OPENMINI_PLUGIN_VST_HDR}
)


# Specific Mac stuff
if (${SYSTEM_IS_MACOSX})
  # Bundle stuff
  set_target_properties(openmini_vst PROPERTIES
    BUNDLE TRUE
    BUNDLE_EXTENSION vst
    XCODE_ATTRIBUTE_WRAPPER_EXTENSION vst
    XCODE_ATTRIBUTE_MACH_O_TYPE mh_bundle
    XCODE_ATTRIBUTE_INFOPLIST_FILE ${CMAKE_CURRENT_BINARY_DIR}/bundle/Info.plist
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/bundle/Info.plist
    RESOURCE ${ARGN}
  )

  # Linker: set framework to use
  add_linker_flags(openmini_vst "-framework Accelerate")

  set(CMAKE_OSX_DEPLOYEMENT_TARGET 10.5)
  set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION com.apple.compilers.llvm.clang.1_0)

endif (${SYSTEM_IS_MACOSX})

target_link_libraries(openmini_vst
                      openmini_lib
                      soundtailor_lib)

set_target_mt(openmini_vst)
