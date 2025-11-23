#pragma once
#include "ll/api/io/Logger.h"
#include "mc/platform/UUID.h"
#include "plotx/Global.hpp"
#include "plotx/core/PlotHandle.hpp"
#include <memory>
#include <string>


namespace plotx {

class PlotX;

// 地皮注册表
class PlotRegistry final {
    struct Impl;
    std::unique_ptr<Impl> impl_;

    void _upgradeDatabase(ll::io::Logger& logger);
    void _loadAdmins(ll::io::Logger& logger);
    void _loadPlots(ll::io::Logger& logger);

public:
    PLOTX_DISALLOW_COPY_AND_MOVE(PlotRegistry);
    explicit PlotRegistry(PlotX& plotx);
    ~PlotRegistry();

    PXAPI bool isAdmin(mce::UUID const& uuid) const;
    PXAPI void addAdmin(mce::UUID const& uuid);
    PXAPI void removeAdmin(mce::UUID const& uuid);


public:
    static constexpr std::string_view VersionKey    = "_version_"; // 版本号
    static constexpr std::string_view AdminsKey     = "admins";    // 管理员
    static constexpr std::string_view PlotKeyPrefix = "plot_";     // 地皮
};


} // namespace plotx