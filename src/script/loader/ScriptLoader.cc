#include "ScriptLoader.hpp"

#include "ScriptMod.hpp"
#include "plotx/PlotX.hpp"
#include "script/EngineManager.hpp"

#include <qjspp/runtime/JsException.hpp>
#include <qjspp/runtime/Locker.hpp>
#include <qjspp/types/Value.hpp>


namespace plotx::script {

namespace fs = std::filesystem;

ScriptLoader::ScriptLoader(PlotX& entry) : ll::mod::ModManager(kModType), main_(entry) {}
ScriptLoader::~ScriptLoader() = default;

ll::Expected<> ScriptLoader::load(ll::mod::Manifest manifest) {
    auto entry = fs::current_path() / ll::mod::getModsRoot() / manifest.name / manifest.entry;
    entry      = fs::absolute(entry);
    if (!fs::exists(entry)) {
        return ll::makeStringError("entry point does not exist: " + entry.string());
    }

    auto engine    = main_.getEngineManager()->newEngine();
    auto data      = engine->getData<EngineData>();
    auto mod       = std::make_shared<ScriptMod>(manifest, data->id_);
    data->mod_     = mod;
    data->modName_ = manifest.name;

    {
        qjspp::Locker lock{engine};
        try {
            engine->loadScript(entry.u8string(), true);
        } catch (qjspp::JsException const& exception) {
            auto&& exc = ll::makeStringError(
                fmt::format(
                    "Failed to load script '{}', error: {}\n{}",
                    manifest.name,
                    exception.message(),
                    exception.stacktrace()
                )
            );
            main_.getEngineManager()->destroyEngine(engine);
            return std::move(exc);
        }
    }
    return mod->onLoad().transform([&, this] { this->addMod(manifest.name, mod); });
}

ll::Expected<> ScriptLoader::enable(std::string_view name) { return ModManager::enable(name); }

ll::Expected<> ScriptLoader::disable(std::string_view name) { return ModManager::disable(name); }

ll::Expected<> ScriptLoader::unload(std::string_view name) {
    try {
        auto mod    = std::static_pointer_cast<ScriptMod>(getMod(name));
        auto engine = main_.getEngineManager()->getEngine(mod->id_);
        if (!engine) {
            ll::makeStringError("Fatal error: engine not found");
        }

        mod->onUnload();
        mod->cleanListeners();

        eraseMod(name);
        main_.getEngineManager()->destroyEngine(mod->id_);
        return {};
    } catch (...) {
        return ll::makeStringError("Failed to unload mod: " + std::string(name));
    }
}


} // namespace plotx::script