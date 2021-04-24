###########
# Get GTest
# https://google.github.io/googletest/quickstart-cmake.html

cmake_minimum_required(VERSION 3.14)
project(my_gtest_test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)



####################
## Create Unit Tests

add_executable(
  tests
  problem.cpp problem.h
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



##################
## Create Runnable

add_executable(
  main
  problem.cpp problem.h
)

target_compile_definitions(main PRIVATE LOCAL)

set_target_properties( main
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)
