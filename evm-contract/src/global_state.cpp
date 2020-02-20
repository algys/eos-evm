#include <eos-evm/evm/global_state.h>

#include <eosio/eosio.hpp>


namespace eos_evm
{

void EosGlobalState::remove(const eevm::Address& address)
{
    if (!exists(address))
        return;

    auto eos_name = get_eos_name_by_address(address);
    EosAccount account(evm_contract, eos_name);
    account.remove();

    auto itr = acc_map_table.find(eos_name.value);
    acc_map_table.erase(itr);
}

eevm::AccountState EosGlobalState::get(const eevm::Address& address)
{
    if (!exists(address))
        return create(address, 0, {});

    auto eos_name = get_eos_name_by_address(address);
    EosAccount* account = new EosAccount(evm_contract, eos_name);
    EosStorage* storage = new EosStorage (evm_contract, eos_name);

    return eevm::AccountState(*account, *storage);
}

eevm::AccountState EosGlobalState::create(
    eosio::name eos_account,
    const eevm::Address& address,
    const uint256_t& balance,
    const eevm::Code& code
)
{
    eosio::print_f("INVOKED create\n");
    if (exists(address))
        return get(address);

    EosAccount* account = new EosAccount(evm_contract, eos_account);
    EosStorage* storage = new EosStorage(evm_contract, eos_account);
    account->create(address, balance, code);

    acc_map_table.emplace(evm_contract, [&](auto& row){
        row.eos_account = eos_account;
        row.evm_address = convert(address);
    });


    return eevm::AccountState(*account, *storage);
}

bool EosGlobalState::exists(const eevm::Address& address)
{
    auto idx = acc_map_table.get_index<"address"_n>();
    auto itr = idx.find(convert(address));

    return itr != idx.end();
}

const eevm::Block& EosGlobalState::get_current_block()
{
    return cur_block;
}

uint256_t EosGlobalState::get_block_hash(uint8_t offset)
{
    return 0u;
}


eosio::name EosGlobalState::get_eos_name_by_address(const eevm::Address& address) const
{
    auto idx = acc_map_table.get_index<"address"_n>();
    auto itr = idx.find(convert(address));

    return itr->eos_account;
}
} // namespace eos_evm
