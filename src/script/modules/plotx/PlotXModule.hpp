#pragma once
#include "qjspp/bind/meta/ClassDefine.hpp"
#include "qjspp/bind/meta/ModuleDefine.hpp"


namespace plotx::script::modules {


struct PlotXModule {
    PlotXModule() = delete;

    // === core ===
    static qjspp::bind::meta::ClassDefine const ScriptPlotHandle;
    static qjspp::bind::meta::ClassDefine const ScriptPlotRegistry;

    // === math ===
    static qjspp::bind::meta::ClassDefine const ScriptPlotAABB;
    static qjspp::bind::meta::ClassDefine const ScriptPlotCoord;

    static qjspp::bind::meta::ModuleDefine const& getModuleDef();
};


} // namespace plotx::script::modules
