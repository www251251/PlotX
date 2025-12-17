#pragma once
#include "plotx/infra/HashedStringView.hpp"

#include <ll/api/Expected.h>

namespace plotx {
class PlotX;
struct PermMeta;
enum class PermCategory;
} // namespace plotx

namespace plotx {

class PermRegistry {
    static std::unordered_map<HashedString, PermMeta, HashedStringHasher, HashedStringEqual> perms;

public:
    PermRegistry() = delete;

    // mod internal use
    static void           buildDefault();
    static ll::Expected<> loadOverrides(PlotX& mod);

    // public api
    static ll::Expected<> registerPerm(HashedStringView key, bool defVal, PermCategory category);

    static bool getDefault(HashedStringView key);

    static std::string_view getName(HashedStringView key);

    static optional_ref<PermMeta> getMeta(HashedStringView key);
};

} // namespace plotx
