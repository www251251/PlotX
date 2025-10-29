#include "ScriptMod.hpp"
#include "ScriptLoader.hpp"

#include "plotx/PlotX.hpp"

#include "qjspp/JsEngine.hpp"
#include "qjspp/Locker.hpp"
#include "script/modules/levilamina/LeviLaminaDef.hpp"

#include <ll/api/event/EventBus.h>


namespace qjspp {

template <>
struct TypeConverter<std::shared_ptr<ll::mod::Mod>> {
    static Value toJs(std::shared_ptr<ll::mod::Mod> sptr) {
        if (!sptr) return Null{};
        return Locker::currentEngineChecked().newInstanceOfShared(
            plotx::script::ScriptMod::kClassDef_,
            std::move(sptr)
        );
    }
    static ll::mod::Mod* toCpp(Value const& val) {
        return static_cast<ll::mod::Mod*>(
            Locker::currentEngineChecked().getNativeInstanceOf(val.asObject(), plotx::script::ScriptMod::kClassDef_)
        );
    }
};

template <>
struct TypeConverter<ll::mod::Mod> : TypeConverter<std::shared_ptr<ll::mod::Mod>> {
    static Value toJs(ll::mod::Mod* ptr) {
        if (!ptr) return Null{};
        return Locker::currentEngineChecked().newInstanceOfView(plotx::script::ScriptMod::kClassDef_, ptr);
    }
    static Value toJs(ll::mod::Mod& ref) { return toJs(&ref); }
};

template <>
struct TypeConverter<plotx::script::ScriptMod> : TypeConverter<ll::mod::Mod> {};

template <>
struct TypeConverter<ll::io::Logger> {
    static Value toJs(ll::io::Logger& ref) {
        return Locker::currentEngineChecked().newInstanceOfView(
            plotx::script::modules::LeviLaminaDef::ScriptLogger,
            &ref
        );
    }
    static ll::io::Logger* toCpp(Value const& val) {
        return static_cast<ll::io::Logger*>(Locker::currentEngineChecked().getNativeInstanceOf(
            val.asObject(),
            plotx::script::modules::LeviLaminaDef::ScriptLogger
        ));
    }
};

template <>
struct TypeConverter<std::filesystem::path> : TypeConverter<std::string> {
    using Base = TypeConverter<std::string>;
    static Value toJs(std::filesystem::path const& path) { return Base::toJs(path.string()); }
};

} // namespace qjspp


namespace plotx::script {

ScriptMod::ScriptMod(ll::mod::Manifest manifest, EngineID id) : ll::mod::Mod(std::move(manifest)), id_(id) {}

ScriptMod::~ScriptMod() {
    cleanListeners();
    release();
}

void ScriptMod::managedListener(ll::event::ListenerId id) { listeners_.push_back(std::move(id)); }
void ScriptMod::managedListener(ll::event::ListenerPtr const& listener) { managedListener(listener->getId()); }

void ScriptMod::cleanListeners() {
    if (listeners_.empty()) {
        return;
    }
    auto& bus = ll::event::EventBus::getInstance();
    for (auto& id : listeners_) {
        if (bus.hasListener(id)) {
            bus.removeListener(id);
        }
    }
    listeners_.clear();
}

std::shared_ptr<ll::mod::Mod> ScriptMod::current() {
    return qjspp::Locker::currentEngineChecked().getData<EngineData>()->mod_.lock();
}


static_assert(std::is_base_of_v<ll::mod::Mod, ScriptMod>);
qjspp::ClassDefine const ScriptMod::kClassDef_ =
    qjspp::defineClass<ScriptMod>("ScriptMod")
        .function("current", &ScriptMod::current)
        .disableConstructor()
        .instanceMethod("onLoad", static_cast<void (ScriptMod::*)(ScriptMod::CallbackFn)>(&ScriptMod::onLoad))
        .instanceMethod("onEnable", static_cast<void (ScriptMod::*)(ScriptMod::CallbackFn)>(&ScriptMod::onEnable))
        .instanceMethod("onDisable", static_cast<void (ScriptMod::*)(ScriptMod::CallbackFn)>(&ScriptMod::onDisable))
        .instanceMethod("onUnload", static_cast<void (ScriptMod::*)(ScriptMod::CallbackFn)>(&ScriptMod::onUnload))
        .instanceMethod("getName", &ScriptMod::getName)
        .instanceMethod("getType", &ScriptMod::getType)
        .instanceMethod("getModDir", &ScriptMod::getModDir)
        .instanceMethod("getDataDir", &ScriptMod::getDataDir)
        .instanceMethod("getConfigDir", &ScriptMod::getConfigDir)
        .instanceMethod("getLangDir", &ScriptMod::getLangDir)
        .instanceMethod("isEnabled", &ScriptMod::isEnabled)
        .instanceMethod("isDisabled", &ScriptMod::isDisabled)
        .instanceMethod("getLogger", &ScriptMod::getLogger)
        .build();


} // namespace plotx::script