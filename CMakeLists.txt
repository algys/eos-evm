find_package(eosio.cdt)

set(CMAKE_AR "/usr/local/bin/eosio-ar")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-cpp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-cpp")

add_subdirectory(libs/eEVM)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/eEVM/include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/libs/eEVM/3rdparty/intx/include"
                    "${CMAKE_CURRENT_SOURCE_DIR}/libs/eEVM/3rdparty"
                    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(EEVM_EOS_SRCS
   "${CMAKE_CURRENT_SOURCE_DIR}/src/evm/account.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/evm/global_state.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/evm/storage.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/evm/types.cpp"
)

add_executable(hello.wasm src/hello.cpp ${EEVM_EOS_SRCS})
target_link_libraries(hello.wasm eevm)

target_compile_options(hello.wasm PUBLIC "-abigen")
target_compile_options(hello.wasm PUBLIC "-abigen_output=./hello.abi")
target_compile_options(hello.wasm PUBLIC "-contract=hello")
target_compile_options(hello.wasm PUBLIC "-lto-opt=O3 -fmerge-all-constants")
