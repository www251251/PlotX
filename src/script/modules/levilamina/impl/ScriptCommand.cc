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
using CommandParamKind = ll::command::ParamKind::Kind;

/**
 * @note runtimeOverload 返回的 RuntimeOverload 属于栈内存
 * @note 如果没有执行 execute 方法，RuntimeOverload 始终属于栈内存，离开作用域销毁
 * @note 如果执行了 execute 方法后，RuntimeOverload 会被 CommandHandle 内部托管，长期有效。
 * @note 此类为代理类，因为 RuntimeOverload 的生命周期不明确，无法被 Js 引擎托管
 */
class RuntimeOverloadProxy {
public:
    struct OverloadParam {
        std::string      name;
        CommandParamKind kind;
        std::string      enumName;
    };
    using OverloadParams = std::vector<OverloadParam>;

    std::optional<RuntimeOverload> overload_;
    OverloadParams                 params_;

    static std::unordered_set<CommandParamKind> const supportedKind_;

    static void areSupported(ParamKindType kind) {
        if (!supportedKind_.contains(static_cast<CommandParamKind>(kind))) {
            throw qjspp::JsException{
                qjspp::JsException::Type::TypeError,
                fmt::format("unsupported parameter kind: {}", static_cast<int>(kind))
            };
        }
    }

    explicit RuntimeOverloadProxy(RuntimeOverload overload) : overload_(std::move(overload)) {}
    PLOTX_DISALLOW_COPY_AND_MOVE(RuntimeOverloadProxy);

    void ensureValid() const {
        if (!overload_) {
            throw qjspp::JsException{
                qjspp::JsException::Type::ReferenceError,
                "RuntimeOverload::execute has already been called"
            };
        }
    }

    RuntimeOverloadProxy& optional(std::string_view name, ParamKindType kind) {
        ensureValid();
        areSupported(kind);
        (void)overload_->optional(name, kind);
        params_.emplace_back(std::string{name}, static_cast<CommandParamKind>(kind));
        return *this;
    }

    RuntimeOverloadProxy& required(std::string_view name, ParamKindType kind) {
        ensureValid();
        areSupported(kind);
        (void)overload_->required(name, kind);
        params_.emplace_back(std::string{name}, static_cast<CommandParamKind>(kind));
        return *this;
    }

    RuntimeOverloadProxy& optional(std::string_view name, ParamKindType enumKind, std::string_view enumName) {
        ensureValid();
        areSupported(enumKind);
        (void)overload_->optional(name, enumKind, enumName);
        params_.emplace_back(std::string{name}, static_cast<CommandParamKind>(enumKind), std::string{enumName});
        return *this;
    }

    RuntimeOverloadProxy& required(std::string_view name, ParamKindType enumKind, std::string_view enumName) {
        ensureValid();
        areSupported(enumKind);
        (void)overload_->required(name, enumKind, enumName);
        params_.emplace_back(std::string{name}, static_cast<CommandParamKind>(enumKind), std::string{enumName});
        return *this;
    }

    RuntimeOverloadProxy& text(std::string_view text) {
        ensureValid();
        (void)overload_->text(text);
        return *this;
    }

    RuntimeOverloadProxy& postfix(std::string_view postfix) {
        ensureValid();
        (void)overload_->postfix(postfix);
        return *this;
    }

    RuntimeOverloadProxy& option(CommandParameterOption option) {
        ensureValid();
        (void)overload_->option(option);
        return *this;
    }

    RuntimeOverloadProxy& deoption(CommandParameterOption option) {
        ensureValid();
        (void)overload_->deoption(option);
        return *this;
    }

    static qjspp::Value execute(void* inst, qjspp::Arguments const& arguments) {
        auto* fuck = static_cast<RuntimeOverloadProxy*>(inst);
        fuck->ensureValid();

        if (arguments.length() < 1) {
            throw qjspp::JsException{"wrong argument count"};
        }
        if (!arguments[0].isFunction()) {
            throw qjspp::JsException{"wrong argument type"};
        }
        auto engine = arguments.engine();
        auto func   = qjspp::ScopedJsValue{engine, arguments[0]};

        auto callback =
            [fn     = std::move(func),
             params = std::move(fuck->params_
             )](CommandOrigin const& origin, CommandOutput& output, ll::command::RuntimeCommand const& command) {
                qjspp::Locker lock{fn.engine()};
                try {
                    // TODO: 解析参数回传引擎
                    auto ori = fn.engine()->newInstanceOfView(
                        MinecraftModule::ScriptCommandOrigin,
                        const_cast<CommandOrigin*>(&origin)
                    );
                    auto args = _convertResult(command, params, origin);
                    fn.value().asFunction().call({}, {ori, qjspp::Null{}, args});
                } catch (qjspp::JsException const& e) {
                    fn.engine()->invokeUnhandledJsException(e, qjspp::UnhandledExceptionOrigin::Callback);
                }
            };

        fuck->overload_->execute(std::move(callback));
        fuck->overload_.reset();
        if (arguments.hasJsManagedResource()) arguments.getJsManagedResource()->finalize();
        return {};
    }

private:
    static qjspp::Object
    _convertResult(ll::command::RuntimeCommand const& cmd, OverloadParams const& params, CommandOrigin const& origin) {
        qjspp::Object result{};
        for (auto const& [name, type, enumName] : params) {
            try {
                bool  isEnumKind = type == ll::command::ParamKind::Enum || type == ll::command::ParamKind::SoftEnum;
                auto& val        = isEnumKind ? cmd[enumName] : cmd[name];
                result.set(name, _convertImpl(val, origin));
            } catch (std::out_of_range&) {}
        }
        return result;
    }
    static qjspp::Value _convertImpl(ll::command::ParamStorageType const& storage, CommandOrigin const& origin) {
        if (!storage.has_value()) {
            return qjspp::Null{};
        }
        if (storage.hold(CommandParamKind::Enum)) {
            return qjspp::String{std::get<ll::command::RuntimeEnum>(storage.value()).name};
        }
        if (storage.hold(CommandParamKind::SoftEnum)) {
            return qjspp::String{std::get<ll::command::RuntimeSoftEnum>(storage.value())};
        }
        if (storage.hold(CommandParamKind::Int)) {
            return qjspp::Number{std::get<int>(storage.value())};
        }
        if (storage.hold(CommandParamKind::Bool)) {
            return qjspp::Boolean{std::get<bool>(storage.value())};
        }
        if (storage.hold(CommandParamKind::Float)) {
            return qjspp::Number{std::get<float>(storage.value())};
        }
        if (storage.hold(CommandParamKind::String)) {
            return qjspp::String{std::get<std::string>(storage.value())};
        }
        if (storage.hold(CommandParamKind::Player)) {
            auto players = std::get<CommandSelector<Player>>(storage.value()).results(origin);
            auto array   = qjspp::Array{players.size()};
            for (auto const& player : players) {
                array.push(newInstanceOfGameWeak(MinecraftModule::ScriptPlayer, player));
            }
            return array;
        }
        if (storage.hold(CommandParamKind::BlockPos)) {
            return qjspp::Locker::currentEngineChecked().newInstanceOfRaw(
                MinecraftModule::ScriptBlockPos,
                new BlockPos{std::get<CommandPosition>(storage.value())
                                 .getBlockPos(CommandVersion::CurrentVersion(), origin, Vec3::ZERO())}
            );
        }
        if (storage.hold(CommandParamKind::Vec3)) {
            return qjspp::Locker::currentEngineChecked().newInstanceOfRaw(
                MinecraftModule::ScriptVec3,
                new Vec3{std::get<CommandPosition>(storage.value())
                             .getPosition(CommandVersion::CurrentVersion(), origin, Vec3::ZERO())}
            );
        }
        if (storage.hold(CommandParamKind::RawText)) {
            return qjspp::String{std::get<CommandRawText>(storage.value()).mText};
        }
        return {};
    }
};
std::unordered_set<CommandParamKind> const RuntimeOverloadProxy::supportedKind_ = {
    CommandParamKind::Enum,
    CommandParamKind::SoftEnum,
    CommandParamKind::Int,
    CommandParamKind::Bool,
    CommandParamKind::Float,
    CommandParamKind::String,
    CommandParamKind::Player,
    CommandParamKind::BlockPos,
    CommandParamKind::Vec3,
    CommandParamKind::RawText,
};

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
                    new RuntimeOverloadProxy(std::move(overload))
                );
            }
        )
        .build();


qjspp::ClassDefine const LeviLaminaModule::ScriptRuntimeOverload =
    qjspp::defineClass<RuntimeOverloadProxy>("RuntimeOverload")
        // .property("$supportedKind", &RuntimeOverloadProxy::supportedKind_)
        .customConstructor([](qjspp::Arguments const& arguments) -> void* {
            // 如果设置 disableConstructor 那么将不会生成清理函数
            // 而当前的API模型需要 RuntimeOverload 不可构造且代理类需要清理函数
            // 所以这里直接返回空指针，禁止构造，绑定也会生成清理函数
            return nullptr;
        })
        .instanceMethod(
            "optional",
            static_cast<RuntimeOverloadProxy& (RuntimeOverloadProxy::*)(std::string_view, ParamKindType)>(
                &RuntimeOverloadProxy::optional
            ),
            static_cast<
                RuntimeOverloadProxy& (RuntimeOverloadProxy::*)(std::string_view, ParamKindType, std::string_view)>(
                &RuntimeOverloadProxy::optional
            )
        )
        .instanceMethod(
            "required",
            static_cast<RuntimeOverloadProxy& (RuntimeOverloadProxy::*)(std::string_view, ParamKindType)>(
                &RuntimeOverloadProxy::required
            ),
            static_cast<
                RuntimeOverloadProxy& (RuntimeOverloadProxy::*)(std::string_view, ParamKindType, std::string_view)>(
                &RuntimeOverloadProxy::required
            )
        )
        .instanceMethod("text", &RuntimeOverloadProxy::text)
        .instanceMethod("postfix", &RuntimeOverloadProxy::postfix)
        .instanceMethod("option", &RuntimeOverloadProxy::option)
        .instanceMethod("deoption", &RuntimeOverloadProxy::deoption)
        .instanceMethod("execute", &RuntimeOverloadProxy::execute)
        .build();

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
