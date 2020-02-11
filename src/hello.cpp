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

        // Create global state
        eevm::SimpleGlobalState gs;

        // Create contract
        const eevm::AccountState contract = gs.create(to, 0, code);
    }
};

EOSIO_DISPATCH(hello, (hi)(create))
