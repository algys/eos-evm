#include <eos-evm/evm/types.h>


namespace eos_evm
{

EosUint256_t convert(const eevm::Address& val)
{
    uint8_t arr[32];
    intx::be::store<256>(arr, val);
    return EosUint256_t(arr);
}

eevm::Address convert(const EosUint256_t& val)
{
    uint8_t arr[32];
    memcpy(arr, val.extract_as_byte_array().data(), 32);
    return intx::be::load<uint256_t, 32>(arr);
}

} // namespace eos_evm
