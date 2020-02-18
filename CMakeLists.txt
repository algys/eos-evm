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

set(TARGET_CONTRACT "evm")

add_executable(${TARGET_CONTRACT} src/evm.cpp ${EEVM_EOS_SRCS})
target_link_libraries(${TARGET_CONTRACT} eevm)

target_compile_options(${TARGET_CONTRACT} PUBLIC "-abigen")
target_compile_options(${TARGET_CONTRACT} PUBLIC "-abigen_output=./${TARGET_CONTRACT}.abi")
target_compile_options(${TARGET_CONTRACT} PUBLIC "-contract=${TARGET_CONTRACT}")
target_compile_options(${TARGET_CONTRACT} PUBLIC "-lto-opt=O3 -fmerge-all-constants")
