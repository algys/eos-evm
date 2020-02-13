#include <eEVM/util.h>

#include <eosiolib/eosio.hpp>

#include <eos-evm/storage.h>


namespace eos_evm
{

void SimpleStorage::store(const uint256_t& key, const uint256_t& value)
{
    auto idx = table.get_index<"key"_n>();
    auto itr = idx.find(convert(key));

    if (itr != idx.end()) {
        table.modify(itr, 0, [&](auto& row){
            row.value = convert(value);
        });
    }
    else {
        table.emplace(evm_contract, [&](auto& row){
            row.key = convert(key;)
            row.value = convert(value);
        });
    }
}

uint256_t SimpleStorage::load(const uint256_t& key)
{
    auto idx = table.get_index<"key"_n>();
    auto itr = idx.find(convert(key));

    if (itr == idx.end())
        return 0;
    return convert(itr->value);
}

bool SimpleStorage::exists(const uint256_t& key)
{
    auto idx = table.get_index<"key"_n>();
    auto itr = idx.find(convert(key));

    return itr != idx.end();
}

bool SimpleStorage::remove(const uint256_t& key)
{
    auto idx = table.get_index<"key"_n>();
    auto itr = idx.find(key);

    if (itr == idx.end())
        return false;

    table.erase(itr);
    return true;
}

} // namespace eos_evm
