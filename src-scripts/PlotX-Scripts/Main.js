import {CommandRegistrar, CustomForm, EventBus, EventPriority, KeyValueDB, ModalFormSelectedButton} from "@levilamina";
import {BlockPos, ModalFormCancelReason, Player} from "@minecraft";
import {PlotAABB} from "@plotx";


let mod = ScriptMod.current();
let logger = mod.getLogger();

mod.onLoad((self) => {
    logger.warn("ScriptMod onLoad triggered");
    return true;
});
mod.onEnable((self) => {
    logger.warn("ScriptMod onEnable triggered");
    regCmd();
    return true;
});
mod.onDisable((self) => {
    logger.warn("ScriptMod onDisable triggered");
    return true;
});
mod.onUnload((self) => {
    logger.warn("ScriptMod onUnload triggered");
    return true;
});


function regCmd() {
    const cmd = CommandRegistrar.getInstance().getOrCreateCommand("test", "测试命令");
    cmd.runtimeOverload().execute(() => {
        logger.warn("test command executed 0");
    })
    cmd.runtimeOverload().text("foo").execute(() => {
        logger.warn("test command executed 1");
    });
}


/**
 * @param {Player} pl
 */
function main(pl) {
    new CustomForm()
        .setTitle("自定义表单")
        .appendLabel("这是一个标签")
        .appendHeader("这是一个标题")
        .appendDivider()
        .appendInput("input", "输入框", "占位符", "默认值", "输入框说明")
        .appendToggle("toggle", "开关", true, "开关说明")
        .appendDropdown("dropdown", "下拉框", ["选项1", "选项2", "选项3"], 0, "下拉框说明")
        .appendSlider("slider", "滑块", 0, // 最小值
            100, // 最大值
            1, // 步长
            50, // 默认值
            "滑块说明")
        .appendStepSlider("stepSlider", "步进滑块", ["选项1", "选项2", "选项3"], 0, // 默认值
            "步进滑块说明")
        .sendTo(pl, (player, result, cancelReason) => {
            logger.debug("Form callback triggered");
            if (result === null) {
                player.sendMessage("表单已取消");
                return;
            }
            player.sendMessage("表单已提交");
            logger.debug(result ? JSON.stringify(result, null, 2) : "表单已取消");
        });
}

const id = EventBus.emplaceListener("PlayerJoinEvent", (ev) => {
    logger.trace("PlayerJoinEvent triggered, id: ", ev.getId());
    let player = ev.self();
    logger.trace("Player: ", player.realName);
    main(player);
    EventBus.removeListener(id);
});

logger.info("Script loaded!");

logger.warn("Enum Bind test: ");
logger.warn("EventPriority: ", JSON.stringify(EventPriority));
logger.warn("ModalFormCancelReason: ", JSON.stringify(ModalFormCancelReason));
logger.warn("ModalFormSelectedButton: ", JSON.stringify(ModalFormSelectedButton));

const db = new KeyValueDB("D:/Codes/PlotCraft-Dev/bin/PlotCraft/data/PlotStorage");

// lambda no this
db.iter((k, v) => {
    logger.warn(`key: ${k}`);
});

// lambda with this
db.iter(function (k, v) {
    logger.warn(`key: ${k}, has: ${this.has(k)}`);
});

try {
    // close
    db.close();
    db.set("test", "testaaaaa"); // error
} catch (e) {
    if (e instanceof ReferenceError) {
        logger.error(e, "\n", e.stack);
    }
}

let min = new BlockPos(0, 0, 0);
let max = new BlockPos(1, 1, 1);
let ab1 = new PlotAABB(min, max);
let ab2 = new PlotAABB(min, max);

logger.warn("AABB: ", ab1.$equals(ab2));
