#include "Telemetry.h"
#include "plotx/BuildInfo.h"

#include "ll-bstats/Telemetry.h"
#include "ll-bstats/bstats/Bukkit.h"


namespace plotx::adapters {


void Telemetry::initConstant() {
    ll_bstats::Telemetry::initConstant();
    payload.service.customCharts.emplace_back(bstats::bukkit::SimplePie{"build_target", BuildInfo::BuildTarget});
}


} // namespace plotx::adapters