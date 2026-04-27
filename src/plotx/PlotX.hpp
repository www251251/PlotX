#pragma once
#include "ll/api/mod/NativeMod.h"

#include <filesystem>
#include <memory>
#include <string_view>

#include "plotx/Global.hpp"

#include "econbridge/IEconomy.h"

namespace ll::thread {
class ThreadPoolExecutor;
}

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

    PXAPI class PlotService* getService() const;

    PXNDAPI ll::thread::ThreadPoolExecutor& getThreadPool() const;

    PXNDAPI std::filesystem::path getConfigPath() const;
    PXNDAPI std::filesystem::path getDatabasePath() const;

    PXAPI econbridge::IEconomy::Ptr getEconomy() const;

    PXAPI void loadConfig() const;
    PXAPI void saveConfig() const;

    PXNDAPI static int                        getDimensionId();
    PXNDAPI static std::shared_ptr<Dimension> getPlotDimension();

private:
    static constexpr std::string_view ConfigFileName  = "config.json";
    static constexpr std::string_view DatabaseDirName = "PlotXDB";

    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace plotx
