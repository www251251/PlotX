#include "plotx/world/GeneratorInitializer.hpp"
#include "PlotXDimension.hpp"

#include "more_dimensions/api/dimension/CustomDimensionManager.h"


namespace plotx::world {


ll::Expected<> GeneratorInitializer::operator()() {
    more_dimensions::CustomDimensionManager::getInstance().addDimension<world::PlotXDimension>(
#if defined(USE_PLOTX)
        "plotx"
#elif defined(USE_PLOTX2)
        "plotx2"
#endif
    );
    return {};
}


} // namespace plotx::world