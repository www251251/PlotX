#include "script/modules/levilamina/LeviLaminaModule.hpp"

#include <ll/api/io/Logger.h>

#include "qjspp/Binding.hpp"


namespace plotx::script::modules {

void formatArgs(qjspp::Arguments const& args, std::ostringstream& oss, size_t offset = 0) {
    static constexpr std::string_view prefix = "[Script] ";
    oss << prefix;

    size_t argc = args.length();
    if (argc < offset) {
        return;
    }

    for (size_t i = offset; i < argc; ++i) {
        auto arg = args[i];
        oss << arg.toString().value();
        if (i < argc - 1) {
            oss << " ";
        }
    }
}

qjspp::Value impl(ll::io::Logger& logger, ll::io::LogLevel level, qjspp::Arguments const& args, size_t offset = 0) {
    std::ostringstream oss;
    formatArgs(args, oss, offset);
    logger.log(level, oss.str());
    return {};
}

qjspp::Value log(void* inst, qjspp::Arguments const& args) {
    if (args.length() < 2) {
        throw qjspp::JsException(qjspp::JsException::Type::TypeError, "log requires at least 2 arguments");
    }
    if (!args[0].isNumber()) {
        throw qjspp::JsException(qjspp::JsException::Type::TypeError, "log requires first argument to be a LogLevel");
    }
    auto logger = static_cast<ll::io::Logger*>(inst);
    auto level  = qjspp::ConvertToCpp<ll::io::LogLevel>(args[0]);
    return impl(*logger, level, args, 1);
}

qjspp::Value fatal(void* inst, qjspp::Arguments const& args) {
    return impl(*static_cast<ll::io::Logger*>(inst), ll::io::LogLevel::Fatal, args);
}
qjspp::Value error(void* inst, qjspp::Arguments const& args) {
    return impl(*static_cast<ll::io::Logger*>(inst), ll::io::LogLevel::Error, args);
}
qjspp::Value warn(void* inst, qjspp::Arguments const& args) {
    return impl(*static_cast<ll::io::Logger*>(inst), ll::io::LogLevel::Warn, args);
}
qjspp::Value info(void* inst, qjspp::Arguments const& args) {
    return impl(*static_cast<ll::io::Logger*>(inst), ll::io::LogLevel::Info, args);
}
qjspp::Value debug(void* inst, qjspp::Arguments const& args) {
    return impl(*static_cast<ll::io::Logger*>(inst), ll::io::LogLevel::Debug, args);
}
qjspp::Value trace(void* inst, qjspp::Arguments const& args) {
    return impl(*static_cast<ll::io::Logger*>(inst), ll::io::LogLevel::Trace, args);
}


using ll::io::Logger;
qjspp::ClassDefine const LeviLaminaModule::ScriptLogger = qjspp::defineClass<Logger>("Logger")
                                                           .disableConstructor()
                                                           .instanceMethod("log", &log)
                                                           .instanceMethod("fatal", &fatal)
                                                           .instanceMethod("error", &error)
                                                           .instanceMethod("warn", &warn)
                                                           .instanceMethod("info", &info)
                                                           .instanceMethod("debug", &debug)
                                                           .instanceMethod("trace", &trace)
                                                           .instanceMethod("getTitle", &Logger::getTitle)
                                                           .instanceMethod("getLevel", &Logger::getLevel)
                                                           .instanceMethod("shouldLog", &Logger::shouldLog)
                                                           .instanceMethod("setLevel", &Logger::setLevel)
                                                           .instanceMethod("setFlushLevel", &Logger::setFlushLevel)
                                                           .instanceMethod("flush", &Logger::flush)
                                                           .build();

using ll::io::LogLevel;
qjspp::EnumDefine const LeviLaminaModule::ScriptLogLevel = qjspp::defineEnum<LogLevel>("LogLevel")
                                                            .value("Off", LogLevel::Off)
                                                            .value("Fatal", LogLevel::Fatal)
                                                            .value("Error", LogLevel::Error)
                                                            .value("Warn", LogLevel::Warn)
                                                            .value("Info", LogLevel::Info)
                                                            .value("Debug", LogLevel::Debug)
                                                            .value("Trace", LogLevel::Trace)
                                                            .build();


} // namespace plotx::script::modules