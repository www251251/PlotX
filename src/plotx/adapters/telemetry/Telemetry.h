#pragma once
#include "ll-bstats/Telemetry.h"

namespace plotx::adapters {


class Telemetry : public ll_bstats::Telemetry {
public:
    using ll_bstats::Telemetry::Telemetry;
    ~Telemetry() override;

    void initConstant() override;
};


} // namespace plotx::adapters