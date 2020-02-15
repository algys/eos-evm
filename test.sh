# prepare accounts
echo "CREATING accounts for testing..."
eos-tester cleos --wallet-url=http://localhost:6666 create account eosio algys EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
eos-tester cleos --wallet-url=http://localhost:6666 create account eosio emil EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
eos-tester cleos --wallet-url=http://localhost:6666 create account eosio alisa EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
eos-tester cleos --wallet-url=http://localhost:6666 create account eosio bob EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV


# send deploy contracts
echo "DEPLOYING EVM contracts..."
eos-tester cleos --wallet-url=http://localhost:6666 push action evm deploy '{"eos_acc":"algys","evm_acc":1}' -p algys
eos-tester cleos --wallet-url=http://localhost:6666 push action evm deploy '{"eos_acc":"emil","evm_acc":2}' -p emil
eos-tester cleos --wallet-url=http://localhost:6666 push action evm deploy '{"eos_acc":"alisa","evm_acc":3}' -p alisa
eos-tester cleos --wallet-url=http://localhost:6666 push action evm deploy '{"eos_acc":"bob","evm_acc":4}' -p bob


# invoke contracts
echo "RUNNING EVM method..."
eos-tester cleos --wallet-url=http://localhost:6666 push action evm run '{"evm_acc":1}' -p algys
eos-tester cleos --wallet-url=http://localhost:6666 push action evm run '{"evm_acc":2}' -p emil
eos-tester cleos --wallet-url=http://localhost:6666 push action evm run '{"evm_acc":3}' -p alisa
eos-tester cleos --wallet-url=http://localhost:6666 push action evm run '{"evm_acc":4}' -p bob


echo "CREATED accounts:"
eos-tester cleos get table evm evm accmap
