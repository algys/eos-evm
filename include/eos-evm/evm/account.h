#pragma once

#include <eEVM/account.h>

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>

#include <eos-evm/evm/types.h>


namespace eos_evm
{

//struct
//TABLE
struct [[eosio::table]] EosAccountRow
{
    EosUint256_t address;
    eevm::Account::Nonce nonce;
    eosio::asset balance;

    uint64_t primary_key() const { return 0; }

    EOSLIB_SERIALIZE(eos_evm::EosAccountRow, (address)(nonce)(balance))
};

typedef eosio::multi_index<
    "account"_n,
    EosAccountRow
> EosAccountTable;


//struct
TABLE EosCodeRow
{
    eevm::Code code;

    uint64_t primary_key() const { return 0; }

    EOSLIB_SERIALIZE(eos_evm::EosCodeRow, (code))
};

typedef eosio::multi_index<
    "code"_n,
    EosCodeRow
> EosCodeTable;


class EosAccount: public eevm::Account
{
public:
    EosAccount(eosio::name evm_contract_, eosio::name account_):
        evm_contract(evm_contract_),
        account(account_),
        account_table(evm_contract_, account.value),
        code_table(evm_contract_, account.value)
    { }

    void create(
        const eevm::Address& address,
        const uint256_t& balance,
        const eevm::Code& code
    );
    void remove();

    virtual eevm::Address get_address() const override;
    void set_address(const eevm::Address& a);

    virtual uint256_t get_balance() const override;
    virtual void set_balance(const uint256_t& b) override;

    virtual eevm::Account::Nonce get_nonce() const override;
    void set_nonce(eevm::Account::Nonce n);
    virtual void increment_nonce() override;

    virtual eevm::Code get_code() const override;
    virtual void set_code(eevm::Code&& c) override;
    virtual bool has_code() override;

private:
    eosio::name evm_contract;
    eosio::name account;
    EosAccountTable account_table;
    EosCodeTable code_table;
};



} // namespace eos_evm


