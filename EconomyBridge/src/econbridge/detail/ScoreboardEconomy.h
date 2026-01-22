#pragma once
#include "econbridge/Concepts.h"
#include "econbridge/IEconomy.h"

struct ScoreboardId;
class Scoreboard;
class Player;
namespace econbridge ::detail {

class ScoreboardEconomy final : public IEconomy {
    std::string scoreBoardName_;

public:
    static bool isAvailable();

    static Player*             uuid2player(mce::UUID const& uuid);
    static ScoreboardId const& getScoreboardId(Scoreboard& scoreboard, Player& player);

    explicit ScoreboardEconomy(std::string scoreBoardName);

    int64_t get(const mce::UUID& uuid) const override;
    bool    set(const mce::UUID& uuid, int64_t amount) override;
    bool    add(const mce::UUID& uuid, int64_t amount) override;
    bool    reduce(const mce::UUID& uuid, int64_t amount) override;
    bool    transfer(const mce::UUID& from, const mce::UUID& to, int64_t amount) override;
};
static_assert(concepts::EconomyImpl<ScoreboardEconomy>);

} // namespace econbridge::detail
