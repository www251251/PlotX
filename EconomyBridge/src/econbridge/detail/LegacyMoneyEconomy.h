#pragma once
#include "econbridge/Concepts.h"
#include "econbridge/IEconomy.h"

#include <optional>

namespace econbridge::detail {

class LegacyMoneyEconomy final : public IEconomy {
public:
    explicit LegacyMoneyEconomy();

    static bool                       isAvailable();
    static std::optional<std::string> uuid2xuid(mce::UUID const& uuid);

    int64_t get(const mce::UUID& uuid) const override;
    bool    set(const mce::UUID& uuid, int64_t amount) override;
    bool    add(const mce::UUID& uuid, int64_t amount) override;
    bool    reduce(const mce::UUID& uuid, int64_t amount) override;
    bool    transfer(const mce::UUID& from, const mce::UUID& to, int64_t amount) override;
};
static_assert(concepts::EconomyImpl<LegacyMoneyEconomy>);

} // namespace econbridge::detail
