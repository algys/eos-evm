#pragma once

#include <eEVM/account.h>

#include <eosio/eosio.hpp>


namespace eos_evm
{

using EosUint256_t = eosio::fixed_bytes<32>;

EosUint256_t convert(const eevm::Address& val);
eevm::Address convert(const EosUint256_t& val);

} // namespace eos_evm
