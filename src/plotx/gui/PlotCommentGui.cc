#include "PlotCommentGui.hpp"

#include "plotx/PlotX.hpp"
#include "plotx/core/PlotHandle.hpp"
#include "plotx/core/PlotRegistry.hpp"
#include "plotx/utils/FeedbackUtils.hpp"
#include "plotx/utils/TimeUtils.hpp"

#include <ll/api/form/CustomForm.h>
#include <ll/api/form/ModalForm.h>
#include <ll/api/form/SimpleForm.h>
#include <ll/api/i18n/I18n.h>
#include <ll/api/service/PlayerInfo.h>
#include <mc/world/actor/player/Player.h>

namespace plotx {

using ll::i18n_literals::operator""_trl;

void PlotCommentGUI::sendTo(Player& player, std::shared_ptr<PlotHandle> handle) {
    auto localeCode = player.getLocaleCode();
    auto ownerInfo  = ll::service::PlayerInfo::getInstance().fromUuid(handle->getOwner());

    auto f = ll::form::SimpleForm{};
    f.setTitle("PlotX - 地皮评论"_trl(localeCode));
    f.setContent(
        "{} 的地皮 {} 评论区"_trl(
            localeCode,
            ownerInfo ? ownerInfo->name : handle->getOwner().asString(),
            handle->getName()
        )
    );
    f.appendButton("撰写评论"_trl(localeCode), [handle](Player& player) { writeComment(player, handle); });

    auto comments = handle->getComments();
    std::ranges::sort(comments, [](CommentModel const& a, CommentModel const& b) {
        return time_utils::parse(a.time_) < time_utils::parse(b.time_);
    });

    auto& playerInfo = ll::service::PlayerInfo::getInstance();
    for (auto const& comment : handle->getComments()) {
        auto info = playerInfo.fromUuid(mce::UUID::fromString(comment.author_));
        f.appendButton(
            "{}\n{} - {}"_trl(localeCode, comment.content_, info ? info->name : comment.author_, comment.time_),
            [handle = std::move(handle), id = comment.id_](Player& player) { showComment(player, handle, id); }
        );
    }
    f.sendTo(player);
}
void PlotCommentGUI::writeComment(Player& player, std::shared_ptr<PlotHandle> handle) {
    auto localeCode = player.getLocaleCode();

    auto f = ll::form::CustomForm{};
    f.setTitle("PlotX - 撰写评论"_trl(localeCode));
    f.appendInput("content", "评论内容"_trl(localeCode), "string");
    f.sendTo(player, [handle = std::move(handle)](Player& player, ll::form::CustomFormResult const& data, auto) {
        if (!data) {
            return;
        }
        auto content = std::get<std::string>(data->at("content"));
        handle->addComment(player.getUuid(), content);
    });
}
void PlotCommentGUI::showComment(Player& player, std::shared_ptr<PlotHandle> handle, CommentID id) {
    auto localeCode = player.getLocaleCode();

    auto comment = handle->getComment(id);
    if (!ensureCommentExists(player, comment)) {
        return;
    }

    bool const isAdmin  = PlotX::getInstance().getPlotRegistry()->isAdmin(player.getUuid());
    bool const isOwner  = handle->isOwner(player.getUuid());
    bool const isAuthor = comment->author_ == player.getUuid().asString();

    auto f = ll::form::SimpleForm{};
    f.setTitle("PlotX - 评论详情"_trl(localeCode));
    f.setContent(
        "ID: {}\n作者: {}\n时间: {}\n内容: {}"_trl(
            localeCode,
            comment->id_,
            comment->author_,
            comment->time_,
            comment->content_
        )
    );
    if (isAdmin || isAuthor) {
        f.appendButton(
            "编辑评论"_trl(localeCode),
            "textures/ui/book_edit_default",
            "path",
            [handle, id](Player& player) { editComment(player, handle, id); }
        );
    }
    if (isAdmin || isOwner || isAuthor) {
        f.appendButton("删除评论"_trl(localeCode), "textures/ui/icon_trash", "path", [handle, id](Player& player) {
            deleteComment(player, handle, id);
        });
    }
    f.appendButton("返回"_trl(localeCode), "textures/ui/icon_import", "path", [handle](Player& player) {
        sendTo(player, handle);
    });
    f.sendTo(player);
}
void PlotCommentGUI::editComment(Player& player, std::shared_ptr<PlotHandle> handle, CommentID id) {
    auto localeCode = player.getLocaleCode();

    auto comment = handle->getComment(id);
    if (!ensureCommentExists(player, comment)) {
        return;
    }

    auto f = ll::form::CustomForm{};
    f.setTitle("PlotX - 编辑评论"_trl(localeCode));
    f.appendInput("content", "评论内容"_trl(localeCode), "string", comment->content_);
    f.sendTo(player, [handle, id](Player& player, ll::form::CustomFormResult const& data, auto) {
        if (!data) {
            return;
        }
        auto content = std::get<std::string>(data->at("content"));
        if (handle->updateCommentContent(id, std::move(content))) {
            showComment(player, handle, id);
        } else {
            feedback_utils::sendErrorText(player, "更新评论内容失败"_trl(player.getLocaleCode()));
        }
    });
}
void PlotCommentGUI::deleteComment(Player& player, std::shared_ptr<PlotHandle> handle, CommentID id) {
    auto localeCode = player.getLocaleCode();

    ll::form::ModalForm{
        "PlotX - 删除评论"_trl(localeCode),
        "您确认删除评论吗？"_trl(localeCode),
        "确认"_trl(localeCode),
        "返回"_trl(localeCode)
    }
        .sendTo(player, [handle, id](Player& player, ll::form::ModalFormResult const& result, auto) {
            if (!result) {
                return;
            }
            if ((bool)result.value()) {
                handle->removeComment(id);
            } else {
                showComment(player, handle, id);
            }
        });
}
bool PlotCommentGUI::ensureCommentExists(Player& player, std::optional<CommentModel> const& model) {
    if (!model) {
        feedback_utils::sendErrorText(player, "评论不存在"_trl(player.getLocaleCode()));
        return false;
    }
    return true;
}

} // namespace plotx