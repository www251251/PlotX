#pragma once
#include "ll-bstats/Telemetry.h"

namespace plotx::adapters {


class Telemetry : public ll_bstats::Telemetry {
public:
    using ll_bstats::Telemetry::Telemetry;

    void initConstant() override;
};


} // namespace plotx::adapters