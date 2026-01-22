#include "NullEconomy.h"

namespace econbridge::detail {

bool    NullEconomy::isAvailable() { return true; }
int64_t NullEconomy::get(const mce::UUID&) const { return 0; }
bool    NullEconomy::set(const mce::UUID&, int64_t) { return true; }
bool    NullEconomy::add(const mce::UUID&, int64_t) { return true; }
bool    NullEconomy::reduce(const mce::UUID&, int64_t) { return true; }
bool    NullEconomy::transfer(const mce::UUID&, const mce::UUID&, int64_t) { return true; }

} // namespace econbridge::detail