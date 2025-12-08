#include "PlotX.hpp"

#include "command/PlotXCommand.hpp"
#include "plotx/core/PlotController.hpp"
#include "plotx/core/PlotEventDriven.hpp"
#include "plotx/core/PlotRegistry.hpp"
#include "plotx/infra/Config.hpp"

#include "ll/api/Config.h"
#include "ll/api/i18n/I18n.h"
#include "ll/api/io/LogLevel.h"
#include "ll/api/mod/RegisterHelper.h"

#include "mc/world/level/dimension/VanillaDimensions.h"

#include <filesystem>
#include <memory>


namespace plotx {


struct PlotX::Impl {
    ll::mod::NativeMod&              self_;
    std::unique_ptr<PlotEventDriven> plotEventDriven_{nullptr};
    std::unique_ptr<PlotRegistry>    registry_{nullptr};

    std::unique_ptr<PlotController> controller_{nullptr};

    explicit Impl() : self_(*ll::mod::NativeMod::current()) {}
};

PlotX::PlotX() : impl_(std::make_unique<Impl>()) {}

PlotX& PlotX::getInstance() {
    static PlotX instance;
    return instance;
}

bool PlotX::load() {
    auto& logger = getSelf().getLogger();
#ifdef PLOTX_DEBUG
    logger.setLevel(ll::io::LogLevel::Trace);
#endif

    logger.debug("Try to load i18n");
    if (auto i18n = ll::i18n::getInstance().load(getSelf().getLangDir()); !i18n) {
        logger.error("Failed to load i18n: ");
        i18n.error().log(logger);
    }

    logger.debug("Try to load config");
    loadConfig();

    logger.debug("Initialize PlotRegistry");
    impl_->registry_   = std::make_unique<PlotRegistry>(*this);
    impl_->controller_ = std::make_unique<PlotController>(*impl_->registry_, *this);

    return true;
}

bool PlotX::enable() {
    impl_->plotEventDriven_ = std::make_unique<PlotEventDriven>();

    PlotXCommand::setup();

    return true;
}

bool PlotX::disable() {
    impl_->plotEventDriven_.reset();
    impl_->registry_.reset();

    return true;
}


ll::mod::NativeMod& PlotX::getSelf() const { return impl_->self_; }
ll::io::Logger&     PlotX::getLogger() const { return impl_->self_.getLogger(); }

PlotRegistry*   PlotX::getPlotRegistry() const { return impl_->registry_.get(); }
PlotController* PlotX::getController() const { return impl_->controller_.get(); }

std::filesystem::path PlotX::getConfigPath() const { return getSelf().getConfigDir() / ConfigFileName; }

std::filesystem::path PlotX::getDatabasePath() const { return getSelf().getDataDir() / DatabaseDirName; }

void PlotX::loadConfig() const {
    auto path = getConfigPath();
    if (!std::filesystem::exists(path) || !ll::config::loadConfig(gConfig_, path)) {
        saveConfig();
    }
}
void PlotX::saveConfig() const {
    auto path = getConfigPath();
    if (!ll::config::saveConfig(gConfig_, path)) {
        getSelf().getLogger().error("Failed to save config to {}", path);
    }
}

int PlotX::getDimensionId() {
#ifdef PLOTX_OVERWORLD
    return 0;
#else
    static int const dimensionId = VanillaDimensions::fromString(DimensionName.data());
    return dimensionId;
#endif
}
bool PlotX::isMoreDimensionsEnv() {
#ifdef PLOTX_OVERWORLD
    return false;
#else
    return true;
#endif
}


} // namespace plotx

LL_REGISTER_MOD(plotx::PlotX, plotx::PlotX::getInstance());
