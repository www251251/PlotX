# EconomyBridge

- [x] LegacyMoney (Windows Symbol call)
- [x] ScoreBoard (Only Online Mode)

## Usage

```lua
add_repositories("iceblcokmc https://github.com/IceBlcokMC/xmake-repo.git")

add_requires("economy_bridge")
```

```cpp
#include "econbridge/detail/LegacyMoneyEconomy.h"
#include "econbridge/detail/NullEconomy.h"
#include "econbridge/detail/ScoreboardEconomy.h"

class MyMod {
    std::shared_ptr<econbridge::IEconomy> economy_;

    bool load() {
        // ...
    }

    bool enable() {
        // ...

        // use LegacyMoney
        if (econbridge::detail::LegacyMoneyEconomy::isAvailable()) {
            economy_ = std::make_shared<econbridge::detail::LegacyMoneyEconomy>();
        } else {
            getLogger().error("LegacyMoney is not available");
        }

        // or use ScoreboradEconomy
        // economy_ = std::make_shared<econbridge::detail::ScoreboardEconomy>("money");

        // or disable economy
        // economy_ = std::make_shared<econbridge::detail::NullEconomy>();
    }
};
```

## LICENSE

LGPL-3.0 or later
