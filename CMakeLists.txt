cmake_minimum_required(VERSION 3.5)
project(eos-evm)
set(EOSIO_WASM_OLD_BEHAVIOR "Off")
# set(CMAKE_AR "/usr/local/bin/eosio-ar")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-cpp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-cpp")
include(ExternalProject)
find_package(eosio.cdt)
add_subdirectory(evm-contract)
add_subdirectory(libs/eEVM)

message(STATUS "Building unit tests.")
ExternalProject_Add(
  contracts_unit_tests
  LIST_SEPARATOR | # Use the alternate list separator
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=Debug
  #-DCMAKE_PREFIX_PATH=${TEST_PREFIX_PATH} -DCMAKE_FRAMEWORK_PATH=${TEST_FRAMEWORK_PATH} -DCMAKE_MODULE_PATH=${TEST_MODULE_PATH} -DEOSIO_ROOT=${EOSIO_ROOT} -DLLVM_DIR=${LLVM_DIR} -DBOOST_ROOT=${BOOST_ROOT}
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/tests
  BINARY_DIR ${CMAKE_BINARY_DIR}/tests
  BUILD_ALWAYS 1
  TEST_COMMAND   ""
  INSTALL_COMMAND ""
)