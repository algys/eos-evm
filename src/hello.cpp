#include <eosio/eosio.hpp>

#include <string>
#include <vector>

#include <eEVM/processor.h>
#include <eEVM/opcode.h>

#include <eos-evm/global_state.h>

#include <eos-evm/account.h>
#include <eos-evm/storage.h>

using namespace eosio;

std::vector<uint8_t> create_bytecode(const std::string& s) {
    std::vector<uint8_t> code;
    constexpr uint8_t mdest = 0x0;
    const uint8_t rsize = s.size() + 1;

    // Store each byte in evm memory
    uint8_t mcurrent = mdest;
    for (const char& c : s) {
        code.push_back(eevm::Opcode::PUSH1);
        code.push_back(c);
        code.push_back(eevm::Opcode::PUSH1);
        code.push_back(mcurrent++);
        code.push_back(eevm::Opcode::MSTORE8);
    }

    // Return
    code.push_back(eevm::Opcode::PUSH1);
    code.push_back(rsize);
    code.push_back(eevm::Opcode::PUSH1);
    code.push_back(mdest);
    code.push_back(eevm::Opcode::RETURN);

    return code;
}

class [[eosio::contract]] hello : public eosio::contract {
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
    void hi(name user) {
        print("Hello,  ", name { user });
    }

    [[eosio::action]]
    void deploy(eosio::name eos_acc, uint8_t evm_acc) {
        // Check auth
        require_auth(eos_acc);

        // Generate EVM code
        const eevm::Code code = create_bytecode(eos_acc.to_string() + ", Hello from EVM !");

        // Create to address
        std::vector<uint8_t> raw_address(20);
        raw_address[19] = evm_acc;
        const eevm::Address to = eevm::from_big_endian(raw_address.data(), raw_address.size());

        // Create global state
        eos_evm::EosGlobalState gs(_self);

        // Create contract
        const eevm::AccountState contract = gs.create(eos_acc, to, 0, code);

        eosio::print_f("DEPLOYED\n");
    }

    [[eosio::action]]
    void run(uint8_t evm_acc) {
        // Create to address
        std::vector<uint8_t> raw_address(20);
        raw_address[19] = evm_acc;
        const eevm::Address to = eevm::from_big_endian(raw_address.data(), raw_address.size());

        //Create sender address [DUMMY]
        raw_address[4] = 4;
        const eevm::Address sender = eevm::from_big_endian(raw_address.data(), raw_address.size());

        // Create global state
        eos_evm::EosGlobalState gs(_self);

        // Create contract
        const eevm::AccountState contract = gs.get(to);

        // Create transaction
        eevm::NullLogHandler ignore;
        eevm::Transaction tx(sender, ignore);

        // Create processor
        eevm::Processor p(gs);

        // Execute code. All execution is associated with a transaction. This
        // transaction is called by sender, executing the code in contract, with empty
        // input (and no trace collection)
        const eevm::ExecResult e = p.run(tx, sender, contract, {}, 0, nullptr);

        // Check the response
        if (e.er != eevm::ExitReason::returned) {
            eosio::print_f("uexpected return code: %\n", (size_t)e.er);
            return;
        }

        // Create string from response data, and print it
        const std::string response(reinterpret_cast<const char*>(e.output.data()));

        eosio::print_f("ok: %\n", response.data());
    }
};

EOSIO_DISPATCH(hello, (hi)(deploy)(run))
