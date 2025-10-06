#include "ll/api/data/KeyValueDB.h"
#include "script/api/Helper.hpp"
#include "script/api/levilamina/defs.hpp"
#include "qjspp/Binding.hpp"
#include "qjspp/Definitions.hpp"
#include "qjspp/JsException.hpp"
#include "qjspp/Types.hpp"
#include "qjspp/Values.hpp"


namespace plotx::script::api::inline levilamina {

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
            args.engine()->invokeUnhandledJsExceptionCallback(exc, qjspp::UnhandledExceptionOrigin::Callback);
            return qjspp::Boolean{false};
        }
    }
    return qjspp::Boolean{true};
}

qjspp::ClassDefine const KeyValueDBDef_ = qjspp::defineClass<KeyValueDB>("KeyValueDB")
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
                                                          args.getWrappedResource()->freeResource();
                                                          return qjspp::Boolean{true};
                                                      }
                                                      return qjspp::Boolean{false};
                                                  }
                                              )
                                              .build();


} // namespace plotx::script::api::inline levilamina