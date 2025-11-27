#pragma once
#include "qjspp/Binding.hpp"
#include "qjspp/Definitions.hpp"
#include "qjspp/Module.hpp"


namespace plotx::script::modules {


struct PlotXModule {
    PlotXModule() = delete;

    // === core ===
    static qjspp::ClassDefine const ScriptPlotHandle;
    static qjspp::ClassDefine const ScriptPlotRegistry;

    // === math ===
    static qjspp::ClassDefine const ScriptPlotAABB;
    static qjspp::ClassDefine const ScriptPlotCoord;

    static qjspp::ModuleDefine const& getModuleDef();
};


} // namespace plotx::script::modules
