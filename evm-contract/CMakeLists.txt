add_contract(evm evm
   "${CMAKE_CURRENT_SOURCE_DIR}/src/evm.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/account.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/global_state.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/storage.cpp"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/types.cpp"
)

target_include_directories(evm.wasm
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../libs/eEVM/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../libs/eEVM/3rdparty/intx/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../libs/eEVM/3rdparty)

target_link_libraries(evm.wasm eevm)