#pragma once
#include <algorithm>
#include <string>


namespace plotx::string_utils {

// 字符长度 (UTF-8)
inline size_t length(std::string const& str) {
    return std::ranges::count_if(str, [](char c) { return (static_cast<unsigned char>(c) & 0xC0) != 0x80; });
}

} // namespace plotx::string_utils