#pragma once
#include "script/EngineData.hpp"

#include <ll/api/event/Listener.h>
#include <ll/api/event/ListenerBase.h>
#include <ll/api/mod/Mod.h>

#include <qjspp/Definitions.hpp>

namespace plotx::script {

class ScriptMod final : public ll::mod::Mod {
    friend class ScriptLoader;

public:
    EngineID const                     id_;
    std::vector<ll::event::ListenerId> listeners_;

    explicit ScriptMod(ll::mod::Manifest manifest, EngineID id);
    ~ScriptMod();

    void managedListener(ll::event::ListenerId id);
    void managedListener(ll::event::ListenerPtr const& listener);

    void cleanListeners();

    static std::shared_ptr<ll::mod::Mod> current();

    static qjspp::ClassDefine const kClassDef_;
};

} // namespace plotx::script