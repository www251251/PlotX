#pragma once
#include "plotx/Global.hpp"

#include <ll/api/event/Cancellable.h>
#include <ll/api/event/player/PlayerEvent.h>

#include <memory>
#include <utility>

namespace plotx {
class PlotHandle;
}
namespace plotx {

/**
 * @brief 管理员移除地皮主人事件
 * @note 事件被取消后，地皮不会被重置为无主状态
 */
class PlayerRemovePlotOwnerEvent final : public ll::event::Cancellable<ll::event::PlayerEvent> {
    std::shared_ptr<PlotHandle> handle_;

public:
    explicit PlayerRemovePlotOwnerEvent(Player& player, std::shared_ptr<PlotHandle> handle)
    : Cancellable(player),
      handle_(std::move(handle)) {}

    PXNDAPI std::shared_ptr<PlotHandle> getHandle() const;
};

} // namespace plotx
