#include "PermRegistry.hpp"
#include "PermKeys.hpp"
#include "PermMeta.hpp"
#include "ll/api/reflection/Serialization.h"
#include "nlohmann/json_fwd.hpp"
#include "plotx/PlotX.hpp"
#include "plotx/infra/Reflection.hpp"

#include <ll/api/io/FileUtils.h>

namespace plotx {

decltype(PermRegistry::perms) PermRegistry::perms{};

void PermRegistry::buildDefault() {
    // === 基础权限 ===
    registerPerm(perm_keys::allowDestroy, false, PermCategory::Core);
    registerPerm(perm_keys::allowPlace, false, PermCategory::Core);
    registerPerm(perm_keys::allowInteract, false, PermCategory::Core);

    // === 战斗权限 ===
    registerPerm(perm_keys::allowPlayerDamage, false, PermCategory::Combat);
    registerPerm(perm_keys::allowMonsterDamage, true, PermCategory::Combat);
    registerPerm(perm_keys::allowPassiveDamage, false, PermCategory::Combat);
    registerPerm(perm_keys::allowSpecialDamage, false, PermCategory::Combat);
    registerPerm(perm_keys::allowCustomSpecialDamage, false, PermCategory::Combat);

    // === 物品权限 ===
    registerPerm(perm_keys::allowPickupItem, false, PermCategory::Item);
    registerPerm(perm_keys::allowDropItem, true, PermCategory::Item);
    registerPerm(perm_keys::allowProjectileCreate, false, PermCategory::Item);

    // === 实体权限 ===
    registerPerm(perm_keys::allowRideEntity, false, PermCategory::Entity);
    registerPerm(perm_keys::allowInteractEntity, false, PermCategory::Entity);
    registerPerm(perm_keys::allowActorDestroy, false, PermCategory::Entity);

    // === 工具权限 ===
    registerPerm(perm_keys::useTools, false, PermCategory::Tools);
    registerPerm(perm_keys::useBucket, false, PermCategory::Tools);
    registerPerm(perm_keys::allowFishingRodAndHook, false, PermCategory::Tools);

    // === 功能方块权限 ===
    registerPerm(perm_keys::useFurnaces, false, PermCategory::Functional);
    registerPerm(perm_keys::useCraftingBlocks, false, PermCategory::Functional);
    registerPerm(perm_keys::useStorageBlocks, false, PermCategory::Functional);
    registerPerm(perm_keys::useRedstoneBlocks, false, PermCategory::Functional);
    registerPerm(perm_keys::useUtilityBlocks, false, PermCategory::Functional);

    // === 装饰交互权限 ===
    registerPerm(perm_keys::useDecorative, false, PermCategory::Decorative);
    registerPerm(perm_keys::useMovement, false, PermCategory::Decorative);
    registerPerm(perm_keys::editSigns, false, PermCategory::Decorative);
    registerPerm(perm_keys::editFlowerPot, false, PermCategory::Decorative);

    // === 特殊权限 ===
    registerPerm(perm_keys::useBed, false, PermCategory::Special);
    registerPerm(perm_keys::useCake, false, PermCategory::Special);
    registerPerm(perm_keys::placeVehicles, false, PermCategory::Special);
    registerPerm(perm_keys::allowAttackDragonEgg, false, PermCategory::Special);

    // === 环境权限 ===
    registerPerm(perm_keys::allowFireSpread, true, PermCategory::Environment);
    registerPerm(perm_keys::allowFarmDecay, true, PermCategory::Environment);
    registerPerm(perm_keys::allowPistonPushOnBoundary, true, PermCategory::Environment);
    registerPerm(perm_keys::allowRedstoneUpdate, true, PermCategory::Environment);
    registerPerm(perm_keys::allowExplode, false, PermCategory::Environment);
    registerPerm(perm_keys::allowBlockFall, false, PermCategory::Environment);
    registerPerm(perm_keys::allowWitherDestroy, false, PermCategory::Environment);
    registerPerm(perm_keys::allowLiquidFlow, true, PermCategory::Environment);
    registerPerm(perm_keys::allowSculkBlockGrowth, true, PermCategory::Environment);
    registerPerm(perm_keys::allowMonsterSpawn, true, PermCategory::Environment);
    registerPerm(perm_keys::allowAnimalSpawn, true, PermCategory::Environment);
    registerPerm(perm_keys::allowEndermanLeaveBlock, false, PermCategory::Environment);
}
ll::Expected<> PermRegistry::loadOverrides(PlotX& mod) {
    auto path = mod.getSelf().getConfigDir() / "PermOverrides.json";
    if (!std::filesystem::exists(path)) {
        auto json = reflection::struct2json(perms).value().dump(2);
        ll::file_utils::writeFile(path, json);
        return {};
    }
    auto rawJson = ll::file_utils::readFile(path);
    if (!rawJson) {
        return ll::makeStringError("Failed to read PermOverrides.json");
    }
    try {
        auto json = nlohmann::json::parse(*rawJson);
        reflection::json2structDiffPatch(perms, json);
        return {};
    } catch (nlohmann::json::exception const& exception) {
        return ll::makeStringError(exception.what());
    }
}
ll::Expected<> PermRegistry::registerPerm(HashedStringView key, bool defVal, PermCategory category) {
    if (perms.contains(key)) {
        return ll::makeStringError(fmt::format("Perm already registered:{} ", key.getString()));
    }
    PermMeta meta{defVal, category, std::string{key.getString()}};
    perms.emplace(key, std::move(meta));
    return {};
}
bool PermRegistry::getDefault(HashedStringView key) {
    return getMeta(key).and_then([&](PermMeta& meta) { return meta.defaultValue; });
}
std::string_view PermRegistry::getName(HashedStringView key) {
    return getMeta(key).and_then([](PermMeta& meta) -> std::string_view { return meta.name; });
}
optional_ref<PermMeta> PermRegistry::getMeta(HashedStringView key) {
    auto iter = perms.find(key);
    if (iter == perms.end()) {
        return nullptr;
    }
    return {iter->second};
}

} // namespace plotx