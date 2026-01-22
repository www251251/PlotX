#pragma once
#include "econbridge/Concepts.h"
#include "econbridge/IEconomy.h"

namespace econbridge::detail {

/**
 * @note This economy does nothing.
 */
class NullEconomy final : public IEconomy {
public:
    static bool isAvailable();

    int64_t get(const mce::UUID& uuid) const override;
    bool    set(const mce::UUID& uuid, int64_t amount) override;
    bool    add(const mce::UUID& uuid, int64_t amount) override;
    bool    reduce(const mce::UUID& uuid, int64_t amount) override;
    bool    transfer(const mce::UUID& from, const mce::UUID& to, int64_t amount) override;
};
static_assert(concepts::EconomyImpl<NullEconomy>);


} // namespace econbridge::detail
