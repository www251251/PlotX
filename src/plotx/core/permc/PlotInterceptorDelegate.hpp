#pragma once
#include "perm_core/interceptor/InterceptorDelegate.hpp"

namespace plotx {
class PlotRegistry;
}
namespace plotx {

class PlotInterceptorDelegate final : public permc::InterceptorDelegate {
    PlotRegistry& registry_;

public:
    explicit PlotInterceptorDelegate(PlotRegistry& registry);
    ~PlotInterceptorDelegate() override;

    permc::PermDecision preCheck(BlockSource& blockSource, BlockPos const& blockPos) override;
    permc::PermDecision preCheck(BlockSource& blockSource, AABB const& aabb) override;

    permc::PermRole getRole(Player& player, BlockSource& blockSource, BlockPos const& blockPos) override;

    optional_ref<permc::PermTable> getPermTable(BlockSource& blockSource, BlockPos const& blockPos) override;

    ll::coro::Generator<permc::PermTable const&> queryMatrix(BlockSource& blockSource, AABB const& aabb) override;

    permc::PermDecision postPolicy(BlockSource& blockSource, BlockPos const& blockPos) override;
    permc::PermDecision postPolicy(BlockSource& blockSource, AABB const& aabb) override;

    permc::PermDecision handlePistonAction(
        BlockSource&    blockSource,
        BlockPos const& pistonPos,
        BlockPos const& pushPos,
        bool permc::EnvironmentPerms::* field
    ) override;

    permc::PermDecision
    handleBlockFall(BlockSource& blockSource, BlockPos const& fallPos, bool permc::EnvironmentPerms::* field) override;

    permc::PermDecision handleSpread(
        BlockSource&    source,
        BlockPos const& fromPos,
        BlockPos const& toPos,
        bool permc::EnvironmentPerms::* field
    ) override;
};

} // namespace plotx