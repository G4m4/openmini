# OpenMini main source folder cmake file

include_directories(
  ${OPENMINI_INCLUDE_DIR}
)

# Retrieve source files from subdirectories
add_subdirectory(filters)
add_subdirectory(generators)
add_subdirectory(modulators)
add_subdirectory(synthesizer)

# Group sources
source_group("filters"
  FILES
  ${OPENMINI_FILTERS_SRC}
  ${OPENMINI_FILTERS_HDR}
)
source_group("generators"
  FILES
  ${OPENMINI_GENERATORS_SRC}
  ${OPENMINI_GENERATORS_HDR}
)
source_group("modulators"
  FILES
  ${OPENMINI_MODULATORS_SRC}
  ${OPENMINI_MODULATORS_HDR}
)
source_group("synthesizer"
  FILES
  ${OPENMINI_SYNTHESIZER_SRC}
  ${OPENMINI_SYNTHESIZER_HDR}
)

# Sources
set(OPENMINI_SRC
  ${OPENMINI_FILTERS_SRC}
  ${OPENMINI_GENERATORS_SRC}
  ${OPENMINI_MODULATORS_SRC}
  ${OPENMINI_SYNTHESIZER_SRC}
)
set(OPENMINI_HDR
  configuration.h
  common.h
  maths.h
  ${OPENMINI_FILTERS_HDR}
  ${OPENMINI_GENERATORS_HDR}
  ${OPENMINI_MODULATORS_HDR}
  ${OPENMINI_SYNTHESIZER_HDR}
)

# Target
add_library(openmini_lib
  ${OPENMINI_SRC}
  ${OPENMINI_HDR}
)

set_target_mt(openmini_lib)
