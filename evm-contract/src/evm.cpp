#include <eosiolib/eosio.hpp>

#include <string>
#include <vector>

#include <eEVM/processor.h>
#include <eEVM/opcode.h>
#include <eEVM/rlp.h>
#include <eEVM/util.h>

#include <eos-evm/evm/global_state.h>
#include <eos-evm/evm/account.h>
#include <eos-evm/evm/storage.h>

using namespace eosio;
using namespace eevm;

struct evm_trx_type {
    uint256_t nonce;
    uint256_t gas_price;
    uint256_t gas_limit;
    uint256_t to;
    uint256_t value;
    std::vector<uint8_t> data;
    uint256_t v,r,s;
};

evm_trx_type decode_raw_trx(const std::vector<char>& bytes) {
    const uint8_t* data = (uint8_t*)bytes.data();
    size_t size = bytes.size();

    auto evm_trx_tuple = eevm::rlp::decode<
        uint256_t, //nonce
        uint256_t, //gas_price
        uint256_t, //gas_limit
        uint256_t, //to
        uint256_t, //value
        std::vector<uint8_t>, //data
        uint256_t, //v
        uint256_t, //r
        uint256_t  //s
    >(data, size);

    return evm_trx_type {
        std::get<0>(evm_trx_tuple),
        std::get<1>(evm_trx_tuple),
        std::get<2>(evm_trx_tuple),
        std::get<3>(evm_trx_tuple),
        std::get<4>(evm_trx_tuple),
        std::get<5>(evm_trx_tuple),
        std::get<6>(evm_trx_tuple),
        std::get<7>(evm_trx_tuple),
        std::get<8>(evm_trx_tuple)
    };
}


class [[eosio::contract]] evm : public eosio::contract {
public:

/* ##########-USES ONLY FOR ABI GENERATION-########### */
    struct [[eosio::table("account")]] EosAccountRow_
    {
        eosio::checksum256 address;
        size_t nonce;
        eosio::asset balance;
    };

    struct [[eosio::table("code")]] EosCodeRow_
    {
        std::vector<uint8_t> code;
    };

    struct [[eosio::table("accmap")]] EosAccountMapRow_
    {
        eosio::name eos_account;
        eosio::checksum256 evm_address;
    };

    struct [[eosio::table("storage")]] EosStorageRow_
    {
        eosio::checksum256 key;
        eosio::checksum256 value;
    };
/* ################################################### */

public:
    using contract::contract;

    [[eosio::action]]
    void raw(const std::vector<char>& raw_trx) {
        // decode RLP transaction
        auto trx = decode_raw_trx(raw_trx);

        //TODO check sign & recover sender
        // Sender mock
        std::vector<uint8_t> raw_address(20);
        raw_address[4] = 4;
        const eevm::Address sender = eevm::from_big_endian(raw_address.data(), raw_address.size());

        // Create global state
        eos_evm::EosGlobalState gs(_self);

        // Check `to` account
        eosio_assert(gs.exists(trx.to), "'to' account doesn't exists");

        // Fetch `to` account
        const eevm::AccountState contract = gs.get(trx.to);

        // Create eEVM transaction
        eevm::NullLogHandler ignore;
        // TODO fix uint64_t
        eevm::Transaction tx(sender, ignore, (uint64_t)trx.value, (uint64_t)trx.gas_price, (uint64_t)trx.gas_limit);

        // Create processor
        eevm::Processor p(gs);

        // Exec transaction
        const eevm::ExecResult e = p.run(tx, sender, contract, trx.data, 0, nullptr);

        // Check the response
        if (e.er != eevm::ExitReason::returned) {
            eosio_assert(false, "EVM error"); //TODO detailed error
        }

        // Create string from response data, and print it TODO normal print
        const std::string response(reinterpret_cast<const char*>(e.output.data()));
        eosio::print_f("result: %\n", response.data());
    }

    [[eosio::action]]
    void create(const name& creator, const std::string& input_string) {
        require_auth(creator);
        eos_evm::EosGlobalState gs(_self);
        const auto keccak_hash = keccak_256(rlp::encode(creator.to_string(), input_string));
        const std::string rightmost_bytes(keccak_hash.begin(), keccak_hash.begin() + 20);
        Address eth_address = to_uint256(rightmost_bytes);
        eosio_assert(gs.exists(eth_address), "creator account already exists");
        gs.create(eth_address, 0, {});
    }

    [[eosio::action]]
    void deploy(const name& creator) {
        require_auth(creator);
        const auto keccak_hash = keccak_256(rlp::encode(creator.to_string()));
    }
};

EOSIO_DISPATCH(evm, (raw)(create)(deploy))
