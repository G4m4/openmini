# @brief OpenMini standalone root cmake file

# On Linux these are required in order to build an audio application
if (${SYSTEM_IS_LINUX})
  find_package(Freetype REQUIRED)
  find_package(X11 REQUIRED)
endif (${SYSTEM_IS_LINUX})

set(OPENMINI_STANDALONE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(SYSTEM
  ${FREETYPE_INCLUDE_DIRS}  # Linux-specific stuff
  ${X11_INCLUDE_DIRS}  # Linux-specific stuff
  ${JUCE_INCLUDE_DIR}  # Juce library root path
)

include_directories(
  ${OPENMINI_INCLUDE_DIR}
  # This is required since Juce-generated files are looking for files there
  ${OPENMINI_STANDALONE_DIR}
)

# Source files declaration
if(${SYSTEM_IS_MACOSX})
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
endif(${SYSTEM_IS_MACOSX})

# Disable warnings for external source
disable_warnings_list("${OPENMINI_JUCE_SRC}")

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

# MSVC-specific issues
if(${COMPILER_IS_MSVC})
  # Known MSVC issue: can't figure out whether we're building a console or windows application
  add_executable(openmini WIN32
    ${OPENMINI_JUCE_SRC}
    ${OPENMINI_SRC}
    ${OPENMINI_HDR}
    ${OPENMINI_IMPLEMENTATION_SRC}
    ${OPENMINI_IMPLEMENTATION_HDR}
    ${OPENMINI_STANDALONE_SRC}
    ${OPENMINI_STANDALONE_HDR}
  )
else()
  add_executable(openmini
    ${OPENMINI_JUCE_SRC}
    ${OPENMINI_SRC}
    ${OPENMINI_HDR}
    ${OPENMINI_IMPLEMENTATION_SRC}
    ${OPENMINI_IMPLEMENTATION_HDR}
    ${OPENMINI_STANDALONE_SRC}
    ${OPENMINI_STANDALONE_HDR}
  )
endif(${COMPILER_IS_MSVC})

# On Linux the application has to be linked with many others libraries
if (${SYSTEM_IS_LINUX})
  target_link_libraries(openmini
                        openmini_lib
                        ${CMAKE_DL_LIBS}
                        ${FREETYPE_LIBRARIES}
                        pthread
                        ${X11_LIBRARIES}
                        asound)
else()
  target_link_libraries(openmini
                        openmini_lib)
endif(${SYSTEM_IS_LINUX})

set_target_mt(openmini)
