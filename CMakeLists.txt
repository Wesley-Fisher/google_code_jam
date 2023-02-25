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
## Create Runnable

add_executable(
  main
  problem.cpp
)

target_compile_definitions(main PRIVATE LOCAL)

set_target_properties( main
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)



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
