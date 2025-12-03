#pragma once
#include "EngineData.hpp"
#include "plotx/Global.hpp"

#include <memory>

#include "qjspp/Forward.hpp"
#include <qjspp/runtime/JsEngine.hpp>

namespace plotx {
class PlotX;
}

namespace plotx::script {


class EngineManager final {
    struct Impl;
    std::unique_ptr<Impl> impl_;

public:
    PLOTX_DISALLOW_COPY_AND_MOVE(EngineManager);

    EngineManager();
    ~EngineManager();

    [[nodiscard]] qjspp::JsEngine* newEngine() const noexcept;

    bool destroyEngine(EngineID id) const;
    bool destroyEngine(qjspp::JsEngine* engine) const;

    qjspp::JsEngine* getEngine(EngineID id) const;

    void        loop() const;
    static void loop(qjspp::JsEngine* engine);

    static void bindContext(qjspp::JsEngine* engine);

    static void onUnhandledJsException(
        qjspp::JsEngine*               engine,
        qjspp::JsException const&      exception,
        qjspp::ExceptionDispatchOrigin origin
    );
};


} // namespace plotx::script