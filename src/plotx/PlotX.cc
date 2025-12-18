#include "PlotX.hpp"
#include "command/PlotXCommand.hpp"
#include "core/Config.hpp"
#include "plotx/core/PlotEventDriven.hpp"
#include "plotx/core/PlotRegistry.hpp"
#include "plotx/core/PlotService.hpp"

#include "ll/api/Config.h"
#include "ll/api/i18n/I18n.h"
#include "ll/api/io/LogLevel.h"
#include "ll/api/mod/RegisterHelper.h"
#include <ll/api/service/Bedrock.h>

#include "mc/world/level/dimension/VanillaDimensions.h"

#include <filesystem>
#include <mc/world/level/Level.h>
#include <memory>

#include "econbridge/detail/LegacyMoneyEconomy.h"
#include "econbridge/detail/NullEconomy.h"
#include "econbridge/detail/ScoreboardEconomy.h"

namespace plotx {


struct PlotX::Impl {
    ll::mod::NativeMod&              self;
    std::unique_ptr<PlotEventDriven> plotEventDriven{nullptr};
    std::unique_ptr<PlotRegistry>    registry{nullptr};

    std::unique_ptr<PlotService> service{nullptr};

    std::shared_ptr<econbridge::IEconomy> economy{nullptr};

    explicit Impl() : self(*ll::mod::NativeMod::current()) {}

    ll::Expected<> initEconomy() {
        switch (gConfig_.economy.type) {
        case Config::EconomyConfig::Type::Null:
            economy = std::make_shared<econbridge::detail::NullEconomy>();
            break;
        case Config::EconomyConfig::Type::LegacyMoney:
            if (econbridge::detail::LegacyMoneyEconomy::isAvailable()) {
                economy = std::make_shared<econbridge::detail::LegacyMoneyEconomy>();
            } else {
                return ll::makeStringError("LegacyMoney.dll is not available");
            }
            break;
        case Config::EconomyConfig::Type::Scoreboard:
            economy = std::make_shared<econbridge::detail::ScoreboardEconomy>(gConfig_.economy.scoreboard);
            break;
        }
        return {};
    }
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
        i18n.error().log(logger);
    }

    logger.debug("Try to load config");
    loadConfig();

    logger.debug("Initialize PlotRegistry");
    impl_->registry = std::make_unique<PlotRegistry>(*this);
    impl_->service  = std::make_unique<PlotService>(*impl_->registry, *this);

    return true;
}

bool PlotX::enable() {
    if (auto exc = impl_->initEconomy(); !exc) {
        exc.error().log(getLogger());
        getLogger().error("Failed to initialize economy, abort loading.");
        return false;
    }

    impl_->plotEventDriven = std::make_unique<PlotEventDriven>();

    PlotXCommand::setup();

    return true;
}

bool PlotX::disable() {
    impl_->plotEventDriven.reset();
    impl_->registry.reset();

    return true;
}


ll::mod::NativeMod& PlotX::getSelf() const { return impl_->self; }
ll::io::Logger&     PlotX::getLogger() const { return impl_->self.getLogger(); }

PlotRegistry* PlotX::getPlotRegistry() const { return impl_->registry.get(); }
PlotService*  PlotX::getService() const { return impl_->service.get(); }

std::filesystem::path PlotX::getConfigPath() const { return getSelf().getConfigDir() / ConfigFileName; }

std::filesystem::path     PlotX::getDatabasePath() const { return getSelf().getDataDir() / DatabaseDirName; }
econbridge::IEconomy::Ptr PlotX::getEconomy() const { return impl_->economy; }

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
std::shared_ptr<Dimension> PlotX::getPlotDimension() {
    auto dimension =
        ll::service::getLevel().transform([](Level& level) { return level.getDimension(getDimensionId()).lock(); });
    if (dimension.has_value()) {
        return *dimension;
    }
    return nullptr;
}

} // namespace plotx

LL_REGISTER_MOD(plotx::PlotX, plotx::PlotX::getInstance());
