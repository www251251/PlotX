#include "ll/api/data/KeyValueDB.h"
#include "qjspp/Binding.hpp"
#include "qjspp/Definitions.hpp"
#include "qjspp/JsException.hpp"
#include "qjspp/Types.hpp"
#include "qjspp/Values.hpp"
#include "script/modules/Helper.hpp"
#include "script/modules/levilamina/LeviLaminaDef.hpp"


namespace plotx::script::modules {

using namespace ll::data;

qjspp::Value iter(void* inst, qjspp::Arguments const& args) {
    if (args.length() != 1) {
        throw qjspp::JsException{"iter expects 1 argument"};
    }
    if (!args[0].isFunction()) {
        throw qjspp::JsException{"iter expects a function as first argument"};
    }
    auto fn = args[0].asFunction();
    auto db = static_cast<KeyValueDB*>(inst);

    for (auto&& [k, v] : db->iter()) {
        try {
            auto result = fn.call(args.thiz(), {qjspp::String{k}, qjspp::String{v}});
            if (result.isBoolean() && !result.asBoolean().value()) {
                break; // break the iteration
            }
        } catch (qjspp::JsException const& exc) {
            args.engine()->invokeUnhandledJsException(exc, qjspp::UnhandledExceptionOrigin::Callback);
            return qjspp::Boolean{false};
        }
    }
    return qjspp::Boolean{true};
}

qjspp::ClassDefine const LeviLaminaDef::KeyValueDBDef_ =
    qjspp::defineClass<ll::data::KeyValueDB>("KeyValueDB")
        .constructor<std::string>()
        .instanceMethod("get", &KeyValueDB::get)
        .instanceMethod("has", &KeyValueDB::has)
        .instanceMethod("set", &KeyValueDB::set)
        .instanceMethod("empty", &KeyValueDB::empty)
        .instanceMethod("del", &KeyValueDB::del)
        .instanceMethod("iter", &iter)
        .instanceMethod(
            "close",
            [](void*, qjspp::Arguments const& args) -> qjspp::Value {
                if (args.hasWrappedResource()) {
                    args.getWrappedResource()->finalize();
                    return qjspp::Boolean{true};
                }
                return qjspp::Boolean{false};
            }
        )
        .build();


} // namespace plotx::script::modules