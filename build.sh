mkdir build
cd build

cmake -DCMAKE_TOOLCHAIN_FILE=/usr/local/lib/cmake/eosio.cdt/EosioWasmToolchain.cmake ..
make
