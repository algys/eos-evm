find_package(eosio.cdt)

set(CMAKE_AR "/usr/opt/eosio.cdt/1.4.1/bin/eosio-ar")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-cpp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-cpp")

add_subdirectory(libs/eEVM)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/eEVM/include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/libs/eEVM/3rdparty/intx/include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/libs/eEVM/3rdparty"
)

#set(SRC_FILES libs/bls/src/bls_c384.cpp libs/mcl/src/fp.cpp)
#add_executable(hello.wasm src/hello.cpp ${SRC_FILES})

set(EEVM_SIMPLE_SRCS
   "${CMAKE_CURRENT_SOURCE_DIR}/libs/eEVM/src/simple/simpleaccount.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/libs/eEVM/src/simple/simpleglobalstate.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/libs/eEVM/src/simple/simplestorage.cpp"
)

add_executable(hello.wasm src/hello.cpp ${EEVM_SIMPLE_SRCS})
target_link_libraries(hello.wasm eevm)

target_compile_options(hello.wasm PUBLIC "-lto-opt=O3 -fmerge-all-constants")
