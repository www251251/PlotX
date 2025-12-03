#include "plotx/core/PlotHandle.hpp"
#include "script/modules/Helper.hpp"
#include "script/modules/plotx/PlotXModule.hpp"

#include "qjspp/bind/meta/ClassDefine.hpp"

#include "mc/platform/UUID.h"

#include <qjspp/bind/builder/ClassDefineBuilder.hpp>

#include "ll/api/reflection/Reflection.h"

namespace qjspp::bind {

template <>
struct TypeConverter<plotx::CommentModel> {
    static Value toJs(plotx::CommentModel const& model) {
        auto obj = Object::newObject();
        ll::reflection::forEachMember(model, [&](std::string_view name, auto&& member) {
            auto scriptTy = bind::ConvertToJs(member);
            obj.set(name, scriptTy);
        });
        return obj;
    }
    static plotx::CommentModel toCpp(Value const& value) {
        auto object = value.asObject();
        auto model  = plotx::CommentModel{};
        ll::reflection::forEachMember(model, [&](std::string_view name, auto& member) {
            if (object.has(name)) {
                using MemberType = std::remove_cvref_t<decltype(member)>;
                member           = bind::ConvertToCpp<MemberType>(object.get(name));
            } else [[unlikely]]
                throw JsException(JsException::Type::TypeError, "Missing member: " + std::string(name));
        });
        return model;
    }
};

} // namespace qjspp::bind

namespace plotx::script::modules {

qjspp::bind::meta::ClassDefine const PlotXModule::ScriptPlotHandle =
    qjspp::bind::defineClass<PlotHandle>("PlotHandle")
        .disableConstructor()
        .property("NotForSale", &NotForSale)
        .instanceMethod("markDirty", &PlotHandle::markDirty)
        .instanceMethod("getCoord", &PlotHandle::getCoord)
        .instanceMethod("getOwner", &PlotHandle::getOwner)
        .instanceMethod("setOwner", &PlotHandle::setOwner)
        .instanceMethod("getName", &PlotHandle::getName)
        .instanceMethod("setName", &PlotHandle::setName)
        .instanceMethod("isForSale", &PlotHandle::isForSale)
        .instanceMethod("getSellingPrice", &PlotHandle::getSellingPrice)
        .instanceMethod("setSellingPrice", &PlotHandle::setSellingPrice)
        .instanceMethod("isMember", &PlotHandle::isMember)
        .instanceMethod("getMembers", &PlotHandle::getMembers)
        .instanceMethod("addMember", &PlotHandle::addMember)
        .instanceMethod("removeMember", &PlotHandle::removeMember)
        .instanceMethod(
            "getComments",
            static_cast<std::vector<CommentModel> const& (PlotHandle::*)() const>(&PlotHandle::getComments),
            static_cast<std::vector<CommentModel> (PlotHandle::*)(mce::UUID const& author) const>(
                &PlotHandle::getComments
            )
        )
        .instanceMethod("getComment", &PlotHandle::getComment)
        .instanceMethod("addComment", &PlotHandle::addComment)
        .instanceMethod("removeComment", &PlotHandle::removeComment)
        .build();

}