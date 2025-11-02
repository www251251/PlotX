#include "script/modules/Helper.hpp"
#include "script/modules/minecraft/MinecraftModule.hpp"

#include <mc/server/commands/CommandOutput.h>

namespace plotx::script::modules {

void impl(CommandOutput& output, qjspp::Arguments const& arguments, bool isSuccess) {
    std::ostringstream oss;
    formatArgsToString(arguments, oss);
    isSuccess ? output.success(oss.str()) : output.error(oss.str());
}

qjspp::ClassDefine const MinecraftModule::ScriptCommandOutput =
    qjspp::defineClass<CommandOutput>("CommandOutput")
        .disableConstructor()
        .instanceMethod(
            "success",
            [](void* inst, qjspp::Arguments const& arguments) -> qjspp::Value {
                impl(*static_cast<CommandOutput*>(inst), arguments, true);
                return {};
            }
        )
        .instanceMethod(
            "error",
            [](void* inst, qjspp::Arguments const& arguments) -> qjspp::Value {
                impl(*static_cast<CommandOutput*>(inst), arguments, false);
                return {};
            }
        )
        .build();

} // namespace plotx::script::modules