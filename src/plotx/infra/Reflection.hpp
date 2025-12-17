#pragma once
#include "ll/api/reflection/Deserialization.h"
#include "ll/api/reflection/Reflection.h"
#include "ll/api/reflection/Serialization.h"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include <type_traits>


namespace plotx::reflection {


template <class T>
[[nodiscard]] inline decltype(auto) struct2json(T& t) {
    return ll::reflection::serialize<nlohmann::ordered_json>(t);
}

template <class T, class J = nlohmann::ordered_json>
inline void json2struct(T& t, J& j) {
    ll::reflection::deserialize(t, j).value();
}

template <class T, class J = nlohmann::ordered_json>
inline void json2structDiffPatch(T& t, J& j) {
    auto diff = struct2json(t).value();
    diff.merge_patch(j);
    json2struct(t, j);
}

template <class T>
concept HasVersion_v =
    ll::reflection::Reflectable<T> && std::integral<std::remove_cvref_t<decltype((std::declval<T>().version))>>;

template <HasVersion_v T, class J = nlohmann::ordered_json>
inline bool _merge_patch(T& obj, J& data) {
    data.erase("version");
    auto patch = ll::reflection::serialize<J>(obj);
    patch.value().merge_patch(data);
    data = *std::move(patch);
    return true;
}

template <HasVersion_v T, class J = nlohmann::ordered_json, class F = bool(T&, J&)>
inline void json2structVersionPatch(J& json, T& obj, F&& fixer = _merge_patch<T, J>) {
    bool noNeedMerge = true;
    if (!json.contains("version") || (int64)(json["version"]) != obj.version) {
        noNeedMerge = false;
    }
    if (noNeedMerge || std::invoke(std::forward<F>(fixer), obj, json)) {
        ll::reflection::deserialize(obj, json).value();
    }
}


} // namespace plotx::reflection