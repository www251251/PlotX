#include "PlotInterceptorDelegate.hpp"

#include "plotx/PlotX.hpp"
#include "plotx/core/PlotHandle.hpp"
#include "plotx/core/PlotRegistry.hpp"
#include "plotx/math/PlotCoord.hpp"

#include <mc/world/actor/player/Player.h>
#include <mc/world/level/BlockSource.h>

#include "perm_core/interceptor/InterceptorHelper.hpp"

namespace plotx {

PlotInterceptorDelegate::PlotInterceptorDelegate(PlotRegistry& registry) : registry_(registry) {}
PlotInterceptorDelegate::~PlotInterceptorDelegate() = default;

std::unique_ptr<permc::PermInterceptor>
PlotInterceptorDelegate::create(PlotRegistry& registry, permc::InterceptorConfig const& config) {
    return std::make_unique<permc::PermInterceptor>(std::make_unique<PlotInterceptorDelegate>(registry), config);
}


permc::PermDecision PlotInterceptorDelegate::preCheck(BlockSource& blockSource, BlockPos const& blockPos) {
    if (blockSource.getDimensionId() != PlotX::getDimensionId()) {
        return permc::PermDecision::Allow; // 不在地皮维度放行
    }
    return permc::PermDecision::Abstain; // 等后续处理
}

permc::PermDecision PlotInterceptorDelegate::preCheck(BlockSource& blockSource, AABB const& aabb) {
    if (blockSource.getDimensionId() != PlotX::getDimensionId()) {
        return permc::PermDecision::Allow; // 不在地皮维度放行
    }
    return permc::PermDecision::Abstain; // 等后续处理
}

permc::PermRole PlotInterceptorDelegate::getRole(Player& player, BlockSource& blockSource, BlockPos const& blockPos) {
    auto const& uuid = player.getUuid();
    if (registry_.isAdmin(uuid)) {
        return permc::PermRole::Admin;
    }
    if (auto plot = registry_.getPlot(PlotCoord{blockPos})) {
        return plot->getPlayerRole(uuid);
    }
    return permc::PermRole::Guest;
}

optional_ref<permc::PermTable>
PlotInterceptorDelegate::getPermTable(BlockSource& blockSource, BlockPos const& blockPos) {
    if (auto plot = registry_.getPlot(PlotCoord{blockPos})) {
        return plot->getPermTable();
    }
    return {}; // null
}

ll::coro::Generator<permc::PermTable const&>
PlotInterceptorDelegate::queryMatrix(BlockSource& blockSource, AABB const& aabb) {
    co_return; // TODO: fix
}

permc::PermDecision PlotInterceptorDelegate::postPolicy(BlockSource& blockSource, BlockPos const& blockPos) {
    return permc::PermDecision::Deny; // 非地皮、无主地皮，默认拒绝
}
permc::PermDecision PlotInterceptorDelegate::postPolicy(BlockSource& blockSource, AABB const& aabb) {
    return permc::PermDecision::Deny; // 非地皮、无主地皮，默认拒绝
}

permc::PermDecision PlotInterceptorDelegate::handlePistonAction(
    BlockSource&    blockSource,
    BlockPos const& pistonPos,
    BlockPos const& pushPos,
    bool permc::EnvironmentPerms::* field
) {
    return permc::PermDecision::Abstain; // TODO: fix
}

permc::PermDecision PlotInterceptorDelegate::handleBlockFall(
    BlockSource&    blockSource,
    BlockPos const& fallPos,
    bool permc::EnvironmentPerms::* field
) {
    if (auto plot = registry_.getPlot(PlotCoord{fallPos})) {
        auto allow = access(plot->getPermTable().environment, field);
        return allow ? permc::PermDecision::Allow : permc::PermDecision::Deny;
    }
    return permc::PermDecision::Abstain;
}

permc::PermDecision PlotInterceptorDelegate::handleSpread(
    BlockSource&    source,
    BlockPos const& fromPos,
    BlockPos const& toPos,
    bool permc::EnvironmentPerms::* field
) {
    return permc::PermDecision::Abstain; // TODO: fix
}

} // namespace plotx