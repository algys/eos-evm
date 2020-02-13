#pragma once

#include <eEVM/account.h>

#include <eosiolib/eosio.hpp>


namespace eos_evm
{

using EosUint256_t = capi_checksum256;
using Address = EosUint256_t;

EosUint256_t convert(const uint256_t& val)
{
    EosUint256_t ret_val;
    intx::le::store(ret_val.hash, val);
    return ret_val;
}

uint256_t convert(const EosUint256_t& val)
{
    uint256_t ret_val;
    ret_val = intx::le::load<uint256_t, 32>(val.hash);
    return ret_val;
}

}
