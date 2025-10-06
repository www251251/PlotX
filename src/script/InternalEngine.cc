#include "InternalEngine.hpp"
#include "fmt/base.h"
#include "fmt/format.h"
#include "magic_enum.hpp"
#include "plotx/PlotX.hpp"
#include <filesystem>
#include <memory>
#include <thread>

#include "api/ModuleDef.hpp"
#include "qjspp/JsEngine.hpp"
#include "qjspp/JsException.hpp"
#include "qjspp/JsScope.hpp"

namespace plotx::script {


InternalEngine::InternalEngine(PlotX& plotx) {
    engine_ = new qjspp::JsEngine();
    qjspp::JsScope lock{engine_};

    engine_->registerNativeModule(GetMinecraftModuleDef());
    engine_->registerNativeModule(GetLeviLaminaModuleDef());
    engine_->registerNativeModule(GetPlotXModuleDef());

    engine_->setUnhandledJsExceptionCallback(
        [](qjspp::JsEngine* engine, qjspp::JsException const& exc, qjspp::UnhandledExceptionOrigin ori) {
            auto& logger = PlotX::getInstance().getSelf().getLogger();
            auto  source = magic_enum::enum_name(ori);

            qjspp::JsScope lock{engine};
            logger.error("[Scripting] Unhandled JS exception in {}: {}\n{}", source, exc.message(), exc.stacktrace());
        }
    );

    auto path = std::filesystem::absolute(plotx.getSelf().getModDir() / "scripting" / "Main.js");
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error(fmt::format("[Scripting] Failed to load script! file not found: {}", path.string()));
    }

    try {
        engine_->loadScript(path, true);
    } catch (qjspp::JsException const& e) {
        plotx.getSelf().getLogger().error("[Scripting] Failed to load script! {}\n{}", e.what(), e.stacktrace());
        throw;
    }

    quit_   = std::make_shared<std::atomic<bool>>(false);
    thread_ = std::thread([engine = engine_, quit = quit_] {
        while (!quit->load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (quit->load() || !engine || engine->isDestroying()) {
                break;
            }
            engine->getTaskQueue()->loopOnce();
        }
    });
}

InternalEngine::~InternalEngine() {
    quit_->store(true);
    if (thread_.joinable()) {
        thread_.join();
    }
    delete engine_;
}

qjspp::JsEngine*       InternalEngine::operator->() { return engine_; }
qjspp::JsEngine const* InternalEngine::operator->() const { return engine_; }


} // namespace plotx::script