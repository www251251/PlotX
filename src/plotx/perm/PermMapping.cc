#include "PermMapping.hpp"

#include "PermKeys.hpp"
#include "plotx/PlotX.hpp"
#include "plotx/infra/Reflection.hpp"

#include <ll/api/io/FileUtils.h>

namespace plotx {

decltype(PermMapping::mapping_) PermMapping::mapping_{};

void PermMapping::buildDefault() {
    // TODO: impl
}

ll::Expected<> PermMapping::loadUserExtension(PlotX& mod) {
    auto path = mod.getSelf().getConfigDir() / "PermMapping.json";
    if (!std::filesystem::exists(path)) {
        auto dump = reflection::struct2json(mapping_).value().dump(2);
        ll::file_utils::writeFile(path, dump);
        return {};
    }
    auto rawJson = ll::file_utils::readFile(path);
    if (!rawJson) {
        return ll::makeStringError("failed to read PermMapping.json");
    }
    try {
        auto json = nlohmann::json::parse(*rawJson);
        reflection::json2structDiffPatch(mapping_, json);
        return {};
    } catch (nlohmann::json::exception const& exception) {
        return ll::makeStringError(exception.what());
    }
}
optional_ref<const HashedString> PermMapping::get(std::string_view typeName) {
    auto it = mapping_.find(typeName);
    if (it != mapping_.end()) {
        return {it->second};
    }
    return nullptr;
}

} // namespace plotx