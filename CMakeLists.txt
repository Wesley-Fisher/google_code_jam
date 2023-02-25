SET(MY_PROJECT "" STRING FORCE)

if("${MYPROJECT}" STREQUAL "")
  message( FATAL_ERROR "MYPROJECT not set" )
endif()

project(CODE_JAM)

find_package(Boost REQUIRED)

####################################
## Create Runnable
####################################

add_executable( main
  ${MYPROJECT}/problem.cpp
)

include_directories( main
  src
)

target_compile_definitions(main PRIVATE LOCAL)

set_target_properties( main
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "../bin"
)


####################################
## Create Unit Tests
####################################

add_executable( tests
  ${MYPROJECT}/problem.cpp
)

include_directories( tests
  src
  ${Boost_INCLUDE_DIR}
)

target_compile_definitions( tests
  PRIVATE TEST
)

target_compile_definitions( tests
  PRIVATE NOMAIN
)

target_link_libraries( tests
  ${Boost_LIBRARIES}
)

set_target_properties( tests
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "../bin"
)


####################################
## Create Verbose Unit Tests
####################################

add_executable( tests_verbose
  ${MYPROJECT}/problem.cpp
)

include_directories( tests_verbose
  src
  ${Boost_INCLUDE_DIR}
)

target_compile_definitions( tests_verbose
  PRIVATE TEST
)

target_compile_definitions( tests_verbose
  PRIVATE NOMAIN
)
target_compile_definitions( tests_verbose
  PRIVATE LOCAL
)

target_link_libraries( tests_verbose
  ${Boost_LIBRARIES}
)

set_target_properties( tests_verbose
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "../bin"
)


##################
## Create Analysis Runnable

add_executable( analysis
   ${MYPROJECT}/problem.cpp
)

target_compile_definitions( analysis
  PRIVATE ANALYSIS
)

set_target_properties( analysis
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "../bin"
)



##################
## Create Verbose Analysis Runnable

add_executable( analysis_verbose
   ${MYPROJECT}/problem.cpp
)

target_compile_definitions( analysis_verbose
  PRIVATE LOCAL
)

target_compile_definitions( analysis_verbose
  PRIVATE ANALYSIS
)

set_target_properties( analysis_verbose
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "../bin"
)
