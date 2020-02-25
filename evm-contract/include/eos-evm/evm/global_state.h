#pragma once

#include <eEVM/globalstate.h>

#include <eos-evm/evm/storage.h>
#include <eos-evm/evm/account.h>

#include <eosiolib/eosio.hpp>
#include <eosiolib/name.hpp>
#include <eosiolib/transaction.hpp>


namespace eos_evm
{


//struct
TABLE EosAccountMapRow
{
    eosio::name eos_account;
    EosUint256_t evm_address;

    uint64_t primary_key() const { return eos_account.value; }
    EosUint256_t get_evm_address() const { return evm_address; }

    EOSLIB_SERIALIZE(EosAccountMapRow, (eos_account)(evm_address))
};

typedef eosio::multi_index<
    "accmap"_n,
    EosAccountMapRow,
    eosio::indexed_by<"address"_n, eosio::const_mem_fun<EosAccountMapRow, EosUint256_t, &EosAccountMapRow::get_evm_address> >
> EosAccountMapTable;


class EosGlobalState : public eevm::GlobalState
{
public:
    EosGlobalState() = delete;
    EosGlobalState(eosio::name evm_contract_):
        evm_contract(evm_contract_),
        acc_map_table(evm_contract_, evm_contract.value)
    {
        cur_block = eevm::Block {
            static_cast<uint64_t>(tapos_block_num()), // number, for now unavailable to fetch real block num
            0u, // difficulty
            0xfffffff, // gas limit
            current_time(), // timestamp
            0u // coinbase
        };
    }

    virtual void remove(const eevm::Address& address) override;

    eevm::AccountState get(const eevm::Address& address) override;
    eevm::AccountState create(
        eosio::name eos_account,
        const eevm::Address& address,
        const uint256_t& balance,
        const eevm::Code& code
    );

    eevm::AccountState create(
        const eevm::Address& address,
        const uint256_t& balance,
        const eevm::Code& code
    ) override {};

    bool exists(const eevm::Address& address);
    bool exists(const eosio::name& account);

    virtual const eevm::Block& get_current_block() override;
    virtual uint256_t get_block_hash(uint8_t offset) override;

private:
    eosio::name evm_contract;
    EosAccountMapTable acc_map_table;
    eevm::Block cur_block;

private:
    eosio::name get_eos_name_by_address(const eevm::Address& addr) const;
};

} // namespace eos_evm
