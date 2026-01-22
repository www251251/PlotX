#pragma once
#include <concepts>
#include <memory>

namespace econbridge {

class IEconomy;

namespace concepts {

template <typename T>
concept EconomyImpl = std::derived_from<T, IEconomy> && std::is_final_v<T> && requires {
    { T::isAvailable() } -> std::same_as<bool>;
};

} // namespace concepts

} // namespace econbridge