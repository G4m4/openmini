# OpenMini main source folder cmake file

include_directories(
  ${OPENMINI_INCLUDE_DIR}
  ${SOUNDTAILOR_INCLUDE_DIR}
)

# Retrieve source files from subdirectories
add_subdirectory(synthesizer)

# Group sources
source_group("synthesizer"
  FILES
  ${OPENMINI_SYNTHESIZER_SRC}
  ${OPENMINI_SYNTHESIZER_HDR}
)

# Sources
set(OPENMINI_SRC
  samplingrate.cc
  ${OPENMINI_SYNTHESIZER_SRC}
)
set(OPENMINI_HDR
  configuration.h
  common.h
  maths.h
  samplingrate.h
  ${OPENMINI_SYNTHESIZER_HDR}
)

# Target
add_library(openmini_lib
  ${OPENMINI_SRC}
  ${OPENMINI_HDR}
)

set_target_mt(openmini_lib)
