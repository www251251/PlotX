#pragma once
#include "plotx/infra/HashedStringView.hpp"

#include "nlohmann/json.hpp"

#include <unordered_map>

namespace plotx {

/**
 * 稀疏存储容器
 */
class PermStorage {
    std::unordered_map<HashedString, bool, HashedStringHasher, HashedStringEqual> data;

public:
    bool get(HashedStringView key) const;
    void set(HashedStringView key, bool value);

    nlohmann::json toJson() const;
    void           fromJson(nlohmann::json const& j);
};

} // namespace plotx