#pragma once
#include "plotx/Global.hpp"
#include "qjspp/JsEngine.hpp"
#include <atomic>
#include <memory>
#include <thread>


namespace plotx {

class PlotX;

namespace script {

class InternalEngine final {
    qjspp::JsEngine*                   engine_;
    std::thread                        thread_;
    std::shared_ptr<std::atomic<bool>> quit_;

public:
    PLOTX_DISALLOW_COPY_AND_MOVE(InternalEngine);
    explicit InternalEngine(PlotX& plotx);
    ~InternalEngine();

    qjspp::JsEngine*       operator->();
    qjspp::JsEngine const* operator->() const;
};

} // namespace script

} // namespace plotx