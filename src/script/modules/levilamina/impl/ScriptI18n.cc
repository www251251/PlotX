#include "script/modules/levilamina/LeviLaminaModule.hpp"

#include "script/EngineData.hpp"

#include "fmt/args.h"
#include "fmt/core.h"
#include "fmt/format.h"

#include "qjspp/bind/builder/ClassDefineBuilder.hpp"
#include <ll/api/i18n/I18n.h>
#include <qjspp/runtime/JsEngine.hpp>
#include <qjspp/runtime/JsException.hpp>
#include <qjspp/types/Arguments.hpp>
#include <qjspp/types/String.hpp>
#include <qjspp/types/Value.hpp>

namespace plotx::script::modules {


struct ScriptI18nProxy {
    static void ensureI18n(std::shared_ptr<EngineData> const& data) {
        if (!data->i18n_) [[unlikely]] {
            data->i18n_ = std::make_unique<ll::i18n::I18n>();
        }
    }

    static qjspp::Value load(qjspp::Arguments const& arguments) {
        if (arguments.length() != 1 || !arguments[0].isString()) {
            throw qjspp::JsException{
                qjspp::JsException::Type::TypeError,
                "I18n.load expects a single string argument (path to the locale file)"
            };
        }

        auto vm   = arguments.engine();
        auto data = vm->getData<EngineData>();
        ensureI18n(data);

        auto path = arguments[0].asString().value();
        if (auto res = data->i18n_->load(path); !res) {
            throw qjspp::JsException{res.error().message()};
        }
        return {};
    }

    static qjspp::Value clear(qjspp::Arguments const& arguments) {
        auto vm   = arguments.engine();
        auto data = vm->getData<EngineData>();
        ensureI18n(data);
        data->i18n_->clear();
        return {};
    }

    static qjspp::Value set(qjspp::Arguments const& arguments) {
        if (arguments.length() != 3 || !arguments[0].isString() || !arguments[1].isString()
            || !arguments[2].isString()) {
            throw qjspp::JsException{
                qjspp::JsException::Type::TypeError,
                "I18n.set expects three string arguments (localeCode, key, value)"
            };
        }
        auto vm   = arguments.engine();
        auto data = vm->getData<EngineData>();
        ensureI18n(data);

        auto localeCode = arguments[0].asString().value();
        auto key        = arguments[1].asString().value();
        auto value      = arguments[2].asString().value();
        data->i18n_->set(localeCode, key, value);
        return {};
    }

    static qjspp::Value get(qjspp::Arguments const& arguments) {
        if (arguments.length() != 2 || !arguments[0].isString() || !arguments[1].isString()) {
            throw qjspp::JsException{
                qjspp::JsException::Type::TypeError,
                "I18n.get expects two string arguments (key, localeCode)"
            };
        }
        auto vm   = arguments.engine();
        auto data = vm->getData<EngineData>();
        ensureI18n(data);

        auto key        = arguments[0].asString().value();
        auto localeCode = arguments[1].asString().value();
        return qjspp::String{data->i18n_->get(key, localeCode)};
    }

    // helper: i18n.format("hello, {}", 'world')
    static qjspp::Value format(qjspp::Arguments const& arguments) {
        if (arguments.length() < 1 || !arguments[0].isString()) [[unlikely]] {
            throw qjspp::JsException{
                qjspp::JsException::Type::TypeError,
                "I18n.format expects at least one string argument (fmt)"
            };
        }
        if (arguments.length() == 1) {
            return arguments[0]; // 只有一个 fmt，没有参数，无法格式化
        }

        auto fmt = arguments[0].asString().value();

        fmt::dynamic_format_arg_store<fmt::format_context> store;
        for (size_t i = 1; i < arguments.length(); ++i) {
            store.push_back(arguments[i].toString().value());
        }

        auto result = fmt::vformat(fmt, store);
        return qjspp::String{result};
    }
};


qjspp::bind::meta::ClassDefine const LeviLaminaModule::ScriptI18n =
    qjspp::bind::defineClass<void>("I18n")
        .function("getDefaultLocaleCode", ll::i18n::getDefaultLocaleCode)
        .function("load", ScriptI18nProxy::load)
        .function("clear", ScriptI18nProxy::clear)
        .function("set", ScriptI18nProxy::set)
        .function("get", ScriptI18nProxy::get)
        .function("format", ScriptI18nProxy::format)
        .build();


} // namespace plotx::script::modules