# @brief OpenMini standalone root cmake file

set(OPENMINI_STANDALONE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(SYSTEM
  ${JUCE_INCLUDE_DIR}  # Juce library root path
)

include_directories(
  ${OPENMINI_INCLUDE_DIR}
  # This is required since Juce-generated files are looking for files there
  ${OPENMINI_STANDALONE_DIR}
)

# Source files declaration
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # On Mac we have to pass through Juce wrappers (.mm files)
  set(OPENMINI_JUCE_SRC
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_basics/juce_audio_basics.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_devices/juce_audio_devices.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_formats/juce_audio_formats.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_processors/juce_audio_processors.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_utils/juce_audio_utils.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_core/juce_core.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_cryptography/juce_cryptography.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_data_structures/juce_data_structures.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_events/juce_events.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_graphics/juce_graphics.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_gui_basics/juce_gui_basics.mm
    ${JUCE_INCLUDE_DIR}/modules/juce_gui_extra/juce_gui_extra.mm
  )
else()
  set(OPENMINI_JUCE_SRC
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_basics/juce_audio_basics.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_devices/juce_audio_devices.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_formats/juce_audio_formats.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_processors/juce_audio_processors.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_audio_utils/juce_audio_utils.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_core/juce_core.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_cryptography/juce_cryptography.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_data_structures/juce_data_structures.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_events/juce_events.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_graphics/juce_graphics.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_gui_basics/juce_gui_basics.cpp
    ${JUCE_INCLUDE_DIR}/modules/juce_gui_extra/juce_gui_extra.cpp
  )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Look for all standalone-specific files
set (OPENMINI_STANDALONE_SRC
     ${OPENMINI_STANDALONE_DIR}/MainApplication.cpp
)
set (OPENMINI_STANDALONE_HDR
     ${OPENMINI_STANDALONE_DIR}/AppConfig.h
     ${OPENMINI_STANDALONE_DIR}/JuceHeader.h
     ${OPENMINI_STANDALONE_DIR}/MainApplication.h
)

source_group("juce_externals" FILES ${OPENMINI_JUCE_SRC})
source_group("openmini" FILES ${OPENMINI_SRC} ${OPENMINI_HDR})
source_group("openmini_implementation" FILES ${OPENMINI_IMPLEMENTATION_SRC} ${OPENMINI_IMPLEMENTATION_HDR})
source_group("standalone_specifics" FILES ${OPENMINI_STANDALONE_SRC} ${OPENMINI_STANDALONE_HDR})

add_executable(openmini
  ${OPENMINI_JUCE_SRC}
  ${OPENMINI_SRC}
  ${OPENMINI_HDR}
  ${OPENMINI_IMPLEMENTATION_SRC}
  ${OPENMINI_IMPLEMENTATION_HDR}
  ${OPENMINI_STANDALONE_SRC}
  ${OPENMINI_STANDALONE_HDR}
)

target_link_libraries(openmini
                      openmini_lib)

set_target_mt(openmini)
