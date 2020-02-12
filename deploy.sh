# create evm account
eos-tester cleos --wallet-url=http://localhost:6666 create account eosio evm EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

# set evm contract code
eos-tester cleos --wallet-url=http://localhost:6666 set code evm /host/build/hello.wasm

# set evm contract abi
eos-tester cleos --wallet-url=http://localhost:6666 set abi evm /host/hello.abi
