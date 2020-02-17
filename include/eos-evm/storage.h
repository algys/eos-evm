#pragma once

#include <eEVM/storage.h>

#include <eosio/eosio.hpp>

#include <eos-evm/types.h>


namespace eos_evm
{


//struct
TABLE EosStorageRow
{
    EosUint256_t key; // real key;
    EosUint256_t value; // value

    uint64_t primary_key() const { return 0; } // predefined key
    EosUint256_t get_key() const { return key; }

    EOSLIB_SERIALIZE(EosStorageRow, (key)(value))
};

typedef eosio::multi_index<
    "storage"_n,
    EosStorageRow,
    eosio::indexed_by<"key"_n, eosio::const_mem_fun<EosStorageRow, EosUint256_t, &EosStorageRow::get_key> >
> EosStorageTable;


class EosStorage: public eevm::Storage
{
public:
    EosStorage() = delete;
    EosStorage(eosio::name evm_contract_, eosio::name account_):
        evm_contract(evm_contract_),
        account(account_),
        table(evm_contract, account.value)
    { }

    void store(const uint256_t& key, const uint256_t& value) override;
    uint256_t load(const uint256_t& key) override;
    bool exists(const uint256_t& key);
    bool remove(const uint256_t& key) override;

private:
    eosio::name evm_contract;
    eosio::name account;
    EosStorageTable table;
};



} //namespace eos_evm
