#pragma once
#include "mc/deps/ecs/WeakEntityRef.h"
#include "mc/deps/ecs/gamerefs_entity/EntityContext.h"
#include "mc/deps/game_refs/WeakRef.h"
#include "mc/platform/UUID.h"
#include "mc/server/ServerPlayer.h"
#include "mc/world/actor/player/Player.h"

#include "qjspp/Binding.hpp"
#include "qjspp/JsEngine.hpp"
#include "qjspp/JsManagedResource.hpp"
#include "qjspp/Locker.hpp"
#include "qjspp/TypeConverter.hpp"
#include "qjspp/Values.hpp"

#include <memory>

#include "script/modules/minecraft/MinecraftModule.hpp"

namespace plotx::script {

inline qjspp::Object newInstanceOfGameWeak(qjspp::ClassDefine const& def, Player* player) {
    auto& engine = qjspp::Locker::currentEngineChecked();
    struct Control {
        WeakRef<EntityContext> player_;
        explicit Control(Player* pl) : player_{pl->getWeakEntity()} {}
    };
    auto control = std::make_unique<Control>(player);
    auto wrap    = qjspp::JsManagedResource::make(
        control.release(),
        [](void* res) -> void* {
            auto control = static_cast<Control*>(res);
            auto player  = control->player_.tryUnwrap<Player>();
            if (player) return static_cast<void*>(player);
            return nullptr;
        },
        [](void* res) -> void {
            // The game weak ref is not owned by the engine, so we don't need to do anything
            delete static_cast<Control*>(res);
        }
    );
    return engine.newInstance(def, std::move(wrap));
}

} // namespace plotx::script


namespace qjspp {


template <>
struct TypeConverter<Player> {
    static Value toJs(Player* player) {
        return plotx::script::newInstanceOfGameWeak(
            plotx::script::modules::MinecraftModule::MinecraftModule::ScriptPlayer,
            player
        );
    }
    static Value toJs(Player& player) { return toJs(&player); }

    static Player* toCpp(Value const& value) {
        if (!value.isObject()) {
            return nullptr;
        }
        return Locker::currentEngineChecked().getNativeInstanceOf<Player>(
            value.asObject(),
            plotx::script::modules::MinecraftModule::MinecraftModule::ScriptPlayer
        );
    }
};
static_assert(internal::IsTypeConverterAvailable_v<Player>);


template <>
struct TypeConverter<ServerPlayer> : TypeConverter<Player> {};
static_assert(internal::IsTypeConverterAvailable_v<ServerPlayer>);

template <>
struct TypeConverter<mce::UUID> {
    static Value toJs(mce::UUID uuid) { // 值传递，创建一个副本
        auto unique = std::make_unique<mce::UUID>(uuid);
        return Locker::currentEngineChecked().newInstanceOfUnique(
            plotx::script::modules::MinecraftModule::ScriptUUID,
            std::move(unique)
        );
    }

    static mce::UUID* toCpp(Value const& value) {
        if (!value.isObject()) {
            return nullptr;
        }
        return Locker::currentEngineChecked().getNativeInstanceOf<mce::UUID>(
            value.asObject(),
            plotx::script::modules::MinecraftModule::ScriptUUID
        );
    }
};
static_assert(internal::IsTypeConverterAvailable_v<mce::UUID>);


template <>
struct TypeConverter<Vec3> {
    static Value toJs(Vec3 vec) {
        return Locker::currentEngineChecked().newInstanceOfRaw(
            plotx::script::modules::MinecraftModule::ScriptVec3,
            new Vec3(vec.x, vec.y, vec.z)
        );
    }
    static Vec3* toCpp(Value const& value) {
        if (!value.isObject()) {
            return nullptr;
        }
        return Locker::currentEngineChecked().getNativeInstanceOf<Vec3>(
            value.asObject(),
            plotx::script::modules::MinecraftModule::ScriptVec3
        );
    }
};


template <>
struct TypeConverter<BlockPos> {
    static Value toJs(BlockPos pos) {
        return Locker::currentEngineChecked().newInstanceOfRaw(
            plotx::script::modules::MinecraftModule::ScriptBlockPos,
            new BlockPos(pos.x, pos.y, pos.z)
        );
    }
    static BlockPos* toCpp(Value const& value) {
        if (!value.isObject()) {
            return nullptr;
        }
        return Locker::currentEngineChecked().getNativeInstanceOf<BlockPos>(
            value.asObject(),
            plotx::script::modules::MinecraftModule::ScriptBlockPos
        );
    }
};

} // namespace qjspp