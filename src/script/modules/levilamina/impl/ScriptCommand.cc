#include "script/modules/Helper.hpp"
#include "script/modules/levilamina/LeviLaminaModule.hpp"

#include "ll/api/command/CommandRegistrar.h"
#include "ll/api/command/runtime/ParamKind.h"
#include "ll/api/command/runtime/RuntimeCommand.h"
#include "ll/api/command/runtime/RuntimeEnum.h"
#include "ll/api/command/runtime/RuntimeOverload.h"
#include "plotx/Global.hpp"
#include "script/loader/ScriptMod.hpp"

#include <ll/api/command/CommandHandle.h>

namespace qjspp {

template <>
struct TypeConverter<CommandFlag> {
    static Value       toJs(CommandFlag flag) { return Number{static_cast<int>(flag.value)}; }
    static CommandFlag toCpp(Value const& value) { return static_cast<CommandFlagValue>(value.asNumber().getInt32()); }
};

template <>
struct TypeConverter<ll::command::CommandRegistrar> {
    static Value toJs(ll::command::CommandRegistrar& registrar) {
        return Locker::currentEngineChecked().newInstanceOfView(
            plotx::script::modules::LeviLaminaModule::ScriptCommandRegistrar,
            &registrar
        );
    }
    static ll::command::CommandRegistrar* toCpp(Value const& value) {
        return Locker::currentEngineChecked().getNativeInstanceOf<ll::command::CommandRegistrar>(
            value.asObject(),
            plotx::script::modules::LeviLaminaModule::ScriptCommandRegistrar
        );
    }
};


} // namespace qjspp


namespace plotx ::script::modules {

using ll::command::CommandRegistrar;
qjspp::ClassDefine const LeviLaminaModule::ScriptCommandRegistrar =
    qjspp::defineClass<CommandRegistrar>("CommandRegistrar")
        .disableConstructor()
        .function("getInstance", &CommandRegistrar::getInstance)
        .instanceMethod(
            "getOrCreateCommand",
            [](void* inst, qjspp::Arguments const& arguments) -> qjspp::Value {
                auto* registrar = static_cast<CommandRegistrar*>(inst);
                auto  argc      = arguments.length();
                if (argc < 1) {
                    throw qjspp::JsException{"too few arguments"};
                }
                if (!arguments[0].isString()) {
                    throw qjspp::JsException{"wrong argument type"};
                }
                std::string name = arguments[0].asString().value();

                std::string description = "";
                if (argc >= 2) {
                    if (!arguments[1].isString()) {
                        throw qjspp::JsException{"wrong argument type"};
                    }
                    description = arguments[1].asString().value();
                }

                CommandPermissionLevel requirement = CommandPermissionLevel::Any;
                if (argc >= 3) {
                    if (!arguments[2].isNumber()) {
                        throw qjspp::JsException{"wrong argument type"};
                    }
                    requirement = qjspp::ConvertToCpp<CommandPermissionLevel>(arguments[2]);
                }

                CommandFlag flag = CommandFlagValue::NotCheat;
                if (argc >= 4) {
                    if (!arguments[3].isNumber()) {
                        throw qjspp::JsException{"wrong argument type"};
                    }
                    flag = qjspp::ConvertToCpp<CommandFlag>(arguments[3]);
                }

                return arguments.engine()->newInstanceOfView(
                    ScriptCommandHandle,
                    &registrar->getOrCreateCommand(
                        name,
                        description,
                        requirement,
                        flag,
                        arguments.engine()->getData<EngineData>()->mod_
                    )
                );
            }
        )
        .build();


using ll::command::CommandHandle;
using ll::command::ParamKindType;
using ll::command::RuntimeOverload;

/**
 * @note runtimeOverload 返回的 RuntimeOverload 属于栈内存
 * @note 如果没有执行 execute 方法，RuntimeOverload 始终属于栈内存，离开作用域销毁
 * @note 如果执行了 execute 方法后，RuntimeOverload 会被 CommandHandle 内部托管，长期有效。
 * @note 此类为代理类，因为 RuntimeOverload 的生命周期不明确，无法被 Js 引擎托管
 */
class FuckUnclearLifecycleRuntimeOverload {
public:
    std::optional<RuntimeOverload> overload_;

    explicit FuckUnclearLifecycleRuntimeOverload(RuntimeOverload overload) : overload_(std::move(overload)) {}
    PLOTX_DISALLOW_COPY_AND_MOVE(FuckUnclearLifecycleRuntimeOverload);

    void tryFuck() const {
        if (!overload_) {
            throw qjspp::JsException{
                qjspp::JsException::Type::ReferenceError,
                "RuntimeOverload::execute has already been called"
            };
        }
    }

    FuckUnclearLifecycleRuntimeOverload& optional(std::string_view name, ParamKindType kind) {
        tryFuck();
        (void)overload_->optional(name, kind);
        return *this;
    }

    FuckUnclearLifecycleRuntimeOverload& required(std::string_view name, ParamKindType kind) {
        tryFuck();
        (void)overload_->required(name, kind);
        return *this;
    }

    FuckUnclearLifecycleRuntimeOverload&
    optional(std::string_view name, ParamKindType enumKind, std::string_view enumName) {
        tryFuck();
        (void)overload_->optional(name, enumKind, enumName);
        return *this;
    }

    FuckUnclearLifecycleRuntimeOverload&
    required(std::string_view name, ParamKindType enumKind, std::string_view enumName) {
        tryFuck();
        (void)overload_->required(name, enumKind, enumName);
        return *this;
    }

    FuckUnclearLifecycleRuntimeOverload& text(std::string_view text) {
        tryFuck();
        (void)overload_->text(text);
        return *this;
    }

    FuckUnclearLifecycleRuntimeOverload& postfix(std::string_view postfix) {
        tryFuck();
        (void)overload_->postfix(postfix);
        return *this;
    }

    FuckUnclearLifecycleRuntimeOverload& option(CommandParameterOption option) {
        tryFuck();
        (void)overload_->option(option);
        return *this;
    }

    FuckUnclearLifecycleRuntimeOverload& deoption(CommandParameterOption option) {
        tryFuck();
        (void)overload_->deoption(option);
        return *this;
    }

    static qjspp::Value execute(void* inst, qjspp::Arguments const& arguments) {
        auto* fuck = static_cast<FuckUnclearLifecycleRuntimeOverload*>(inst);
        fuck->tryFuck();

        if (arguments.length() < 1) {
            throw qjspp::JsException{"wrong argument count"};
        }
        if (!arguments[0].isFunction()) {
            throw qjspp::JsException{"wrong argument type"};
        }
        auto engine = arguments.engine();
        auto func   = qjspp::ScopedJsValue{engine, arguments[0]};

        auto callback = [fn = std::move(func)](
                            CommandOrigin const&               origin,
                            CommandOutput&                     output,
                            ll::command::RuntimeCommand const& command
                        ) {
            qjspp::Locker lock{fn.engine()};
            try {
                // TODO: 解析参数回传引擎
                fn.value().asFunction().call();
            } catch (qjspp::JsException const& e) {
                fn.engine()->invokeUnhandledJsException(e, qjspp::UnhandledExceptionOrigin::Callback);
            }
        };

        fuck->overload_->execute(std::move(callback));
        fuck->overload_.reset();
        if (arguments.hasJsManagedResource()) arguments.getJsManagedResource()->finalize();
        return {};
    }
};

using FuckRuntimeOverload = FuckUnclearLifecycleRuntimeOverload;

qjspp::ClassDefine const LeviLaminaModule::ScriptCommandHandle =
    qjspp::defineClass<CommandHandle>("CommandHandle")
        .disableConstructor()
        .instanceMethod(
            "runtimeOverload",
            [](void* inst, qjspp::Arguments const& arguments) -> qjspp::Value {
                auto* handle   = static_cast<CommandHandle*>(inst);
                auto  engine   = arguments.engine();
                auto  overload = handle->runtimeOverload(engine->getData<EngineData>()->mod_);
                return arguments.engine()->newInstanceOfRaw(
                    ScriptRuntimeOverload,
                    new FuckRuntimeOverload(std::move(overload))
                );
            }
        )
        .build();


qjspp::ClassDefine const LeviLaminaModule::ScriptRuntimeOverload =
    qjspp::defineClass<FuckRuntimeOverload>("RuntimeOverload")
        .customConstructor([](qjspp::Arguments const& arguments) -> void* {
            // 如果设置 disableConstructor 那么将不会生成清理函数
            // 而当前的API模型需要 RuntimeOverload 不可构造且代理类需要清理函数
            // 所以这里直接返回空指针，禁止构造，绑定也会生成清理函数
            return nullptr;
        })
        .instanceMethod(
            "optional",
            static_cast<FuckRuntimeOverload& (FuckRuntimeOverload::*)(std::string_view, ParamKindType)>(
                &FuckRuntimeOverload::optional
            ),
            static_cast<FuckRuntimeOverload& (FuckRuntimeOverload::*)(std::string_view, ParamKindType, std::string_view
            )>(&FuckRuntimeOverload::optional)
        )
        .instanceMethod(
            "required",
            static_cast<FuckRuntimeOverload& (FuckRuntimeOverload::*)(std::string_view, ParamKindType)>(
                &FuckRuntimeOverload::required
            ),
            static_cast<FuckRuntimeOverload& (FuckRuntimeOverload::*)(std::string_view, ParamKindType, std::string_view
            )>(&FuckRuntimeOverload::required)
        )
        .instanceMethod("text", &FuckRuntimeOverload::text)
        .instanceMethod("postfix", &FuckRuntimeOverload::postfix)
        .instanceMethod("option", &FuckRuntimeOverload::option)
        .instanceMethod("deoption", &FuckRuntimeOverload::deoption)
        .instanceMethod("execute", &FuckRuntimeOverload::execute)
        .build();

using CommandParamKind = ll::command::ParamKind::Kind;
qjspp::EnumDefine const LeviLaminaModule::ScriptCommandParamKind =
    qjspp::defineEnum<CommandParamKind>("CommandParamKind")
        .value("Int", CommandParamKind::Int)
        .value("Bool", CommandParamKind::Bool)
        .value("Float", CommandParamKind::Float)
        .value("Dimension", CommandParamKind::Dimension)
        .value("String", CommandParamKind::String)
        .value("Enum", CommandParamKind::Enum)
        .value("SoftEnum", CommandParamKind::SoftEnum)
        .value("Actor", CommandParamKind::Actor)
        .value("Player", CommandParamKind::Player)
        .value("BlockPos", CommandParamKind::BlockPos)
        .value("Vec3", CommandParamKind::Vec3)
        .value("RawText", CommandParamKind::RawText)
        .value("Message", CommandParamKind::Message)
        .value("JsonValue", CommandParamKind::JsonValue)
        .value("Item", CommandParamKind::Item)
        .value("BlockName", CommandParamKind::BlockName)
        .value("BlockState", CommandParamKind::BlockState)
        .value("Effect", CommandParamKind::Effect)
        .value("ActorType", CommandParamKind::ActorType)
        .value("Command", CommandParamKind::Command)
        .value("RelativeFloat", CommandParamKind::RelativeFloat)
        .value("IntegerRange", CommandParamKind::IntegerRange)
        .value("FilePath", CommandParamKind::FilePath)
        .value("WildcardInt", CommandParamKind::WildcardInt)
        .value("WildcardActor", CommandParamKind::WildcardActor)
        .build();
static_assert(CommandParamKind::Count == 25, "CommandParamKind::Count is not up to date");


} // namespace plotx::script::modules
