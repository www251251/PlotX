#pragma once
#include <cstdint>
#include <memory>
#include <string>

namespace mce {
class UUID;
}

namespace econbridge {

class IEconomy {
public:
    using Ptr = std::shared_ptr<IEconomy>;

    virtual ~IEconomy() = default;

    // 获取玩家余额
    virtual int64_t get(mce::UUID const& uuid) const = 0;

    // 设置玩家余额
    virtual bool set(mce::UUID const& uuid, int64_t amount) = 0;

    // 增加玩家余额
    virtual bool add(mce::UUID const& uuid, int64_t amount) = 0;

    // 扣除玩家余额
    virtual bool reduce(mce::UUID const& uuid, int64_t amount) = 0;

    // 转账
    virtual bool transfer(mce::UUID const& from, mce::UUID const& to, int64_t amount) = 0;
};

} // namespace econbridge