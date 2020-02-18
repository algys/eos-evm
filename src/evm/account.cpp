#include <eos-evm/evm/account.h>

#include <eosiolib/eosio.hpp>


namespace eos_evm
{

void EosAccount::remove()
{
    const auto itr = account_table.find(0);
    account_table.erase(itr);
}

void EosAccount::create(
    const eevm::Address& address,
    const uint256_t& balance,
    const eevm::Code& code
)
{
    account_table.emplace(evm_contract, [&](auto& row){
        row.address = convert(address);
        row.balance.set_amount((uint64_t)balance);
    });

    if (!code.empty()) {
        code_table.emplace(evm_contract, [&](auto& row){
            row.code = code;
        });
    }
}

eevm::Address EosAccount::get_address() const
{
    const auto itr = account_table.find(0);
    return convert(itr->address);
}

void EosAccount::set_address(const eevm::Address& address)
{
    auto itr = account_table.find(0);
    account_table.modify(itr, evm_contract, [&](auto& row){
        row.address = convert(address);
    });
}

uint256_t EosAccount::get_balance() const
{
    const auto itr = account_table.find(0);
    return itr->balance.amount;
}

void EosAccount::set_balance(const uint256_t& b)
{
    auto itr = account_table.find(0);
    account_table.modify(itr, evm_contract, [&](auto& row){
        row.balance.set_amount((uint64_t)b);
    });
}

eevm::Account::Nonce EosAccount::get_nonce() const
{
    const auto itr = account_table.find(0);
    return itr->nonce;
}

void EosAccount::set_nonce(eevm::Account::Nonce n)
{
    auto itr = account_table.find(0);
    account_table.modify(itr, evm_contract, [&](auto& row){
        row.nonce = n;
    });
}

void EosAccount::increment_nonce()
{
    auto itr = account_table.find(0);
    account_table.modify(itr, evm_contract, [&](auto& row){
        row.nonce++;
    });
}

eevm::Code EosAccount::get_code() const
{
    const auto itr = code_table.find(0);
    if (itr != code_table.end())
        return itr->code;
    return eevm::Code();
}

void EosAccount::set_code(eevm::Code&& code)
{
    auto itr = code_table.find(0);
    if (itr != code_table.end())
        code_table.modify(itr, evm_contract, [&](auto& row){
            row.code = std::move(code);
        });
    else
        code_table.emplace(evm_contract, [&](auto& row){
            row.code = std::move(code);
        });
}

bool EosAccount::has_code()
{
    const auto itr = code_table.find(0);
    return itr != code_table.end();
}

} // namespace eos_evm

