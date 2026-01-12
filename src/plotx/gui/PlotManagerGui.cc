#include "PlotManagerGui.hpp"

#include "PlayerPicker.hpp"
#include "PlotCommentGui.hpp"
#include "plotx/PlotX.hpp"
#include "plotx/core/PlotHandle.hpp"
#include "plotx/core/PlotRegistry.hpp"
#include "plotx/core/PlotService.hpp"
#include "plotx/utils/FeedbackUtils.hpp"

#include "perm_core/gui/PermGUI.hpp"

#include <ll/api/form/CustomForm.h>
#include <ll/api/form/ModalForm.h>
#include <ll/api/form/SimpleForm.h>
#include <ll/api/i18n/I18n.h>
#include <ll/api/service/PlayerInfo.h>
#include <stdexcept>

namespace plotx {

using ll::i18n_literals::operator""_trl;

void PlotManagerGUI::sendTo(Player& player, std::shared_ptr<PlotHandle> handle) {
    if (!handle) {
        throw std::runtime_error("PlotManagerGUI::sendTo: handle is null");
    }
    auto localeCode = player.getLocaleCode();

    auto ownerInfo = ll::service::PlayerInfo::getInstance().fromUuid(handle->getOwner());

    bool const isAdmin  = PlotX::getInstance().getPlotRegistry()->isAdmin(player.getUuid());
    bool const isOwner  = handle->isOwner(player.getUuid());
    bool const isMember = handle->isMember(player.getUuid());
    bool const isGuest  = !isAdmin && !isOwner && !isMember;

    auto f = ll::form::SimpleForm{};
    f.setTitle("PlotX - 地皮管理"_trl(localeCode));
    f.setContent(
        "位置: {},{}\n所有者: {}\n名称: {}\n状态: {}"_trl(
            localeCode,
            handle->getCoord().x,
            handle->getCoord().z,
            ownerInfo ? ownerInfo->name : handle->getOwner().asString(),
            handle->getName(),
            handle->isForSale() ? "出售中 (价格:{})"_trl(localeCode, handle->getSellingPrice())
                                : "未出售"_trl(localeCode)
        )
    );
    if (!isGuest) {
        f.appendButton("传送到此地皮"_trl(localeCode), "textures/ui/move", "path", [handle](Player& player) {
            if (auto exp = PlotX::getInstance().getService()->teleportToPlot(player, handle)) {
                auto localeCode = player.getLocaleCode();
                feedback_utils::notifySuccess(
                    player,
                    "传送到地皮 {} 位置: {}"_trl(localeCode, handle->getName(), handle->getCoord().min.toString())
                );
            } else {
                feedback_utils::sendError(player, exp.error());
            }
        });
    }
    if (isOwner || isAdmin) {
        f.appendButton("权限管理"_trl(localeCode), "textures/ui/gear", "path", [handle](Player& player) {
            permc::PermGUI::sendTo(
                player,
                player.getLocaleCode(),
                handle->getPermTable(),
                [whandle = std::weak_ptr{handle}](Player& player, permc::PermTable newTable) {
                    auto localeCode = player.getLocaleCode();
                    if (auto h = whandle.lock()) {
                        h->setPermTable(std::move(newTable));
                        feedback_utils::notifySuccess(player, "权限已更新"_trl(localeCode));
                    }
                }
            );
        });
        f.appendButton("编辑名称"_trl(localeCode), "textures/ui/book_edit_default", "path", [handle](Player& player) {
            handleEditName(player, handle, std::nullopt);
        });
        f.appendButton("地皮出售"_trl(localeCode), "textures/ui/MCoin", "path", [handle](Player& player) {
            handleEditSellStatus(player, handle);
        });
        f.appendButton("成员管理"_trl(localeCode), "textures/ui/share_microsoft", "path", [handle](Player& player) {
            showMembers(player, handle);
        });
    }
    f.appendButton("地皮评论"_trl(localeCode), "textures/ui/icon_sign", "path", [handle](Player& player) {
        PlotCommentGUI::sendTo(player, handle);
    });
    f.sendTo(player);
}

void PlotManagerGUI::handleEditName(
    Player&                     player,
    std::shared_ptr<PlotHandle> handle,
    std::optional<std::string>  lastInput
) {
    auto localeCode = player.getLocaleCode();

    auto f = ll::form::CustomForm{};
    f.setTitle("PlotX - 编辑地皮名称"_trl(localeCode));
    f.appendInput("name", "地皮名称"_trl(localeCode), "string", lastInput ? lastInput.value() : handle->getName());
    f.sendTo(
        player,
        [handle     = std::move(handle),
         localeCode = std::move(localeCode)](Player& player, ll::form::CustomFormResult const& data, auto) {
            if (!data) {
                return;
            }
            auto name = std::get<std::string>(data->at("name"));
            if (auto exp = PlotX::getInstance().getService()->changePlotName(player, handle, name)) {
                sendTo(player, handle);
            } else {
                feedback_utils::askRetry(player, exp.error(), [handle, name](Player& player) {
                    handleEditName(player, handle, name);
                });
            }
        }
    );
}
void PlotManagerGUI::handleEditSellStatus(Player& player, std::shared_ptr<PlotHandle> handle) {
    auto localeCode = player.getLocaleCode();

    auto f = ll::form::SimpleForm{};
    f.setTitle("PlotX - 地皮出售"_trl(localeCode));

    if (handle->isForSale()) {
        f.setContent("当前地皮正在出售，价格: {}"_trl(localeCode, handle->getSellingPrice()));
        f.appendButton("更改价格"_trl(localeCode), "textures/ui/book_edit_default", "path", [handle](Player& player) {
            sellPlotOrEditSellPrice(player, handle, true);
        });
        f.appendButton("取消出售"_trl(localeCode), "textures/ui/cancel", "path", [handle](Player& player) {
            handle->setSellingPrice(NotForSale);
            handleEditSellStatus(player, handle);
        });
    } else {
        f.setContent("当前地皮未出售"_trl(localeCode));
        f.appendButton("出售地皮"_trl(localeCode), "textures/ui/icon_minecoin_9x9", "path", [handle](Player& player) {
            sellPlotOrEditSellPrice(player, handle, false);
        });
    }
    f.appendButton("返回"_trl(localeCode), "textures/ui/icon_import", "path", [handle](Player& player) {
        sendTo(player, handle);
    });
    f.sendTo(player);
}
void PlotManagerGUI::sellPlotOrEditSellPrice(Player& player, std::shared_ptr<PlotHandle> handle, bool edit) {
    auto localeCode = player.getLocaleCode();

    auto f = ll::form::CustomForm{};
    f.setTitle("PlotX - 地皮出售"_trl(localeCode));
    f.appendInput("price", "出售价格"_trl(localeCode), "number", edit ? std::to_string(handle->getSellingPrice()) : "");
    f.sendTo(player, [handle](Player& player, ll::form::CustomFormResult const& data, auto) {
        if (!data) {
            return;
        }
        auto priceStr = std::get<std::string>(data->at("price"));
        try {
            int price = std::stoll(priceStr);
            if (price < 0) {
                feedback_utils::sendErrorText(player, "价格不能为负数"_trl(player.getLocaleCode()));
                return;
            }
            handle->setSellingPrice(price);
        } catch (...) {
            feedback_utils::sendErrorText(player, "无效的价格"_trl(player.getLocaleCode()));
        }
    });
}
void PlotManagerGUI::showMembers(Player& player, std::shared_ptr<PlotHandle> handle) {
    auto localeCode = player.getLocaleCode();

    auto f = ll::form::SimpleForm{};
    f.setTitle("PlotX - 成员管理"_trl(localeCode));
    f.setContent("点击成员以进行移除, 或点击按钮添加成员"_trl(localeCode));
    f.appendButton("返回"_trl(localeCode), "textures/ui/icon_import", "path", [handle](Player& player) {
        sendTo(player, handle);
    });
    f.appendButton("添加成员"_trl(localeCode), "textures/ui/color_plus", "path", [handle](Player& player) {
        chooseAddType(player, handle);
    });

    auto& members = handle->getMembers();
    auto& infoDb  = ll::service::PlayerInfo::getInstance();
    for (auto& member : members) {
        auto uuid = mce::UUID::fromString(member);
        auto info = infoDb.fromUuid(uuid);
        f.appendButton(info ? info->name : member, [handle, uuid](Player& player) {
            if (auto exp = PlotX::getInstance().getService()->modifyPlotMember(player, handle, uuid, false)) {
                showMembers(player, handle);
            } else {
                feedback_utils::sendError(player, exp.error());
            }
        });
    }
    f.sendTo(player);
}
void PlotManagerGUI::chooseAddType(Player& player, std::shared_ptr<PlotHandle> handle) {
    auto localeCode = player.getLocaleCode();

    ll::form::ModalForm{
        "PlotX - 添加成员"_trl(localeCode),
        "选择添加方式"_trl(localeCode),
        "添加在线玩家"_trl(localeCode),
        "添加离线玩家"_trl(localeCode)
    }
        .sendTo(player, [handle](Player& player, ll::form::ModalFormResult const& result, auto) {
            if (!result) {
                return;
            }
            if ((bool)result.value()) {
                addMemberFromOnline(player, handle);
            } else {
                addMemberFromOffline(player, handle);
            }
        });
}
void PlotManagerGUI::addMemberFromOnline(Player& player, std::shared_ptr<PlotHandle> handle) {
    PlayerPicker::sendTo(player, [handle](Player& player, mce::UUID picked) {
        if (auto exp = PlotX::getInstance().getService()->modifyPlotMember(player, handle, picked, true)) {
            showMembers(player, handle);
        } else {
            feedback_utils::sendError(player, exp.error());
        }
    });
}
void PlotManagerGUI::addMemberFromOffline(Player& player, std::shared_ptr<PlotHandle> handle) {
    auto localeCode = player.getLocaleCode();

    auto f = ll::form::CustomForm{};
    f.setTitle("PlotX - 添加成员"_trl(localeCode));
    f.appendInput("name", "玩家名"_trl(localeCode), "请输入要添加的玩家名"_trl(localeCode));
    f.sendTo(player, [handle](Player& player, ll::form::CustomFormResult const& data, auto) {
        if (!data) {
            return;
        }
        auto name = std::get<std::string>(data->at("name"));
        if (name.empty()) {
            feedback_utils::sendErrorText(player, "玩家名不能为空"_trl(player.getLocaleCode()));
            return;
        }
        auto entry = ll::service::PlayerInfo::getInstance().fromName(name);
        if (!entry) {
            feedback_utils::sendErrorText(player, "未找到玩家 {} 的信息"_trl(player.getLocaleCode(), name));
            return;
        }
        if (auto exp = PlotX::getInstance().getService()->modifyPlotMember(player, handle, entry->uuid, true)) {
            showMembers(player, handle);
        } else {
            feedback_utils::sendError(player, exp.error());
        }
    });
}


} // namespace plotx