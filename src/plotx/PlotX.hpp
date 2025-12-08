#pragma once

#include "ll/api/mod/NativeMod.h"
#include <filesystem>
#include <memory>
#include <string_view>

#include "plotx/Global.hpp"

namespace plotx {


class PlotX {
    PlotX();

public:
    bool load();

    bool enable();

    bool disable();

    [[nodiscard]] ll::mod::NativeMod& getSelf() const;

    [[nodiscard]] ll::io::Logger& getLogger() const;

public:
    PXAPI static PlotX& getInstance();

    PXAPI class PlotRegistry* getPlotRegistry() const;

    PXAPI class PlotController* getController() const;

    PXNDAPI std::filesystem::path getConfigPath() const;
    PXNDAPI std::filesystem::path getDatabasePath() const;

    PXAPI void loadConfig() const;
    PXAPI void saveConfig() const;

    PXNDAPI static int  getDimensionId();
    PXNDAPI static bool isMoreDimensionsEnv();

private:
    static constexpr std::string_view ConfigFileName  = "config.json";
    static constexpr std::string_view DimensionName   = "plotx";
    static constexpr std::string_view DatabaseDirName = "PlotXDB";

    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace plotx
