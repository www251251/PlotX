#include "Telemetry.h"
#include "plotx/BuildInfo.h"

#include "ll-bstats/Telemetry.h"
#include "ll-bstats/bstats/Bukkit.h"


namespace plotx::adapters {

Telemetry::~Telemetry() = default;

void Telemetry::initConstant() {
    ll_bstats::Telemetry::initConstant();
    payload.addCustomChart(bstats::bukkit::SimplePie{"flavor", BuildInfo::Flavor.data()});
}


} // namespace plotx::adapters