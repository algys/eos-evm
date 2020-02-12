#include <eosiolib/eosio.hpp>

#include <string>
#include <vector>

#include <eEVM/processor.h>
#include <eEVM/simple/simpleglobalstate.h>
#include <eEVM/opcode.h>

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
    using contract::contract;

    [[eosio::action]]
    void hi(name user) {
        print("Hello, ", name { user });
    }

    [[eosio::action]]
    void create(const std::string& hello) {
        const eevm::Code code = create_bytecode(hello);

        // Create to address
        std::vector<uint8_t> raw_address(20);
        raw_address[3] = 3;
        const eevm::Address to = eevm::from_big_endian(raw_address.data(), raw_address.size());

        //Create sender address
        raw_address[4] = 4;
        const eevm::Address sender = eevm::from_big_endian(raw_address.data(), raw_address.size());

        // Create global state
        eevm::SimpleGlobalState gs;

        // Create contract
        const eevm::AccountState contract = gs.create(to, 0, code);

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

EOSIO_DISPATCH(hello, (hi)(create))
