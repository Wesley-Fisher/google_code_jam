SET(MY_PROJECT "" STRING FORCE)

if("${MYPROJECT}" STREQUAL "")
  message( FATAL_ERROR "MYPROJECT not set" )
endif()

project(CODE_JAM)


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

return()

####################
## Create Unit Tests

add_executable(
  tests

  problem.cpp
  ../../src/main_standard.h
)

target_compile_definitions(tests PRIVATE TEST)
target_compile_definitions(tests PRIVATE NOMAIN)

target_link_libraries(
  tests
  gtest_main
)

set_target_properties( tests
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

include(GoogleTest)
gtest_discover_tests(tests)



####################
## Create Verbose Unit Tests

add_executable(
  tests_verbose
  problem.cpp
)

target_compile_definitions(tests_verbose PRIVATE TEST)
target_compile_definitions(tests_verbose PRIVATE NOMAIN)
target_compile_definitions(tests_verbose PRIVATE LOCAL)

target_link_libraries(
  tests_verbose
  gtest_main
)


set_target_properties( tests_verbose
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

include(GoogleTest)
gtest_discover_tests(tests_verbose)





##################
## Create Analysis Runnable

add_executable(
  analysis
  problem.cpp
)

target_compile_definitions(analysis PRIVATE ANALYSIS)

set_target_properties( analysis
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)



##################
## Create Verbose Analysis Runnable

add_executable(
  analysis_verbose
  problem.cpp
)

target_compile_definitions(analysis_verbose PRIVATE LOCAL)
target_compile_definitions(analysis_verbose PRIVATE ANALYSIS)

set_target_properties( analysis_verbose
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)
