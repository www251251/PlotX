#include "EngineManager.hpp"

#include "EngineData.hpp"
#include "loader/ScriptMod.hpp"
#include "modules/levilamina/LeviLaminaModule.hpp"
#include "modules/minecraft/MinecraftModule.hpp"
#include "modules/plotx/PlotXModule.hpp"
#include "plotx/PlotX.hpp"

namespace plotx::script {

struct EngineManager::Impl {
    std::thread             thread_;
    std::condition_variable cv_;
    std::atomic_bool        abort_;
    std::mutex              cvMutex_;

    std::shared_mutex                                              stlMutex_;
    std::atomic<EngineID>                                          next_;
    std::unordered_map<EngineID, std::unique_ptr<qjspp::JsEngine>> engines_;
};


EngineManager::EngineManager() : impl_(std::make_unique<Impl>()) {
    impl_->next_   = 1;
    impl_->abort_  = false;
    impl_->thread_ = std::thread([this] {
        while (!impl_->abort_.load()) {
            std::unique_lock lock{impl_->cvMutex_};
            impl_->cv_.wait_for(lock, std::chrono::milliseconds(50), [this] { return impl_->abort_.load(); });
            if (impl_->abort_.load()) {
                break;
            }
            lock.unlock();
            loop();
        }
    });
}

EngineManager::~EngineManager() {
    impl_->abort_.store(true);
    impl_->cv_.notify_all();
    if (impl_->thread_.joinable()) {
        impl_->thread_.join();
    }
    {
        std::unique_lock lock{impl_->stlMutex_};
        impl_->engines_.clear();
    }
    impl_.reset();
}

qjspp::JsEngine* EngineManager::newEngine() const noexcept {
    auto engine = std::make_unique<qjspp::JsEngine>();
    auto id     = this->impl_->next_++;
    auto data   = std::make_shared<EngineData>(id);
    engine->setData(data); // 注册引擎数据

    bindContext(engine.get()); // 绑定上下文

    std::unique_lock lock{impl_->stlMutex_};
    this->impl_->engines_[id] = std::move(engine);
    return this->impl_->engines_[id].get();
}

bool EngineManager::destroyEngine(EngineID id) const {
    std::unique_lock lock{impl_->stlMutex_};

    auto iter = impl_->engines_.find(id);
    if (iter != impl_->engines_.end()) {
        impl_->engines_.erase(iter);
        return true;
    }
    return false;
}

bool EngineManager::destroyEngine(qjspp::JsEngine* engine) const {
    return destroyEngine(engine->getData<EngineData>()->id_);
}

qjspp::JsEngine* EngineManager::getEngine(EngineID id) const {
    std::shared_lock lock{impl_->stlMutex_};

    auto iter = impl_->engines_.find(id);
    if (iter != impl_->engines_.end()) {
        return iter->second.get();
    }
    return nullptr;
}

void EngineManager::loop() const {
    std::shared_lock lock{impl_->stlMutex_};
    for (auto& [_, engine] : impl_->engines_) {
        try {
            loop(engine.get());
        } catch (qjspp::JsException const& exception) {
            engine->invokeUnhandledJsException(exception, qjspp::UnhandledExceptionOrigin::Unknown);
        }
    }
}

void EngineManager::loop(qjspp::JsEngine* engine) {
    qjspp::Locker lock{engine};
    engine->getTaskQueue()->loopOnce();
}

void EngineManager::bindContext(qjspp::JsEngine* engine) {
    qjspp::Locker lock{engine};

    // 注册脚本类
    engine->registerClass(ScriptMod::kClassDef_);

    // 注册模块
    engine->registerModule(modules::MinecraftModule::getModuleDef());
    engine->registerModule(modules::LeviLaminaModule::getModuleDef());
    engine->registerModule(modules::PlotXModule::getModuleDef());

    // 注册异常处理回调
    engine->setUnhandledJsExceptionCallback(onUnhandledJsException);
}

void EngineManager::onUnhandledJsException(
    qjspp::JsEngine*                engine,
    qjspp::JsException const&       exception,
    qjspp::UnhandledExceptionOrigin origin
) {
    auto& logger = PlotX::getInstance().getSelf().getLogger();

    qjspp::Locker lock{engine};
    logger.error("[Script] Unhandled js exception: {}\n{}", exception.what(), exception.stacktrace());
}


} // namespace plotx::script