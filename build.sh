mkdir build
cd build

cmake -DCMAKE_TOOLCHAIN_FILE=/usr/lib/cmake/eosio.cdt/EosioWasmToolchain.cmake ..

make
