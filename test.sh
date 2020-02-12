
# send hello action
eos-tester cleos --wallet-url=http://localhost:6666 push action evm create '{"hello":"Hello_from_EVM"}' -p evm
