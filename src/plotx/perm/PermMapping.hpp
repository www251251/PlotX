#pragma once
#include "plotx/infra/HashedStringView.hpp"

#include <ll/api/Expected.h>
#include <unordered_map>

namespace plotx {
class PlotX;

struct PermMapping {
    using AnyTypeName = HashedString;
    using PermKey     = HashedString;

    PermMapping() = delete;

    static void           buildDefault();
    static ll::Expected<> loadUserExtension(PlotX& mod);

    static optional_ref<const HashedString> get(std::string_view typeName);

private:
    static std::unordered_map<AnyTypeName, PermKey, HashedStringHasher, HashedStringEqual> mapping_;
};

} // namespace plotx
