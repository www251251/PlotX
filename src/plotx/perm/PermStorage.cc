#pragma once
#include "PermStorage.hpp"

#include "PermRegistry.hpp"

namespace plotx {

bool PermStorage::get(HashedStringView key) const {
    auto it = data.find(key);
    if (it != data.end()) {
        return it->second;
    }
    return PermRegistry::getDefault(key);
}
void PermStorage::set(HashedStringView key, bool value) {
    bool def = PermRegistry::getDefault(key);

    if (value == def) {
        // 如果新值 == 默认值,则删除
        auto it = data.find(key);
        if (it != data.end()) {
            data.erase(it);
        }
    } else {
        // 如果新值 != 默认值，存下来
        HashedString ownedKey(key.getHash(), key.getString().data());
        data.insert_or_assign(std::move(ownedKey), value);
    }
}
nlohmann::json PermStorage::toJson() const {
    nlohmann::json j = nlohmann::json::object();
    for (const auto& [hashKey, val] : data) {
        j[hashKey.getString()] = val;
    }
    return j;
}
void PermStorage::fromJson(nlohmann::json const& j) {
    data.clear();
    if (!j.is_object()) return;

    for (auto& [keyStr, valJson] : j.items()) {
        if (!valJson.is_boolean()) continue;

        bool val = valJson.get<bool>();

        HashedString hKey(keyStr);

        // 再次执行稀疏检查 (防止 JSON 里存了废数据)
        bool def = PermRegistry::getDefault(HashedStringView{hKey});
        if (val != def) {
            data.emplace(std::move(hKey), val);
        }
    }
}

} // namespace plotx