#pragma once
#include "mc/_HeaderOutputPredefine.h"
#include "mc/world/level/block/Block.h"
#include <array>

#include "plotx/infra/Concepts.hpp"

namespace plotx::world::helper {


constexpr int ChunkWidth       = 16;                                         // 区块宽度(长宽)
constexpr int WorldMaxHeight   = 320;                                        // 世界最大高度
constexpr int WorldMinHeight   = -64;                                        // 世界最小高度
constexpr int WorldTotalHeight = WorldMaxHeight - WorldMinHeight;            // 世界总高度
constexpr int ChunkYCount      = ChunkWidth * ChunkWidth;                    // 区块Y轴数量
constexpr int BufferSize       = ChunkWidth * ChunkWidth * WorldTotalHeight; // 缓冲区大小


using ArrayBuffer   = std::array<Block const*, BufferSize>;
using BufferSpanMut = buffer_span_mut<Block const*>;
using BufferIndex   = int;


[[nodiscard]] inline bool isMultipleOf16(int num) noexcept { return num % 16 == 0; }

// Minecraft 内存布局：XZY（X：北南，Z：东西，Y：高度）
// 索引公式：(x * ChunkWidth + z) * WorldTotalHeight + y
[[nodiscard]] inline BufferIndex getIndex(int x, int y, int z) { return (x * ChunkWidth + z) * WorldTotalHeight + y; }

template <HasXYZ_v T>
[[nodiscard]] inline BufferIndex getIndex(T const& t) {
    return getIndex(t.x, t.y, t.z);
}

[[nodiscard]] inline auto readBuffer(BufferSpanMut const& buffer, BufferIndex index) { return *buffer.mBegin + index; }
[[nodiscard]] inline auto readBuffer(ArrayBuffer const& buffer, BufferIndex index) { return buffer[index]; }

inline void updateBuffer(ArrayBuffer& buffer, BufferIndex index, Block const* block) { buffer[index] = block; }
inline void updateBuffer(BufferSpanMut& buffer, BufferIndex index, Block const* block) {
    auto element = buffer.mBegin + index;
    *element     = block;
}

inline void fillLayer(ArrayBuffer& buffer, Block const* block, int yLevel) {
    BufferIndex startIndex = yLevel;

    for (int i = 0; i < ChunkYCount; ++i) {
        buffer[startIndex + i * WorldTotalHeight] = block;
    }
}
inline void fillLayer(BufferSpanMut& buffer, Block const* block, int yLevel) {
    BufferIndex startIndex = yLevel;

    for (int i = 0; i < ChunkYCount; ++i) {
        *(buffer.mBegin + startIndex + i * WorldTotalHeight) = block;
    }
}

inline void fillRange(ArrayBuffer& buffer, Block const* block, int startY, int endY) {
    for (int y = startY; y < endY; y++) {
        fillLayer(buffer, block, y);
    }
}
inline void fillRange(BufferSpanMut& buffer, Block const* block, int startY, int endY) {
    for (int y = startY; y < endY; y++) {
        fillLayer(buffer, block, y);
    }
}


} // namespace plotx::world::helper